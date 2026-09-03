; V86MMGR -- reconstructed 32-bit disassembly (VxD / LE format)
; 15 objects, 12207 instructions recovered by recursive descent

Declare_Virtual_Device V86MMGR, 1, 0, V86MMGR_Control, 00006h, Undefined_Init_Order
;   DDK version 0400h   init_order 0A0000000h   flags 00000h
;   service table: obj1:0x1148, 25 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1334  R-X
V86MMGR_Load_Client_Ptr:
        push   ecx
        movzx  ecx, ah
        mov    fs, word ptr [ecx + ebp]
        movzx  ecx, al
        mov    esi, dword ptr [ecx + ebp]
        test   word ptr [ebx], 0x180
        jne    loc_1_0018
        movzx  esi, si
loc_1_0018:
        pop    ecx
        ret    
        db  08Bh, 0C0h
V86MMGR_Allocate_Buffer:
        push   eax
        pushfd 
        jae    loc_1_0031
        mov    ax, fs
        lsl    eax, eax
        inc    eax
        je     loc_1_0031
        sub    eax, esi
        cmp    eax, ecx
        jae    loc_1_0031
        mov    ecx, eax
loc_1_0031:
        mov    eax, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    edi, dword ptr [eax + ebx + 0x14]
        sub    edi, ecx
        jb     loc_1_0080
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_0058
        push   ecx
        add    ecx, edi
        cmp    ecx, dword ptr [eax + ebx + 0x18]
        jne    loc_1_0057
        xor    ecx, ecx
        VMMCall  VMM.Begin_V86_Serialization     ; cd203c010100
loc_1_0057:
        pop    ecx
loc_1_0058:
        mov    dword ptr [eax + ebx + 0x14], edi
        popfd  
        jae    loc_1_0079
        push   esi
        push   edi
        push   ecx
        cld    
        add    edi, dword ptr [eax + ebx + 0x20]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr fs:[esi]
        mov    ecx, dword ptr [esp]
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr fs:[esi]
        pop    ecx
        pop    edi
        pop    esi
loc_1_0079:
        or     edi, dword ptr [eax + ebx + 0x1c]
        pop    eax
        clc    
        ret    
loc_1_0080:
        popfd  
        pop    eax
        stc    
        ret    
V86MMGR_Free_Buffer:
        jae    loc_1_00BC
        pushal 
        mov    edi, fs
        lar    edx, edi
        test   dh, 8
        jne    loc_1_00E2
        test   dh, 2
        je     loc_1_00E2
        mov    es, edi
        mov    edi, esi
loc_1_009A:
        add    ebx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    esi, dword ptr [ebx + 0x14]
        add    esi, dword ptr [ebx + 0x20]
        cld    
loc_1_00A7:
        mov    eax, ecx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_00B5:
        mov    ax, ds
        mov    es, ax
        popal  
loc_1_00BC:
        push   edx
        push   ecx
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        add    edx, ebx
        add    dword ptr [edx + 0x14], ecx
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_00DF
        mov    ecx, dword ptr [edx + 0x14]
        cmp    ecx, dword ptr [edx + 0x18]
        jne    loc_1_00DF
        VMMCall  VMM.End_V86_Serialization       ; cd2072010100
loc_1_00DF:
        pop    ecx
        pop    edx
        ret    
loc_1_00E2:
        push   0
        push   fs
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        mov    ecx, dword ptr [esp + 0x18]
        lea    edi, [esi + eax]
        jmp    loc_1_009A
V86MMGR_Xlat_API:
        push   eax
        movzx  eax, byte ptr [edx]
        inc    edx
        mov    eax, dword ptr [eax*4 + 0x68c] ; -> obj1:0x068C
        xchg   dword ptr [esp], eax
        ret    
loc_1_0109:
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        push   eax
        movzx  eax, byte ptr [edx]
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        pop    eax
        VMMCall  VMM.End_Nest_Exec               ; cd2086800100
loc_1_0120:
        add    edx, 4
        jmp    dword ptr [edx - 4]
loc_1_0126:
        push   ecx
        movzx  ecx, word ptr [edx]
        add    edx, 2
        call   sub_1_015F
        pop    ecx
        ret    
loc_1_0134:
        push   ecx
        movzx  ecx, byte ptr [edx]
        mov    cx, word ptr [ecx + ebp]
        inc    edx
        call   sub_1_015F
loc_1_0142:
        pop    ecx
        ret    
loc_1_0144:
        push   ecx
        push   0x142                         ; -> loc_1_0142
        push   fs
        push   eax
        push   esi
        movzx  eax, word ptr [edx + 4]
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
        call   dword ptr [edx]
        add    edx, 4
        jmp    loc_1_016C
sub_1_015F:
        push   fs
        push   eax
        push   esi
        mov    ax, word ptr [edx]
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
loc_1_016C:
        push   edi
        stc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        movzx  eax, al
        add    eax, ebp
        push   dword ptr [eax]
        mov    word ptr [eax], di
        push   eax
        movzx  eax, byte ptr [edx + 1]
        shr    edi, 0x10
        xchg   word ptr [ebp + eax + 0x24], di
        add    edx, 2
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006000600
        mov    word ptr [ebp + eax + 0x24], di
        pop    eax
        pop    dword ptr [eax]
        stc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        pop    edi
        pop    esi
        pop    eax
        pop    fs
        ret    
loc_1_01A9:
        push   fs
        push   eax
        push   ecx
        push   esi
        push   edi
        mov    ax, word ptr [edx]
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
        push   es
        mov    ax, fs
        mov    es, ax
        mov    edi, esi
        lsl    ecx, eax
        sub    ecx, edi
        inc    ecx
        cld    
        xor    al, al
        repne scasb al, byte ptr es:[edi]
        pop    es
        je     loc_1_01D5
        lea    edi, [esi + 0x100]
loc_1_01D5:
        sub    edi, esi
        lea    ecx, [edi + 3]
        stc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        movzx  eax, byte ptr [edx]
        add    eax, ebp
        push   dword ptr [eax]
        mov    word ptr [eax], di
        push   eax
        movzx  eax, byte ptr [edx + 1]
        shr    edi, 0x10
        xchg   word ptr [ebp + eax + 0x24], di
        push   eax
        mov    al, byte ptr [edx - 1]
        add    edx, 2
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006000600
        mov    edx, eax
        pop    eax
        mov    word ptr [ebp + eax + 0x24], di
        pop    eax
        pop    dword ptr [eax]
        cmp    dl, 8
        cmc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        pop    fs
        ret    
        db  000h, 000h, 000h
loc_1_0224:
        cmp    byte ptr [ebp + 0x1d], 0x87
        je     loc_1_023A
        cmp    byte ptr [ebp + 0x1d], 0x88
        je     loc_1_0232
        stc    
        ret    
loc_1_0232:
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_1_023A:
        movzx  esi, word ptr [ebp + 4]
        movzx  ecx, word ptr [ebp + 0x38]
        shl    ecx, 4
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        movzx  ecx, word ptr [ebp + 0x18]
        call   sub_1_0255
        clc    
        ret    
sub_1_0255:
        pushfd 
        sti    
        cmp    ecx, 0x8000
        ja     loc_1_0351
        mov    eax, ecx
        shl    eax, 1
        dec    eax
        cmp    ax, word ptr [esi + 0x10]
        ja     loc_1_0351
        cmp    ax, word ptr [esi + 0x18]
        ja     loc_1_0351
        mov    dl, byte ptr [esi + 0x1c]
        mov    dh, byte ptr [esi + 0x1f]
        movzx  edi, dx
        shl    edi, 0x10
        mov    di, word ptr [esi + 0x1a]
        mov    dl, byte ptr [esi + 0x14]
        mov    dh, byte ptr [esi + 0x17]
        shl    edx, 0x10
        mov    dx, word ptr [esi + 0x12]
        mov    esi, edx
        shl    ecx, 1
        cmp    edi, 0x110000
        jae    loc_1_02D5
        mov    eax, edi
        add    eax, ecx
        cmp    eax, 0x100000
        jbe    loc_1_02EB
        cmp    edi, 0x100000
        jb     loc_1_0351
        call   sub_15_00DE
        jb     loc_1_02D5
        jne    loc_1_0351
        xchg   edi, eax
        add    eax, ecx
        cmp    eax, 0x110000
        ja     loc_1_0351
        jmp    loc_1_02EE
loc_1_02D5:
        push   ecx
        push   0
        push   ecx
        push   edi
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        pop    ecx
        mov    edi, eax
        inc    eax
        jne    loc_1_02EE
        jmp    loc_1_0351
loc_1_02EB:
        add    edi, dword ptr [ebx + 4]
loc_1_02EE:
        cmp    esi, 0x110000
        jae    loc_1_031F
        mov    edx, esi
        add    edx, ecx
        cmp    edx, 0x100000
        jbe    loc_1_0335
        cmp    esi, 0x100000
        jb     loc_1_0351
        call   sub_15_00DE
        jb     loc_1_031F
        jne    loc_1_0351
        cmp    edx, 0x110000
        ja     loc_1_0351
        mov    esi, eax
        jmp    loc_1_0338
loc_1_031F:
        push   ecx
        push   0
        push   ecx
        push   esi
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        pop    ecx
        mov    esi, eax
        inc    eax
        jne    loc_1_0338
        jmp    loc_1_0351
loc_1_0335:
        add    esi, dword ptr [ebx + 4]
loc_1_0338:
        push   ecx
        push   esi
        push   edi
        call   sub_15_011D
        add    esp, 0xc
        mov    byte ptr [ebp + 0x1d], 0
        or     byte ptr [ebp + 0x2c], 0x40
        and    byte ptr [ebp + 0x2c], 0xfe
loc_1_034F:
        popfd  
        ret    
loc_1_0351:
        mov    byte ptr [ebp + 0x1d], 2
        and    byte ptr [ebp + 0x2c], 0xbf
        or     byte ptr [ebp + 0x2c], 1
        jmp    loc_1_034F
        db  000h
sub_1_0360:
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    edi, dword ptr [0x6b0]        ; -> obj1:0x06B0
        cmp    edi, dword ptr [esi + ebx + 0x28]
        je     loc_1_03AA
        xchg   dword ptr [esi + ebx + 0x28], edi
        mov    esi, dword ptr [0xeb4]        ; -> obj1:0x0EB4
loc_1_037C:
        cmp    dword ptr [esi*4 + 0x6b4], edi ; -> obj1:0x06B4
        jbe    loc_1_03A1
        mov    eax, dword ptr [esi*4 + 0xab4] ; -> obj1:0x0AB4
        and    eax, 0xffffff
        push   0
        push   1
        push   esi
        push   ebx
        push   eax
        VMMCall  VMM._LinMapIntoV86              ; cd2062000100
        add    esp, 0x14
loc_1_03A1:
        inc    esi
        cmp    esi, dword ptr [0xeb8]        ; -> obj1:0x0EB8
        jbe    loc_1_037C
loc_1_03AA:
        ret    
V86MMGR_Map_Pages:
        test   ecx, ecx
        jne    loc_1_03B6
        or     edi, 0xffffffff
        movzx  esi, di
        ret    
loc_1_03B6:
        pushal 
        mov    eax, esi
        and    eax, 0xfff
        lea    edi, [ecx + eax + 0xfff]
        shr    edi, 0xc
        mov    ecx, esi
        jecxz  loc_1_03E4
        shr    ecx, 0xc
        push   0
        push   edi
        push   ecx
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        test   eax, eax
        je     loc_1_04F0
loc_1_03E4:
        mov    ecx, edi
        test   esi, esi
        je     loc_1_043C
        mov    ebx, dword ptr [0xeb4]        ; -> obj1:0x0EB4
loc_1_03F0:
        lea    edx, [ecx + ebx - 1]
        cmp    edx, dword ptr [0xeb8]        ; -> obj1:0x0EB8
        ja     loc_1_043C
        mov    edx, esi
        shr    edx, 0xc
        mov    edi, ecx
        mov    ebp, ebx
loc_1_0405:
        mov    eax, dword ptr [ebx*4 + 0xab4] ; -> obj1:0x0AB4
        and    eax, 0xffffff
        cmp    eax, edx
        jne    loc_1_042F
        inc    ebx
        inc    edx
        dec    edi
        jne    loc_1_0405
        mov    edi, ecx
        mov    ebx, ebp
loc_1_041E:
        inc    byte ptr [ebx*4 + 0xab7]      ; -> loc_1_0AB7
        inc    ebx
        loop   loc_1_041E
        mov    ebx, ebp
        jmp    loc_1_04B2
loc_1_042F:
        cmp    edi, ecx
        jb     loc_1_03F0
        inc    ebx
        cmp    ebx, dword ptr [0xeb8]        ; -> obj1:0x0EB8
        jb     loc_1_03F0
loc_1_043C:
        mov    edi, ecx
        mov    eax, dword ptr [0xeb8]        ; -> obj1:0x0EB8
loc_1_0443:
        movzx  ebx, word ptr [eax*2 + 0xf28] ; -> obj1:0x0F28
        test   bh, 4
        je     loc_1_045A
        test   bl, 1
        jne    loc_1_045A
        dec    edi
        je     loc_1_0467
        jmp    loc_1_045C
loc_1_045A:
        mov    edi, ecx
loc_1_045C:
        dec    eax
        cmp    eax, dword ptr [0xeb4]        ; -> obj1:0x0EB4
        jae    loc_1_0443
        jmp    loc_1_04D9
loc_1_0467:
        push   eax
        mov    edi, ecx
        mov    bx, 1
        test   esi, esi
        jne    loc_1_047B
        or     bx, 0x8000
        xor    edx, edx
        jmp    loc_1_0491
loc_1_047B:
        shr    esi, 0xc
        or     esi, 0x1000000
        mov    edx, dword ptr [0x6b0]        ; -> obj1:0x06B0
        inc    edx
        mov    dword ptr [0x6b0], edx        ; -> obj1:0x06B0
loc_1_0491:
        mov    dword ptr [eax*4 + 0x6b4], edx ; -> obj1:0x06B4
        mov    dword ptr [eax*4 + 0xab4], esi ; -> obj1:0x0AB4
        or     word ptr [eax*2 + 0xf28], bx  ; -> obj1:0x0F28
        inc    eax
        inc    esi
        loop   loc_1_0491
        sub    dword ptr [0xec0], edi        ; -> obj1:0x0EC0
        pop    ebx
loc_1_04B2:
        mov    eax, edi
        shl    eax, 0x10
        mov    ax, bx
        xchg   dword ptr [esp + 4], eax
        and    eax, 0xfff
        shl    ebx, 0xc
        add    eax, ebx
        mov    dword ptr [esp], eax
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        call   sub_1_0360
        popal  
        clc    
        ret    
loc_1_04D9:
        xchg   ecx, eax
        mov    ecx, dword ptr [esp + 4]
        jecxz  loc_1_04F0
        shr    ecx, 0xc
        push   0
        push   eax
        push   ecx
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
loc_1_04F0:
        popal  
        stc    
        ret    
V86MMGR_Free_Page_Map_Region:
        pushal 
        movzx  edi, si
        shr    esi, 0x10
        je     loc_1_0578
loc_1_04FC:
        test   byte ptr [edi*2 + 0xf29], 0x80 ; -> obj1:0x0F29
        jne    loc_1_051F
        push   0
        push   1
        push   dword ptr [edi*4 + 0xab4]     ; -> obj1:0x0AB4
        mov    byte ptr [esp + 3], 0
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
loc_1_051F:
        dec    byte ptr [edi*4 + 0xab7]      ; -> loc_1_0AB7
        jne    loc_1_0574
        xor    eax, eax
        mov    dword ptr [edi*4 + 0x6b4], eax ; -> obj1:0x06B4
        mov    dword ptr [edi*4 + 0xab4], eax ; -> obj1:0x0AB4
        and    word ptr [edi*2 + 0xf28], 0x7ffe ; -> obj1:0x0F28
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
loc_1_0548:
        VMMCall  VMM._GetNulPageHandle           ; cd206a000100
        push   0
        push   0
        push   1
        push   edi
        push   ebx
        push   eax
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_1_0548
        inc    dword ptr [0xec0]             ; -> obj1:0x0EC0
loc_1_0574:
        inc    edi
        dec    esi
        jne    loc_1_04FC
loc_1_0578:
        popal  
        ret    
V86MMGR_Get_Mapping_Info:
        mov    ch, byte ptr [0xebc]          ; -> obj1:0x0EBC
        mov    cl, byte ptr [0xec0]          ; -> obj1:0x0EC0
        ret    
        db  000h
V86MMGR_Get_Version:
        mov    eax, 0x30a
        clc    
        ret    
V86MMGR_Control:
        cmp    eax, 0x17
        jae    loc_1_059B
        jmp    dword ptr [eax*4 + 0x59d]     ; -> obj1:0x059D
loc_1_059B:
        clc    
        ret    
        ; ---- dispatch table (23 entries) ----
        dd  loc_4_1A5C
        dd  sub_4_1AEA
        dd  loc_4_1E4A
        dd  sub_9_2EF0
        dd  loc_8_0000
        dd  loc_8_0092
        dd  loc_9_2F16
        dd  loc_11_0618
        dd  loc_1_059B
        dd  loc_11_0475
        dd  loc_1_059B
        dd  loc_12_00F9
        dd  loc_12_00F9
        dd  loc_13_0070
        dd  loc_14_00E4
        dd  loc_1_059B
        dd  loc_1_059B
        dd  loc_1_059B
        dd  loc_1_059B
        dd  loc_1_059B
        dd  loc_1_059B
        dd  loc_1_059B
        dd  loc_12_00EC
sub_1_05F9:
        push   ecx
        push   edx
        mov    ecx, eax
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        pop    edx
        pop    ecx
        ret    
sub_1_060C:
        push   esi
        push   edi
        push   edx
        push   ebx
        push   eax
        add    ecx, 3
        cmp    ecx, dword ptr [0x1210]       ; -> obj1:0x1210
        jbe    loc_1_0622
        mov    ecx, dword ptr [0x1210]       ; -> obj1:0x1210
loc_1_0622:
        or     eax, eax
        je     loc_1_0642
        cmp    ecx, dword ptr [0x1210]       ; -> obj1:0x1210
        jb     loc_1_0642
        push   ecx
        VxDCall  EBIOS.EBIOS_Get_Version         ; cd2000001200
        jb     loc_1_0641
        or     eax, eax
        je     loc_1_0641
        shl    ecx, 2
        sub    dword ptr [esp], ecx
loc_1_0641:
        pop    ecx
loc_1_0642:
        shr    ecx, 2
        sub    ecx, dword ptr [0x1208]       ; -> obj1:0x1208
        ja     loc_1_0652
        mov    ecx, 1
loc_1_0652:
        pop    eax
        pop    ebx
        pop    edx
        pop    edi
        pop    esi
        ret    
loc_1_0658:
        cmp    byte ptr [ebp + 0x1d], 0x43
        je     loc_1_0670
        mov    ecx, dword ptr [0x12a4]       ; -> obj1:0x12A4
        mov    edx, dword ptr [0x12a8]       ; -> obj1:0x12A8
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_1_0670:
        VMMCall  VMM.Simulate_Iret               ; cd2047800100
        add    byte ptr [eax], al
loc_1_0678:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        cmpsd  dword ptr [esi], dword ptr es:[edi]
        add    byte ptr [eax], al
        add    byte ptr [ebp - 0x4b000000], dh ; -> loc_1_00B5
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_068B:
        add    byte ptr [ecx], cl            ; -> loc_1_0109
        add    dword ptr [eax], eax
        add    byte ptr [esi], ah            ; -> loc_1_0126
        add    dword ptr [eax], eax
        add    byte ptr [ecx + eax], dh      ; -> loc_1_0134
        add    byte ptr [eax], al
        inc    esp
        add    dword ptr [eax], eax
        add    byte ptr [ecx + 0x20000001], ch ; -> loc_1_01A9
        add    dword ptr [eax], eax
        add    byte ptr [eax], al            ; -> loc_10_0000
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_9_0000
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x1000001], ch ; -> loc_1_01A9
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0AB7:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        ; ---- dispatch table (9 entries) ----
        dd  loc_9_2C28
        dd  loc_9_2C7D
        dd  loc_9_2D8B
        dd  loc_9_2D9B
        dd  loc_9_2E29
        dd  loc_9_2C23
        dd  loc_9_2C23
        dd  loc_9_2C23
        dd  loc_9_2C23
        db  000h, 004h, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        dd  V86MMGR_Get_Version      ; fixup
        dd  V86MMGR_Allocate_V86_Pages ; fixup
        dd  V86MMGR_Set_EMS_XMS_Limits ; fixup
        dd  V86MMGR_Get_EMS_XMS_Limits ; fixup
        dd  V86MMGR_Set_Mapping_Info ; fixup
        dd  V86MMGR_Get_Mapping_Info ; fixup
        dd  V86MMGR_Xlat_API         ; fixup
        dd  V86MMGR_Load_Client_Ptr  ; fixup
        dd  V86MMGR_Allocate_Buffer  ; fixup
        dd  V86MMGR_Free_Buffer      ; fixup
        dd  V86MMGR_Get_Xlat_Buff_State ; fixup
        dd  V86MMGR_Set_Xlat_Buff_State ; fixup
        dd  V86MMGR_Get_VM_Flat_Sel  ; fixup
        dd  V86MMGR_Map_Pages        ; fixup
        dd  V86MMGR_Free_Page_Map_Region ; fixup
        dd  V86MMGR_LocalGlobalReg   ; fixup
        dd  V86MMGR_GetPgStatus      ; fixup
        dd  V86MMGR_SetLocalA20      ; fixup
        dd  V86MMGR_ResetBasePages   ; fixup
        dd  V86MMGR_SetAvailMapPgs   ; fixup
        dd  V86MMGR_NoUMBInitCalls   ; fixup
        dd  V86MMGR_Get_EMS_XMS_Avail ; fixup
        dd  V86MMGR_Toggle_HMA       ; fixup
        dd  V86MMGR_Dev_Init         ; fixup
        dd  V86MMGR_Alloc_UM_Page    ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00006h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'V86MMGR '                  ; DDB_Name
        dd  0A0000000h                  ; DDB_Init_Order
        dd  V86MMGR_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  V86MMGR_PM_API           ; DDB_PM_API_Proc
        dd  obj1:0x1148              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_1218:
        jb     0x121a
        db  000h, 080h
loc_1_121C:
        cmp    al, 0x30
        add    byte ptr [eax - 0x7fffcfbf], al ; -> obj9:0x80003041
loc_1_1224:
        jp     0x1227
        add    byte ptr [eax - 0x7ffffe42], al ; -> obj15:0x800001BE
        cmc    
        add    dword ptr [eax], eax
        sbb    byte ptr [edx + eax], 0x80    ; -> obj15:0x8000025C
        ret    3
        db  080h
        dd  sub_15_0568              ; fixup
        dd  loc_15_0630              ; fixup
        dd  obj15:0x800006C7         ; fixup
        dd  obj15:0x80000886         ; fixup
        dd  obj15:0x80000960         ; fixup
        dd  obj15:0x8000098B         ; fixup
        dd  loc_15_0A12              ; fixup
        dd  loc_15_0B06              ; fixup
        dd  obj9:0x8000310C          ; fixup
        dd  obj9:0x800031C1          ; fixup
        dd  obj9:0x80003218          ; fixup
loc_1_1264:
        lds    eax, ptr [edx]
        add    byte ptr [eax - 0x7fffcf5f], al ; -> obj9:0x800030A1
        sub    al, 3
        add    byte ptr [eax - 0x7ffffcba], al ; -> obj15:0x80000346
        pop    ebx
        add    eax, dword ptr [eax]
        add    byte ptr [edi + 0x800003], 0  ; -> obj15:0x80000387
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1328:
        insb   byte ptr es:[edi], dx
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
        db  0FFh, 0FFh, 0FFh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x2f0  R-X
        db  005h, 000h, 000h, 000h
loc_2_0004:
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        pushal 
        add    byte ptr [eax - 0x6bfe6a00], bl
        add    bl, al
        add    al, byte ptr [ebp + 0x52]
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb  dx, byte ptr [esi]
        jae    loc_2_0098
        jo     loc_2_0095
        outsd  dx, dword ptr [esi]
        jb     0x9c
        and    byte ptr fs:[ebp + 0x78], ah
        jo     0x90
        outsb  dx, byte ptr [esi]
        sub    eax, 0x6f6d656d
        jb     loc_2_00B3
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0xa5
        jb     0x62
        popal  
        insb   byte ptr es:[edi], dx
        jb     loc_2_00AB
        popal  
        jns    loc_2_006A
        imul   ebp, dword ptr [esi + 0x73], 0x6c6c6174
        and    byte ptr cs:[edx + 0x65], dl
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jbe    0xc0
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0xca
        jb     0x87
        jb     loc_2_00D9
loc_2_006A:
        insd   dword ptr es:[edi], dx
        and    byte ptr [ebx + 0x4f], al
        dec    esi
        inc    esi
        dec    ecx
        inc    edi
        push   ebx
        pop    ecx
        push   ebx
        add    byte ptr cs:[ebp + 0x52], al
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        popal  
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_2_00A6
        imul   ebp, dword ptr [esi + 0x73], 0x6c6c6174
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ebx], dh
        cmp    byte ptr [esi], dh
loc_2_0095:
        and    byte ptr [ebp + 0x6e], ah
loc_2_0098:
        push   0x65636e61
        and    byte ptr fs:[ebp + 0x6f], ch
        and    byte ptr gs:[eax + 0x4d], bl
loc_2_00A6:
        push   ebx
        and    byte ptr [edx + esi*2 + 0x69], ah
loc_2_00AB:
        jbe    0x112
        jb     loc_2_00DD
        add    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
loc_2_00B3:
        outsd  dx, dword ptr fs:[esi]
        ja     0x12a
        and    byte ptr [ebx + 0x61], ah
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_2_00DF
        jae    loc_2_0126
        je     loc_2_00E3
        jne    0x135
        and    byte ptr [ecx + 0x6e], ah
        and    byte ptr [ebp + 0x70], dh
        jo     loc_2_0132
        jb     loc_2_00EF
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x14e
        and    byte ptr [edx + 0x6c], ah
        outsd  dx, dword ptr [esi]
loc_2_00D9:
        arpl   word ptr [ebx + 0x20], bp
        popal  
loc_2_00DD:
        je     0xff
loc_2_00DF:
        jae    loc_2_0146
        insd   dword ptr es:[di], dx
loc_2_00E3:
        outsb  dx, byte ptr gs:[esi]
        je     loc_2_0107
        and    eax, 0xa0d2e78
        inc    ebp
        js     0x152
loc_2_00EF:
        insb   byte ptr es:[edi], dx
        jne    loc_2_0156
        and    byte ptr gs:[eax + ebp*2 + 0x69], dh
        jae    loc_2_0119
        popal  
        jb     0x163
        jae    loc_2_0173
        and    byte ptr [ebx + 0x70], dh
        popal  
        arpl   word ptr [ebp + 0x20], sp
loc_2_0107:
        bound  edi, qword ptr [ecx + 0x20]
        jne    loc_2_017F
        imul   ebp, dword ptr [esi + 0x67], 0x65687420
        and    byte ptr [ebx + 0x79], dh
        outsb  dx, byte ptr [esi]
        je     0x17a
loc_2_0119:
        js     0x13b
        outsd  dx, dword ptr [esi]
        and    byte ptr [ecx + 0x6f], bh
        jne    0x194
        and    byte ptr [ebp + 0x65], ch
        insd   dword ptr es:[edi], dx
loc_2_0126:
        outsd  dx, dword ptr [esi]
        jb     loc_2_01A2
        and    byte ptr [ebp + 0x61], ch
        outsb  dx, byte ptr [esi]
        popal  
        jb     0x160
loc_2_0132:
        or     eax, 0x726f460a
        and    byte ptr [ebp + 0x6f], ch
        jb     0x1a1
        and    byte ptr [ecx + 0x6e], ch
        outsw  dx, word ptr [esi]
        jb     loc_2_01B0
        popal  
        je     0x1af
loc_2_0146:
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x65], dh
        and    byte ptr gs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [edx + 0x45], dl
        inc    ecx
        inc    esp
        dec    ebp
loc_2_0156:
        inc    ebp
        push   edi
        push   edx
        dec    ecx
        and    byte ptr [esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr cs:[ecx + edi*2 + 0x70], dl
        and    byte ptr gs:[edi + 0x49], dl
        dec    esi
        and    byte ptr [edi], ch
        push   ebx
        and    byte ptr [edi + ebp*2 + 0x20], dh
        jae    0x1e7
loc_2_0173:
        popal  
        jb     loc_2_01EA
        or     eax, 0x6e69570a
        outsd  dx, dword ptr fs:[esi]
        ja     0x1f2
loc_2_017F:
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [ebx + 0x74], dh
        popal  
        outsb  dx, byte ptr [esi]
        popal  
        jb     0x1ef
        and    byte ptr [ebp + 0x6f], ch
        and    byte ptr gs:[ecx + 0x6e], ah
        and    byte ptr fs:[ebx + 0x68], ah
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        jae    0x200
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [edx + 0x65], dl
loc_2_01A2:
        popal  
        and    byte ptr fs:[ebp + 0x65], cl
        and    byte ptr [ecx + 0x63], ch
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr cs:[edi + 0x69], dl
loc_2_01B0:
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0x228
        and    byte ptr [ebx + 0x61], ah
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_2_01DD
        jae    loc_2_0224
        je     loc_2_01E1
        jne    0x233
        and    byte ptr [ecx + 0x6e], ah
        and    byte ptr [ebp + 0x70], dh
        jo     loc_2_0230
        jb     loc_2_01ED
        insd   dword ptr es:[edi], dx
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0x24c
        and    byte ptr [edx + 0x6c], ah
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x20], bp
        popal  
        je     0x1fd
loc_2_01DD:
        jae    loc_2_0244
        insd   dword ptr es:[di], dx
loc_2_01E1:
        outsb  dx, byte ptr gs:[esi]
        je     loc_2_0205
        and    eax, 0xa0d2e78
loc_2_01EA:
        inc    ebp
        js     0x250
loc_2_01ED:
        insb   byte ptr es:[edi], dx
        jne    loc_2_0254
        and    byte ptr gs:[eax + ebp*2 + 0x69], dh
        jae    loc_2_0217
        popal  
        jb     0x261
        jae    loc_2_0271
        and    byte ptr [ebx + 0x70], dh
        popal  
        arpl   word ptr [ebp + 0x20], sp
loc_2_0205:
        bound  edi, qword ptr [ecx + 0x20]
        jne    loc_2_027D
        imul   ebp, dword ptr [esi + 0x67], 0x65687420
        and    byte ptr [ebx + 0x79], dh
        outsb  dx, byte ptr [esi]
        je     0x278
loc_2_0217:
        js     0x239
        outsd  dx, dword ptr [esi]
        and    byte ptr [ecx + 0x6f], bh
        jne    0x292
        and    byte ptr [ebp + 0x65], ch
        insd   dword ptr es:[edi], dx
loc_2_0224:
        outsd  dx, dword ptr [esi]
        jb     loc_2_02A0
        and    byte ptr [ebp + 0x61], ch
        outsb  dx, byte ptr [esi]
        popal  
        jb     0x25e
loc_2_0230:
        or     eax, 0x726f460a
        and    byte ptr [ebp + 0x6f], ch
        jb     0x29f
        and    byte ptr [ecx + 0x6e], ch
        outsw  dx, word ptr [esi]
        jb     loc_2_02AE
        popal  
        je     0x2ad
loc_2_0244:
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x65], dh
        and    byte ptr gs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [edx + 0x45], dl
        inc    ecx
        inc    esp
        dec    ebp
loc_2_0254:
        inc    ebp
        push   edi
        push   edx
        dec    ecx
        and    byte ptr [esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr cs:[ecx + edi*2 + 0x70], dl
        and    byte ptr gs:[edi + 0x49], dl
        dec    esi
        and    byte ptr [edi], ch
        push   ebx
        and    byte ptr [edi + ebp*2 + 0x20], dh
        jae    loc_2_02E5
loc_2_0271:
        popal  
        jb     0x2e8
        or     eax, 0x6e69570a
        outsd  dx, dword ptr fs:[esi]
        ja     0x2f0
loc_2_027D:
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [ebx + 0x74], dh
        popal  
        outsb  dx, byte ptr [esi]
        popal  
        jb     loc_2_02ED
        and    byte ptr [ebp + 0x6f], ch
        and    byte ptr gs:[ecx + 0x6e], ah
        and    byte ptr fs:[ebx + 0x68], ah
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        jae    0x2fe
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [edx + 0x65], dl
loc_2_02A0:
        popal  
        and    byte ptr fs:[ebp + 0x65], cl
        and    byte ptr [ecx + 0x63], ch
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr cs:[ebp + 0x52], al
loc_2_02AE:
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        popal  
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_2_02DA
        imul   ebp, dword ptr [esi + 0x73], 0x6c6c6174
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ebx], dh
        cmp    byte ptr [esi], dh
        and    byte ptr [ebp + 0x6e], ah
        push   0x65636e61
        and    byte ptr fs:[ebp + 0x6f], ch
        and    byte ptr gs:[ebp + 0x78], ah
loc_2_02DA:
        jo     0x33d
        outsb  dx, byte ptr [esi]
        sub    eax, 0x6f6d656d
loc_2_02E5:
        jb     0x360
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0x352
loc_2_02ED:
        jb     0x31d
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x82  R-X
        db  001h, 000h, 000h, 000h
loc_3_0004:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_3_000C:
        add    byte ptr [eax], al
        jb     loc_3_0010
loc_3_0010:
        inc    ebp
        push   edx
        push   edx
        dec    edi
        push   edx
        cmp    ah, byte ptr [eax]
        inc    ebx
        popal  
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x3e
        jb     0x85
        je     0x97
        jb     0x92
        and    byte ptr [edi + ebp*2 + 0x20], dh
        dec    ebp
        push   ebx
        sub    eax, 0x20534f44
        jne    0x97
        and    byte ptr [edi + ebp*2 + 0x20], dh
        je     0xa0
        and    byte ptr gs:[ebx + 0x74], dh
        popal  
        je     0xa4
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ebp + 0x78], ah
        jo     0xac
        outsb  dx, byte ptr [esi]
        sub    eax, 0x6f6d656d
        jb     0xcf
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0xc1
        jb     0x8c
        and    byte ptr [ecx + 0x6f], bl
        jne    0x83
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        jb     0xd2
        jae    0xe3
        popal  
        jb     0xe6
        and    byte ptr [ecx + 0x6f], bh
        jne    0xe9
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        jo     0xf2
        je     0xe4
        jb     0xaf
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x32a4  R-X
        db  0BEh
        dd  loc_1_0678               ; fixup
        db  0CDh, 020h, 0EFh, 000h, 001h, 000h, 0E9h
        dd  loc_11_0000              ; fixup
sub_4_0010:
        enter  0x24, 0
        pushal 
        xor    eax, eax
        mov    dword ptr [ebp - 0x28], eax
        mov    dword ptr [ebp - 0x2c], eax
        cmp    dword ptr [8], 0              ; -> obj6:0x0008
        je     loc_4_016D
        cmp    dword ptr [0x98], 0           ; -> obj6:0x0098
        je     loc_4_0047
        movzx  edx, word ptr [0x2e8]         ; -> obj6:0x02E8
        or     edx, edx
        jne    loc_4_0159
        jmp    loc_4_016D
loc_4_0047:
        lea    esi, [ebp - 0x28]
        xor    eax, eax
        push   eax
        push   esi
        push   eax
        VMMCall  VMM._Get_Device_V86_Pages_Array ; cd2074000100
        add    esp, 0xc
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        mov    dword ptr [0], eax            ; -> obj6:0x0000
        cmp    eax, 0xdf
        jae    loc_4_0096
        mov    edx, 0xc0
        cmp    edx, eax
        jae    loc_4_0086
        mov    edx, eax
        mov    ecx, 0xe0
        sub    ecx, edx
        jbe    loc_4_0096
        cmp    ecx, 0x10
        jb     loc_4_0096
        jmp    loc_4_008B
loc_4_0086:
        mov    ecx, 0x20
loc_4_008B:
        call   sub_4_0170
        jae    loc_4_0160
loc_4_0096:
        mov    eax, dword ptr [0]            ; -> obj6:0x0000
        cmp    eax, 0xff
        jae    loc_4_00CD
        mov    edx, 0xe0
        cmp    edx, eax
        jae    loc_4_00BD
        mov    edx, eax
        mov    ecx, 0x100
        sub    ecx, edx
        jbe    loc_4_00CD
        cmp    ecx, 0x10
        jb     loc_4_00CD
        jmp    loc_4_00C2
loc_4_00BD:
        mov    ecx, 0x20
loc_4_00C2:
        call   sub_4_0170
        jae    loc_4_0160
loc_4_00CD:
        mov    eax, dword ptr [0]            ; -> obj6:0x0000
        cmp    eax, 0xbf
        jae    loc_4_0100
        mov    edx, 0xa0
        cmp    edx, eax
        jae    loc_4_00F4
        mov    edx, eax
        mov    ecx, 0xc0
        sub    ecx, edx
        jbe    loc_4_0100
        cmp    ecx, 0x10
        jb     loc_4_0100
        jmp    loc_4_00F9
loc_4_00F4:
        mov    ecx, 0x20
loc_4_00F9:
        call   sub_4_0170
        jae    loc_4_0160
loc_4_0100:
        mov    eax, dword ptr [0]            ; -> obj6:0x0000
        mov    edx, 0xa0
        cmp    edx, eax
        jae    loc_4_0120
        mov    edx, eax
        mov    ecx, 0x100
        sub    ecx, edx
        jbe    loc_4_012C
        cmp    ecx, 0x10
        jb     loc_4_012C
        jmp    loc_4_0125
loc_4_0120:
        mov    ecx, 0x60
loc_4_0125:
        call   sub_4_0170
        jae    loc_4_0160
loc_4_012C:
        cmp    dword ptr [0x10], 0           ; -> obj6:0x0010
        je     loc_4_016D
        mov    edx, dword ptr [0x10]         ; -> obj6:0x0010
        shr    edx, 8
        lea    edx, [edx*2 + 0xf28]          ; -> obj1:0x0F28
        mov    ecx, 0x10
loc_4_014A:
        or     byte ptr [edx + 1], 1
        add    edx, 2
        loop   loc_4_014A
        mov    edx, dword ptr [0x10]         ; -> obj6:0x0010
loc_4_0159:
        shr    edx, 8
        mov    dword ptr [ebp - 4], edx
        stc    
loc_4_0160:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x28], eax
        mov    dword ptr [ebp - 0x2c], 0x10
loc_4_016D:
        popal  
        leave  
        ret    
sub_4_0170:
        lea    edx, [edx*2 + 0xf28]          ; -> obj1:0x0F28
        xor    eax, eax
        mov    dword ptr [ebp - 4], eax
loc_4_017C:
        test   byte ptr [edx], 0x40
        jne    loc_4_01B2
        test   word ptr [edx], 0xeb6
        jne    loc_4_01C5
        push   eax
        push   edx
        sub    edx, 0xf28                    ; -> obj1:0x0F28
        shr    edx, 1
        push   edx
        mov    eax, edx
        shr    eax, 5
        and    edx, 0x1f
        bt     dword ptr [esi + eax*4], edx
        jb     loc_4_01BE
        pop    edx
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_01B0
        test   dl, 3
        jne    loc_4_01BA
        mov    dword ptr [ebp - 4], edx
loc_4_01B0:
        pop    edx
        pop    eax
loc_4_01B2:
        inc    eax
        cmp    eax, 0x10
        jae    loc_4_01D1
        jmp    loc_4_01CA
loc_4_01BA:
        pop    edx
        pop    eax
        jmp    loc_4_01C5
loc_4_01BE:
        pop    edx
        pop    edx
        or     byte ptr [edx + 1], 8
        pop    eax
loc_4_01C5:
        xor    eax, eax
        mov    dword ptr [ebp - 4], eax
loc_4_01CA:
        add    edx, 2
        loop   loc_4_017C
        stc    
        ret    
loc_4_01D1:
        clc    
        ret    
        db  000h
sub_4_01D4:
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        mov    dword ptr [0], eax            ; -> obj6:0x0000
        push   4
        push   0x50
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_4_09D2
        shr    eax, 4
        mov    dword ptr [0x3237], eax       ; -> obj4:0x3237
        cmp    dword ptr [0x11d8], 3         ; -> obj1:0x11D8
        cmc    
        jae    loc_4_07E6
        mov    esi, dword ptr [0x11d8]       ; -> obj1:0x11D8
        push   0
        push   0x218c
        push   esi
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        inc    eax
        je     loc_4_09D2
        dec    eax
        mov    dword ptr [0x9c], eax         ; -> obj6:0x009C
        mov    edx, esi
        shr    edx, 0xc
        mov    dword ptr [0x323c], edx       ; -> obj4:0x323C
        movzx  edx, word ptr [eax + 2]
        add    edx, esi
        dec    edx
        shr    edx, 0xc
        mov    dword ptr [0x3240], edx       ; -> obj4:0x3240
        test   byte ptr [eax], 3
        jne    loc_4_09D2
        lea    edx, [eax + 0xa]
        mov    esi, edx
        mov    ecx, 0x40
loc_4_025F:
        test   byte ptr [esi], 1
        je     loc_4_0284
        test   byte ptr [esi], 2
        je     loc_4_0284
        push   6
        sub    esi, edx
        mov    eax, esi
        idiv   byte ptr [esp]
        add    esp, 4
        movzx  esi, al
        shl    esi, 0xa
        mov    word ptr [0x2e8], si          ; -> obj6:0x02E8
        jmp    loc_4_0289
loc_4_0284:
        add    esi, 6
        loop   loc_4_025F
loc_4_0289:
        mov    esi, dword ptr [0x9c]         ; -> obj6:0x009C
        lea    esi, [esi + 0xa]
        mov    edi, 0xa8                     ; -> obj6:0x00A8
        mov    ecx, 0x40
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        push   eax
loc_4_02A3:
        push   ecx
        push   edi
        test   byte ptr [esi], 1
        je     loc_4_02B5
        test   byte ptr [esi], 2
        je     loc_4_02BE
        or     byte ptr [edi + 2], 0x10
        jmp    loc_4_02BE
loc_4_02B5:
        or     byte ptr [edi + 2], 1
        jmp    loc_4_038B
loc_4_02BE:
        mov    ecx, edi
        mov    edx, 0xa8                     ; -> obj6:0x00A8
        sub    ecx, edx
        shr    ecx, 1
        cmp    ecx, dword ptr [esp + 8]
        jae    loc_4_02E4
loc_4_02CF:
        or     byte ptr [edi + 2], 2
        cmp    ecx, dword ptr [0]            ; -> obj6:0x0000
        jb     loc_4_0363
        jmp    loc_4_0373
loc_4_02E4:
        mov    al, byte ptr [esi + 1]
        or     al, al
        je     loc_4_0312
loc_4_02EB:
        mov    byte ptr [edi], al
        mov    byte ptr [edi + 1], 4
        cmp    al, 0xff
        je     loc_4_0306
        movzx  eax, word ptr [esi + 2]
        shl    eax, 2
        mov    word ptr [edi + 4], ax
        or     byte ptr [edi + 2], 0x80
        jmp    loc_4_0326
loc_4_0306:
        or     byte ptr [edi + 2], 0x20
        mov    word ptr [edi + 4], 0
        jmp    loc_4_0326
loc_4_0312:
        cmp    ecx, dword ptr [0]            ; -> obj6:0x0000
        jb     loc_4_0326
        cmp    ecx, 0xa0
        jb     loc_4_02CF
        mov    al, 0xff
        jmp    loc_4_02EB
loc_4_0326:
        mov    edi, ecx
        cmp    ecx, dword ptr [0]            ; -> obj6:0x0000
        jb     loc_4_0363
        push   0
        push   0
        push   4
        push   edi
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        or     eax, eax
        je     loc_4_09D2
        lea    edi, [edi*2 + 0xf28]          ; -> obj1:0x0F28
        mov    ecx, 4
        mov    eax, 0x1140
loc_4_0359:
        or     word ptr [edi], ax
        add    edi, 2
        loop   loc_4_0359
        jmp    loc_4_0373
loc_4_0363:
        cmp    byte ptr [esi + 1], 0
        je     loc_4_0373
        cmp    byte ptr [esi + 1], 0xff
        jne    loc_4_09D2
loc_4_0373:
        movzx  eax, byte ptr [esi + 4]
        lea    edx, [eax + 0x2a8]            ; -> obj6:0x02A8
        pop    eax
        push   eax
        mov    ecx, 0xa8                     ; -> obj6:0x00A8
        sub    eax, ecx
        shr    eax, 3
        mov    byte ptr [edx], al
loc_4_038B:
        pop    edi
        pop    ecx
        add    edi, 8
        add    esi, 6
        dec    ecx
        jne    loc_4_02A3
        pop    eax
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        push   0x101
        push   0x692
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_4_09D2
        mov    dword ptr [esi + ebx + 0x5c], eax
        mov    dword ptr [0x98], eax         ; -> obj6:0x0098
        lea    edi, [eax + 0x240]
        mov    esi, 0x2a8                    ; -> obj6:0x02A8
        mov    ecx, 0x10
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        lea    edi, [eax + 4]
        mov    esi, 0xa8                     ; -> obj6:0x00A8
        mov    ecx, 0x80
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, eax
        mov    ax, word ptr [0x2e8]          ; -> obj6:0x02E8
        mov    word ptr [edi + 0x68a], ax
        mov    eax, dword ptr [0x9c]         ; -> obj6:0x009C
        mov    eax, dword ptr [eax + 6]
        mov    dword ptr [edi], eax
        lea    edx, [edi + 4]
        mov    ecx, 0x40
        xor    eax, eax
loc_4_040A:
        test   byte ptr [edx + 2], 1
        jne    loc_4_0411
        inc    eax
loc_4_0411:
        add    edx, 8
        loop   loc_4_040A
        mov    byte ptr [edi + 0x68f], al
        or     eax, eax
        jne    loc_4_0427
        or     byte ptr [0x1204], 4          ; -> obj1:0x1204
loc_4_0427:
        mov    esi, eax
        shl    eax, 1
        add    eax, esi
        add    eax, 4
        mov    word ptr [edi + 0x688], ax
        test   byte ptr [0x1204], 4          ; -> obj1:0x1204
        jne    loc_4_045A
        mov    esi, dword ptr [0x9c]         ; -> obj6:0x009C
        or     ah, ah
        jne    loc_4_09D2
        cmp    al, byte ptr [esi + 0x18a]
        ja     loc_4_09D2
loc_4_045A:
        push   1
        push   0x36
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_4_09D2
        mov    dword ptr [edi + 0x280], eax
        mov    dword ptr [eax], 0xffffffff
        mov    dword ptr [eax + 4], 0xffffffff
        mov    byte ptr [eax + 0x2c], 0x44
        mov    esi, dword ptr [0x9c]         ; -> obj6:0x009C
        lea    esi, [esi + 0x18b]
        cld    
        lodsb  al, byte ptr [esi]
        movzx  eax, al
        mov    dword ptr [0x1284], eax       ; -> obj1:0x1284
        mov    ecx, 0x10
        mul    ecx
        add    esi, eax
        cld    
        lodsb  al, byte ptr [esi]
        movzx  ecx, al
        or     ecx, ecx
        je     loc_4_0620
loc_4_04B1:
        push   ecx
        cmp    byte ptr [esi], 0
        je     loc_4_0615
        test   byte ptr [esi + 1], 4
        jne    loc_4_0615
        cmp    byte ptr [esi], 0xff
        jae    loc_4_09D2
        movzx  ecx, word ptr [esi + 0xa]
        xor    eax, eax
        jecxz  loc_4_0501
        shl    ecx, 2
        cmp    ecx, 0x10000
        ja     loc_4_09D2
        push   ecx
        shl    ecx, 2
        mov    eax, dword ptr [esi + 0xc]
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        pop    ecx
        inc    eax
        je     loc_4_09D2
loc_4_0501:
        dec    eax
        mov    edx, eax
        push   edx
        push   ecx
        push   1
        push   0x36
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        pop    ecx
        pop    edx
        or     eax, eax
        je     loc_4_09D2
        push   edx
        movzx  edx, byte ptr [esi]
        mov    dword ptr [edi + edx*4 + 0x280], eax
        mov    byte ptr [eax + 0x2d], dl
        pop    edx
        mov    dword ptr [eax + 0x28], ecx
        or     byte ptr [eax + 0x2c], 0x80
        or     ecx, ecx
        jne    loc_4_053A
        or     byte ptr [eax + 0x2c], 4
loc_4_053A:
        test   byte ptr [esi + 1], 1
        je     loc_4_0558
        push   ecx
        push   esi
        push   edi
        or     byte ptr [eax + 0x2c], 1
        lea    edi, [eax + 0x2e]
        lea    esi, [esi + 2]
        mov    ecx, 8
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        pop    ecx
loc_4_0558:
        or     ecx, ecx
        je     loc_4_0615
        push   edi
        push   esi
        push   edx
        push   eax
        push   ecx
        mov    esi, edx
        xor    edx, edx
        xor    edi, edi
        dec    edi
loc_4_056C:
        cld    
        lodsd  eax, dword ptr [esi]
        test   al, 1
        je     loc_4_09D2
        shr    eax, 0xc
        cmp    eax, edi
        je     loc_4_057E
        inc    edx
loc_4_057E:
        mov    edi, eax
        inc    edi
        loop   loc_4_056C
        push   edx
        shl    edx, 2
        add    edx, 2
        push   0x101
        push   edx
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        pop    edx
        or     eax, eax
        je     loc_4_09D2
        pop    ecx
        pop    edi
        mov    dword ptr [edi], eax
        pop    esi
        mov    edi, eax
        mov    word ptr [edi], dx
        lea    edi, [edi + 2]
        mov    edx, dword ptr [esi]
        shr    edx, 0xc
        mov    word ptr [edi + 2], dx
loc_4_05B7:
        cld    
        lodsd  eax, dword ptr [esi]
        shr    eax, 0xc
        cmp    eax, edx
        je     loc_4_05EC
        pushal 
        movzx  eax, word ptr [edi + 2]
        shl    eax, 0xc
        movzx  ecx, word ptr [edi]
        shl    ecx, 0xc
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        inc    eax
        popal  
        je     loc_4_09D2
        add    edi, 4
        mov    word ptr [edi + 2], ax
        mov    edx, eax
loc_4_05EC:
        inc    word ptr [edi]
        inc    edx
        loop   loc_4_05B7
        movzx  eax, word ptr [edi + 2]
        shl    eax, 0xc
        movzx  ecx, word ptr [edi]
        shl    ecx, 0xc
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        inc    eax
        je     loc_4_09D2
        pop    esi
        pop    edi
loc_4_0615:
        pop    ecx
        add    esi, 0x10
        dec    ecx
        jne    loc_4_04B1
loc_4_0620:
        mov    eax, dword ptr [0x9c]         ; -> obj6:0x009C
        cmp    word ptr [eax + 4], 0x10a
        jb     loc_4_07BC
        cld    
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [0x94], eax         ; -> obj6:0x0094
        cld    
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [0x3294], eax       ; -> obj4:0x3294
        cld    
        lodsb  al, byte ptr [esi]
        movzx  ecx, al
        jecxz  loc_4_066B
loc_4_0646:
        mov    edx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_4_0666
        cmp    edx, 0xa0
        jb     loc_4_0666
        push   ecx
        push   0
        push   eax
        push   edx
        VMMCall  VMM._AddFreePhysPage            ; cd20d2000100
        add    esp, 0xc
        pop    ecx
loc_4_0666:
        add    esi, 8
        loop   loc_4_0646
loc_4_066B:
        cld    
        lodsb  al, byte ptr [esi]
        movzx  eax, al
        mov    dword ptr [0x127c], eax       ; -> obj1:0x127C
        mov    ecx, esi
        sub    ecx, dword ptr [0x9c]         ; -> obj6:0x009C
        mov    dword ptr [0x1280], ecx       ; -> obj1:0x1280
        mov    ecx, 0xc
        mul    ecx
        add    esi, eax
        cld    
        lodsb  al, byte ptr [esi]
        movzx  eax, al
        mov    dword ptr [0x1288], eax       ; -> obj1:0x1288
        mov    ecx, esi
        sub    ecx, dword ptr [0x9c]         ; -> obj6:0x009C
        mov    dword ptr [0x128c], ecx       ; -> obj1:0x128C
        mov    ecx, 4
        mul    ecx
        add    esi, eax
        mov    eax, dword ptr [0x9c]         ; -> obj6:0x009C
        cmp    word ptr [eax + 4], 0x10b
        jb     loc_4_07BC
        push   esi
        push   edi
        mov    edi, 0x3244                   ; -> obj4:0x3244
        mov    ecx, 0x1b
        cld    
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_06D6
        or     byte ptr [0x1205], 4          ; -> obj1:0x1205
loc_4_06D6:
        pop    edi
        pop    esi
        mov    ecx, esi
        sub    ecx, dword ptr [0x9c]         ; -> obj6:0x009C
        mov    dword ptr [0x1290], ecx       ; -> obj1:0x1290
        lea    esi, [esi + 0x14]
        mov    ecx, esi
        sub    ecx, dword ptr [0x9c]         ; -> obj6:0x009C
        mov    dword ptr [0x1294], ecx       ; -> obj1:0x1294
        lea    esi, [esi + 0x14]
        cmp    word ptr [eax + 4], 0x114
        jb     loc_4_07BC
        lodsd  eax, dword ptr [esi]
        mov    edx, eax
        lodsd  eax, dword ptr [esi]
        or     eax, eax
        je     loc_4_071D
        push   eax
        push   0
        push   eax
        push   edx
        VMMCall  VMM._AddFreePhysPage            ; cd20d2000100
        add    esp, 0xc
        pop    eax
loc_4_071D:
        lodsb  al, byte ptr [esi]
        movzx  ecx, al
        test   ecx, ecx
        je     loc_4_07BC
loc_4_0729:
        push   ecx
        lodsd  eax, dword ptr [esi]
        xchg   edx, eax
        lodsd  eax, dword ptr [esi]
        mov    ecx, edx
        neg    ecx
        and    ecx, 0xfff
        add    edx, 0xfff
        and    edx, 0xfffff000
        sub    eax, ecx
        jb     loc_4_07B4
        shr    eax, 0xc
        je     loc_4_07B4
        shr    edx, 0xc
        push   eax
        push   edx
        cmp    edx, dword ptr [0x3240]       ; -> obj4:0x3240
        ja     loc_4_07A3
        lea    ecx, [eax + edx]
        dec    ecx
        cmp    ecx, dword ptr [0x323c]       ; -> obj4:0x323C
        jb     loc_4_07A3
        mov    edx, esi
        mov    esi, dword ptr [0x9c]         ; -> obj6:0x009C
        sub    edx, esi
        movzx  ecx, word ptr [esi + 2]
        push   ecx
        push   edx
        push   0
        push   ecx
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        pop    edx
        pop    ecx
        test   eax, eax
        je     loc_4_079C
        push   edi
        mov    edi, eax
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        lea    esi, [edx + eax]
        xchg   dword ptr [0x9c], eax         ; -> obj6:0x009C
        mov    dword ptr [0xa0], eax         ; -> obj6:0x00A0
        jmp    loc_4_07A3
loc_4_079C:
        add    esi, edx
        add    esp, 8
        jmp    loc_4_07B4
loc_4_07A3:
        pop    edx
        pop    eax
        push   0
        push   1
        push   eax
        push   edx
        VMMCall  VMM._AddFreePhysPage            ; cd20d2000100
        add    esp, 0x10
loc_4_07B4:
        pop    ecx
        dec    ecx
        jne    loc_4_0729
loc_4_07BC:
        push   edi
        call   sub_4_07E7
        pop    edi
        jb     loc_4_09D2
        test   byte ptr [0x1204], 4          ; -> obj1:0x1204
        clc    
        jne    loc_4_07E6
        lea    eax, [edi + 4]
        mov    ecx, 0x80000001
        call   sub_11_0074
        jb     loc_4_09D2
loc_4_07E6:
        ret    
sub_4_07E7:
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        mov    dword ptr [0], eax            ; -> obj6:0x0000
        VMMCall  VMM._GetNulPageHandle           ; cd206a000100
        mov    dword ptr [0xef2], eax        ; -> obj1:0x0EF2
        push   0
        push   eax
        VMMCall  VMM._PageGetSizeAddr            ; cd2058000100
        pop    ecx
        pop    ecx
        mov    dword ptr [0xef6], edx        ; -> obj1:0x0EF6
        mov    dword ptr [0xf1a], 0xffffffff ; -> obj1:0x0F1A
        mov    byte ptr [0xf1e], 0x20        ; -> obj1:0x0F1E
        mov    byte ptr [0xf1f], 0xff        ; -> obj1:0x0F1F
        cmp    dword ptr [0x11d8], 2         ; -> obj1:0x11D8
        jne    loc_4_0858
        mov    edi, 0x304c                   ; -> loc_4_304C
        xor    esi, esi
        xor    eax, eax
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        cmp    al, 0xff
        je     loc_4_0858
        movzx  esi, word ptr [0xc]           ; -> loc_2_000C
        add    esi, dword ptr [4]            ; -> loc_2_0004
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
loc_4_0858:
        xor    eax, eax
        test   byte ptr [0x1204], 4          ; -> obj1:0x1204
        jne    loc_4_0890
        mov    edi, 0x306b                   ; -> obj4:0x306B
        xor    esi, esi
        mov    eax, 0xffff
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        movzx  eax, ax
        add    eax, 3
        shr    eax, 2
        cmp    dword ptr [0x11d8], 2         ; -> obj1:0x11D8
        jbe    loc_4_0890
        or     eax, eax
        jne    loc_4_0890
        mov    eax, 0x10
loc_4_0890:
        mov    dword ptr [8], eax            ; -> obj6:0x0008
        cmp    dword ptr [0x11d8], 2         ; -> obj1:0x11D8
        ja     loc_4_08CE
        mov    edi, 0x3073                   ; -> obj4:0x3073
        xor    esi, esi
        xor    eax, eax
        VMMCall  VMM.Get_Profile_Hex_Int         ; cd20af000100
        or     eax, eax
        je     loc_4_08CE
        add    eax, 0x3ff
        and    eax, 0xfffffc00
        cmp    eax, 0xa000
        jb     loc_4_08CE
        cmp    eax, 0xf000
        ja     loc_4_08CE
        mov    dword ptr [0x10], eax         ; -> obj6:0x0010
loc_4_08CE:
        mov    edi, 0x305f                   ; -> obj4:0x305F
        xor    esi, esi
        movzx  eax, byte ptr [0x323b]        ; -> loc_4_323B
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        mov    byte ptr [0x323b], al         ; -> loc_4_323B
        cmp    al, 0xff
        jne    loc_4_08F2
        or     byte ptr [0x1205], 8          ; -> obj1:0x1205
loc_4_08F2:
        mov    edi, dword ptr [0x3237]       ; -> obj4:0x3237
        shl    edi, 4
        push   edi
        add    edi, dword ptr [ebx + 4]
        mov    esi, 0x31ec                   ; -> obj4:0x31EC
        mov    ecx, 0x50
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        cmp    byte ptr [0x323b], 0xff       ; -> loc_4_323B
        je     loc_4_09D0
        cmp    dword ptr [0x1284], 0         ; -> obj1:0x1284
        jne    loc_4_0931
        push   ebx
        xor    ebx, ebx
        mov    eax, edi
        VxDCall  DOSMGR.DOSMGR_Add_Device        ; cd2005001500
        pop    ebx
        jb     loc_4_0999
loc_4_0931:
        mov    eax, dword ptr [0x3237]       ; -> obj4:0x3237
        shl    eax, 0x10
        mov    ax, word ptr [0x31f2]         ; -> loc_4_31F2
        mov    esi, 0x12cb                   ; -> loc_9_12CB
        xor    edx, edx
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        jb     loc_4_0999
        mov    ax, word ptr [0x31f4]         ; -> obj4:0x31F4
        mov    esi, 0x12e7                   ; -> loc_9_12E7
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        jb     loc_4_0999
        mov    ax, word ptr [0x31fe]         ; -> obj4:0x31FE
        mov    esi, 0x12fc                   ; -> loc_9_12FC
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        jb     loc_4_0999
        mov    ax, 0x3d
        mov    dword ptr [0x14], eax         ; -> obj6:0x0014
        mov    esi, 0x2793                   ; -> loc_9_2793
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        jb     loc_4_0999
        mov    ax, 0x41
        mov    esi, 0x1321                   ; -> loc_9_1321
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
loc_4_0999:
        jb     loc_4_09D2
        mov    eax, 0x67
        mov    esi, 0x1331                   ; -> loc_9_1331
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    edi, dword ptr [ebx + 4]
        mov    eax, dword ptr [edi + 0x19c]
        mov    dword ptr [4], eax            ; -> obj6:0x0004
        mov    eax, dword ptr [0x3237]       ; -> obj4:0x3237
        shl    eax, 0x10
        mov    ax, 0x40
        mov    dword ptr [0x18], eax         ; -> obj6:0x0018
        mov    dword ptr [edi + 0x19c], eax
loc_4_09D0:
        clc    
        ret    
loc_4_09D2:
        movzx  esi, word ptr [0x14]          ; -> obj2:0x0014
        add    esi, dword ptr [4]            ; -> loc_2_0004
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
sub_4_09E7:
        mov    eax, dword ptr [0x94]         ; -> obj6:0x0094
        or     eax, eax
        je     loc_4_09FF
        mov    esi, 0x1329                   ; -> loc_9_1329
        xor    edx, edx
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        jb     loc_4_09D2
loc_4_09FF:
        cmp    byte ptr [0x323b], 0xff       ; -> loc_4_323B
        je     loc_4_0A29
        cmp    dword ptr [0x1284], 0         ; -> obj1:0x1284
        je     loc_4_0A29
        mov    eax, dword ptr [0x3237]       ; -> obj4:0x3237
        shl    eax, 4
        or     eax, eax
        je     loc_4_0A29
        push   ebx
        xor    ebx, ebx
        VxDCall  DOSMGR.DOSMGR_Add_Device        ; cd2005001500
        pop    ebx
        jb     loc_4_09D2
loc_4_0A29:
        ret    
sub_4_0A2A:
        cmp    byte ptr [0x323b], 1          ; -> loc_4_323B
        jne    loc_4_0B0B
        cmp    dword ptr [0x98], 0           ; -> obj6:0x0098
        je     loc_4_0A52
        test   byte ptr [0x1204], 4          ; -> obj1:0x1204
        jne    loc_4_0AF0
        jmp    loc_4_0B0B
loc_4_0A52:
        cmp    dword ptr [0x10], 0           ; -> obj6:0x0010
        jne    loc_4_0B0B
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        add    eax, 3
        and    eax, 0xfffffffc
        cmp    eax, 0xa0
        jae    loc_4_0A78
        mov    eax, 0xa0
loc_4_0A78:
        mov    ecx, 0x100
        sub    ecx, eax
        jbe    loc_4_0AF0
        sub    esp, 0x24
        mov    esi, esp
        push   eax
        push   ecx
        xor    eax, eax
        push   eax
        push   esi
        push   eax
        VMMCall  VMM._Get_Device_V86_Pages_Array ; cd2074000100
        add    esp, 0xc
        pop    ecx
        pop    eax
        lea    eax, [eax*2 + 0xf28]          ; -> obj1:0x0F28
        xor    edx, edx
loc_4_0AA1:
        test   byte ptr [eax], 0x40
        jne    loc_4_0AC6
        test   word ptr [eax], 0xeb6
        jne    loc_4_0AE6
        push   eax
        push   edx
        sub    eax, 0xf28                    ; -> obj1:0x0F28
        shr    eax, 1
        mov    edx, eax
        shr    eax, 5
        and    edx, 0x1f
        bt     dword ptr [esi + eax*4], edx
        pop    edx
        pop    eax
        jb     loc_4_0AE6
loc_4_0AC6:
        or     edx, edx
        jne    loc_4_0ADB
        mov    edx, eax
        sub    edx, 0xf28                    ; -> obj1:0x0F28
        shr    edx, 1
        test   dl, 3
        jne    loc_4_0AE6
        xor    ebx, ebx
loc_4_0ADB:
        inc    ebx
        cmp    ebx, 0x10
        jb     loc_4_0AE8
        add    esp, 0x24
        jmp    loc_4_0B0B
loc_4_0AE6:
        xor    edx, edx
loc_4_0AE8:
        add    eax, 2
        loop   loc_4_0AA1
        add    esp, 0x24
loc_4_0AF0:
        mov    eax, dword ptr [0x3237]       ; -> obj4:0x3237
        or     eax, eax
        je     loc_4_0B0B
        shl    eax, 4
        add    eax, 0xa
        mov    byte ptr [eax], 0x26
        or     byte ptr [0x1205], 1          ; -> obj1:0x1205
loc_4_0B0B:
        ret    
sub_4_0B0C:
        cmp    dword ptr [0x11d8], 2         ; -> obj1:0x11D8
        ja     loc_4_0B1A
        call   sub_4_07E7
loc_4_0B1A:
        mov    eax, dword ptr [0x3237]       ; -> obj4:0x3237
        shl    eax, 0x10
        mov    ax, 0x45
        mov    dword ptr [0x1298], eax       ; -> obj1:0x1298
        mov    ax, 0x4a
        mov    esi, 0                        ; -> loc_15_0000
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        jb     loc_4_0B73
        shr    eax, 0x10
        shl    eax, 4
        add    eax, 0x45
        mov    dword ptr [0x31e0], eax       ; -> loc_4_31E0
        mov    dword ptr [0x31e4], 6         ; -> loc_4_31E4
        mov    dword ptr [0x31e8], 0x200     ; -> loc_4_31E8
        push   0
        push   0x31d8                        ; -> loc_4_31D8
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        pop    ecx
        pop    ecx
        or     eax, eax
        jne    loc_4_0B88
loc_4_0B73:
        movzx  esi, word ptr [0xe]           ; -> loc_2_000E
        add    esi, dword ptr [4]            ; -> loc_2_0004
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
loc_4_0B88:
        cmp    byte ptr [0x323b], 0xff       ; -> loc_4_323B
        je     loc_4_0BAF
        mov    esi, 0x13e4                   ; -> loc_9_13E4
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        mov    ecx, eax
        movzx  edx, cx
        shr    ecx, 0x10
        mov    eax, 0x67
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
loc_4_0BAF:
        clc    
        ret    
        db  000h, 000h, 000h
V86MMGR_Set_Mapping_Info:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_4_0BF9
        cmp    al, byte ptr [0x3260]         ; -> loc_4_3260
        jbe    loc_4_0BC9
        mov    byte ptr [0x3260], al         ; -> loc_4_3260
loc_4_0BC9:
        cmp    ah, byte ptr [0x3261]         ; -> obj4:0x3261
        jbe    loc_4_0BD7
        mov    byte ptr [0x3261], ah         ; -> obj4:0x3261
loc_4_0BD7:
        add    byte ptr [0x3262], bl         ; -> loc_4_3262
        cmp    bh, byte ptr [0x3263]         ; -> obj4:0x3263
        jbe    loc_4_0BEB
        mov    byte ptr [0x3263], bh         ; -> obj4:0x3263
loc_4_0BEB:
        cmp    cl, byte ptr [0x3264]         ; -> loc_4_3264
        jbe    loc_4_0BF9
        mov    byte ptr [0x3264], cl         ; -> loc_4_3264
loc_4_0BF9:
        ret    
sub_4_0BFA:
        pushal 
        movzx  esi, byte ptr [0x3260]        ; -> loc_4_3260
        movzx  eax, byte ptr [0x3263]        ; -> obj4:0x3263
        add    esi, eax
        mov    al, byte ptr [0x3262]         ; -> loc_4_3262
        add    esi, eax
        movzx  edi, byte ptr [0x3261]        ; -> obj4:0x3261
        movzx  eax, byte ptr [0x3264]        ; -> loc_4_3264
        add    edi, eax
        mov    al, byte ptr [0x3262]         ; -> loc_4_3262
        add    edi, eax
        cmp    edi, esi
        jae    loc_4_0C2F
        mov    edi, esi
loc_4_0C2F:
        mov    dword ptr [0x3265], edi       ; -> obj4:0x3265
        call   sub_4_0010
        jecxz  loc_4_0C42
        jae    loc_4_0C42
        xor    ecx, ecx
        xor    eax, eax
loc_4_0C42:
        push   eax
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        xor    edx, edx
loc_4_0C4C:
        test   word ptr [eax*2 + 0xf28], 0xf86 ; -> obj1:0x0F28
        jne    loc_4_0C59
        inc    edx
loc_4_0C59:
        inc    eax
        cmp    eax, 0xff
        jbe    loc_4_0C4C
        pop    eax
        sub    edx, ecx
        je     loc_4_0C79
        cmp    edi, edx
        jbe    loc_4_0D22
        cmp    esi, edx
        ja     loc_4_0CA5
        mov    edi, edx
        jmp    loc_4_0D22
loc_4_0C79:
        push   eax
        push   ecx
        push   0x808
        push   0
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        pop    ecx
        pop    ecx
        mov    edx, eax
        pop    ecx
        pop    eax
        shr    edx, 0xc
        xchg   edi, esi
        cmp    edx, esi
        jae    loc_4_0C9F
        xchg   edi, esi
        cmp    edx, esi
        jb     loc_4_0CA3
        mov    esi, edx
loc_4_0C9F:
        xor    edi, edi
        jmp    loc_4_0CDA
loc_4_0CA3:
        xor    edx, edx
loc_4_0CA5:
        cmp    ecx, 0x10
        jbe    loc_4_0CB9
        lea    edi, [ecx + edx - 0x10]
        cmp    edi, esi
        jb     loc_4_0CB9
        mov    ecx, 0x10
        jmp    loc_4_0D22
loc_4_0CB9:
        mov    edi, edx
        pushal 
        xor    eax, eax
        xor    esi, esi
        mov    edi, 0x3140                   ; -> loc_4_3140
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        test   eax, eax
        popal  
        jne    loc_4_0CDA
        add    edi, ecx
        xor    eax, eax
        xor    ecx, ecx
        cmp    edi, esi
        jae    loc_4_0D22
loc_4_0CDA:
        pushal 
        sub    esi, edi
        shl    esi, 0xc
        push   0x408
        push   esi
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_4_0E34
        shr    eax, 0xc
        mov    dword ptr [0xeb4], eax        ; -> obj1:0x0EB4
        shr    esi, 0xc
        mov    dword ptr [0xebc], esi        ; -> obj1:0x0EBC
loc_4_0D07:
        or     byte ptr [eax*2 + 0xf29], 0x14 ; -> obj1:0x0F29
        inc    eax
        dec    esi
        jne    loc_4_0D07
        dec    eax
        mov    dword ptr [0xeb8], eax        ; -> obj1:0x0EB8
        popal  
        test   edi, edi
        je     loc_4_0DAE
loc_4_0D22:
        push   eax
        push   ecx
        jecxz  loc_4_0D31
loc_4_0D26:
        or     byte ptr [eax*2 + 0xf29], 1   ; -> obj1:0x0F29
        inc    eax
        loop   loc_4_0D26
loc_4_0D31:
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
loc_4_0D38:
        test   word ptr [eax*2 + 0xf28], 0xf86 ; -> obj1:0x0F28
        jne    loc_4_0D8F
        test   byte ptr [eax*2 + 0xf29], 0x10 ; -> obj1:0x0F29
        jne    loc_4_0D64
        push   eax
        push   0
        push   0
        push   1
        push   eax
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        test   eax, eax
        pop    eax
        je     loc_4_0D8F
loc_4_0D64:
        or     byte ptr [eax*2 + 0xf29], 0x14 ; -> obj1:0x0F29
        inc    dword ptr [0xebc]             ; -> obj1:0x0EBC
        cmp    dword ptr [0xeb8], eax        ; -> obj1:0x0EB8
        jg     loc_4_0D7F
        mov    dword ptr [0xeb8], eax        ; -> obj1:0x0EB8
loc_4_0D7F:
        cmp    dword ptr [0xeb4], eax        ; -> obj1:0x0EB4
        jb     loc_4_0D8C
        mov    dword ptr [0xeb4], eax        ; -> obj1:0x0EB4
loc_4_0D8C:
        dec    edi
        je     loc_4_0D97
loc_4_0D8F:
        inc    eax
        cmp    eax, 0xff
        jbe    loc_4_0D38
loc_4_0D97:
        pop    ecx
        pop    eax
        or     edi, edi
        jne    loc_4_0C79
        jecxz  loc_4_0DAE
loc_4_0DA3:
        and    byte ptr [eax*2 + 0xf29], 0xfe ; -> obj1:0x0F29
        inc    eax
        loop   loc_4_0DA3
loc_4_0DAE:
        mov    ecx, dword ptr [0xebc]        ; -> obj1:0x0EBC
        mov    dword ptr [0xec0], ecx        ; -> obj1:0x0EC0
        movzx  ecx, byte ptr [0x3260]        ; -> loc_4_3260
        mov    edi, dword ptr [0x3265]       ; -> obj4:0x3265
        cmp    edi, dword ptr [0xebc]        ; -> obj1:0x0EBC
        ja     loc_4_0DD6
        movzx  ecx, byte ptr [0x3261]        ; -> obj4:0x3261
loc_4_0DD6:
        mov    byte ptr [0x68b], cl          ; -> loc_1_068B
        shl    ecx, 0xc
        xor    esi, esi
        VxDCall  V86MMGR.V86MMGR_Map_Pages       ; cd200d000600
        jae    loc_4_0E17
        movzx  ecx, byte ptr [0x3260]        ; -> loc_4_3260
        mov    byte ptr [0x68b], cl          ; -> loc_1_068B
        shl    ecx, 0xc
        xor    esi, esi
        VxDCall  V86MMGR.V86MMGR_Map_Pages       ; cd200d000600
        jae    loc_4_0E17
        push   0x408
        push   ecx
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_4_0E34
        mov    edi, eax
loc_4_0E17:
        shr    edi, 0xc
        mov    eax, edi
        mov    byte ptr [0x68a], al          ; -> obj1:0x068A
        call   sub_11_05D8
        mov    esi, 0x360                    ; -> sub_1_0360
        VMMCall  VMM.Call_When_Task_Switched     ; cd202a000100
        popal  
        clc    
        ret    
loc_4_0E34:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
        add    byte ptr [eax], al
sub_4_0E3C:
        enter  0x28, 0
        pushal 
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        movzx  eax, word ptr [0x3274]        ; -> loc_4_3274
        xor    esi, esi
        mov    edi, 0x31ae                   ; -> obj4:0x31AE
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        or     eax, eax
        je     loc_4_0EA6
        cmp    eax, 0x400
        jae    loc_4_0E7B
        mov    word ptr [0x3274], ax         ; -> loc_4_3274
loc_4_0E7B:
        push   0x80
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   1
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        test   eax, eax
        je     loc_4_0EA6
        mov    dword ptr [0x326c], eax       ; -> loc_4_326C
        mov    dword ptr [0x3270], edx       ; -> loc_4_3270
loc_4_0EA6:
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        mov    dword ptr [0], eax            ; -> obj6:0x0000
        xor    eax, eax
        mov    dword ptr [ebp - 4], eax
        push   ebx
        VMMCall  VMM.Get_Machine_Info            ; cd20bb000100
        mov    edx, ebx
        pop    ebx
        test   edx, 0x40000
        je     loc_4_0EE0
        or     byte ptr [ebp - 4], 1
        push   0x28a4                        ; -> loc_4_28A4
        push   0xf28                         ; -> obj1:0x0F28
        call   sub_4_1665
        pop    ecx
        pop    ecx
        jmp    loc_4_0F24
loc_4_0EE0:
        test   edx, 0x80000
        je     loc_4_0F24
        or     byte ptr [ebp - 4], 2
        push   0
        push   0x940
        VMMCall  VMM._Allocate_Temp_V86_Data_Area ; cd20a9000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_4_0F24
        shr    eax, 4
        mov    dword ptr [0x3276], eax       ; -> loc_4_3276
        mov    edx, dword ptr [ebx + 8]
        push   ebx
        push   edx
        push   eax
        shl    eax, 4
        push   eax
        push   0xf28                         ; -> obj1:0x0F28
        call   sub_4_174F
        add    esp, 0x14
        VMMCall  VMM._Free_Temp_V86_Data_Area    ; cd20aa000100
loc_4_0F24:
        lea    edx, [ebp - 0x28]
        push   0
        push   edx
        push   0
        VMMCall  VMM._Get_Device_V86_Pages_Array ; cd2074000100
        add    esp, 0xc
        or     eax, eax
        je     loc_4_11D2
        test   byte ptr [ebp - 4], 1
        jne    loc_4_0F76
        xor    eax, eax
        xor    esi, esi
        mov    edi, 0x31cf                   ; -> loc_4_31CF
        xor    edx, edx
        VMMCall  VMM.Get_Profile_String          ; cd20b3000100
        or     edx, edx
        je     loc_4_0F5E
        cmp    byte ptr [edx], 0
        je     loc_4_0F5E
        dec    eax
loc_4_0F5E:
        xor    esi, esi
        mov    edi, 0x31bf                   ; -> obj4:0x31BF
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        or     eax, eax
        je     loc_4_0F76
        push   ebx
        call   sub_4_1941
        pop    ebx
loc_4_0F76:
        push   ebx
        mov    edi, 0x3080                   ; -> obj4:0x3080
        mov    edx, 0xffffffbf
        mov    ebx, 0x80
        mov    eax, 0x4000
        call   sub_4_11D5
        mov    edi, 0x3096                   ; -> obj4:0x3096
        mov    edx, 0xffffffbf
        mov    ebx, 0x80
        mov    eax, 0x1000
        call   sub_4_11D5
        mov    edi, 0x308b                   ; -> obj4:0x308B
        mov    edx, 0xffffff7f
        mov    ebx, 0x40
        mov    eax, 0x4000
        call   sub_4_11D5
        mov    edi, 0x30a7                   ; -> obj4:0x30A7
        mov    edx, 0xffffff7f
        mov    ebx, 0x40
        mov    eax, 0x1000
        call   sub_4_11D5
        mov    al, 0x78
        VMMCall  VMM.Get_Debug_Options           ; cd20dc000100
        jne    loc_4_100F
        mov    ecx, 0x100
        mov    eax, dword ptr [0]            ; -> obj6:0x0000
        sub    ecx, eax
        jbe    loc_4_100F
        lea    eax, [eax*2 + 0xf28]          ; -> obj1:0x0F28
        mov    edx, 0xffffffbf
        mov    ebx, 0x80
loc_4_1004:
        and    word ptr [eax], dx
        or     word ptr [eax], bx
        add    eax, 2
        loop   loc_4_1004
loc_4_100F:
        mov    eax, dword ptr [0x9c]         ; -> obj6:0x009C
        or     eax, eax
        je     loc_4_108A
        cmp    word ptr [eax + 4], 0x10a
        jb     loc_4_108A
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        mov    ebx, edx
        and    edx, 0xfffffffc
        lea    ecx, [eax + 0xa]
        test   edx, 3
        stc    
        jne    loc_4_1045
        shr    edx, 2
        mov    eax, edx
        shl    eax, 1
        shl    edx, 2
        add    edx, eax
        add    edx, ecx
loc_4_1045:
        mov    ecx, 0x100
        sub    ecx, ebx
        jbe    loc_4_108A
        mov    esi, ebx
        lea    esi, [esi*2 + 0xf28]          ; -> obj1:0x0F28
loc_4_1057:
        push   ecx
        movzx  ecx, bl
        and    cl, 3
        mov    al, byte ptr [edx + 5]
        shl    ecx, 1
        shr    al, cl
        and    al, 3
        cmp    al, 0
        je     loc_4_107B
        cmp    al, 3
        je     loc_4_107B
        cmp    al, 1
        je     loc_4_1078
        or     byte ptr [esi], 0x80
        jmp    loc_4_107B
loc_4_1078:
        or     byte ptr [esi], 0x40
loc_4_107B:
        pop    ecx
        add    esi, 2
        inc    ebx
        test   bl, 3
        jne    loc_4_1088
        add    edx, 6
loc_4_1088:
        loop   loc_4_1057
loc_4_108A:
        call   sub_4_12B1
        pop    ebx
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        mov    ecx, eax
        xor    edx, edx
        lea    edx, [edx*2 + 0xf28]          ; -> obj1:0x0F28
loc_4_10A1:
        or     word ptr [edx], 0x804
        add    edx, 2
        loop   loc_4_10A1
        mov    ebx, 0xf0
        mov    ecx, 0x10
        test   byte ptr [ebp - 4], 1
        je     loc_4_10C5
        mov    ebx, 0xe0
        mov    ecx, 0x20
loc_4_10C5:
        mov    eax, dword ptr [0]            ; -> obj6:0x0000
        cmp    ebx, eax
        jae    loc_4_10DA
        sub    eax, ebx
        sub    ecx, eax
        jbe    loc_4_10F2
        mov    ebx, dword ptr [0]            ; -> obj6:0x0000
loc_4_10DA:
        mov    edi, ebx
        lea    edi, [edi*2 + 0xf28]          ; -> obj1:0x0F28
loc_4_10E3:
        test   word ptr [edi], 0x6740
        jne    loc_4_10ED
        or     byte ptr [edi], 2
loc_4_10ED:
        add    edi, 2
        loop   loc_4_10E3
loc_4_10F2:
        mov    ecx, 0x100
        sub    ecx, dword ptr [0]            ; -> obj6:0x0000
        jbe    loc_4_11A2
        mov    ebx, dword ptr [0]            ; -> obj6:0x0000
        mov    edx, ebx
        lea    ebx, [ebx*2 + 0xf28]          ; -> obj1:0x0F28
loc_4_1112:
        test   byte ptr [ebx], 0x80
        jne    loc_4_1166
        test   byte ptr [ebx + 1], 0x10
        jne    loc_4_1127
        bt     dword ptr [ebp - 0x28], edx
        jae    loc_4_1127
        or     byte ptr [ebx + 1], 8
loc_4_1127:
        test   byte ptr [ebx + 1], 7
        jne    loc_4_1132
        test   byte ptr [ebx], 6
        jne    loc_4_1166
loc_4_1132:
        cmp    edx, 0xa0
        jb     loc_4_1142
        cmp    edx, 0xc0
        jb     loc_4_1166
loc_4_1142:
        or     eax, 0xffffffff
        cmp    edx, 0xff
        ja     loc_4_1160
        test   word ptr [ebx], 0x67c0
        jne    loc_4_1161
        test   byte ptr [ebx + 1], 0x10
        jne    loc_4_1160
        bt     dword ptr [ebp - 0x28], edx
        jb     loc_4_1161
loc_4_1160:
        inc    eax
loc_4_1161:
        call   sub_4_13C8
loc_4_1166:
        inc    edx
        add    ebx, 2
        loop   loc_4_1112
        mov    ecx, dword ptr [0x3270]       ; -> loc_4_3270
        jecxz  loc_4_11A2
        mov    ecx, 0x100
        mov    ebx, dword ptr [0]            ; -> obj6:0x0000
        cmp    ebx, 0xc0
        jae    loc_4_118C
        mov    ebx, 0xc0
loc_4_118C:
        sub    ecx, ebx
        mov    edx, ebx
        lea    ebx, [ebx*2 + 0xf28]          ; -> obj1:0x0F28
loc_4_1197:
        call   sub_4_1513
        inc    edx
        add    ebx, 2
        loop   loc_4_1197
loc_4_11A2:
        clc    
loc_4_11A3:
        pushfd 
        xor    ecx, ecx
        xchg   dword ptr [0x326c], ecx       ; -> loc_4_326C
        jecxz  loc_4_11B9
        push   0
        push   ecx
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_4_11B9:
        popfd  
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        popal  
        leave  
        ret    
loc_4_11D2:
        stc    
        jmp    loc_4_11A3
sub_4_11D5:
        push   eax
        push   edx
        xor    esi, esi
        VMMCall  VMM.Get_Profile_String          ; cd20b3000100
        mov    esi, edx
        pop    edx
        jb     loc_4_11F2
        jmp    loc_4_11F6
loc_4_11E6:
        xchg   esi, edx
        VMMCall  VMM.Get_Next_Profile_String     ; cd20b4000100
        xchg   esi, edx
        jae    loc_4_11F6
loc_4_11F2:
        add    esp, 4
        ret    
loc_4_11F6:
        xchg   esi, edx
        VMMCall  VMM.Convert_Hex_String          ; cd20b0000100
        xchg   esi, edx
        or     eax, eax
        je     loc_4_11E6
        mov    ecx, eax
        cld    
        lodsb  al, byte ptr [esi]
        cmp    al, 0x2d
        jne    loc_4_11E6
        xchg   esi, edx
        VMMCall  VMM.Convert_Hex_String          ; cd20b0000100
        xchg   esi, edx
        or     eax, eax
        je     loc_4_11E6
        shl    eax, 4
        shl    ecx, 4
        push   ebx
        push   eax
        mov    ebx, dword ptr [esp + 8]
        dec    ebx
        not    ebx
        and    ecx, ebx
        not    ebx
        add    eax, ebx
        not    ebx
        and    eax, ebx
        dec    eax
        cmp    eax, dword ptr [esp]
        jae    loc_4_123D
        add    eax, dword ptr [esp + 8]
loc_4_123D:
        add    esp, 4
        pop    ebx
        push   edx
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        cmp    dword ptr [esp + 4], 0x1000
        je     loc_4_1258
        shr    edx, 2
        shl    edx, 2
loc_4_1258:
        cmp    ecx, 0xfffff
        jbe    loc_4_1265
        mov    ecx, 0xfffff
loc_4_1265:
        cmp    ecx, edx
        jae    loc_4_126B
        mov    ecx, edx
loc_4_126B:
        cmp    eax, 0xfffff
        jbe    loc_4_1277
        mov    eax, 0xfffff
loc_4_1277:
        cmp    eax, edx
        jae    loc_4_127D
        mov    eax, edx
loc_4_127D:
        pop    edx
        cmp    ecx, eax
        jae    loc_4_11E6
        xchg   ecx, eax
        shr    ecx, 0xc
        shr    eax, 0xc
        inc    ecx
        sub    ecx, eax
        push   ecx
        lea    eax, [eax*2 + 0xf28]          ; -> obj1:0x0F28
        pop    ecx
loc_4_1299:
        and    word ptr [eax], dx
        or     word ptr [eax], bx
        add    eax, 2
        loop   loc_4_1299
        jmp    loc_4_11E6
loc_4_12A9:
        push   esp
        dec    edi
        push   ebx
        dec    eax
        dec    ecx
        inc    edx
        inc    ecx
        db  020h
sub_4_12B1:
        mov    edi, 0xfe010
        mov    esi, 0x12a9                   ; -> loc_4_12A9
        mov    ecx, 8
        cld    
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_13A4
        cmp    byte ptr [0xfe024], 0x52
        jne    loc_4_1307
        mov    ecx, 0xe8
        mov    eax, 0xef
        bt     dword ptr [0xfe025], 2
        jae    loc_4_1302
        mov    cx, word ptr [0xfe02b]
        mov    ax, word ptr [0xfe02a]
        shl    eax, 6
        je     loc_4_1307
        lea    eax, [ecx + eax - 1]
        shr    ecx, 8
        shr    eax, 8
loc_4_1302:
        call   sub_4_13A5
loc_4_1307:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    edx, dword ptr [ebx + 8]
        mov    byte ptr [edx + 0x14], 0x80
        mov    byte ptr [edx + 0x1d], 0x15
        mov    eax, 0x13
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   byte ptr [edx + 0x2c], 1
        jne    loc_4_132F
        cmp    byte ptr [edx + 0x1d], 0x70
        je     loc_4_134E
loc_4_132F:
        mov    byte ptr [edx + 0x14], 0x81
        mov    byte ptr [edx + 0x1d], 0x15
        mov    eax, 0x13
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   byte ptr [edx + 0x2c], 1
        jne    loc_4_1383
        cmp    byte ptr [edx + 0x1d], 0x70
        jne    loc_4_1383
loc_4_134E:
        mov    ecx, 0xe0
        mov    eax, 0xe7
        bt     dword ptr [0xfe025], 3
        jae    loc_4_137E
        mov    cx, word ptr [0xfe02e]
        mov    ax, word ptr [0xfe02d]
        shl    eax, 6
        je     loc_4_1383
        lea    eax, [ecx + eax - 1]
        shr    ecx, 8
        shr    eax, 8
loc_4_137E:
        call   sub_4_13A5
loc_4_1383:
        cmp    byte ptr [0xffffa], 0x27
        jne    loc_4_13A4
        mov    dx, 0x8080
        in     al, dx
        test   al, 4
        je     loc_4_13A4
        mov    ecx, 0xe0
        mov    eax, 0xef
        call   sub_4_13A5
loc_4_13A4:
        ret    
sub_4_13A5:
        cmp    ecx, 0xa0
        jb     loc_4_13C7
        xchg   ecx, eax
        inc    ecx
        sub    ecx, eax
        lea    eax, [eax*2 + 0xf28]          ; -> obj1:0x0F28
loc_4_13B8:
        test   byte ptr [eax], 0x40
        jne    loc_4_13C2
        or     word ptr [eax], 0x80
loc_4_13C2:
        add    eax, 2
        loop   loc_4_13B8
loc_4_13C7:
        ret    
sub_4_13C8:
        push   eax
        mov    esi, edx
        shl    esi, 0xc
        cmp    dword ptr [esp], 0
        je     loc_4_13F4
        push   edx
        push   ecx
        push   0
        push   0x1000
        push   esi
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        pop    ecx
        pop    edx
        cmp    eax, -1
        je     loc_4_1511
        mov    esi, eax
loc_4_13F4:
        cmp    word ptr [esi], 0xaa55
        jne    loc_4_1470
        push   ecx
        push   edx
        push   ebx
        push   esi
        cld    
        movzx  ecx, byte ptr [esi + 2]
        xchg   cl, ch
        shl    ecx, 1
        mov    bl, 1
        jecxz  loc_4_1432
        mov    edx, dword ptr [esp + 8]
        shl    edx, 0xc
        push   esi
        and    esi, 0xfff
        add    edx, esi
        pop    esi
        add    edx, ecx
        cmp    edx, 0x100000
        jae    loc_4_1432
        mov    edx, ecx
        xor    ebx, ebx
loc_4_142B:
        lodsb  al, byte ptr [esi]
        add    bl, al
        loop   loc_4_142B
        mov    eax, edx
loc_4_1432:
        or     bl, bl
        pop    esi
        pop    ebx
        pop    edx
        pop    ecx
        jne    loc_4_1470
        and    esi, 0xfff
        add    eax, esi
        dec    eax
        shr    eax, 0xc
        je     loc_4_1468
        inc    eax
        push   ecx
        cmp    eax, ecx
        jbe    loc_4_1450
        mov    eax, ecx
loc_4_1450:
        mov    ecx, eax
loc_4_1452:
        or     byte ptr [ebx], 2
        add    ebx, 2
        loop   loc_4_1452
        sub    ebx, 2
        pop    ecx
        dec    eax
        sub    ecx, eax
        add    edx, eax
        jmp    loc_4_1511
loc_4_1468:
        or     byte ptr [ebx], 2
        jmp    loc_4_1511
loc_4_1470:
        mov    eax, esi
        and    eax, 0xfff
        add    eax, 0x200
        add    esi, 0x200
        cmp    eax, 0x1000
        jb     loc_4_13F4
        sub    esi, 0x200
        and    esi, 0xfffff000
loc_4_1499:
        cmp    edx, 0xef
        ja     loc_4_1511
        cli    
        movzx  eax, word ptr [esi + 0xa0]
        push   eax
        movzx  eax, word ptr [esi + 0xa2]
        push   eax
        mov    word ptr [esi + 0xa0], 0
        mov    word ptr [esi + 0xa2], 0xffff
        lea    edi, [esi + 0xa0]
        cld    
        cld    
        cld    
        cld    
        movzx  eax, word ptr [edi]
        cld    
        cld    
        cld    
        cld    
        cld    
        cld    
        cld    
        cld    
        cld    
        cld    
        cld    
        cld    
        cmp    eax, 0
        pop    eax
        mov    word ptr [esi + 0xa2], ax
        pop    eax
        mov    word ptr [esi + 0xa0], ax
        sti    
        jne    loc_4_14F8
        or     byte ptr [ebx], 4
        jmp    loc_4_1511
loc_4_14F8:
        mov    eax, esi
        and    eax, 0xfff
        add    eax, 0x800
        add    esi, 0x800
        cmp    eax, 0x1000
        jb     loc_4_1499
loc_4_1511:
        pop    eax
        ret    
sub_4_1513:
        test   word ptr [ebx], 0x886
        stc    
        jne    loc_4_1640
        bt     dword ptr [ebp - 0x28], edx
        jb     loc_4_1640
        push   ebp
        sub    esp, 0x20
        mov    ebp, esp
        xor    eax, eax
        cmp    edx, 0xc0
        jb     loc_4_1548
        cmp    edx, 0xff
        ja     loc_4_1548
        cmp    word ptr [ebx], 0
        je     loc_4_1548
        dec    eax
loc_4_1548:
        pushal 
        cld    
        or     eax, 0xffffffff
        mov    ecx, 8
        mov    edi, ebp
        rep stosd dword ptr es:[edi], eax
        mov    edi, dword ptr [0x3270]       ; -> loc_4_3270
        mov    esi, edx
        shl    esi, 0xc
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_4_1588
        push   edx
        push   ecx
        push   0
        push   0x1000
        push   esi
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        pop    ecx
        pop    edx
        cmp    eax, -1
        je     loc_4_1662
        mov    esi, eax
loc_4_1588:
        mov    ecx, 0x400
        rep movsd dword ptr es:[edi], dword ptr [esi]
        sub    esi, 2
        sub    edi, 4
        std    
        mov    ecx, 0x400
        xor    edx, edx
        mov    eax, dword ptr [esi - 2]
loc_4_15A0:
        xor    eax, 0xffffffff
        push   eax
        pop    eax
        lodsw  ax, word ptr [esi]
        shl    eax, 0x10
        lodsw  ax, word ptr [esi]
        cmp    eax, dword ptr [edi]
        jne    loc_4_15B6
        add    edx, 0x10000
loc_4_15B6:
        movzx  ebx, al
        btr    dword ptr [ebp], ebx
        adc    edx, 0
        movzx  ebx, ah
        btr    dword ptr [ebp], ebx
        adc    edx, 0
        ror    eax, 0x10
        movzx  ebx, al
        btr    dword ptr [ebp], ebx
        adc    edx, 0
        movzx  ebx, ah
        btr    dword ptr [ebp], ebx
        adc    edx, 0
        ror    eax, 0x10
        sub    edi, 4
        loop   loc_4_15A0
        cld    
        cmp    dx, word ptr [0x3274]         ; -> loc_4_3274
        jb     loc_4_1643
        push   edx
        shr    edx, 0x10
        cmp    edx, 0x80
        pop    edx
        jb     loc_4_1643
        popal  
        pushal 
        cmp    edx, 0xe0
        jb     loc_4_1631
        cmp    edx, 0xef
        ja     loc_4_1631
        test   byte ptr [ebx + 0x20], 2
        je     loc_4_1631
        or     byte ptr [ebx], 8
        mov    edi, dword ptr [0x3270]       ; -> loc_4_3270
        lea    esi, [edx + 0x10]
        shl    esi, 0xc
        mov    ecx, 0x400
        cld    
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        je     loc_4_1662
loc_4_1631:
        popal  
        or     byte ptr [ebx], 0xa
        add    esp, 0x20
        stc    
        jmp    loc_4_163F
loc_4_163B:
        add    esp, 0x20
        clc    
loc_4_163F:
        pop    ebp
loc_4_1640:
        cld    
        cmc    
        ret    
loc_4_1643:
        dec    dx
        je     loc_4_1662
        popal  
        pushal 
        cmp    edx, 0xed
        jb     loc_4_1662
        cmp    edx, 0xef
        ja     loc_4_1662
        test   byte ptr [0x1100], 2          ; -> obj1:0x1100
        jne    loc_4_1631
loc_4_1662:
        popal  
        jmp    loc_4_163B
sub_4_1665:
        enter  8, 0
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 0xc]
        xor    eax, eax
        mov    dword ptr [ebp - 8], eax
        cld    
        lodsw  ax, word ptr [esi]
        mov    dword ptr [ebp - 4], eax
        lodsw  ax, word ptr [esi]
        cmp    ax, 2
        jne    loc_4_1741
        xor    eax, eax
loc_4_1688:
        push   eax
        or     al, 8
        cli    
        out    0x96, al
        jmp    loc_4_1690
loc_4_1690:
        mov    edx, 0x100
        in     al, dx
        jmp    loc_4_1698
loc_4_1698:
        mov    ah, al
        inc    edx
        in     al, dx
        xchg   al, ah
        mov    ecx, dword ptr [ebp - 4]
        push   esi
loc_4_16A2:
        cmp    ax, word ptr [esi]
        je     loc_4_16C3
loc_4_16A7:
        movzx  ebx, word ptr [esi + 0xa]
        lea    esi, [esi + ebx*2 + 0xc]
        loop   loc_4_16A2
        pop    esi
        mov    al, 0
        out    0x96, al
        sti    
        pop    eax
        inc    eax
        cmp    eax, 8
        jb     loc_4_1688
        jmp    loc_4_1745
loc_4_16C3:
        push   eax
        push   ecx
        push   esi
        add    esi, 2
        mov    edi, 4
        mov    edx, 0x102
        xor    ebx, ebx
loc_4_16D5:
        xor    eax, eax
        cld    
        lodsw  ax, word ptr [esi]
        mov    ecx, eax
        xchg   cl, ch
        or     ah, al
        je     loc_4_16E9
        in     al, dx
        and    al, ch
        ror    al, cl
        or     bl, al
loc_4_16E9:
        inc    edx
        dec    edi
        jne    loc_4_16D5
        cld    
        xor    eax, eax
        lodsw  ax, word ptr [esi]
        cmp    ebx, eax
        jae    loc_4_1739
        shl    ebx, 1
        movzx  eax, word ptr [ebx + esi]
        xor    ecx, ecx
        mov    cl, ah
        xor    ah, ah
        jecxz  loc_4_1739
        mov    edx, eax
        add    edx, ecx
        dec    edx
        cmp    eax, dword ptr [0]            ; -> obj6:0x0000
        jae    loc_4_1716
        mov    eax, dword ptr [0]            ; -> obj6:0x0000
loc_4_1716:
        cmp    edx, 0x100
        jb     loc_4_1723
        mov    edx, 0xff
loc_4_1723:
        sub    edx, eax
        jb     loc_4_1739
        inc    edx
        mov    ecx, edx
        lea    eax, [eax*2 + 0xf28]          ; -> obj1:0x0F28
loc_4_1731:
        or     byte ptr [eax], 0x80
        add    eax, 2
        loop   loc_4_1731
loc_4_1739:
        pop    esi
        pop    ecx
        pop    eax
        jmp    loc_4_16A7
loc_4_1741:
        xor    eax, eax
        jmp    loc_4_174A
loc_4_1745:
        mov    eax, 1
loc_4_174A:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_4_174F:
        enter  8, 0
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [ebp + 0x14]
        movzx  ecx, word ptr [edx + 0x34]
        shl    ecx, 0x10
        mov    cx, word ptr [edx + 0x30]
        mov    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [edx + 0x34], ax
        mov    word ptr [edx + 0x30], 0x93c
        mov    ecx, 1
loc_4_1779:
        mov    word ptr [edx + 0x1c], 0xd800
        mov    byte ptr [edx + 0x18], cl
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [edx + 0x2c], 0
        jb     loc_4_1867
        test   byte ptr [edx + 0x14], 2
        je     loc_4_1883
        movzx  eax, byte ptr [edx + 0x15]
        mov    dword ptr [ebp - 4], eax
loc_4_17AA:
        mov    word ptr [edx + 0x1c], 0xd801
        mov    byte ptr [edx + 0x18], cl
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [edx + 0x19], al
        mov    word ptr [edx + 4], 0
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [edx + 0x3c], ax
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        bt     word ptr [edx + 0x2c], 0
        jb     loc_4_1875
        mov    esi, dword ptr [ebp + 0xc]
        test   byte ptr [esi + 0x22], 2
        je     loc_4_1875
        test   byte ptr [esi + 0x22], 0x40
        jne    loc_4_1875
        lea    esi, [esi + 0x73]
        push   ecx
        mov    ecx, 9
loc_4_17FD:
        mov    eax, dword ptr [esi + 2]
        and    eax, 0xffffff
        movzx  ebx, word ptr [esi + 5]
        shl    eax, 8
        shl    ebx, 0xa
        add    ebx, eax
        dec    ebx
        shr    eax, 0xc
        shr    ebx, 0xc
        cmp    eax, 0x100
        jae    loc_4_1859
        cmp    ebx, dword ptr [0]            ; -> obj6:0x0000
        jb     loc_4_1859
        cmp    eax, dword ptr [0]            ; -> obj6:0x0000
        jae    loc_4_1834
        mov    eax, dword ptr [0]            ; -> obj6:0x0000
loc_4_1834:
        cmp    ebx, 0xff
        jbe    loc_4_1841
        mov    ebx, 0xff
loc_4_1841:
        sub    ebx, eax
        jb     loc_4_1859
        inc    ebx
        push   ecx
        lea    eax, [eax*2 + 0xf28]          ; -> obj1:0x0F28
        mov    ecx, ebx
loc_4_1850:
        or     byte ptr [eax], 0x80
        add    eax, 2
        loop   loc_4_1850
        pop    ecx
loc_4_1859:
        test   byte ptr [esi], 0x80
        je     loc_4_1874
        add    esi, 7
        dec    ecx
        jne    loc_4_17FD
        pop    ecx
        jmp    loc_4_1875
loc_4_1867:
        mov    al, byte ptr [edx + 0x1d]
        cmp    al, 0x83
        je     loc_4_1883
        cmp    al, 0x80
        je     loc_4_1889
        jmp    loc_4_18A1
loc_4_1874:
        pop    ecx
loc_4_1875:
        inc    byte ptr [ebp - 3]
        mov    eax, dword ptr [ebp - 4]
        cmp    ah, al
        jb     loc_4_17AA
loc_4_1883:
        inc    ecx
        jmp    loc_4_1779
loc_4_1889:
        mov    eax, 1
loc_4_188E:
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [edx + 0x30], cx
        shr    ecx, 0x10
        mov    word ptr [edx + 0x34], cx
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_4_18A1:
        xor    eax, eax
        jmp    loc_4_188E
V86MMGR_SetAvailMapPgs:
        pushal 
        or     ecx, ecx
        je     loc_4_193E
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        cmp    eax, dword ptr [esp + 0x1c]
        jae    loc_4_193E
        mov    eax, dword ptr [esp + 0x1c]
        mov    edx, eax
        add    edx, ecx
        cmp    edx, 0x100
        ja     loc_4_193E
        mov    edx, eax
        lea    eax, [eax*2 + 0xf28]          ; -> obj1:0x0F28
        push   eax
loc_4_18D8:
        test   byte ptr [eax + 1], 0x45
        jne    loc_4_193D
        test   byte ptr [eax + 1], 2
        je     loc_4_18EC
        test   byte ptr [eax + 1], 0x20
        jne    loc_4_18F1
        jmp    loc_4_193D
loc_4_18EC:
        test   byte ptr [eax], 0xb0
        jne    loc_4_193D
loc_4_18F1:
        add    eax, 2
        inc    edx
        loop   loc_4_18D8
        pop    eax
        mov    ecx, dword ptr [esp + 0x18]
        mov    edx, dword ptr [esp + 0x1c]
loc_4_1900:
        test   byte ptr [eax + 1], 2
        je     loc_4_190A
        and    byte ptr [eax + 1], 0xfd
loc_4_190A:
        test   byte ptr [eax + 1], 0x10
        jne    loc_4_1934
        test   byte ptr [eax + 1], 8
        jne    loc_4_1930
        push   edx
        push   ecx
        push   eax
        push   0
        push   0
        push   1
        push   edx
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        or     eax, eax
        pop    eax
        pop    ecx
        pop    edx
        je     loc_4_1934
loc_4_1930:
        or     byte ptr [eax + 1], 0x10
loc_4_1934:
        add    eax, 2
        inc    edx
        loop   loc_4_1900
        clc    
loc_4_193B:
        popal  
        ret    
loc_4_193D:
        pop    eax
loc_4_193E:
        stc    
        jmp    loc_4_193B
sub_4_1941:
        lea    edi, [ebp - 0x28]
        mov    edx, 0xa20
        call   sub_4_1959
        mov    edx, 0xa24
        call   sub_4_1959
        ret    
sub_4_1959:
        in     al, dx
        mov    esi, eax
        and    esi, 0xfc
        or     esi, 0x100
        shl    esi, 0xb
        cmp    esi, 0xa0000
        jb     loc_4_1A4E
        cmp    esi, 0xeffff
        ja     loc_4_1A4E
        mov    ah, al
        mov    ecx, 5
loc_4_198A:
        jmp    loc_4_198C
loc_4_198C:
        in     al, dx
        cmp    al, ah
        jne    loc_4_1A4E
        loop   loc_4_198A
        mov    eax, esi
        shr    eax, 0xc
        mov    ecx, 2
        call   sub_4_1A51
        jb     loc_4_1A4E
        mov    edx, 0x1f00
        xor    eax, eax
loc_4_19B3:
        mov    ah, byte ptr [edx + esi]
        and    ah, 0xf
        add    al, ah
        add    edx, 2
        cmp    edx, 0x1f60
        jbe    loc_4_19B3
        and    al, 0xf
        jne    loc_4_1A4E
        mov    al, byte ptr [esi + 0x1e00]
        and    eax, 0xfe
        cmp    al, 0xa0
        jb     loc_4_1A4E
        cmp    al, 0xef
        ja     loc_4_1A4E
        mov    cl, byte ptr [esi + 0x1e01]
        shr    ecx, 2
        and    ecx, 3
        mov    edx, 2
        shl    edx, cl
        mov    ecx, edx
        push   eax
        push   ecx
        call   sub_4_1A51
        pop    ecx
        pop    eax
        jb     loc_4_1A4E
        push   eax
        push   ecx
        push   0
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        pop    ecx
        pop    eax
        lea    eax, [eax*2 + 0xf28]          ; -> obj1:0x0F28
loc_4_1A1B:
        or     word ptr [eax], 0x804
        add    eax, 2
        loop   loc_4_1A1B
        shr    esi, 0xc
        push   0
        push   0
        push   2
        push   esi
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        lea    esi, [esi*2 + 0xf28]          ; -> obj1:0x0F28
        or     word ptr [esi], 0x804
        add    esi, 2
        or     word ptr [esi], 0x804
        clc    
loc_4_1A4D:
        ret    
loc_4_1A4E:
        stc    
        jmp    loc_4_1A4D
sub_4_1A51:
        bt     dword ptr [edi], eax
        jb     loc_4_1A5A
        inc    eax
        loop   sub_4_1A51
        clc    
loc_4_1A5A:
        ret    
        db  000h
loc_4_1A5C:
        VMMCall  VMM._GetNulPageHandle           ; cd206a000100
        mov    dword ptr [0x1200], eax       ; -> obj1:0x1200
        push   0
        push   0x284
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        pop    ecx
        pop    ecx
        test   eax, eax
        jne    loc_4_1A85
loc_4_1A7A:
        call   sub_9_3018
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_4_1A85:
        mov    dword ptr [0x11fc], eax       ; -> obj1:0x11FC
        xor    eax, eax
        xor    esi, esi
        mov    edi, 0x30b7                   ; -> obj4:0x30B7
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        or     al, al
        je     loc_4_1AA4
        or     byte ptr [0x1204], 0x10       ; -> obj1:0x1204
loc_4_1AA4:
        xor    eax, eax
        xor    esi, esi
        mov    edi, 0x30c4                   ; -> obj4:0x30C4
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        or     al, al
        je     loc_4_1ABE
        or     byte ptr [0x1204], 8          ; -> obj1:0x1204
loc_4_1ABE:
        mov    eax, 0x15
        mov    esi, 0x224                    ; -> loc_1_0224
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        call   sub_4_01D4
        jb     loc_4_1A7A
        call   sub_4_1E68
        jb     loc_4_1A7A
        call   sub_4_09E7
        jb     loc_4_1A7A
        inc    byte ptr [0x1214]             ; -> obj1:0x1214
        ret    
sub_4_1AEA:
        pushal 
        cmp    byte ptr [0x3280], 2          ; -> obj4:0x3280
        ja     loc_4_1CE4
        je     loc_4_1BB9
        pushal 
        mov    eax, 4
        mov    edi, 0x318e                   ; -> obj4:0x318E
        xor    esi, esi
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        add    eax, 3
        shr    eax, 2
        or     eax, eax
        jne    loc_4_1B1C
        inc    eax
loc_4_1B1C:
        cmp    eax, 0x10
        jbe    loc_4_1B26
        mov    eax, 0x10
loc_4_1B26:
        mov    ah, al
        xor    ebx, ebx
        mov    ecx, ebx
        VxDCall  V86MMGR.V86MMGR_Set_Mapping_Info ; cd2004000600
        popal  
        mov    eax, 0x800
        xor    esi, esi
        mov    edi, 0x30d1                   ; -> obj4:0x30D1
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        push   eax
        mov    eax, 0
        xor    esi, esi
        mov    edi, 0x30df                   ; -> obj4:0x30DF
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        push   eax
        mov    eax, 0x800
        xor    esi, esi
        mov    edi, 0x30ff                   ; -> obj4:0x30FF
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        push   eax
        mov    eax, 0
        xor    esi, esi
        mov    edi, 0x310d                   ; -> obj4:0x310D
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        push   eax
        xor    eax, eax
        xor    esi, esi
        mov    edi, 0x30f0                   ; -> obj4:0x30F0
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        push   eax
        xor    eax, eax
        xor    esi, esi
        mov    edi, 0x311e                   ; -> obj4:0x311E
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        xor    ecx, ecx
        or     eax, eax
        je     loc_4_1BA7
        or     ecx, 4
loc_4_1BA7:
        pop    eax
        or     eax, eax
        je     loc_4_1BAF
        or     ecx, 8
loc_4_1BAF:
        pop    esi
        pop    edi
        pop    eax
        pop    edx
        VxDCall  V86MMGR.V86MMGR_Set_EMS_XMS_Limits ; cd2002000600
loc_4_1BB9:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    ebp, dword ptr [ebx + 8]
        cmp    byte ptr [0x3280], 2          ; -> obj4:0x3280
        jae    loc_4_1C08
        push   ebx
        call   sub_4_0B0C
        pop    ebx
        jb     loc_4_1CE7
        push   ebx
        call   sub_4_246D
        pop    ebx
        jb     loc_4_1CE7
        cmp    byte ptr [0x3280], 0          ; -> obj4:0x3280
        je     loc_4_1C08
        inc    byte ptr [0x3280]             ; -> obj4:0x3280
        jmp    loc_4_1CCF
loc_4_1C08:
        mov    byte ptr [0x3280], 3          ; -> obj4:0x3280
        call   sub_4_2418
        jb     loc_4_1CE7
        call   sub_4_1D0A
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        cmp    eax, 0xa0
        jae    loc_4_1CC1
        push   eax
        VxDCall  EBIOS.EBIOS_Get_Version         ; cd2000001200
        jb     loc_4_1C57
        or     eax, eax
        je     loc_4_1C57
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        cmp    edx, eax
        jbe    loc_4_1C57
        cmp    edx, 0xa0
        jae    loc_4_1C57
        dec    edx
        mov    dword ptr [0x327c], edx       ; -> loc_4_327C
loc_4_1C57:
        xor    eax, eax
        xor    esi, esi
        mov    edi, 0x317e                   ; -> obj4:0x317E
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        jb     loc_4_1C6E
        or     eax, eax
        je     loc_4_1CC0
        jmp    loc_4_1C99
loc_4_1C6E:
        pop    eax
        push   eax
        cmp    eax, 0x90
        ja     loc_4_1CC0
        test   al, 0xf
        jne    loc_4_1CC0
        mov    ecx, dword ptr [0x327c]       ; -> loc_4_327C
        inc    ecx
        sub    ecx, eax
        jbe    loc_4_1CC0
        lea    eax, [eax*2 + 0xf28]          ; -> obj1:0x0F28
loc_4_1C8D:
        test   word ptr [eax], 0x2fa6
        jne    loc_4_1CC0
        add    eax, 2
        loop   loc_4_1C8D
loc_4_1C99:
        VxDCall  DOSMGR.DOSMGR_Get_Version       ; cd2000001500
        jb     loc_4_1CC0
        cmp    eax, 0x30a
        jb     loc_4_1CC0
        VxDCall  DOSMGR.DOSMGR_BackFill_Allowed  ; cd200a001500
        jb     loc_4_1CC0
        push   0
        push   dword ptr [0x327c]            ; -> loc_4_327C
        VMMCall  VMM._SetLastV86Page             ; cd20d4000100
        pop    ecx
        pop    ecx
loc_4_1CC0:
        pop    eax
loc_4_1CC1:
        call   sub_4_0BFA
        jb     loc_4_1CE7
        call   0
        jb     loc_4_1CE7
loc_4_1CCF:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
loc_4_1CE4:
        clc    
loc_4_1CE5:
        popal  
        ret    
loc_4_1CE7:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        call   sub_9_3018
loc_4_1D01:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
        stc    
        jmp    loc_4_1CE5
sub_4_1D0A:
        enter  0x24, 0
        pushal 
        lea    edx, [ebp - 0x24]
        push   0
        push   edx
        push   0
        VMMCall  VMM._Get_Device_V86_Pages_Array ; cd2074000100
        add    esp, 0xc
        or     eax, eax
        je     loc_4_1D77
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        mov    ebx, eax
        mov    esi, 0x110
        sub    esi, ebx
        jbe    loc_4_1D77
        mov    edi, ebx
        lea    ebx, [ebx*2 + 0xf28]          ; -> obj1:0x0F28
loc_4_1D3F:
        test   word ptr [ebx], 0x1080
        jne    loc_4_1D50
        bt     dword ptr [ebp - 0x24], edi
        jae    loc_4_1D50
        or     byte ptr [ebx + 1], 8
loc_4_1D50:
        test   word ptr [ebx], 0x3f60
        jne    loc_4_1D70
        test   byte ptr [ebx], 0x86
        je     loc_4_1D70
        push   0
        push   0
        push   1
        push   edi
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        or     byte ptr [ebx + 1], 8
loc_4_1D70:
        inc    edi
        add    ebx, 2
        dec    esi
        jne    loc_4_1D3F
loc_4_1D77:
        popal  
        leave  
        ret    
V86MMGR_Dev_Init:
        call   sub_4_0E3C
        jb     loc_4_1D01
        mov    byte ptr [0x3281], 1          ; -> loc_4_3281
        pushal 
        xor    esi, esi
        mov    edi, 0x9f
        VMMCall  VMM.MemArb_Reserve_Pages        ; cd205f010100
        mov    esi, 0xa0
        mov    edi, 0xbf
        VMMCall  VMM.MemArb_Reserve_Pages        ; cd205f010100
        inc    edi
        mov    edx, edi
        lea    edx, [edx*2 + 0xf28]          ; -> obj1:0x0F28
loc_4_1DB0:
        test   word ptr [edx], 0x88e
        je     loc_4_1DBF
        mov    esi, edi
        VMMCall  VMM.MemArb_Reserve_Pages        ; cd205f010100
loc_4_1DBF:
        add    edx, 2
        inc    edi
        cmp    edi, 0x100
        jb     loc_4_1DB0
        popal  
        ret    
V86MMGR_Alloc_UM_Page:
        push   edx
        push   eax
        push   ecx
        jecxz  loc_4_1DF2
        mov    edx, ecx
        cmp    byte ptr [0x3281], 0          ; -> loc_4_3281
        je     loc_4_1DF2
        xor    eax, eax
        mov    al, 0xc0
loc_4_1DE1:
        test   word ptr [eax*2 + 0xf28], 0xf86 ; -> obj1:0x0F28
        je     loc_4_1DFA
loc_4_1DED:
        inc    eax
        test   ah, ah
        je     loc_4_1DE1
loc_4_1DF2:
        pop    ecx
        pop    eax
        sub    ecx, edx
        pop    edx
        xor    eax, eax
        ret    
loc_4_1DFA:
        mov    ecx, dword ptr [esp]
loc_4_1DFD:
        dec    ecx
        je     loc_4_1E19
        inc    eax
        test   ah, ah
        jne    loc_4_1DF2
        test   word ptr [eax*2 + 0xf28], 0xf86 ; -> obj1:0x0F28
        je     loc_4_1DFD
        cmp    ecx, edx
        jae    loc_4_1DED
        mov    edx, ecx
        jmp    loc_4_1DED
loc_4_1E19:
        mov    ecx, dword ptr [esp]
        push   eax
        push   edx
        sub    eax, ecx
        inc    eax
        push   0
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        pop    edx
        pop    eax
        mov    ecx, dword ptr [esp]
loc_4_1E35:
        or     byte ptr [eax*2 + 0xf29], 8   ; -> obj1:0x0F29
        dec    eax
        loop   loc_4_1E35
        inc    eax
        pop    ecx
        add    esp, 4
        shl    eax, 0xc
        pop    edx
        ret    
loc_4_1E4A:
        mov    byte ptr [0x1215], 1          ; -> obj1:0x1215
        VxDCall  PAGESWAP.PageSwap_Get_Version   ; cd2000000700
        or     eax, eax
        je     loc_4_1E61
        mov    byte ptr [0x1215], bl         ; -> obj1:0x1215
loc_4_1E61:
        call   sub_4_0A2A
        clc    
        ret    
sub_4_1E68:
        mov    ecx, 1
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jb     loc_4_2058
        mov    dword ptr [0x132c], eax       ; -> obj1:0x132C
        xor    ecx, ecx
        VMMCall  VMM.GetSet_HMA_Info             ; cd20bc000100
        mov    dword ptr [0x1330], edx       ; -> obj1:0x1330
        or     eax, eax
        je     loc_4_1E9B
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        call   sub_15_0097
loc_4_1E9B:
        xor    eax, eax
        xor    esi, esi
        mov    edi, 0x3161                   ; -> obj4:0x3161
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        jb     loc_4_1EB3
        je     loc_4_1EB3
        mov    dword ptr [0x1330], eax       ; -> obj1:0x1330
loc_4_1EB3:
        mov    eax, dword ptr [0x3294]       ; -> obj4:0x3294
        or     eax, eax
        je     loc_4_1F33
        push   0
        push   0x40
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        inc    eax
        je     loc_4_2058
        dec    eax
        mov    esi, eax
        xor    edx, edx
        push   esi
        mov    ecx, 0x10
loc_4_1EDC:
        lodsd  eax, dword ptr [esi]
        test   al, 1
        jne    loc_4_1EE2
        inc    edx
loc_4_1EE2:
        shr    eax, 0xc
        cmp    eax, 0x110
        jae    loc_4_1EF7
        cmp    eax, 0x100
        jae    loc_4_1EF6
        inc    edx
        jmp    loc_4_1EF7
loc_4_1EF6:
        inc    edx
loc_4_1EF7:
        loop   loc_4_1EDC
        pop    esi
        mov    eax, dword ptr [0x9c]         ; -> obj6:0x009C
        test   byte ptr [eax], 8
        jne    loc_4_1F13
        and    byte ptr [0x1204], 0xdf       ; -> obj1:0x1204
        VMMCall  VMM.Set_Physical_HMA_Alias      ; cd20dd000100
        jmp    loc_4_1F33
loc_4_1F13:
        or     edx, edx
        jne    loc_4_1F33
        mov    ecx, 0x10
loc_4_1F1C:
        cld    
        lodsd  eax, dword ptr [esi]
        shr    eax, 0xc
        push   ecx
        push   0
        push   1
        push   eax
        VMMCall  VMM._AddFreePhysPage            ; cd20d2000100
        add    esp, 0xc
        pop    ecx
        loop   loc_4_1F1C
loc_4_1F33:
        push   0
        push   0
        push   0x10
        push   0x100
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_1F52
        or     byte ptr [0x1204], 0x80       ; -> obj1:0x1204
loc_4_1F52:
        mov    edi, 0x100
        lea    edi, [edi*2 + 0xf28]          ; -> obj1:0x0F28
        mov    ecx, 0x10
        mov    eax, 0x1224
        test   byte ptr [0x1204], 0x80       ; -> obj1:0x1204
        je     loc_4_1F76
        mov    eax, 0x24
loc_4_1F76:
        or     word ptr [edi], ax
        add    edi, 2
        loop   loc_4_1F76
        call   sub_11_0C9C
        jb     loc_4_2058
        call   sub_4_2072
        jb     loc_4_2058
        xor    ecx, ecx
        VMMCall  VMM.GetSet_HMA_Info             ; cd20bc000100
        xor    esi, esi
loc_4_1F9E:
        mov    dword ptr [0x129c], ecx       ; -> obj1:0x129C
        mov    dword ptr [0x3298], ecx       ; -> loc_4_3298
        movzx  eax, cx
        shr    ecx, 0x10
        shl    ecx, 4
        add    eax, ecx
        mov    dword ptr [0x12a0], eax       ; -> obj1:0x12A0
        mov    ecx, dword ptr [eax]
        mov    dword ptr [0x1322], ecx       ; -> obj1:0x1322
        mov    dl, byte ptr [eax + 4]
        mov    byte ptr [0x1326], dl         ; -> obj1:0x1326
        cmp    cl, 0xeb
        je     loc_4_2014
        cmp    cl, 0xea
        je     loc_4_2007
        or     esi, esi
        je     loc_4_2058
        inc    byte ptr [0x32a0]             ; -> loc_4_32A0
        mov    ecx, dword ptr [esi + 1]
        mov    dword ptr [0x129c], ecx       ; -> obj1:0x129C
        mov    dword ptr [0x3298], edi       ; -> loc_4_3298
        mov    dword ptr [0x12a0], esi       ; -> obj1:0x12A0
        mov    ecx, dword ptr [esi]
        mov    dword ptr [0x1322], ecx       ; -> obj1:0x1322
        mov    dl, byte ptr [esi + 4]
        mov    byte ptr [0x1326], dl         ; -> obj1:0x1326
        jmp    loc_4_203F
loc_4_2007:
        mov    ecx, dword ptr [eax + 1]
        mov    esi, eax
        mov    edi, dword ptr [0x3298]       ; -> loc_4_3298
        jmp    loc_4_1F9E
loc_4_2014:
        cmp    dl, 0x90
        jne    loc_4_2058
        mov    eax, ecx
        shr    eax, 0x10
        cmp    eax, 0x9090
        jne    loc_4_2058
        movzx  eax, ch
        add    eax, 2
        mov    ecx, dword ptr [0x129c]       ; -> obj1:0x129C
        add    cx, ax
        mov    dword ptr [0x329c], eax       ; -> loc_4_329C
        mov    dword ptr [0x129c], ecx       ; -> obj1:0x129C
loc_4_203F:
        mov    eax, dword ptr [0x3298]       ; -> loc_4_3298
        mov    esi, 0x2824                   ; -> loc_4_2824
        VMMCall  VMM.Install_V86_Break_Point     ; cd2009000100
        jb     loc_4_2058
        inc    byte ptr [0x1327]             ; -> obj1:0x1327
        ret    
loc_4_2058:
        call   sub_9_3018
        movzx  esi, word ptr [0xe]           ; -> loc_2_000E
        add    esi, dword ptr [4]            ; -> loc_2_0004
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
sub_4_2072:
        pushal 
        cmp    dword ptr [0x1284], 0         ; -> obj1:0x1284
        je     loc_4_239F
        mov    edi, dword ptr [0x9c]         ; -> obj6:0x009C
        lea    edi, [edi + 0xa]
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        and    eax, 0xfffffffc
        push   eax
        mov    ecx, eax
        test   ecx, 3
        stc    
        jne    loc_4_20AD
        shr    ecx, 2
        mov    eax, ecx
        shl    eax, 1
        shl    ecx, 2
        add    ecx, eax
        add    ecx, edi
loc_4_20AD:
        mov    edi, ecx
        pop    edx
        mov    ecx, 0x100
        sub    ecx, edx
        shr    ecx, 2
        xor    eax, eax
loc_4_20BC:
        push   eax
        test   byte ptr [edi], 4
        je     loc_4_2154
        push   ecx
        push   edx
        mov    eax, edx
        lea    eax, [eax*2 + 0xf28]          ; -> obj1:0x0F28
        test   byte ptr [eax + 1], 4
        jne    loc_4_2152
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_4_20E7
        test   byte ptr [eax + 1], 2
        je     loc_4_2152
        jmp    loc_4_210D
loc_4_20E7:
        test   byte ptr [eax + 1], 0x10
        pushfd 
        or     byte ptr [eax + 1], 0x32
        popfd  
        jne    loc_4_210D
        push   edx
        push   0
        push   0
        push   1
        push   edx
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        pop    edx
        or     eax, eax
        je     loc_4_23A2
loc_4_210D:
        movzx  eax, byte ptr [edi + 1]
        cmp    al, 0xff
        jne    loc_4_2119
        mov    eax, dword ptr [esp + 8]
loc_4_2119:
        cmp    eax, dword ptr [0x1284]       ; -> obj1:0x1284
        jae    loc_4_2402
        mov    ecx, 0x10
        mul    ecx
        add    eax, dword ptr [0x9c]         ; -> obj6:0x009C
        mov    eax, dword ptr [eax + 0x18c]
        pop    edx
        push   edx
        push   0
        push   1
        push   edx
        push   ebx
        push   eax
        VMMCall  VMM._PhysIntoV86                ; cd205e000100
        add    esp, 0x14
        or     eax, eax
        je     loc_4_2402
loc_4_2152:
        pop    edx
        pop    ecx
loc_4_2154:
        inc    edx
        test   byte ptr [edi], 8
        je     loc_4_21EC
        push   ecx
        push   edx
        mov    eax, edx
        lea    eax, [eax*2 + 0xf28]          ; -> obj1:0x0F28
        test   byte ptr [eax + 1], 4
        jne    loc_4_21EA
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_4_217F
        test   byte ptr [eax + 1], 2
        je     loc_4_21EA
        jmp    loc_4_21A5
loc_4_217F:
        test   byte ptr [eax + 1], 0x10
        pushfd 
        or     byte ptr [eax + 1], 0x32
        popfd  
        jne    loc_4_21A5
        push   edx
        push   0
        push   0
        push   1
        push   edx
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        pop    edx
        or     eax, eax
        je     loc_4_23A2
loc_4_21A5:
        movzx  eax, byte ptr [edi + 1]
        cmp    al, 0xff
        jne    loc_4_21B1
        mov    eax, dword ptr [esp + 8]
loc_4_21B1:
        cmp    eax, dword ptr [0x1284]       ; -> obj1:0x1284
        jae    loc_4_2402
        mov    ecx, 0x10
        mul    ecx
        add    eax, dword ptr [0x9c]         ; -> obj6:0x009C
        mov    eax, dword ptr [eax + 0x190]
        pop    edx
        push   edx
        push   0
        push   1
        push   edx
        push   ebx
        push   eax
        VMMCall  VMM._PhysIntoV86                ; cd205e000100
        add    esp, 0x14
        or     eax, eax
        je     loc_4_2402
loc_4_21EA:
        pop    edx
        pop    ecx
loc_4_21EC:
        inc    edx
        test   byte ptr [edi], 0x10
        je     loc_4_22C0
        push   ecx
        push   edx
        mov    eax, edx
        lea    eax, [eax*2 + 0xf28]          ; -> obj1:0x0F28
        test   byte ptr [eax + 1], 4
        jne    loc_4_22BE
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_4_221F
        test   byte ptr [eax + 1], 2
        je     loc_4_22BE
        jmp    loc_4_2279
loc_4_221F:
        test   byte ptr [eax + 1], 0x10
        pushfd 
        or     byte ptr [eax + 1], 0x32
        popfd  
        jne    loc_4_2279
        push   edx
        push   eax
        push   0
        push   0
        push   1
        push   edx
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        pop    ecx
        pop    edx
        or     eax, eax
        jne    loc_4_2279
        cmp    edx, 0xc6
        jne    loc_4_2274
        mov    eax, dword ptr [0x1294]       ; -> obj1:0x1294
        inc    eax
        je     loc_4_2274
        dec    eax
        add    eax, dword ptr [0x9c]         ; -> obj6:0x009C
        cmp    dword ptr [eax], 0x334d4d45
        jne    loc_4_2274
        cmp    word ptr [eax + 4], 0x3638
        jne    loc_4_2274
        and    byte ptr [ecx + 1], 0xcd
        or     byte ptr [ecx + 1], 8
        jmp    loc_4_22BE
loc_4_2274:
        jmp    loc_4_23A2
loc_4_2279:
        movzx  eax, byte ptr [edi + 1]
        cmp    al, 0xff
        jne    loc_4_2285
        mov    eax, dword ptr [esp + 8]
loc_4_2285:
        cmp    eax, dword ptr [0x1284]       ; -> obj1:0x1284
        jae    loc_4_2402
        mov    ecx, 0x10
        mul    ecx
        add    eax, dword ptr [0x9c]         ; -> obj6:0x009C
        mov    eax, dword ptr [eax + 0x194]
        pop    edx
        push   edx
        push   0
        push   1
        push   edx
        push   ebx
        push   eax
        VMMCall  VMM._PhysIntoV86                ; cd205e000100
        add    esp, 0x14
        or     eax, eax
        je     loc_4_2402
loc_4_22BE:
        pop    edx
        pop    ecx
loc_4_22C0:
        inc    edx
        test   byte ptr [edi], 0x20
        je     loc_4_238D
        push   ecx
        push   edx
        mov    eax, edx
        lea    eax, [eax*2 + 0xf28]          ; -> obj1:0x0F28
        test   byte ptr [eax + 1], 4
        jne    loc_4_238B
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_4_22F3
        test   byte ptr [eax + 1], 2
        je     loc_4_238B
        jmp    loc_4_234A
loc_4_22F3:
        test   byte ptr [eax + 1], 0x10
        pushfd 
        or     byte ptr [eax + 1], 0x32
        popfd  
        jne    loc_4_234A
        push   edx
        push   eax
        push   0
        push   0
        push   1
        push   edx
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        pop    ecx
        pop    edx
        or     eax, eax
        jne    loc_4_234A
        cmp    edx, 0xc7
        jne    loc_4_2348
        mov    eax, dword ptr [0x1294]       ; -> obj1:0x1294
        inc    eax
        je     loc_4_2348
        dec    eax
        add    eax, dword ptr [0x9c]         ; -> obj6:0x009C
        cmp    dword ptr [eax], 0x334d4d45
        jne    loc_4_2348
        cmp    word ptr [eax + 4], 0x3638
        jne    loc_4_2348
        and    byte ptr [ecx + 1], 0xcd
        or     byte ptr [ecx + 1], 8
        jmp    loc_4_238B
loc_4_2348:
        jmp    loc_4_23A2
loc_4_234A:
        movzx  eax, byte ptr [edi + 1]
        cmp    al, 0xff
        jne    loc_4_2356
        mov    eax, dword ptr [esp + 8]
loc_4_2356:
        cmp    eax, dword ptr [0x1284]       ; -> obj1:0x1284
        jae    loc_4_2402
        mov    ecx, 0x10
        mul    ecx
        add    eax, dword ptr [0x9c]         ; -> obj6:0x009C
        mov    eax, dword ptr [eax + 0x198]
        pop    edx
        push   edx
        push   0
        push   1
        push   edx
        push   ebx
        push   eax
        VMMCall  VMM._PhysIntoV86                ; cd205e000100
        add    esp, 0x14
        or     eax, eax
        je     loc_4_2402
loc_4_238B:
        pop    edx
        pop    ecx
loc_4_238D:
        inc    edx
        pop    eax
        test   byte ptr [edi], 0x3c
        je     loc_4_2395
        inc    eax
loc_4_2395:
        add    edi, 6
        dec    ecx
        jne    loc_4_20BC
loc_4_239F:
        clc    
loc_4_23A0:
        popal  
        ret    
loc_4_23A2:
        call   sub_9_3018
        shl    edx, 8
        cmp    edx, 0xb000
        jb     loc_4_23E4
        cmp    edx, 0xb700
        ja     loc_4_23E4
        push   edx
        xor    esi, esi
        mov    edi, 0x3040                   ; -> loc_4_3040
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        pop    edx
        jb     loc_4_23D1
        je     loc_4_23D1
        or     eax, eax
        je     loc_4_23E4
loc_4_23D1:
        push   edx
        push   0xc0004
        push   0                             ; -> obj2:0x0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        jmp    loc_4_23F5
loc_4_23E4:
        push   edx
        push   0xc0003
        push   0                             ; -> obj2:0x0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
loc_4_23F5:
        mov    esi, eax
        pushal 
        mov    esi, esi
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
loc_4_2402:
        add    esp, 0xc
        mov    eax, 0x20005
        xor    edx, edx
        xor    ecx, ecx
        inc    ecx
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        stc    
        jmp    loc_4_23A0
sub_4_2418:
        test   byte ptr [0x1204], 0x20       ; -> obj1:0x1204
        jne    loc_4_246B
        mov    word ptr [ebp + 0x1c], 0x1607
        mov    word ptr [ebp + 0x10], 6
        mov    word ptr [ebp + 0x18], 0
        xor    eax, eax
        mov    word ptr [ebp + 0x14], ax
        mov    word ptr [ebp + 4], ax
        mov    word ptr [ebp], ax
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    word ptr [ebp + 0x1c], 0
        je     loc_4_2466
        xor    esi, esi
        xor    eax, eax
        mov    edi, 0x3170                   ; -> loc_4_3170
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        or     eax, eax
        je     loc_4_246B
loc_4_2466:
        call   sub_4_2886
loc_4_246B:
        clc    
        ret    
sub_4_246D:
        xor    al, al
        xchg   byte ptr [0x1327], al         ; -> obj1:0x1327
        or     al, al
        je     loc_4_2484
        mov    eax, dword ptr [0x3298]       ; -> loc_4_3298
        VMMCall  VMM.Remove_V86_Break_Point      ; cd200a000100
loc_4_2484:
        xor    edx, edx
        mov    ecx, dword ptr [0x9c]         ; -> obj6:0x009C
        jecxz  loc_4_249C
        cmp    word ptr [ecx + 4], 0x10a
        jb     loc_4_249C
        mov    edx, dword ptr [0x127c]       ; -> obj1:0x127C
loc_4_249C:
        mov    dword ptr [0x127c], edx       ; -> obj1:0x127C
        cmp    dword ptr [0x12b8], 0         ; -> obj1:0x12B8
        jne    loc_4_2509
        mov    esi, dword ptr [0x127c]       ; -> obj1:0x127C
        or     esi, esi
        jne    loc_4_24BA
        mov    esi, 0x80
loc_4_24BA:
        lea    ecx, [esi + esi]
        push   0x100
        push   ecx
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_4_257F
        mov    dword ptr [0x12bc], eax       ; -> obj1:0x12BC
        mov    edi, eax
        mov    ecx, esi
        or     eax, 0xffffffff
        cld    
        rep stosw word ptr es:[edi], ax
        mov    eax, esi
        imul   eax, eax, 0x10
        push   0x101
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_4_257F
        mov    dword ptr [0x12b4], eax       ; -> obj1:0x12B4
        mov    dword ptr [0x12b8], esi       ; -> obj1:0x12B8
loc_4_2509:
        cmp    dword ptr [0x127c], 0         ; -> obj1:0x127C
        je     loc_4_258A
        mov    esi, dword ptr [0x9c]         ; -> obj6:0x009C
        add    esi, dword ptr [0x1280]       ; -> obj1:0x1280
        mov    ecx, dword ptr [0x127c]       ; -> obj1:0x127C
        mov    edi, dword ptr [0x12bc]       ; -> obj1:0x12BC
        mov    edx, dword ptr [0x12b8]       ; -> obj1:0x12B8
loc_4_2530:
        test   byte ptr [esi + 2], 1
        jne    loc_4_255C
        push   ecx
        push   edx
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 4]
        jecxz  loc_4_2558
        shl    ecx, 0xa
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        test   eax, eax
        je     loc_4_2058
loc_4_2558:
        pop    ecx
        pop    edx
        jmp    loc_4_2562
loc_4_255C:
        mov    ax, word ptr [esi]
        cld    
        stosw  word ptr es:[edi], ax
loc_4_2562:
        sub    edx, 1
        jb     loc_4_256C
        add    esi, 0xc
        loop   loc_4_2530
loc_4_256C:
        sub    edi, dword ptr [0x12bc]       ; -> obj1:0x12BC
        shr    edi, 1
        mov    dword ptr [0x12ac], edi       ; -> obj1:0x12AC
        jmp    loc_4_266C
loc_4_257F:
        call   sub_9_3018
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_4_258A:
        mov    word ptr [ebp + 0x1c], 0x4309
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    al, ah
        jne    loc_4_2618
        cmp    al, 0x43
        jne    loc_4_2618
        movzx  eax, word ptr [ebp + 0x38]
        shl    eax, 4
        movzx  ecx, word ptr [ebp + 0x10]
        add    eax, ecx
        cmp    byte ptr [eax], 1
        jne    loc_4_2618
        movzx  edi, byte ptr [eax + 1]
        movzx  ecx, word ptr [eax + 2]
        movzx  esi, word ptr [eax + 6]
        shl    esi, 4
        movzx  eax, word ptr [eax + 4]
        add    esi, eax
loc_4_25CD:
        cmp    byte ptr [esi], 2
        jne    loc_4_2614
        push   ecx
        cmp    dword ptr [esi + 6], 8
        jb     loc_4_25F2
        mov    eax, dword ptr [esi + 2]
        shr    eax, 2
        inc    eax
        push   0
        push   1
        push   eax
        VMMCall  VMM._GetPhysPageInfo            ; cd205d010100
        add    esp, 0xc
        cmp    al, 2
        je     loc_4_260B
loc_4_25F2:
        mov    eax, dword ptr [esi + 2]
        mov    edx, dword ptr [esi + 6]
        shl    eax, 0xa
        shl    edx, 0xa
        push   0
        push   edx
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
loc_4_260B:
        pop    ecx
        test   eax, eax
        je     loc_4_2058
loc_4_2614:
        add    esi, edi
        loop   loc_4_25CD
loc_4_2618:
        mov    edi, dword ptr [0x12bc]       ; -> obj1:0x12BC
        mov    ecx, dword ptr [0x12b8]       ; -> obj1:0x12B8
loc_4_2624:
        mov    word ptr [ebp + 0x14], 0
        mov    byte ptr [ebp + 0x1d], 9
        mov    eax, dword ptr [0x129c]       ; -> obj1:0x129C
        call   sub_1_05F9
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cmp    word ptr [ebp + 0x1c], 1
        jne    loc_4_264E
        movzx  eax, word ptr [ebp + 0x14]
        cld    
        stosw  word ptr es:[edi], ax
        loop   loc_4_2624
loc_4_264E:
        mov    eax, dword ptr [0x12bc]       ; -> obj1:0x12BC
        sub    edi, eax
        mov    ecx, edi
        shr    ecx, 1
        mov    dword ptr [0x12ac], ecx       ; -> obj1:0x12AC
        push   0
        push   edi
        push   eax
        VMMCall  VMM._HeapReAllocate             ; cd2050000100
        add    esp, 0xc
loc_4_266C:
        mov    eax, 0xe
        mov    ecx, dword ptr [0x12ac]       ; -> obj1:0x12AC
        shl    ecx, 2
        add    ecx, 0x1c
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_4_2058
        mov    dword ptr [0x12b0], esi       ; -> obj1:0x12B0
        mov    eax, dword ptr [0x9c]         ; -> obj6:0x009C
        or     eax, eax
        je     loc_4_26DD
        cmp    word ptr [eax + 4], 0x10a
        jb     loc_4_26DD
        cmp    dword ptr [0x1288], 0         ; -> obj1:0x1288
        jne    loc_4_26AE
        test   byte ptr [eax], 0x10
        jne    loc_4_26DD
loc_4_26AE:
        mov    ecx, dword ptr [0x1288]       ; -> obj1:0x1288
        mov    esi, dword ptr [0x9c]         ; -> obj6:0x009C
        add    esi, dword ptr [0x128c]       ; -> obj1:0x128C
        mov    edi, 0x12c0                   ; -> obj1:0x12C0
        jecxz  loc_4_26D8
loc_4_26C7:
        movzx  eax, word ptr [esi]
        movzx  edx, word ptr [esi + 2]
        call   sub_4_27CB
        add    esi, 4
        loop   loc_4_26C7
loc_4_26D8:
        jmp    loc_4_2774
loc_4_26DD:
        inc    byte ptr [0x1321]             ; -> obj1:0x1321
        or     eax, 0xffffffff
        xor    esi, esi
        mov    edi, 0x3151                   ; -> loc_4_3151
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        or     al, al
        je     loc_4_2774
        cmp    byte ptr [0x32a1], 0          ; -> obj4:0x32A1
        jne    loc_4_2774
        mov    edi, 0x12c0                   ; -> obj1:0x12C0
loc_4_2705:
        cmp    byte ptr [0x1320], 0x10       ; -> obj1:0x1320
        jae    loc_4_2774
        mov    word ptr [ebp + 0x14], 0xffff
        mov    byte ptr [ebp + 0x1d], 0x10
        mov    eax, dword ptr [0x129c]       ; -> obj1:0x129C
        call   sub_1_05F9
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cmp    byte ptr [ebp + 0x10], 0xb0
        jne    loc_4_2774
        mov    byte ptr [ebp + 0x1d], 0x10
        mov    eax, dword ptr [0x129c]       ; -> obj1:0x129C
        call   sub_1_05F9
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cmp    word ptr [ebp + 0x1c], 1
        jne    loc_4_2774
        movzx  eax, word ptr [ebp + 0x10]
        movzx  edx, word ptr [ebp + 0x14]
        call   sub_4_27CB
        jae    loc_4_2705
        movzx  eax, word ptr [ebp + 0x10]
        mov    word ptr [edi + 4], ax
        xor    eax, eax
        mov    word ptr [edi], ax
        mov    word ptr [edi + 2], ax
        inc    byte ptr [0x1320]             ; -> obj1:0x1320
        add    edi, 6
        jmp    loc_4_2705
loc_4_2774:
        mov    eax, dword ptr [0x12a0]       ; -> obj1:0x12A0
        add    eax, dword ptr [ebx + 4]
        pushfd 
        cli    
        mov    byte ptr [eax], 0xea
        mov    ecx, dword ptr [0x1298]       ; -> obj1:0x1298
        mov    dword ptr [eax + 1], ecx
        popfd  
        mov    eax, 0x2f
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        mov    dword ptr [0x12a4], ecx       ; -> obj1:0x12A4
        mov    dword ptr [0x12a8], edx       ; -> obj1:0x12A8
        mov    esi, 0x658                    ; -> loc_1_0658
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        jb     loc_4_257F
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 0x10
        mov    eax, 0x2f
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        jmp    sub_11_0CE9
sub_4_27CB:
        cmp    byte ptr [0x1320], 0x10       ; -> obj1:0x1320
        jae    loc_4_2822
        push   eax
        push   ecx
        movzx  ecx, al
        neg    cl
        sub    edx, ecx
        pop    ecx
        add    ax, 0xff
        xor    al, al
        cmp    edx, 0x100
        jl     loc_4_2821
        xor    dl, dl
        mov    word ptr [edi], ax
        mov    word ptr [edi + 2], dx
        pop    eax
        mov    word ptr [edi + 4], ax
        inc    byte ptr [0x1320]             ; -> obj1:0x1320
        shr    edx, 8
        movzx  eax, word ptr [edi]
        shr    eax, 8
        push   edx
        lea    eax, [eax*2 + 0xf28]          ; -> obj1:0x0F28
        pop    edx
loc_4_2812:
        or     byte ptr [eax + 1], 0x40
        add    eax, 2
        dec    edx
        jne    loc_4_2812
        add    edi, 6
        clc    
        ret    
loc_4_2821:
        pop    eax
loc_4_2822:
        stc    
        ret    
loc_4_2824:
        cmp    byte ptr [ebp + 0x1d], 0xb
        je     loc_4_283E
loc_4_282A:
        inc    byte ptr [0x3280]             ; -> obj4:0x3280
        pushal 
        call   sub_4_1AEA
        popal  
        jb     loc_4_2058
loc_4_283D:
        ret    
loc_4_283E:
        cmp    dword ptr [0x127c], 0         ; -> obj1:0x127C
        jne    loc_4_282A
        cmp    byte ptr [0x32a0], 0          ; -> loc_4_32A0
        jne    loc_4_285B
        mov    eax, dword ptr [0x329c]       ; -> loc_4_329C
        add    word ptr [ebp + 0x24], ax
        jmp    loc_4_283D
loc_4_285B:
        mov    eax, dword ptr [0x129c]       ; -> obj1:0x129C
        mov    word ptr [ebp + 0x24], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x28], ax
        jmp    loc_4_283D
V86MMGR_SetLocalA20:
        test   byte ptr [0x1204], 0x20       ; -> obj1:0x1204
        jne    loc_4_287D
        pushal 
        call   sub_4_2886
        popal  
loc_4_287D:
        ret    
V86MMGR_NoUMBInitCalls:
        mov    byte ptr [0x32a1], 1          ; -> obj4:0x32A1
        ret    
sub_4_2886:
        bts    dword ptr [0x1204], 6         ; -> obj1:0x1204
        jb     loc_4_287D
        mov    esi, 0x3284                   ; -> obj4:0x3284
        mov    edi, 0x1224                   ; -> loc_1_1224
        mov    ecx, 4
        rep movsd dword ptr es:[edi], dword ptr [esi]
        ret    
        db  000h, 000h
loc_4_28A4:
        sub    al, 0
        add    al, byte ptr [eax]
        jmp    edi
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 0CEh, 002h, 0FFh, 0E1h, 000h, 000h
        db  01Eh, 001h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 0CAh, 002h, 0CCh, 002h, 0CEh, 002h, 0D0h, 002h, 0D2h, 002h, 0D4h, 002h, 0D6h, 002h
        db  0D8h, 002h, 0DAh, 002h, 0DCh, 002h, 0DEh, 002h, 0F5h, 0EFh, 000h, 000h, 081h, 006h, 000h, 000h
        db  012h, 001h, 00Dh, 000h, 0C0h, 008h, 0C8h, 008h, 000h, 000h, 000h, 000h, 0D0h, 008h, 0D8h, 008h
        db  000h, 000h, 000h, 000h, 0C0h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 010h, 0F5h, 0EFh
        db  000h, 000h, 000h, 000h, 081h, 005h, 00Ch, 002h, 00Ch, 000h, 0C0h, 004h, 0C4h, 004h, 0C8h, 004h
        db  0CCh, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h
        db  0DCh, 004h, 012h, 070h, 040h, 003h, 007h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h
        db  000h, 000h, 0C8h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 008h, 000h, 000h, 000h, 000h
        db  000h, 000h, 0C8h, 004h, 0CCh, 004h, 000h, 000h, 000h, 000h, 0D8h, 004h, 0DCh, 004h, 012h, 070h
        db  00Ch, 007h, 070h, 004h, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0CEh, 002h, 0CEh, 002h, 0CEh, 002h
        db  0CEh, 002h, 0CEh, 002h, 0CEh, 002h, 0CEh, 002h, 0CEh, 002h, 000h, 000h, 000h, 000h, 0C8h, 002h
        db  0CCh, 002h, 000h, 000h, 000h, 000h, 0D8h, 002h, 0DCh, 002h, 0CCh, 004h, 0CCh, 004h, 0CCh, 004h
        db  0CCh, 004h, 0CCh, 004h, 0CCh, 004h, 0CCh, 004h, 0CCh, 004h, 000h, 0E0h, 01Ch, 002h, 000h, 000h
        db  000h, 000h, 000h, 000h, 008h, 000h, 0C0h, 004h, 0C4h, 004h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h
        db  0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 000h, 0E0h, 000h, 000h, 000h, 000h, 01Eh, 001h, 000h, 000h
        db  010h, 000h, 0C0h, 002h, 0C2h, 002h, 0C4h, 002h, 0C6h, 002h, 0C8h, 002h, 0CAh, 002h, 0CCh, 002h
        db  0CEh, 002h, 0D0h, 002h, 0D2h, 002h, 0D4h, 002h, 0D6h, 002h, 0D8h, 002h, 0DAh, 002h, 0DCh, 002h
        db  0DEh, 002h, 07Fh, 0EFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 0C6h, 002h
        db  07Fh, 0EFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 0CAh, 001h, 0EFh, 0EFh
        db  00Eh, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 0D0h, 008h, 0C8h, 008h
        db  0D8h, 008h, 000h, 000h, 0D6h, 002h, 0CEh, 002h, 0DEh, 002h, 04Fh, 0F0h, 0F0h, 004h, 000h, 000h
        db  000h, 000h, 000h, 000h, 00Ch, 000h, 0DCh, 002h, 0DEh, 002h, 0D8h, 002h, 0DAh, 002h, 0D4h, 002h
        db  0D6h, 002h, 0D0h, 002h, 0D2h, 002h, 0CCh, 003h, 0CEh, 002h, 0C8h, 002h, 0CAh, 002h, 04Eh, 0E0h
        db  0F0h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ch, 000h, 0DEh, 002h, 0DCh, 002h, 0DAh, 002h
        db  0D8h, 002h, 0D6h, 002h, 0D4h, 002h, 0D2h, 002h, 0D0h, 002h, 0CEh, 002h, 0CCh, 002h, 0CAh, 002h
        db  0C8h, 002h, 04Eh, 0E0h, 0F0h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ch, 000h, 0DEh, 002h
        db  0DCh, 002h, 0DAh, 002h, 0D8h, 002h, 0D6h, 002h, 0D4h, 002h, 0D2h, 002h, 0D0h, 002h, 0CEh, 002h
        db  0CCh, 002h, 0CAh, 002h, 0C8h, 002h, 042h, 060h, 018h, 003h, 000h, 000h, 000h, 000h, 000h, 000h
        db  004h, 000h, 0C0h, 006h, 0C8h, 006h, 0D0h, 006h, 0D8h, 006h, 07Fh, 067h, 000h, 000h, 000h, 000h
        db  01Fh, 001h, 000h, 000h, 010h, 000h, 0C0h, 002h, 0C2h, 002h, 0C4h, 002h, 0C6h, 002h, 0C8h, 002h
        db  0CAh, 002h, 0CCh, 002h, 0CEh, 002h, 0D0h, 002h, 0D2h, 002h, 0D4h, 002h, 0D6h, 002h, 0D8h, 002h
        db  0DAh, 002h, 0DCh, 002h, 0DEh, 002h, 001h, 0E0h, 000h, 000h, 000h, 000h, 01Eh, 001h, 000h, 000h
        db  010h, 000h, 0C0h, 002h, 0C2h, 002h, 0C4h, 002h, 0C6h, 002h, 0C8h, 002h, 0CAh, 002h, 0CCh, 002h
        db  0CEh, 002h, 0D0h, 002h, 0D2h, 002h, 0D4h, 002h, 0D6h, 002h, 0D8h, 002h, 0DAh, 002h, 0DCh, 002h
        db  0DEh, 002h, 001h, 0E0h, 01Eh, 001h, 00Ch, 006h, 000h, 000h, 000h, 000h, 039h, 000h, 0C0h, 002h
        db  0C2h, 002h, 0C4h, 002h, 0C6h, 002h, 0C8h, 002h, 0CAh, 002h, 0CCh, 002h, 0CEh, 002h, 0D0h, 002h
        db  0D2h, 002h, 0D4h, 002h, 0D6h, 002h, 0D8h, 002h, 0DAh, 002h, 0DCh, 002h, 0DEh, 002h, 0C0h, 004h
        db  000h, 000h, 0C4h, 004h, 000h, 000h, 0C8h, 004h, 000h, 000h, 0CCh, 004h, 000h, 000h, 0D0h, 004h
        db  000h, 000h, 0D4h, 004h, 000h, 000h, 0D8h, 004h, 000h, 000h, 0DCh, 004h, 000h, 000h, 0C0h, 008h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0C8h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 008h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 010h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 010h
        db  0F0h, 0EFh, 000h, 000h, 00Fh, 000h, 001h, 002h, 003h, 004h, 039h, 000h, 0C0h, 002h, 0C2h, 002h
        db  0C4h, 002h, 0C6h, 002h, 0C8h, 002h, 0CAh, 002h, 0CCh, 002h, 0CEh, 002h, 0D0h, 002h, 0D2h, 002h
        db  0D4h, 002h, 0D6h, 002h, 0D8h, 002h, 0DAh, 002h, 0DCh, 002h, 0DEh, 002h, 0C0h, 004h, 000h, 000h
        db  0C4h, 004h, 000h, 000h, 0C8h, 004h, 000h, 000h, 0CCh, 004h, 000h, 000h, 0D0h, 004h, 000h, 000h
        db  0D4h, 004h, 000h, 000h, 0D8h, 004h, 000h, 000h, 0DCh, 004h, 000h, 000h, 0C0h, 008h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0C8h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 008h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0D8h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 010h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 010h, 060h, 060h
        db  000h, 000h, 047h, 006h, 000h, 000h, 000h, 000h, 01Eh, 000h, 000h, 000h, 0C0h, 004h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0C4h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0C8h, 004h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0CCh, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 004h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0D4h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 004h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0DCh, 004h, 04Eh, 070h, 0F0h, 004h, 000h, 000h, 000h, 000h, 000h, 000h
        db  00Fh, 000h, 0D0h, 010h, 0D4h, 008h, 0D8h, 008h, 0D0h, 004h, 0D0h, 004h, 0D4h, 004h, 0D0h, 004h
        db  0D0h, 004h, 0D4h, 004h, 0D4h, 004h, 0D8h, 004h, 0D8h, 004h, 0CCh, 004h, 0D8h, 004h, 0C0h, 004h
        db  04Eh, 070h, 0F0h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 0D8h, 004h, 0DCh, 004h, 0DCh, 004h, 0CCh, 004h, 0C0h, 004h, 0C0h, 004h, 0CCh, 004h
        db  0C0h, 004h, 0CCh, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0CCh, 004h, 04Fh, 07Fh, 000h, 000h
        db  000h, 000h, 000h, 000h, 007h, 000h, 006h, 000h, 0C8h, 003h, 0CCh, 003h, 0D0h, 003h, 0D4h, 003h
        db  0D8h, 003h, 0DCh, 003h, 070h, 08Fh, 000h, 000h, 00Fh, 000h, 0E0h, 001h, 000h, 000h, 041h, 000h
        db  0C0h, 002h, 0C2h, 002h, 0C4h, 002h, 0C6h, 002h, 0C8h, 002h, 0CAh, 002h, 0CCh, 002h, 0CEh, 002h
        db  0D0h, 002h, 0D2h, 002h, 0D4h, 002h, 0D6h, 002h, 0D8h, 002h, 0DAh, 002h, 0DCh, 002h, 0DEh, 002h
        db  0C0h, 004h, 000h, 000h, 0C4h, 004h, 000h, 000h, 0C8h, 004h, 000h, 000h, 0CCh, 004h, 000h, 000h
        db  0D0h, 004h, 000h, 000h, 0D4h, 004h, 000h, 000h, 0D8h, 004h, 000h, 000h, 0DCh, 004h, 000h, 000h
        db  0C0h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 0C8h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0D0h, 008h, 000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 008h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C0h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0D0h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0C0h, 020h, 014h, 0E0h, 004h, 000h, 000h, 000h, 001h, 000h, 001h, 007h, 006h, 000h, 0C0h, 008h
        db  0D0h, 008h, 0C8h, 008h, 0D8h, 008h, 0C0h, 010h, 0D0h, 010h, 014h, 0E0h, 000h, 000h, 001h, 000h
        db  000h, 000h, 00Eh, 000h, 010h, 000h, 0C0h, 002h, 0D0h, 002h, 0C2h, 002h, 0D2h, 002h, 0C4h, 002h
        db  0D4h, 002h, 0C6h, 002h, 0D6h, 002h, 0C8h, 002h, 0D8h, 002h, 0CAh, 002h, 0DAh, 002h, 0CCh, 002h
        db  0DCh, 002h, 0CEh, 002h, 0DEh, 002h, 0F8h, 0FFh, 01Ch, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  008h, 000h, 0C0h, 004h, 0C4h, 004h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h
        db  0DCh, 004h, 074h, 0FAh, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 0C0h, 008h
        db  0D0h, 008h, 074h, 0FAh, 000h, 000h, 000h, 000h, 007h, 000h, 000h, 000h, 008h, 000h, 0C0h, 004h
        db  0C4h, 004h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 0C0h, 06Fh
        db  000h, 000h, 01Ch, 002h, 000h, 000h, 000h, 000h, 008h, 000h, 0C0h, 004h, 0C4h, 004h, 0C8h, 004h
        db  0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 0C0h, 06Fh, 000h, 000h, 000h, 000h
        db  01Fh, 000h, 000h, 000h, 01Dh, 000h, 0C0h, 004h, 0C0h, 008h, 000h, 000h, 0C0h, 010h, 0C4h, 004h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0C8h, 004h, 0C8h, 008h, 000h, 000h, 000h, 000h, 0CCh, 004h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 004h, 0D0h, 008h, 000h, 000h, 0D0h, 010h, 0D4h, 004h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 004h, 0D8h, 008h, 000h, 000h, 000h, 000h, 0DCh, 004h
        db  0C1h, 06Fh, 000h, 000h, 01Ch, 002h, 000h, 000h, 000h, 000h, 008h, 000h, 0C0h, 004h, 0C4h, 004h
        db  0C8h, 004h, 0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 0C1h, 06Fh, 000h, 000h
        db  000h, 000h, 01Fh, 000h, 000h, 000h, 01Dh, 000h, 0C0h, 004h, 0C0h, 008h, 000h, 000h, 0C0h, 010h
        db  0C4h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0C8h, 004h, 0C8h, 008h, 000h, 000h, 000h, 000h
        db  0CCh, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 004h, 0D0h, 008h, 000h, 000h, 0D0h, 010h
        db  0D4h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 004h, 0D8h, 008h, 000h, 000h, 000h, 000h
        db  0DCh, 004h, 0C2h, 06Fh, 000h, 000h, 01Ch, 002h, 000h, 000h, 000h, 000h, 008h, 000h, 0C0h, 004h
        db  0C4h, 004h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 0C2h, 06Fh
        db  000h, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 01Dh, 000h, 0C0h, 004h, 0C0h, 008h, 000h, 000h
        db  0C0h, 010h, 0C4h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0C8h, 004h, 0C8h, 008h, 000h, 000h
        db  000h, 000h, 0CCh, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0D0h, 004h, 0D0h, 008h, 000h, 000h
        db  0D0h, 010h, 0D4h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 004h, 0D8h, 008h, 000h, 000h
        db  000h, 000h, 0DCh, 004h, 0C8h, 061h, 000h, 000h, 000h, 000h, 00Eh, 001h, 000h, 000h, 008h, 000h
        db  0C0h, 004h, 0C4h, 004h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h, 0DCh, 004h
        db  0C8h, 061h, 000h, 000h, 01Fh, 000h, 000h, 000h, 000h, 000h, 01Fh, 000h, 0C0h, 002h, 0C2h, 002h
        db  0C4h, 002h, 0C6h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h, 004h, 000h, 000h
        db  0C4h, 004h, 000h, 000h, 0C8h, 004h, 000h, 000h, 0CCh, 004h, 000h, 000h, 0D0h, 004h, 000h, 000h
        db  0D4h, 004h, 000h, 000h, 0D8h, 004h, 000h, 000h, 0DCh, 004h, 0C9h, 061h, 000h, 000h, 000h, 000h
        db  00Eh, 001h, 000h, 000h, 008h, 000h, 0C0h, 004h, 0C4h, 004h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h
        db  0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 0C9h, 061h, 000h, 000h, 01Fh, 000h, 000h, 000h, 000h, 000h
        db  01Fh, 000h, 0C0h, 002h, 0C2h, 002h, 0C4h, 002h, 0C6h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 0C0h, 004h, 000h, 000h, 0C4h, 004h, 000h, 000h, 0C8h, 004h, 000h, 000h, 0CCh, 004h
        db  000h, 000h, 0D0h, 004h, 000h, 000h, 0D4h, 004h, 000h, 000h, 0D8h, 004h, 000h, 000h, 0DCh, 004h
        db  0EFh, 06Dh, 038h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 007h, 000h, 0C0h, 010h, 0D0h, 010h
        db  0E0h, 010h, 000h, 000h, 0C8h, 008h, 0D8h, 008h, 0E8h, 008h, 027h, 061h, 0C0h, 006h, 000h, 000h
        db  000h, 000h, 000h, 000h, 004h, 000h, 0C8h, 002h, 0CAh, 002h, 0CEh, 002h, 0DEh, 002h, 0E5h, 0EFh
        db  000h, 000h, 01Ch, 002h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 000h, 000h, 0C8h, 004h
        db  0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 01Fh, 060h, 00Eh, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 008h, 000h, 0C0h, 004h, 0C4h, 004h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h
        db  0D4h, 004h, 0D8h, 004h, 0DCh, 004h, 01Fh, 062h, 00Eh, 001h, 000h, 000h, 000h, 000h, 000h, 000h
        db  008h, 000h, 000h, 000h, 000h, 000h, 0C8h, 004h, 0CCh, 004h, 0D0h, 004h, 0D4h, 004h, 0D8h, 004h
        db  0DCh, 004h, 000h, 000h, 000h, 000h
loc_4_3040:
        inc    esp
        jne    loc_4_30A4
        insb   byte ptr es:[edi], dx
        inc    esp
        imul   esi, dword ptr [ebx + 0x70], 0x79616c
loc_4_304C:
        dec    ecx
        inc    edi
        dec    esi
        dec    edi
        push   edx
        inc    ebp
        dec    ecx
        dec    esi
        push   ebx
        push   esp
        inc    ecx
        dec    esp
        dec    esp
        inc    ebp
        inc    esp
        inc    ebp
        dec    ebp
        dec    ebp
        add    byte ptr [esi + 0x4f], cl
        inc    ebp
        dec    ebp
        dec    ebp
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [ebp + 0x4d], al
        dec    ebp
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [ebp + 0x4d], al
        dec    ebp
        push   eax
        inc    ecx
        inc    edi
        inc    ebp
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        add    byte ptr [ebp + 0x4d], al
        dec    ebp
        inc    ebp
        pop    eax
        inc    ebx
        dec    esp
        push   ebp
        inc    esp
        inc    ebp
        add    byte ptr [ebp + 0x4d], al
        dec    ebp
        dec    ecx
        dec    esi
        inc    ebx
        dec    esp
        push   ebp
        inc    esp
        inc    ebp
        add    byte ptr [edx + 0x45], dl
        push   ebx
        inc    ebp
        push   edx
        push   esi
        inc    ebp
        inc    esp
        dec    eax
        dec    ecx
        inc    edi
        dec    eax
        inc    ecx
        push   edx
loc_4_30A4:
        inc    ebp
        inc    ecx
        add    byte ptr [ebp + 0x53], dl
        inc    ebp
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        dec    eax
        dec    ecx
        inc    edi
        dec    eax
        inc    ecx
        push   edx
        inc    ebp
        inc    ecx
        add    byte ptr [ecx + 0x4c], al
        dec    esp
        pop    eax
        dec    ebp
        push   ebx
        dec    esp
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        inc    esp
        add    byte ptr [ecx + 0x4c], al
        dec    esp
        inc    ebp
        dec    ebp
        push   ebx
        dec    esp
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        inc    esp
        add    byte ptr [ebx + 0x59], dl
        push   ebx
        push   esi
        dec    ebp
        inc    ebp
        dec    ebp
        push   ebx
        dec    esp
        dec    ecx
        dec    ebp
        dec    ecx
        push   esp
        add    byte ptr [ebx + 0x59], dl
        push   ebx
        push   esi
        dec    ebp
        inc    ebp
        dec    ebp
        push   ebx
        push   edx
        inc    ebp
        push   ecx
        push   ebp
        dec    ecx
        push   edx
        inc    ebp
        inc    esp
        add    byte ptr [ebx + 0x59], dl
        push   ebx
        push   esi
        dec    ebp
        inc    ebp
        dec    ebp
        push   ebx
        dec    esp
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        inc    esp
        add    byte ptr [ebx + 0x59], dl
        push   ebx
        push   esi
        dec    ebp
        pop    eax
        dec    ebp
        push   ebx
        dec    esp
        dec    ecx
        dec    ebp
        dec    ecx
        push   esp
        add    byte ptr [ebx + 0x59], dl
        push   ebx
        push   esi
        dec    ebp
        pop    eax
        dec    ebp
        push   ebx
        push   edx
        inc    ebp
        push   ecx
        push   ebp
        dec    ecx
        push   edx
        inc    ebp
        inc    esp
        add    byte ptr [ebx + 0x59], dl
        push   ebx
        push   esi
        dec    ebp
        pop    eax
        dec    ebp
        push   ebx
        dec    esp
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        inc    esp
        add    byte ptr [eax + 0x4d], bl
        push   ebx
        dec    ebp
        popal  
        js     loc_4_3180
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ecx + 0x6c], sp
        dec    eax
        popal  
        outsb  dx, byte ptr [esi]
        insb   byte ptr es:[edi], dx
        jae    loc_4_3140
loc_4_3140:
        push   edx
        inc    ebp
        push   ebx
        inc    ebp
        push   edx
        push   esi
        inc    ebp
        push   eax
        inc    ecx
        inc    edi
        inc    ebp
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        db  000h
loc_4_3151:
        pop    eax
        dec    ebp
        push   ebx
        push   ebp
        dec    ebp
        inc    edx
        dec    ecx
        dec    esi
        dec    ecx
        push   esp
        inc    ebx
        inc    ecx
        dec    esp
        dec    esp
        push   ebx
        add    byte ptr [ecx + 0x32], al
        xor    byte ptr [ebp + 0x4e], al
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        inc    ebx
        dec    edi
        push   ebp
        dec    esi
        push   esp
        db  000h
loc_4_3170:
        inc    esi
        outsd  dx, dword ptr [esi]
        jb     0x31d7
        inc    ecx
        xor    dh, byte ptr [eax]
        dec    esp
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ecx + 0x6c], sp
        add    byte ptr [eax + 0x45], dl
loc_4_3180:
        push   edx
        inc    esi
        dec    edi
        push   edx
        dec    ebp
        inc    edx
        inc    ecx
        inc    ebx
        dec    ebx
        inc    esi
        dec    ecx
        dec    esp
        dec    esp
        add    byte ptr [eax + 0x6c], bl
        popal  
        je     0x31d5
        jne    loc_4_31FB
        jb     0x31ec
        imul   edi, dword ptr [edx + 0x65], 0x65735500
        push   edx
        inc    ecx
        dec    ebp
        outsw  dx, word ptr [esi]
        jb     loc_4_31F5
        push   ebp
        dec    esp
        jo     loc_4_320C
        add    byte ptr gs:[bp + si + 0x4f], dl
        dec    ebp
        push   ebx
        arpl   word ptr [ecx + 0x6e], sp
        push   esp
        push   0x68736572
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        add    byte ptr fs:[edi + ebp*2 + 0x6b], dl
        outsb  dx, byte ptr gs:[esi]
        push   edx
        imul   ebp, dword ptr [esi + 0x67], 0x72616553
        arpl   word ptr [eax], bp
loc_4_31CF:
        outsb  dx, byte ptr [esi]
        je     loc_4_324A
        outsd  dx, dword ptr [esi]
        jb     loc_4_3241
        add    byte ptr [eax], al
loc_4_31D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_31E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_31E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_31E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 0C0h
loc_4_31F2:
        cmp    dword ptr [eax], eax
        db  03Bh
loc_4_31F5:
        add    byte ptr [ebp + 0x4d], al
        dec    ebp
        pop    eax
        pop    eax
loc_4_31FB:
        pop    eax
        pop    eax
        xor    byte ptr [ebx], al
        dec    ebp
        dec    ecx
        inc    ebx
        push   edx
        dec    edi
        push   ebx
        dec    edi
        inc    esi
        push   esp
        and    byte ptr [ebp + 0x58], al
loc_4_320C:
        push   eax
        inc    ecx
        dec    esi
        inc    esp
        inc    ebp
        inc    esp
        and    byte ptr [ebp + 0x45], cl
        dec    ebp
        dec    edi
        push   edx
        pop    ecx
        and    byte ptr [ebp + 0x41], cl
        dec    esi
        inc    ecx
        inc    edi
        inc    ebp
        push   edx
        and    byte ptr [ebx], dh
        cmp    byte ptr [esi], dh
        nop    
        retf   
        db  090h, 0CBh, 0FFh, 0FFh, 0FFh, 0FAh, 090h, 0CFh, 090h, 0CBh, 0EBh, 003h, 090h, 090h, 090h, 090h
        db  000h, 000h, 000h, 000h
loc_4_323B:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_3241:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x49], cl
        inc    ebx
        push   edx
        dec    edi
        push   ebx
loc_4_324A:
        dec    edi
        inc    esi
        push   esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebp + 0x4d], al
        dec    ebp
        xor    edi, dword ptr [eax]
        and    byte ptr ss:[eax], al
loc_4_3260:
        add    dword ptr [ecx], eax
loc_4_3262:
        add    byte ptr [eax], al
loc_4_3264:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_326C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_3270:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_3274:
        adc    al, 0
loc_4_3276:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_327C:
        lahf   
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_3281:
        add    byte ptr [eax], al
        add    byte ptr [ebx + eax], ch      ; -> obj15:0x8000032C
        add    byte ptr [eax - 0x7ffffcba], al ; -> obj15:0x80000346
        pop    ebx
        add    eax, dword ptr [eax]
        add    byte ptr [edi + 0x800003], 0  ; -> obj15:0x80000387
        add    byte ptr [eax], al
loc_4_3298:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_329C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_32A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x14  R-X
V86MMGR_Get_Xlat_Buff_State:
        mov    eax, dword ptr [0x11fc]       ; -> obj1:0x11FC
        add    eax, ebx
        mov    ecx, dword ptr [eax + 0x14]
        mov    edx, dword ptr [eax + 0x18]
        movzx  eax, word ptr [eax + 0x1e]
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x2ec  RW-
        db  000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_6_001C:
        fcom   qword ptr [eax + eax]
        add    cl, ah                        ; -> loc_9_14E1
        adc    al, 0
        add    byte ptr [ebp + 0x15], dh     ; -> loc_9_1575
        add    byte ptr [eax], al
        jle    0x40
        add    byte ptr [eax], al
        inc    esp
        sbb    dword ptr [eax], eax
        add    ch, cl                        ; -> loc_9_19CD
        sbb    dword ptr [eax], eax
        add    al, bl                        ; -> loc_9_14D8
        adc    al, 0
        add    byte ptr [ebx], dl            ; -> loc_9_1B13
        sbb    eax, dword ptr [eax]
        add    dl, al                        ; -> loc_9_1BC2
        sbb    eax, dword ptr [eax]
        add    byte ptr [edi + 0x1c], dl     ; -> loc_9_1C57
        add    byte ptr [eax], al
        push   edi
        sbb    al, 0
        add    byte ptr [esp + ebx], bl      ; -> loc_9_1C5C
        add    byte ptr [esi - 0x31ffffe4], al ; -> loc_9_1C86
        sbb    al, 0
        add    byte ptr [0xe00001d], ch      ; -> loc_9_1D2D
        push   ds
        add    byte ptr [eax], al
        dd  loc_9_1FC7               ; fixup
        dd  loc_9_202E               ; fixup
        dd  loc_9_24A0               ; fixup
        dd  loc_9_24BE               ; fixup
        dd  loc_9_2596               ; fixup
        dd  loc_9_265B               ; fixup
        dd  loc_9_26E1               ; fixup
        dd  loc_9_2857               ; fixup
        dd  loc_9_2AF0               ; fixup
        dd  loc_9_2B9C               ; fixup
        dd  loc_9_167E               ; fixup
        dd  loc_9_2BF9               ; fixup
        dd  loc_9_2E69               ; fixup
        dd  loc_9_2E6E               ; fixup
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
        db  000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0x296  R-X
        db  00Dh, 00Ah, 045h, 052h, 052h, 04Fh, 052h, 03Ah, 020h, 043h, 061h, 06Eh, 06Eh, 06Fh, 074h, 020h
        db  064h, 069h, 073h, 061h, 062h, 06Ch, 065h, 020h, 074h, 068h, 065h, 020h, 069h, 06Eh, 073h, 074h
        db  061h, 06Ch, 06Ch, 065h, 064h, 020h, 033h, 038h, 036h, 020h, 065h, 078h, 070h, 061h, 06Eh, 064h
        db  065h, 064h, 02Dh, 06Dh, 065h, 06Dh, 06Fh, 072h, 079h, 020h, 064h, 072h, 069h, 076h, 065h, 072h
        db  02Eh, 00Dh, 00Ah, 024h, 00Dh, 00Ah, 045h, 052h, 052h, 04Fh, 052h, 03Ah, 020h, 041h, 070h, 070h
        db  06Ch, 069h, 063h, 061h, 074h, 069h, 06Fh, 06Eh, 073h, 020h, 061h, 072h, 065h, 020h, 061h, 06Ch
        db  072h, 065h, 061h, 064h, 079h, 020h, 075h, 073h, 069h, 06Eh, 067h, 020h, 065h, 078h, 070h, 061h
        db  06Eh, 064h, 065h, 064h, 020h, 06Dh, 065h, 06Dh, 06Fh, 072h, 079h, 02Eh, 00Dh, 00Ah, 024h, 00Dh
        db  00Ah, 045h, 052h, 052h, 04Fh, 052h, 03Ah, 020h, 043h, 061h, 06Eh, 06Eh, 06Fh, 074h, 020h, 069h
        db  06Eh, 073h, 074h, 061h, 06Ch, 06Ch, 020h, 074h, 068h, 065h, 020h, 033h, 038h, 036h, 020h, 065h
        db  06Eh, 068h, 061h, 06Eh, 063h, 065h, 064h, 020h, 06Dh, 06Fh, 064h, 065h, 020h, 065h, 078h, 070h
        db  061h, 06Eh, 064h, 065h, 064h, 02Dh, 06Dh, 065h, 06Dh, 06Fh, 072h, 079h, 020h, 064h, 072h, 069h
        db  076h, 065h, 072h, 02Eh, 000h, 000h, 045h, 04Dh, 04Dh, 058h, 058h, 058h, 058h, 030h, 000h, 04Dh
        db  049h, 043h, 052h, 04Fh, 053h, 04Fh, 046h, 054h, 020h, 045h, 058h, 050h, 041h, 04Eh, 044h, 045h
        db  044h, 020h, 04Dh, 045h, 04Dh, 04Fh, 052h, 059h, 020h, 04Dh, 041h, 04Eh, 041h, 047h, 045h, 052h
        db  020h, 033h, 038h, 036h, 043h, 04Fh, 04Dh, 050h, 041h, 051h, 020h, 045h, 058h, 050h, 041h, 04Eh
        db  044h, 045h, 044h, 020h, 04Dh, 045h, 04Dh, 04Fh, 052h, 059h, 020h, 04Dh, 041h, 04Eh, 041h, 047h
        db  045h, 052h, 020h, 033h, 038h, 036h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 000h
        db  000h, 033h, 0C0h, 08Eh, 0D8h, 0A1h, 09Eh, 001h, 08Eh, 0D8h, 0BEh, 014h, 000h, 0FCh, 0BFh, 0CFh
        db  000h, 00Eh, 007h, 0B9h, 025h, 000h, 0FCh, 0F3h, 0A6h, 074h, 00Eh, 0BEh, 014h, 000h, 0BFh, 0F4h
        db  000h, 0B9h, 022h, 000h, 0FCh, 0F3h, 0A6h, 075h, 011h, 033h, 0F6h, 02Eh, 08Ch, 01Eh, 018h, 001h
        db  08Bh, 04Ch, 012h, 02Eh, 089h, 00Eh, 016h, 001h, 0F8h, 0C3h, 0F9h, 0EBh, 0FCh, 033h, 0C0h, 02Eh
        db  0FFh, 01Eh, 016h, 001h, 0F6h, 0C4h, 001h, 075h, 00Ah, 0B8h, 001h, 001h, 02Eh, 0FFh, 01Eh, 016h
        db  001h, 072h, 002h, 0F8h, 0C3h, 0F9h, 0EBh, 0FCh, 033h, 0C0h, 02Eh, 0FFh, 01Eh, 016h, 001h, 0F6h
        db  0C4h, 001h, 075h, 026h, 0B8h, 000h, 046h, 0CDh, 067h, 080h, 0FCh, 000h, 075h, 01Ch, 08Bh, 0C8h
        db  0B8h, 000h, 04Bh, 0CDh, 067h, 080h, 0FCh, 000h, 075h, 010h, 080h, 0F9h, 040h, 072h, 005h, 00Bh
        db  0DBh, 074h, 001h, 04Bh, 083h, 0FBh, 000h, 0F9h, 075h, 001h, 0F8h, 0C3h, 033h, 0C0h, 08Eh, 0D8h
        db  0A1h, 09Eh, 001h, 08Eh, 0D8h, 0BEh, 00Ah, 000h, 0BFh, 0C6h, 000h, 00Eh, 007h, 0B9h, 008h, 000h
        db  0FCh, 0F3h, 0A6h, 075h, 002h, 0F8h, 0C3h, 0F9h, 0EBh, 0FCh, 0E8h, 0DFh, 0FFh, 00Fh, 082h, 09Ah
        db  000h, 0B2h, 001h, 0B8h, 007h, 033h, 0CDh, 021h, 00Eh, 01Fh, 0BAh, 0C6h, 000h, 0B8h, 000h, 03Dh
        db  0CDh, 021h, 09Ch, 050h, 0B2h, 000h, 0B8h, 007h, 033h, 0CDh, 021h, 058h, 09Dh, 073h, 00Ah, 083h
        db  0F8h, 004h, 075h, 04Bh, 0BAh, 07Fh, 000h, 0EBh, 064h, 08Bh, 0D8h, 0B8h, 000h, 044h, 0CDh, 021h
        db  072h, 038h, 0F7h, 0C2h, 080h, 000h, 074h, 032h, 0F7h, 0C2h, 000h, 040h, 074h, 02Ch, 0BAh, 01Ah
        db  001h, 0B9h, 006h, 000h, 0B8h, 002h, 044h, 0CDh, 021h, 072h, 01Fh, 03Bh, 0C1h, 075h, 01Bh, 08Bh
        db  0F2h, 08Bh, 044h, 004h, 086h, 0E0h, 03Dh, 000h, 001h, 072h, 00Fh, 03Dh, 063h, 001h, 077h, 00Ah
        db  066h, 08Bh, 014h, 0B8h, 000h, 03Eh, 0CDh, 021h, 0EBh, 034h, 0B8h, 000h, 03Eh, 0CDh, 021h, 066h
        db  0BAh, 002h, 000h, 000h, 000h, 0E8h, 0D9h, 0FEh, 072h, 024h, 0E8h, 02Bh, 0FFh, 072h, 017h, 066h
        db  0BAh, 001h, 000h, 000h, 000h, 0E8h, 005h, 0FFh, 073h, 014h, 0BAh, 000h, 000h, 00Eh, 01Fh, 0B8h
        db  000h, 009h, 0CDh, 021h, 0EBh, 011h, 0BAh, 044h, 000h, 0EBh, 0F2h, 066h, 033h, 0D2h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 000h, 000h, 0F8h, 0C3h, 0F9h, 0C3h, 000h, 0F7h, 0C3h, 001h, 000h, 075h, 011h
        db  0E8h, 047h, 0FFh, 072h, 001h, 0C3h, 0B8h, 002h, 080h, 033h, 0DBh, 066h, 033h, 0D2h, 033h, 0F6h
        db  0C3h, 0B8h, 001h, 080h, 0EBh, 0F3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 8  vsize=0x210  R-X
loc_8_0000:
        mov    eax, dword ptr [0x98]         ; -> obj6:0x0098
        or     eax, eax
        je     loc_8_0027
        mov    ecx, 0x692
        add    ecx, eax
        dec    ecx
        shr    eax, 0xc
        shr    ecx, 0xc
        sub    ecx, eax
        inc    ecx
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
loc_8_0027:
        call   sub_8_004D
        ret    
        db  000h, 000h, 000h
sub_8_0030:
        add    ecx, eax
        dec    ecx
        shr    eax, 0xc
        shr    ecx, 0xc
        sub    ecx, eax
        inc    ecx
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        cmp    eax, 1
        ret    
sub_8_004D:
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        or     esi, esi
        je     loc_8_0090
        mov    eax, dword ptr [0x12b4]       ; -> obj1:0x12B4
        imul   ecx, dword ptr [0x12b8], 0x10 ; -> obj1:0x12B8
        call   sub_8_0030
        jb     loc_8_008C
        mov    eax, dword ptr [0x12bc]       ; -> obj1:0x12BC
        mov    ecx, dword ptr [0x12ac]       ; -> obj1:0x12AC
        shl    ecx, 1
        call   sub_8_0030
        jb     loc_8_008C
        mov    eax, dword ptr [esi + ebx + 0x60]
        push   0x20
        pop    ecx
        call   sub_8_0030
        jae    loc_8_0090
loc_8_008C:
        mov    dword ptr [esi + ebx + 0x60], eax
loc_8_0090:
        clc    
        ret    
loc_8_0092:
        pushal 
        xor    al, al
        xchg   byte ptr [0x1327], al         ; -> obj1:0x1327
        or     al, al
        je     loc_8_00AA
        mov    eax, dword ptr [0x3298]       ; -> loc_4_3298
        VMMCall  VMM.Remove_V86_Break_Point      ; cd200a000100
loc_8_00AA:
        call   sub_8_004D
        test   eax, eax
        je     loc_8_019E
        sub    esp, 0x6c
        mov    edi, esp
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    ebp, dword ptr [ebx + 8]
        cmp    byte ptr [0x1321], 0          ; -> obj1:0x1321
        je     loc_8_0103
        mov    esi, 0x12c0                   ; -> obj1:0x12C0
        movzx  ecx, byte ptr [0x1320]        ; -> obj1:0x1320
        jecxz  loc_8_0103
loc_8_00E2:
        movzx  eax, word ptr [esi + 4]
        mov    word ptr [ebp + 0x14], ax
        mov    byte ptr [ebp + 0x1d], 0x11
        mov    eax, dword ptr [0x129c]       ; -> obj1:0x129C
        call   sub_1_05F9
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        add    esi, 6
        loop   loc_8_00E2
loc_8_0103:
        cmp    dword ptr [0x127c], 0         ; -> obj1:0x127C
        jne    loc_8_013D
        mov    esi, dword ptr [0x12bc]       ; -> obj1:0x12BC
        mov    ecx, dword ptr [0x12ac]       ; -> obj1:0x12AC
        jecxz  loc_8_013D
loc_8_011A:
        cld    
        lodsw  ax, word ptr [esi]
        inc    ax
        je     loc_8_013B
        dec    ax
        mov    word ptr [ebp + 0x14], ax
        mov    byte ptr [ebp + 0x1d], 0xa
        mov    eax, dword ptr [0x129c]       ; -> obj1:0x129C
        call   sub_1_05F9
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
loc_8_013B:
        loop   loc_8_011A
loc_8_013D:
        mov    esi, dword ptr [0x12b4]       ; -> obj1:0x12B4
        mov    ecx, dword ptr [0x12b8]       ; -> obj1:0x12B8
        jecxz  loc_8_018D
loc_8_014B:
        test   byte ptr [esi + 0xc], 1
        je     loc_8_0188
        push   ecx
        movzx  ecx, word ptr [esi + 0xa]
        jecxz  loc_8_0181
loc_8_0158:
        mov    ax, word ptr [esi + 8]
        mov    word ptr [ebp + 0x14], ax
        mov    byte ptr [ebp + 0x1d], 0xd
        mov    eax, dword ptr [0x129c]       ; -> obj1:0x129C
        call   sub_1_05F9
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cmp    word ptr [ebp + 0x1c], 1
        jne    loc_8_0181
        dec    word ptr [esi + 0xa]
        loop   loc_8_0158
loc_8_0181:
        mov    word ptr [esi + 0xc], 0
        pop    ecx
loc_8_0188:
        add    esi, 0x10
        loop   loc_8_014B
loc_8_018D:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    esi, esp
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        add    esp, 0x6c
loc_8_019E:
        popal  
        clc    
        ret    
sub_8_01A1:
        pushfd 
        mov    eax, dword ptr [0x12a0]       ; -> obj1:0x12A0
        or     eax, eax
        je     loc_8_01CA
        cmp    dword ptr [0x1322], 0         ; -> obj1:0x1322
        je     loc_8_01CA
        cli    
        xor    ecx, ecx
        xchg   dword ptr [0x1322], ecx       ; -> obj1:0x1322
        mov    dword ptr [eax], ecx
        xor    cl, cl
        xchg   byte ptr [0x1326], cl         ; -> obj1:0x1326
        mov    byte ptr [eax + 4], cl
loc_8_01CA:
        mov    edx, dword ptr [0x1330]       ; -> obj1:0x1330
        inc    edx
        je     loc_8_020B
        dec    edx
        test   byte ptr [0x1204], 0x20       ; -> obj1:0x1204
        jne    loc_8_01E6
        test   byte ptr [0x1204], 0x40       ; -> obj1:0x1204
        je     loc_8_0202
loc_8_01E6:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        or     esi, esi
        je     loc_8_0202
        mov    edi, dword ptr [esi + ebx + 0x60]
        or     edi, edi
        je     loc_8_0202
        movzx  edx, word ptr [edi + 0x16]
loc_8_0202:
        xor    ecx, ecx
        dec    ecx
        VMMCall  VMM.GetSet_HMA_Info             ; cd20bc000100
loc_8_020B:
        popfd  
        clc    
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 9  vsize=0x3505  R-X
loc_9_0000:
        test   byte ptr [ebx + 1], 1
        jne    loc_9_0034
        push   eax
        movzx  eax, byte ptr [edx]
        push   eax
        cmp    eax, 0x24
        jb     loc_9_0013
        add    eax, 0x24
loc_9_0013:
        push   dword ptr [eax + ebp]
        inc    edx
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006000600
        pop    edx
        xchg   word ptr [eax + ebp], dx
        movzx  edx, dx
        shl    edx, 4
        call   sub_10_006E
        pop    edx
        mov    word ptr [edx + ebp], ax
        pop    eax
        ret    
loc_9_0034:
        ret    
V86MMGR_Set_Xlat_Buff_State:
        push   edi
        mov    edi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        add    edi, ebx
        mov    dword ptr [edi + 0x14], ecx
        mov    dword ptr [edi + 0x18], edx
        mov    word ptr [edi + 0x1e], ax
        push   eax
        movzx  eax, ax
        shl    eax, 4
        add    eax, dword ptr [ebx + 4]
        mov    dword ptr [edi + 0x20], eax
        pop    eax
        pop    edi
        ret    
V86MMGR_Get_VM_Flat_Sel:
        mov    eax, dword ptr [0x11fc]       ; -> obj1:0x11FC
        movzx  eax, word ptr [eax + ebx + 0x24]
        test   eax, eax
        je     loc_9_0067
        ret    
loc_9_0067:
        push   ecx
        push   edx
        push   0
        push   0
        push   0x92
        push   0xfffff
        push   dword ptr [ebx + 4]
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        VMMCall  VMM._Allocate_LDT_Selector      ; cd2078000100
        add    esp, 0x14
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    word ptr [edx + ebx + 0x24], ax
        pop    edx
        pop    ecx
        ret    
        db  000h, 000h, 000h
sub_9_00A4:
        enter  0x44, 0
        push   esi
        push   ebx
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        mov    dword ptr [0], eax            ; -> obj6:0x0000
        mov    edx, dword ptr [esi + ebx + 0x5c]
        mov    edi, edx
        or     edx, edx
        jne    loc_9_09A2
        cmp    dword ptr [8], 0              ; -> obj6:0x0008
        stc    
        je     loc_9_09A2
        test   byte ptr [esi + ebx + 0x2c], 2
        stc    
        jne    loc_9_09A2
        push   0x101
        push   0x692
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        stc    
        je     loc_9_09A2
        mov    dword ptr [esi + ebx + 0x5c], eax
        lea    edi, [eax + 0x240]
        mov    edx, eax
        xor    eax, eax
        dec    eax
        mov    ecx, 0x10
        cld    
        rep stosd dword ptr es:[edi], eax
        mov    edi, edx
        or     byte ptr [edi + 0x68e], 4
        push   0x101
        push   0x36
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_9_09DD
        mov    dword ptr [edi + 0x280], eax
        mov    edx, dword ptr [esi + ebx]
        mov    dword ptr [eax], edx
        push   eax
        push   0
        push   edx
        VMMCall  VMM._PageGetSizeAddr            ; cd2058000100
        pop    ecx
        pop    ecx
        pop    ecx
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ecx + 0x28], eax
        mov    byte ptr [ecx + 0x2c], 0x40
        mov    dword ptr [edi + 0x67c], eax
        add    dword ptr [0xc], eax          ; -> obj6:0x000C
        lea    esi, [ebp - 0x44]
        push   0
        push   esi
        push   ebx
        VMMCall  VMM._Get_Device_V86_Pages_Array ; cd2074000100
        add    esp, 0xc
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        mov    dword ptr [ebp - 0x1c], eax
        mov    edx, eax
        add    eax, 3
        and    eax, 0xfffffffc
        mov    ecx, eax
        sub    ecx, edx
        mov    dword ptr [ebp - 4], ecx
        or     ecx, ecx
        je     loc_9_02D7
        push   eax
        push   ecx
        xor    eax, eax
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        test   byte ptr [edx + ebx + 0x2e], 1
        je     loc_9_01A4
        or     eax, 0x80
loc_9_01A4:
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        pop    ecx
        or     eax, eax
        je     loc_9_09DC
        push   edi
        push   ecx
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    dword ptr [esi + ebx + 4], eax
        push   edx
        mov    edx, dword ptr [edi + 0x280]
        pop    edi
        mov    esi, dword ptr [edx + 4]
        shl    ecx, 0xa
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    ecx
        pop    edi
        call   sub_9_0EA5
        jb     loc_9_09A6
        push   ecx
        push   0
        push   0
        push   ecx
        push   dword ptr [ebp - 0x1c]
        push   ebx
        push   eax
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        pop    ecx
        or     eax, eax
        je     loc_9_09A6
        mov    edx, dword ptr [edi + 0x280]
        sub    dword ptr [edx + 0x28], ecx
        jae    loc_9_021C
        mov    dword ptr [edx + 0x28], 0
loc_9_021C:
        sub    dword ptr [edi + 0x67c], ecx
        jae    loc_9_022E
        mov    dword ptr [edi + 0x67c], 0
loc_9_022E:
        sub    dword ptr [0xc], ecx          ; -> obj6:0x000C
        jae    loc_9_0240
        mov    dword ptr [0xc], 0            ; -> obj6:0x000C
loc_9_0240:
        mov    eax, dword ptr [ebp - 0x1c]
        add    eax, ecx
        mov    dword ptr [ebp - 0x20], eax
        push   edi
        mov    esi, dword ptr [edx + 4]
        mov    edi, esi
        shl    ecx, 0xc
        add    esi, ecx
        mov    ecx, dword ptr [edx + 0x28]
        shl    ecx, 0xa
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        cmp    dword ptr [edx + 0x28], 0
        jne    loc_9_028D
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     loc_9_0276
        push   edx
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
        pop    edx
loc_9_0276:
        xor    eax, eax
        mov    ecx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    dword ptr [ecx + ebx], eax
        mov    dword ptr [edx], eax
        dec    eax
        mov    dword ptr [edx + 4], eax
        or     byte ptr [edx + 0x2c], 4
        jmp    loc_9_02D6
loc_9_028D:
        push   edx
        push   0
        push   dword ptr [edx + 0x28]
        push   dword ptr [edx]
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        mov    esi, edx
        pop    edx
        or     eax, eax
        je     loc_9_09DC
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        call   sub_9_0EA5
        jb     loc_9_09DC
        push   0
        push   0
        push   dword ptr [edx + 0x28]
        push   dword ptr [ebp - 0x20]
        push   ebx
        push   eax
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        or     eax, eax
        je     loc_9_09DC
loc_9_02D6:
        pop    eax
loc_9_02D7:
        mov    edx, dword ptr [edi + 0x280]
        lea    edx, [edi + 4]
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        lea    esi, [esi + ebx + 0x64]
loc_9_02EF:
        mov    byte ptr [edx + 2], 1
        add    edx, 8
        add    esi, 8
        loop   loc_9_02EF
        mov    ecx, dword ptr [edi + 0x280]
        mov    ecx, dword ptr [ecx + 0x28]
        xor    eax, eax
loc_9_0306:
        mov    word ptr [edx + 4], ax
        mov    byte ptr [edx + 2], 0x40
        cmp    ecx, 4
        jb     loc_9_0348
        test   byte ptr [esi + 1], 0xe
        jne    loc_9_0342
        test   byte ptr [esi + 3], 0xe
        jne    loc_9_0342
        test   byte ptr [esi + 5], 0xe
        jne    loc_9_0342
        test   byte ptr [esi + 7], 0xe
        jne    loc_9_0342
        mov    byte ptr [edx + 1], 4
loc_9_032F:
        add    esi, 8
        add    edx, 8
        add    eax, 4
        sub    ecx, 4
        ja     loc_9_0306
        jmp    loc_9_05A8
loc_9_0342:
        mov    byte ptr [edx + 2], 1
        jmp    loc_9_032F
loc_9_0348:
        mov    byte ptr [edx + 1], cl
        push   edx
        push   ecx
        lea    ecx, [edi + 4]
        sub    edx, ecx
        shr    edx, 1
        pop    eax
        add    edx, eax
        mov    dword ptr [ebp - 8], edx
        sub    eax, 4
        neg    eax
        mov    dword ptr [ebp - 0xc], eax
        mov    ecx, eax
        mov    eax, edx
        shr    eax, 5
        and    edx, 0x1f
        lea    esi, [ebp - 0x44]
loc_9_036F:
        bt     dword ptr [esi + eax*4], edx
        jb     loc_9_0389
loc_9_0375:
        inc    edx
        cmp    edx, 0x20
        jb     loc_9_037E
        xor    edx, edx
        inc    eax
loc_9_037E:
        loop   loc_9_036F
        pop    edx
loc_9_0381:
        add    edx, 8
        jmp    loc_9_05AF
loc_9_0389:
        push   edx
        push   eax
        shl    eax, 5
        add    eax, edx
        lea    eax, [ebx + eax*2 + 0x64]
        add    eax, dword ptr [0x11fc]       ; -> obj1:0x11FC
        test   byte ptr [eax + 1], 0xe
        je     loc_9_03A4
        xor    eax, eax
        jmp    loc_9_03A8
loc_9_03A4:
        test   byte ptr [eax + 1], 0x10
loc_9_03A8:
        pop    eax
        pop    edx
        jne    loc_9_0375
        pop    edx
        or     byte ptr [edx + 2], 1
        mov    dword ptr [ebp - 0xc], 0
        push   edx
        movzx  ecx, byte ptr [edx + 1]
        xor    eax, eax
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        test   byte ptr [edx + ebx + 0x2e], 1
        je     loc_9_03D1
        or     eax, 0x80
loc_9_03D1:
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        or     eax, eax
        je     loc_9_09DC
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    dword ptr [esi + ebx + 8], eax
        mov    esi, edx
        pop    edx
        push   edx
        push   eax
        push   edi
        push   esi
        movzx  ecx, byte ptr [edx + 1]
        mov    esi, dword ptr [edi + 0x280]
        sub    dword ptr [esi + 0x28], ecx
        jae    loc_9_0416
        mov    dword ptr [esi + 0x28], 0
loc_9_0416:
        sub    dword ptr [edi + 0x67c], ecx
        jae    loc_9_0428
        mov    dword ptr [edi + 0x67c], 0
loc_9_0428:
        sub    dword ptr [0xc], ecx          ; -> obj6:0x000C
        jae    loc_9_043A
        mov    dword ptr [0xc], 0            ; -> obj6:0x000C
loc_9_043A:
        pop    edi
        mov    eax, dword ptr [esi + 0x28]
        shl    eax, 0xc
        mov    esi, dword ptr [esi + 4]
        add    esi, eax
        shl    ecx, 0xa
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    eax
        pop    edx
        push   edx
        lea    ecx, [edi + 4]
        sub    edx, ecx
        shr    edx, 1
        mov    dword ptr [ebp - 8], edx
        pop    edx
        push   edx
        movzx  ecx, byte ptr [edx + 1]
        call   sub_9_0EA5
        jb     loc_9_09C1
        push   0
        push   0
        push   ecx
        push   dword ptr [ebp - 8]
        push   ebx
        push   eax
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        or     eax, eax
        je     loc_9_09C1
        mov    esi, dword ptr [edi + 0x280]
        cmp    dword ptr [esi + 0x28], 0
        jne    loc_9_04BA
        mov    edx, dword ptr [esi]
        or     edx, edx
        je     loc_9_04A3
        push   0
        push   edx
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_9_04A3:
        xor    eax, eax
        mov    ecx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    dword ptr [ecx + ebx], eax
        mov    dword ptr [esi], eax
        dec    eax
        mov    dword ptr [esi + 4], eax
        or     byte ptr [esi + 0x2c], 4
        jmp    loc_9_04D7
loc_9_04BA:
        push   0
        push   dword ptr [esi + 0x28]
        push   dword ptr [esi]
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        or     eax, eax
        je     loc_9_09DC
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], edx
loc_9_04D7:
        pop    edx
        lea    esi, [ebp - 0x44]
        jmp    loc_9_0381
loc_9_04E0:
        push   edx
        push   eax
        shl    eax, 5
        add    eax, edx
        lea    eax, [ebx + eax*2 + 0x64]
        add    eax, dword ptr [0x11fc]       ; -> obj1:0x11FC
        test   byte ptr [eax + 1], 0xe
        je     loc_9_04F9
        jmp    loc_9_052C
loc_9_04F9:
        test   byte ptr [eax + 1], 0x10
        jne    loc_9_052E
        or     byte ptr [eax + 1], 8
        pop    edx
        pop    ecx
        push   ecx
        push   edx
        inc    ecx
        add    eax, 2
        and    ecx, 3
        je     loc_9_052E
        sub    ecx, 4
        neg    ecx
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        inc    ebx
loc_9_051B:
        test   byte ptr [eax + 1], 0x1e
        jne    loc_9_0525
        or     byte ptr [eax + 1], 8
loc_9_0525:
        inc    ebx
        add    eax, 2
        loop   loc_9_051B
        pop    ebx
loc_9_052C:
        xor    eax, eax
loc_9_052E:
        pop    eax
        pop    edx
        jne    loc_9_05D6
        pop    edx
        or     byte ptr [edx + 2], 1
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_9_062D
        push   edx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp - 0xc]
        mov    edi, dword ptr [ebp - 8]
        mov    edx, edi
        lea    edi, [ebx + edi*2 + 0x64]
        add    edi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        xor    ecx, ecx
loc_9_055C:
        bts    word ptr [edi], 0xc
        jae    loc_9_0567
        jecxz  loc_9_0580
        jmp    loc_9_056E
loc_9_0567:
        inc    ecx
        add    edi, 2
        dec    esi
        jne    loc_9_055C
loc_9_056E:
        push   0
        push   ebx
        push   ecx
        push   edx
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        or     esi, esi
        je     loc_9_0599
loc_9_0580:
        add    edi, 2
        dec    esi
        je     loc_9_0599
        xor    ecx, ecx
        mov    edx, edi
        mov    eax, dword ptr [0x11fc]       ; -> obj1:0x11FC
        lea    eax, [eax + ebx + 0x64]
        sub    edx, eax
        shr    edx, 1
        jmp    loc_9_055C
loc_9_0599:
        pop    edi
        pop    esi
        pop    edx
        mov    dword ptr [ebp - 0xc], 0
        jmp    loc_9_062D
loc_9_05A8:
        mov    dword ptr [ebp - 0xc], 0
loc_9_05AF:
        mov    eax, edx
        lea    ecx, [edi + 4]
        sub    eax, ecx
        shr    eax, 1
loc_9_05B8:
        mov    dword ptr [ebp - 0x10], eax
        push   edx
        mov    edx, eax
        shr    eax, 5
        and    edx, 0x1f
        mov    ecx, 4
        lea    esi, [ebp - 0x44]
loc_9_05CC:
        bt     dword ptr [esi + eax*4], edx
        jb     loc_9_04E0
loc_9_05D6:
        inc    edx
        cmp    edx, 0x20
        jb     loc_9_05DF
        xor    edx, edx
        inc    eax
loc_9_05DF:
        loop   loc_9_05CC
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_9_05ED
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 8], eax
loc_9_05ED:
        add    dword ptr [ebp - 0xc], 4
        pop    edx
        mov    byte ptr [edx + 2], 0x20
        mov    byte ptr [edx], 0xff
        mov    byte ptr [edx + 1], 4
        mov    word ptr [edx + 4], 0
        mov    eax, dword ptr [0]            ; -> obj6:0x0000
        cmp    dword ptr [ebp - 0x10], eax
        jb     loc_9_062D
        push   edx
        call   sub_9_0EA5
        push   0x10
        push   0
        push   4
        push   dword ptr [ebp - 0x10]
        push   ebx
        push   dword ptr [0x1200]            ; -> obj1:0x1200
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        pop    edx
loc_9_062D:
        add    edx, 8
        mov    eax, edx
        lea    ecx, [edi + 4]
        sub    eax, ecx
        shr    eax, 1
        cmp    eax, 0x100
        jb     loc_9_05B8
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_9_06A6
        push   esi
        push   edi
        mov    esi, dword ptr [ebp - 0xc]
        mov    edi, dword ptr [ebp - 8]
        mov    edx, edi
        lea    edi, [ebx + edi*2 + 0x64]
        add    edi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        xor    ecx, ecx
loc_9_0660:
        bts    word ptr [edi], 0xc
        jae    loc_9_066B
        jecxz  loc_9_0684
        jmp    loc_9_0672
loc_9_066B:
        inc    ecx
        add    edi, 2
        dec    esi
        jne    loc_9_0660
loc_9_0672:
        push   0
        push   ebx
        push   ecx
        push   edx
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        or     esi, esi
        je     loc_9_069D
loc_9_0684:
        add    edi, 2
        dec    esi
        je     loc_9_069D
        xor    ecx, ecx
        mov    edx, edi
        mov    eax, dword ptr [0x11fc]       ; -> obj1:0x11FC
        lea    eax, [eax + ebx + 0x64]
        sub    edx, eax
        shr    edx, 1
        jmp    loc_9_0660
loc_9_069D:
        pop    edi
        pop    esi
        mov    dword ptr [ebp - 0xc], 0
loc_9_06A6:
        cmp    dword ptr [0], 0xdf           ; -> obj6:0x0000
        jae    loc_9_0717
        mov    edx, 0xc0
        cmp    edx, dword ptr [0]            ; -> obj6:0x0000
        jae    loc_9_06E1
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        add    edx, 3
        shr    edx, 2
        shl    edx, 2
        mov    ecx, 0xe0
        sub    ecx, edx
        jbe    loc_9_0717
        cmp    ecx, 0x10
        jb     loc_9_0717
        shr    ecx, 2
        jmp    loc_9_06E6
loc_9_06E1:
        mov    ecx, 8
loc_9_06E6:
        lea    eax, [edi + 4]
        test   edx, 3
        stc    
        jne    loc_9_06F6
        shl    edx, 1
        add    edx, eax
loc_9_06F6:
        xor    eax, eax
loc_9_06F8:
        test   byte ptr [edx + 2], 1
        jne    loc_9_0710
        or     eax, eax
        jne    loc_9_0704
        mov    esi, edx
loc_9_0704:
        inc    eax
        cmp    eax, 4
        jae    loc_9_0871
        jmp    loc_9_0712
loc_9_0710:
        xor    eax, eax
loc_9_0712:
        add    edx, 8
        loop   loc_9_06F8
loc_9_0717:
        cmp    dword ptr [0], 0xff           ; -> obj6:0x0000
        jae    loc_9_0788
        mov    edx, 0xe0
        cmp    edx, dword ptr [0]            ; -> obj6:0x0000
        jae    loc_9_0752
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        add    edx, 3
        shr    edx, 2
        shl    edx, 2
        mov    ecx, 0x100
        sub    ecx, edx
        jbe    loc_9_0788
        cmp    ecx, 0x10
        jb     loc_9_0788
        shr    ecx, 2
        jmp    loc_9_0757
loc_9_0752:
        mov    ecx, 8
loc_9_0757:
        lea    eax, [edi + 4]
        test   edx, 3
        stc    
        jne    loc_9_0767
        shl    edx, 1
        add    edx, eax
loc_9_0767:
        xor    eax, eax
loc_9_0769:
        test   byte ptr [edx + 2], 1
        jne    loc_9_0781
        or     eax, eax
        jne    loc_9_0775
        mov    esi, edx
loc_9_0775:
        inc    eax
        cmp    eax, 4
        jae    loc_9_0871
        jmp    loc_9_0783
loc_9_0781:
        xor    eax, eax
loc_9_0783:
        add    edx, 8
        loop   loc_9_0769
loc_9_0788:
        cmp    dword ptr [0], 0xbf           ; -> obj6:0x0000
        jae    loc_9_07F9
        mov    edx, 0xa0
        cmp    edx, dword ptr [0]            ; -> obj6:0x0000
        jae    loc_9_07C3
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        add    edx, 3
        shr    edx, 2
        shl    edx, 2
        mov    ecx, 0xc0
        sub    ecx, edx
        jbe    loc_9_07F9
        cmp    ecx, 0x10
        jb     loc_9_07F9
        shr    ecx, 2
        jmp    loc_9_07C8
loc_9_07C3:
        mov    ecx, 8
loc_9_07C8:
        lea    eax, [edi + 4]
        test   edx, 3
        stc    
        jne    loc_9_07D8
        shl    edx, 1
        add    edx, eax
loc_9_07D8:
        xor    eax, eax
loc_9_07DA:
        test   byte ptr [edx + 2], 1
        jne    loc_9_07F2
        or     eax, eax
        jne    loc_9_07E6
        mov    esi, edx
loc_9_07E6:
        inc    eax
        cmp    eax, 4
        jae    loc_9_0871
        jmp    loc_9_07F4
loc_9_07F2:
        xor    eax, eax
loc_9_07F4:
        add    edx, 8
        loop   loc_9_07DA
loc_9_07F9:
        cmp    dword ptr [0], 0xff           ; -> obj6:0x0000
        jae    loc_9_0866
        mov    edx, 0xa0
        cmp    edx, dword ptr [0]            ; -> obj6:0x0000
        jae    loc_9_0834
        mov    edx, dword ptr [0]            ; -> obj6:0x0000
        add    edx, 3
        shr    edx, 2
        shl    edx, 2
        mov    ecx, 0x100
        sub    ecx, edx
        jbe    loc_9_0866
        cmp    ecx, 0x10
        jb     loc_9_0866
        shr    ecx, 2
        jmp    loc_9_0839
loc_9_0834:
        mov    ecx, 0x18
loc_9_0839:
        lea    eax, [edi + 4]
        test   edx, 3
        stc    
        jne    loc_9_0849
        shl    edx, 1
        add    edx, eax
loc_9_0849:
        xor    eax, eax
loc_9_084B:
        test   byte ptr [edx + 2], 1
        jne    loc_9_085F
        or     eax, eax
        jne    loc_9_0857
        mov    esi, edx
loc_9_0857:
        inc    eax
        cmp    eax, 4
        jae    loc_9_0871
        jmp    loc_9_0861
loc_9_085F:
        xor    eax, eax
loc_9_0861:
        add    edx, 8
        loop   loc_9_084B
loc_9_0866:
        mov    word ptr [edi + 0x68a], 0
        jmp    loc_9_0892
loc_9_0871:
        mov    edx, esi
        lea    ecx, [edi + 4]
        sub    edx, ecx
        shl    edx, 7
        mov    word ptr [edi + 0x68a], dx
        mov    edx, esi
        mov    ecx, 4
loc_9_0889:
        or     byte ptr [edx + 2], 0x10
        add    edx, 8
        loop   loc_9_0889
loc_9_0892:
        movzx  edx, word ptr [edi + 0x68a]
        or     edx, edx
        je     loc_9_08B3
        lea    ecx, [edi + 4]
        test   edx, 0x3ff
        stc    
        jne    loc_9_08AE
        shr    edx, 7
        add    edx, ecx
loc_9_08AE:
        jmp    loc_9_094A
loc_9_08B3:
        cmp    dword ptr [0x10], 0           ; -> obj6:0x0010
        je     loc_9_090F
        mov    edx, dword ptr [0x10]         ; -> obj6:0x0010
        shr    edx, 8
        lea    edx, [ebx + edx*2 + 0x64]
        add    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    ecx, 0x10
loc_9_08D4:
        test   byte ptr [edx + 1], 0xe
        jne    loc_9_090F
        add    edx, 2
        loop   loc_9_08D4
        lea    eax, [edi + 4]
        mov    edx, dword ptr [0x10]         ; -> obj6:0x0010
        mov    word ptr [edi + 0x68a], dx
        test   edx, 0x3ff
        stc    
        jne    loc_9_08FD
        shr    edx, 7
        add    edx, eax
loc_9_08FD:
        push   edx
        mov    ecx, 4
loc_9_0903:
        mov    byte ptr [edx + 2], 0x10
        add    edx, 8
        loop   loc_9_0903
        pop    edx
        jmp    loc_9_094A
loc_9_090F:
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        mov    edx, eax
        inc    edx
        add    edx, 3
        and    edx, 0xfffffffc
        mov    ecx, 0x100
        sub    ecx, edx
        jbe    loc_9_0947
        shr    ecx, 2
        jecxz  loc_9_0947
        lea    eax, [edi + 4]
        test   edx, 3
        stc    
        jne    loc_9_093C
        shl    edx, 1
        add    edx, eax
loc_9_093C:
        test   byte ptr [edx + 2], 1
        je     loc_9_094A
        add    edx, 8
        loop   loc_9_093C
loc_9_0947:
        lea    edx, [edi + 4]
loc_9_094A:
        mov    dword ptr [ebp - 0x14], edx
        lea    eax, [edi + 0x204]
        mov    dword ptr [ebp - 0x18], eax
        lea    esi, [edi + 0x240]
        lea    ecx, [edi + 4]
loc_9_095F:
        test   byte ptr [edx + 2], 1
        jne    loc_9_096F
        mov    eax, edx
        sub    eax, ecx
        shr    eax, 3
        mov    byte ptr [esi], al
        inc    esi
loc_9_096F:
        add    edx, 8
        cmp    edx, dword ptr [ebp - 0x18]
        jb     loc_9_097A
        lea    edx, [edi + 4]
loc_9_097A:
        cmp    edx, dword ptr [ebp - 0x14]
        jne    loc_9_095F
        lea    ecx, [edi + 0x240]
        sub    esi, ecx
        mov    ecx, esi
        mov    byte ptr [edi + 0x68f], cl
        mov    edx, ecx
        shl    ecx, 1
        add    ecx, edx
        add    ecx, 4
        mov    word ptr [edi + 0x688], cx
        clc    
        mov    edx, edi
loc_9_09A2:
        pop    ebx
        pop    esi
        leave  
        ret    
loc_9_09A6:
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        xor    eax, eax
        xchg   dword ptr [esi + ebx + 4], eax
        or     eax, eax
        je     loc_9_09C1
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_9_09C1:
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        xor    eax, eax
        xchg   dword ptr [esi + ebx + 8], eax
        or     eax, eax
        je     loc_9_09DC
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_9_09DC:
        pop    eax
loc_9_09DD:
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        xor    edi, edi
        xchg   dword ptr [esi + ebx + 0x5c], edi
        or     edi, edi
        stc    
        je     loc_9_09A2
        xor    eax, eax
        xchg   dword ptr [edi + 0x280], eax
        or     eax, eax
        je     loc_9_0A05
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_9_0A05:
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        stc    
        jmp    loc_9_09A2
sub_9_0A13:
        pushal 
        bts    word ptr [edi + 0x68e], 5
        jb     loc_9_0AC6
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        mov    dword ptr [0], eax            ; -> obj6:0x0000
        lea    esi, [edi + 4]
        mov    ecx, 0x40
loc_9_0A37:
        test   byte ptr [esi + 2], 3
        jne    loc_9_0AB3
        mov    al, byte ptr [edx + 0x2d]
        cmp    al, byte ptr [esi]
        jne    loc_9_0AB3
        push   ecx
        push   edx
        push   esi
        test   byte ptr [edx + 0x2c], 4
        jne    loc_9_0A9B
        movzx  eax, word ptr [esi + 4]
        cmp    eax, dword ptr [edx + 0x28]
        jae    loc_9_0A9B
        mov    ecx, dword ptr [edx]
        test   byte ptr [edx + 0x2c], 0x80
        jne    loc_9_0AD4
        movzx  edx, byte ptr [esi + 1]
        push   ecx
        lea    ecx, [edi + 4]
        sub    esi, ecx
        shr    esi, 1
        pop    ecx
        test   byte ptr [edi + 0x68e], 4
        jne    loc_9_0A7D
        cmp    esi, dword ptr [0]            ; -> obj6:0x0000
        cmc    
        jae    loc_9_0AAE
loc_9_0A7D:
        call   sub_9_0EA5
        jb     loc_9_0A98
        push   0
        push   eax
        push   edx
        push   esi
        push   ebx
        push   ecx
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
loc_9_0A94:
        or     eax, eax
        jne    loc_9_0AAE
loc_9_0A98:
        stc    
        jmp    loc_9_0AAE
loc_9_0A9B:
        mov    eax, esi
        xor    esi, esi
        mov    ecx, 1
        mov    edx, 0xef2                    ; -> obj1:0x0EF2
        call   sub_9_0CA2
loc_9_0AAE:
        pop    esi
        pop    edx
        pop    ecx
        jb     loc_9_0AC9
loc_9_0AB3:
        add    esi, 8
        dec    ecx
        jne    loc_9_0A37
        btr    word ptr [edi + 0x68e], 5
loc_9_0AC6:
        clc    
loc_9_0AC7:
        popal  
        ret    
loc_9_0AC9:
        btr    word ptr [edi + 0x68e], 5
        jmp    loc_9_0AC7
loc_9_0AD4:
        movzx  edx, byte ptr [esi + 1]
        push   ecx
        lea    ecx, [edi + 4]
        sub    esi, ecx
        shr    esi, 1
        pop    ecx
        test   byte ptr [edi + 0x68e], 4
        jne    loc_9_0AF3
        cmp    esi, dword ptr [0]            ; -> obj6:0x0000
        cmc    
        jae    loc_9_0AAE
loc_9_0AF3:
        push   0
        push   eax
        push   edx
        push   esi
        push   ebx
        push   ecx
        call   sub_9_10D3
        add    esp, 0x18
        jmp    loc_9_0A94
sub_9_0B04:
        pushal 
        mov    eax, 0x57333800
        cld    
        stosd  dword ptr es:[edi], eax
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        push   eax
        push   0
        add    ebx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    ebx, dword ptr [ebx + 0x5c]
        or     ebx, ebx
        je     loc_9_0B2D
        movzx  ebx, byte ptr [ebx + 0x68e]
        mov    dword ptr [esp], ebx
loc_9_0B2D:
        mov    ecx, 0x40
        xor    ebx, ebx
loc_9_0B34:
        test   byte ptr [esi + 2], 1
        jne    loc_9_0B80
        mov    al, byte ptr [esi]
        mov    byte ptr [edi], al
        test   byte ptr [esp], 4
        jne    loc_9_0B4A
        cmp    ebx, dword ptr [esp + 4]
        jb     loc_9_0B75
loc_9_0B4A:
        cmp    byte ptr [esi + 1], 4
        je     loc_9_0B75
        test   byte ptr [esi + 2], 2
        jne    loc_9_0B75
        mov    eax, esi
        mov    edx, dword ptr [esp + 0xc]
        sub    eax, edx
        shr    eax, 3
        and    eax, 0x3f
        mov    ah, byte ptr [esi + 1]
        shl    ah, 6
        and    ah, 0xc0
        or     al, ah
        mov    edx, dword ptr [esp + 8]
        mov    byte ptr [edx], al
loc_9_0B75:
        movzx  eax, word ptr [esi + 4]
        mov    word ptr [edi + 1], ax
        add    edi, 3
loc_9_0B80:
        add    esi, 8
        lea    ebx, [ebx + 4]
        dec    ecx
        jne    loc_9_0B34
        add    esp, 8
        popal  
        ret    
sub_9_0B8E:
        enter  8, 0
        pushal 
        xor    eax, eax
        mov    dword ptr [ebp - 4], eax
        dec    eax
        mov    dword ptr [ebp - 8], eax
        cld    
        lodsd  eax, dword ptr [esi]
        movzx  ecx, al
        jecxz  loc_9_0BD0
        push   eax
        mov    eax, ecx
        and    al, 0x3f
        and    cl, 0xc0
        shr    cl, 6
        mov    dword ptr [ebp - 4], ecx
        or     ecx, ecx
        je     loc_9_0C9E
        lea    eax, [edx + eax*8]
        mov    dword ptr [ebp - 8], eax
        sub    eax, edx
        lea    edx, [edi + eax + 4]
        test   byte ptr [edx + 2], 1
        jne    loc_9_0C9E
        pop    eax
loc_9_0BD0:
        and    eax, 0xffffff00
        cmp    eax, 0x57333800
        jne    loc_9_0C9F
        push   esi
        push   edx
        lea    esi, [edi + 4]
        mov    ebx, esi
        mov    ecx, 0x40
loc_9_0BEC:
        mov    al, byte ptr [esi + 2]
        and    al, 0x13
        mov    byte ptr [edx + 2], al
        add    edx, 8
        add    esi, 8
        loop   loc_9_0BEC
        pop    edx
        pop    esi
        movzx  eax, byte ptr [edi + 0x68f]
        or     eax, eax
        je     loc_9_0C9F
        mov    ecx, 0x40
        mov    edi, edx
        add    edi, 0x200
loc_9_0C1A:
        test   byte ptr [edx + 2], 1
        je     loc_9_0C2A
        add    edx, 8
        add    ebx, 8
        loop   loc_9_0C1A
        jmp    loc_9_0C9F
loc_9_0C2A:
        mov    ecx, eax
loc_9_0C2C:
        cmp    byte ptr [esi], 0xff
        je     loc_9_0C36
        cmp    byte ptr [esi], 0xff
        jae    loc_9_0C9F
loc_9_0C36:
        mov    al, byte ptr [esi]
        test   byte ptr [edx + 2], 2
        je     loc_9_0C42
        cmp    byte ptr [ebx], al
        jne    loc_9_0C9F
loc_9_0C42:
        mov    byte ptr [edx], al
        mov    al, 4
        cmp    edx, dword ptr [ebp - 8]
        jne    loc_9_0C4E
        mov    eax, dword ptr [ebp - 4]
loc_9_0C4E:
        test   byte ptr [edx + 2], 2
        je     loc_9_0C57
        mov    al, byte ptr [ebx + 1]
loc_9_0C57:
        mov    byte ptr [edx + 1], al
        movzx  eax, word ptr [esi + 1]
        test   byte ptr [edx + 2], 2
        je     loc_9_0C6A
        cmp    word ptr [ebx + 4], ax
        jne    loc_9_0C9F
loc_9_0C6A:
        mov    word ptr [edx + 4], ax
        cmp    byte ptr [edx], 0
        jne    loc_9_0C77
        or     byte ptr [edx + 2], 0x40
loc_9_0C77:
        cmp    byte ptr [edx], 0xff
        jne    loc_9_0C80
        or     byte ptr [edx + 2], 0x20
loc_9_0C80:
        add    esi, 3
loc_9_0C83:
        add    edx, 8
        add    ebx, 8
        cmp    ecx, 1
        je     loc_9_0C98
        cmp    edx, edi
        jae    loc_9_0C9F
        test   byte ptr [edx + 2], 1
        jne    loc_9_0C83
loc_9_0C98:
        loop   loc_9_0C2C
        clc    
loc_9_0C9B:
        popal  
        leave  
        ret    
loc_9_0C9E:
        pop    eax
loc_9_0C9F:
        stc    
        jmp    loc_9_0C9B
sub_9_0CA2:
        push   2
        jmp    loc_9_0CAC
sub_9_0CA6:
        push   1
        jmp    loc_9_0CAC
sub_9_0CAA:
        push   0
loc_9_0CAC:
        enter  0xc, 0
        pushal 
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        mov    dword ptr [0], eax            ; -> obj6:0x0000
        shl    ecx, 2
        mov    dword ptr [ebp - 4], ecx
        shl    esi, 2
        mov    dword ptr [ebp - 8], esi
        test   byte ptr [edx + 0x2c], 0x20
        jne    loc_9_0CF8
        cmp    esi, dword ptr [edx + 0x28]
        jae    loc_9_0E9F
        add    esi, ecx
        cmp    esi, dword ptr [edx + 0x28]
        jbe    loc_9_0CF8
        test   byte ptr [edx + 0x2c], 0x40
        je     loc_9_0E9F
        sub    esi, dword ptr [edx + 0x28]
        cmp    esi, 4
        jae    loc_9_0E9F
        sub    dword ptr [ebp - 4], esi
loc_9_0CF8:
        cmp    dword ptr [ebp + 4], 0
        je     loc_9_0D44
        cmp    dword ptr [ebp + 4], 1
        je     loc_9_0D09
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_9_0D22
loc_9_0D09:
        mov    eax, dword ptr [ebp - 0x10]
        lea    esi, [edi + 4]
        test   eax, 0x3ff
        stc    
        jne    loc_9_0D1C
        shr    eax, 7
        add    eax, esi
loc_9_0D1C:
        jb     loc_9_0E99
loc_9_0D22:
        lea    esi, [edi + 0x204]
        mov    ecx, dword ptr [ebp - 0x14]
loc_9_0D2B:
        cmp    eax, esi
        jae    loc_9_0E99
        test   byte ptr [eax + 2], 1
        jne    loc_9_0E99
        add    eax, 8
        loop   loc_9_0D2B
        jmp    loc_9_0D66
loc_9_0D44:
        lea    esi, [edi + 0x240]
        mov    ecx, dword ptr [ebp - 0x14]
loc_9_0D4D:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, 0x40
        jae    loc_9_0E99
        cmp    byte ptr [eax + esi], 0xff
        je     loc_9_0E99
        inc    eax
        loop   loc_9_0D4D
loc_9_0D66:
        mov    esi, dword ptr [edx]
        lea    edi, [edi + 4]
loc_9_0D6B:
        mov    edx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 0x2c]
        cmp    dword ptr [ebp + 4], 0
        je     loc_9_0D94
        cmp    dword ptr [ebp + 4], 1
        je     loc_9_0D82
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_9_0DA2
loc_9_0D82:
        mov    eax, dword ptr [ebp - 0x10]
        test   eax, 0x3ff
        stc    
        jne    loc_9_0D92
        shr    eax, 7
        add    eax, edi
loc_9_0D92:
        jmp    loc_9_0DA2
loc_9_0D94:
        mov    ecx, dword ptr [ebp - 0x10]
        movzx  eax, byte ptr [ecx + eax + 0x240]
        lea    eax, [edi + eax*8]
loc_9_0DA2:
        mov    cl, byte ptr [edx + 0x2d]
        mov    dword ptr [ebp - 0xc], eax
        test   byte ptr [eax + 2], 2
        je     loc_9_0DB6
        cmp    byte ptr [eax], cl
        jne    loc_9_0E99
loc_9_0DB6:
        mov    byte ptr [eax], cl
        mov    cl, byte ptr [edx + 0x2c]
        and    cl, 0xe0
        and    byte ptr [eax + 2], 0x13
        or     byte ptr [eax + 2], cl
        mov    ecx, dword ptr [ebp - 8]
        test   byte ptr [eax + 2], 2
        je     loc_9_0DD8
        cmp    word ptr [eax + 4], cx
        jne    loc_9_0E99
loc_9_0DD8:
        mov    word ptr [eax + 4], cx
        mov    ecx, dword ptr [ebp - 4]
        cmp    ecx, 4
        jbe    loc_9_0DE9
        mov    ecx, 4
loc_9_0DE9:
        mov    byte ptr [eax + 1], cl
        sub    eax, edi
        shr    eax, 1
        push   edi
        mov    edi, dword ptr [ebp - 0x2c]
        test   byte ptr [edi + 0x68e], 4
        pop    edi
        jne    loc_9_0E0A
        cmp    eax, dword ptr [0]            ; -> obj6:0x0000
        jb     loc_9_0E99
loc_9_0E0A:
        test   byte ptr [edx + 0x2c], 0x80
        jne    loc_9_0E77
        mov    edx, dword ptr [ebp - 0xc]
        test   byte ptr [edx + 2], 2
        jne    loc_9_0E46
        push   edx
        mov    edx, dword ptr [ebp - 0x18]
        test   byte ptr [edx + 0x2c], 0x20
        jne    loc_9_0E2F
        call   sub_9_0EA5
        jae    loc_9_0E2F
        pop    edx
        xor    eax, eax
        jmp    loc_9_0E42
loc_9_0E2F:
        pop    edx
        push   0x10
        push   dword ptr [ebp - 8]
        push   ecx
        push   eax
        push   ebx
        push   esi
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
loc_9_0E42:
        or     eax, eax
        je     loc_9_0E93
loc_9_0E46:
        cmp    dword ptr [ebp + 4], 0
        je     loc_9_0E61
        cmp    dword ptr [ebp + 4], 1
        je     loc_9_0E58
        add    dword ptr [ebp - 0x10], 8
        jmp    loc_9_0E64
loc_9_0E58:
        add    dword ptr [ebp - 0x10], 0x400
        jmp    loc_9_0E64
loc_9_0E61:
        inc    dword ptr [ebp - 0x10]
loc_9_0E64:
        add    dword ptr [ebp - 8], 4
        sub    dword ptr [ebp - 4], 4
        ja     loc_9_0D6B
        clc    
        popal  
loc_9_0E74:
        leave  
        pop    ecx
        ret    
loc_9_0E77:
        mov    edx, dword ptr [ebp - 0xc]
        test   byte ptr [edx + 2], 2
        jne    loc_9_0E46
        push   0
        push   dword ptr [ebp - 8]
        push   ecx
        push   eax
        push   ebx
        push   esi
        call   sub_9_10D3
        add    esp, 0x18
        jmp    loc_9_0E42
loc_9_0E93:
        popal  
        mov    ah, 0x80
        stc    
        jmp    loc_9_0E74
loc_9_0E99:
        popal  
        mov    ah, 0x8b
        stc    
        jmp    loc_9_0E74
loc_9_0E9F:
        popal  
        mov    ah, 0x8a
        stc    
        jmp    loc_9_0E74
sub_9_0EA5:
        push   edx
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        test   byte ptr [edx + ebx + 0x2c], 0x20
        jne    loc_9_0EBA
        call   sub_9_13F2
        jae    loc_9_0EBA
loc_9_0EBA:
        pop    edx
        ret    
sub_9_0EBC:
        push   edi
        push   esi
        push   edx
        push   ecx
        push   ebx
        or     ecx, ecx
        je     loc_9_0F5F
        bts    word ptr [edi + 0x68e], 5
        jb     loc_9_0F65
        test   ecx, 0xffffff00
        jne    loc_9_0F69
        cmp    byte ptr [edi + 0x68f], 0
        je     loc_9_0F69
        cmp    cl, byte ptr [edi + 0x68f]
        ja     loc_9_0F69
        or     eax, eax
        je     loc_9_0F28
loc_9_0EF9:
        push   ecx
        push   esi
        push   edx
        movzx  eax, word ptr [esi + 2]
        movzx  esi, word ptr [esi]
        cmp    esi, 0xffff
        jne    loc_9_0F12
        xor    esi, esi
        mov    edx, 0xef2                    ; -> obj1:0x0EF2
loc_9_0F12:
        mov    ecx, 1
        call   sub_9_0CA6
        pop    edx
        pop    esi
        pop    ecx
        jb     loc_9_0F6B
        add    esi, 4
        loop   loc_9_0EF9
        jmp    loc_9_0F55
loc_9_0F28:
        push   ecx
        push   esi
        push   edx
        movzx  eax, word ptr [esi + 2]
        movzx  esi, word ptr [esi]
        cmp    esi, 0xffff
        jne    loc_9_0F41
        xor    esi, esi
        mov    edx, 0xef2                    ; -> obj1:0x0EF2
loc_9_0F41:
        mov    ecx, 1
        call   sub_9_0CAA
        pop    edx
        pop    esi
        pop    ecx
        jb     loc_9_0F6B
        add    esi, 4
        loop   loc_9_0F28
loc_9_0F55:
        btr    word ptr [edi + 0x68e], 5
        clc    
loc_9_0F5F:
        pop    ebx
        pop    ecx
        pop    edx
        pop    esi
        pop    edi
        ret    
loc_9_0F65:
        mov    ah, 0x80
        jmp    loc_9_0F74
loc_9_0F69:
        mov    ah, 0x8b
loc_9_0F6B:
        btr    word ptr [edi + 0x68e], 5
loc_9_0F74:
        stc    
        jmp    loc_9_0F5F
sub_9_0F77:
        bt     dword ptr [esi + ebx + 0x2c], 3
        jae    loc_9_0FE1
        bts    word ptr [edi + 0x68e], 0
        cmc    
        jae    loc_9_0FE1
        cmp    word ptr [edi + 0x68a], 0
        je     loc_9_0FE1
        pushal 
        movzx  edx, word ptr [edi + 0x68a]
        lea    ecx, [edi + 4]
        test   edx, 0x3ff
        stc    
        jne    loc_9_0FAE
        shr    edx, 7
        add    edx, ecx
loc_9_0FAE:
        sub    edx, ecx
        shr    edx, 1
        lea    edx, [ebx + edx*2 + 0x64]
        add    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    esi, edx
        mov    ecx, 0x10
loc_9_0FC3:
        test   byte ptr [edx + 1], 0xe
        stc    
        jne    loc_9_0FE0
        add    edx, 2
        loop   loc_9_0FC3
        mov    edx, esi
        mov    ecx, 0x10
loc_9_0FD6:
        or     byte ptr [edx + 1], 1
        add    edx, 2
        loop   loc_9_0FD6
        clc    
loc_9_0FE0:
        popal  
loc_9_0FE1:
        ret    
sub_9_0FE2:
        bt     dword ptr [esi + ebx + 0x2c], 3
        jae    loc_9_10CE
        bts    word ptr [edi + 0x68e], 1
        cmc    
        jae    loc_9_10CE
        pushal 
        lea    esi, [edi + 0x240]
        movzx  ecx, byte ptr [edi + 0x68f]
        or     ecx, ecx
        je     loc_9_10CC
loc_9_1014:
        cld    
        lodsb  al, byte ptr [esi]
        movzx  eax, al
        shl    eax, 2
        push   ecx
        push   eax
        lea    edx, [edi + 4]
        test   eax, 3
        stc    
        jne    loc_9_102D
        shl    eax, 1
        add    eax, edx
loc_9_102D:
        jb     loc_9_10CF
        test   byte ptr [eax + 2], 2
        jne    loc_9_1078
        pop    eax
        push   eax
        lea    eax, [ebx + eax*2 + 0x64]
        add    eax, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    ecx, 4
loc_9_104A:
        test   byte ptr [eax + 1], 0xe
        je     loc_9_1073
        cmp    dword ptr [0x98], 0           ; -> obj6:0x0098
        stc    
        je     loc_9_10CF
        pop    eax
        push   eax
        lea    edx, [edi + 4]
        test   eax, 3
        stc    
        jne    loc_9_106B
        shl    eax, 1
        add    eax, edx
loc_9_106B:
        jb     loc_9_10CF
        or     byte ptr [eax + 2], 2
        jmp    loc_9_1078
loc_9_1073:
        add    eax, 2
        loop   loc_9_104A
loc_9_1078:
        pop    eax
        pop    ecx
        dec    ecx
        jne    loc_9_1014
        lea    esi, [edi + 0x240]
        movzx  ecx, byte ptr [edi + 0x68f]
        or     ecx, ecx
        je     loc_9_10CC
loc_9_108E:
        cld    
        lodsb  al, byte ptr [esi]
        movzx  eax, al
        shl    eax, 2
        push   eax
        lea    edx, [edi + 4]
        test   eax, 3
        stc    
        jne    loc_9_10A6
        shl    eax, 1
        add    eax, edx
loc_9_10A6:
        jb     loc_9_10D0
        test   byte ptr [eax + 2], 2
        pop    eax
        jne    loc_9_10C9
        lea    eax, [ebx + eax*2 + 0x64]
        add    eax, dword ptr [0x11fc]       ; -> obj1:0x11FC
        push   ecx
        mov    ecx, 4
loc_9_10BF:
        or     byte ptr [eax + 1], 1
        add    eax, 2
        loop   loc_9_10BF
        pop    ecx
loc_9_10C9:
        dec    ecx
        jne    loc_9_108E
loc_9_10CC:
        clc    
loc_9_10CD:
        popal  
loc_9_10CE:
        ret    
loc_9_10CF:
        pop    eax
loc_9_10D0:
        pop    eax
        jmp    loc_9_10CD
sub_9_10D3:
        enter  0x14, 0
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        movzx  ecx, word ptr [esi]
        lea    esi, [esi + 2]
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 0x14], esi
        mov    dword ptr [ebp - 8], ecx
        xor    eax, eax
loc_9_10EE:
        movzx  edx, word ptr [esi]
        add    eax, edx
        add    esi, 4
        loop   loc_9_10EE
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 0x10], 0
        mov    ecx, dword ptr [ebp + 0x18]
        jecxz  loc_9_112E
        cmp    ecx, dword ptr [ebp - 0xc]
        jae    loc_9_1185
        mov    edi, dword ptr [ebp - 4]
loc_9_110F:
        movzx  edx, word ptr [edi]
        xor    eax, eax
        sub    ecx, edx
        je     loc_9_1125
        jb     loc_9_111F
        add    edi, 4
        jmp    loc_9_110F
loc_9_111F:
        add    ecx, edx
        mov    eax, ecx
        jmp    loc_9_1128
loc_9_1125:
        add    edi, 4
loc_9_1128:
        mov    dword ptr [ebp - 0x14], edi
        mov    dword ptr [ebp - 0x10], eax
loc_9_112E:
        mov    ecx, dword ptr [ebp + 0x14]
        add    ecx, dword ptr [ebp + 0x18]
        cmp    ecx, dword ptr [ebp - 0xc]
        ja     loc_9_1185
        mov    ecx, dword ptr [ebp + 0x14]
        or     ecx, ecx
        je     loc_9_117D
        mov    esi, dword ptr [ebp - 0x14]
loc_9_1143:
        xor    eax, eax
        xchg   dword ptr [ebp - 0x10], eax
        push   ecx
        movzx  edi, word ptr [esi]
        sub    edi, eax
        cmp    edi, ecx
        jbe    loc_9_1154
        mov    edi, ecx
loc_9_1154:
        movzx  ecx, word ptr [esi + 2]
        add    ecx, eax
        push   dword ptr [ebp + 0x1c]
        push   edi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   ecx
        VMMCall  VMM._PhysIntoV86                ; cd205e000100
        add    esp, 0x14
        pop    ecx
        or     eax, eax
        je     loc_9_1180
        add    esi, 4
        add    dword ptr [ebp + 0x10], edi
        sub    ecx, edi
        ja     loc_9_1143
loc_9_117D:
        xor    eax, eax
        dec    eax
loc_9_1180:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_9_1185:
        xor    eax, eax
        jmp    loc_9_1180
        db  000h, 000h, 000h
sub_9_118C:
        bts    dword ptr [0x1204], 0         ; -> obj1:0x1204
        jb     loc_9_12B1
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        mov    dword ptr [0], eax            ; -> obj6:0x0000
        mov    eax, dword ptr [4]            ; -> obj6:0x0004
        inc    eax
        je     loc_9_11B4
        dec    eax
        mov    dword ptr [0x19c], eax
loc_9_11B4:
        cmp    dword ptr [0x98], 0           ; -> obj6:0x0098
        je     loc_9_12A2
        test   byte ptr [0x1204], 4          ; -> obj1:0x1204
        jne    loc_9_12A2
        mov    edi, dword ptr [0x98]         ; -> obj6:0x0098
        mov    esi, edi
        lea    edi, [edi + 0x204]
        mov    ecx, 0xf
        xor    eax, eax
        cld    
        repe scasd eax, dword ptr es:[edi]
        jne    loc_9_12B3
        mov    edi, esi
        mov    eax, dword ptr [edi]
        sub    esi, esi
        xchg   dword ptr [0xa0], esi         ; -> obj6:0x00A0
        test   esi, esi
        je     loc_9_1202
        mov    dword ptr [0x9c], esi         ; -> obj6:0x009C
loc_9_1202:
        mov    esi, dword ptr [0x9c]         ; -> obj6:0x009C
        cmp    eax, dword ptr [esi + 6]
        jne    loc_9_12B3
        lea    esi, [edi + 4]
        mov    ebx, esi
        mov    ecx, 0x40
loc_9_121B:
        test   byte ptr [esi + 2], 3
        jne    loc_9_1298
        mov    eax, esi
        sub    eax, ebx
        shr    eax, 1
        cmp    eax, dword ptr [0]            ; -> obj6:0x0000
        jae    loc_9_123B
        cmp    byte ptr [esi], 0
        je     loc_9_1298
        cmp    byte ptr [esi], 0xff
        jne    loc_9_12B3
        jmp    loc_9_1298
loc_9_123B:
        movzx  edx, byte ptr [esi]
        or     edx, edx
        je     loc_9_1298
        cmp    dl, 0xff
        je     loc_9_1298
        cmp    edx, 0xff
        jae    loc_9_1298
        mov    edx, dword ptr [edi + edx*4 + 0x280]
        or     edx, edx
        je     loc_9_1298
        test   byte ptr [edx + 0x2c], 0x80
        je     loc_9_12B3
        push   edi
        mov    edi, dword ptr [0x9c]         ; -> obj6:0x009C
        lea    edi, [edi + 0xa]
        push   ecx
        mov    ecx, eax
        test   ecx, 3
        stc    
        jne    loc_9_1284
        shr    ecx, 2
        mov    eax, ecx
        shl    eax, 1
        shl    ecx, 2
        add    ecx, eax
        add    ecx, edi
loc_9_1284:
        jb     loc_9_12B3
        mov    al, byte ptr [esi]
        mov    byte ptr [ecx + 1], al
        movzx  eax, word ptr [esi + 4]
        shr    eax, 2
        mov    word ptr [ecx + 2], ax
        pop    ecx
        pop    edi
loc_9_1298:
        add    esi, 8
        dec    ecx
        jne    loc_9_121B
loc_9_12A2:
        mov    eax, dword ptr [0x94]         ; -> obj6:0x0094
        or     eax, eax
        je     loc_9_12B1
        VMMCall  VMM.Remove_V86_Break_Point      ; cd200a000100
loc_9_12B1:
        clc    
        ret    
loc_9_12B3:
        movzx  esi, word ptr [0xc]           ; -> loc_3_000C
        add    esi, dword ptr [4]            ; -> loc_3_0004
        mov    eax, 1
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
loc_9_12CB:
        inc    word ptr [ebp + 0x24]
        movzx  eax, word ptr [ebp + 0x38]
        shl    eax, 4
        movzx  ecx, word ptr [ebp + 0x10]
        add    eax, ecx
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    dword ptr [esi + ebx + 0x58], eax
        ret    
loc_9_12E7:
        inc    word ptr [ebp + 0x24]
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    eax, dword ptr [esi + ebx + 0x58]
        mov    word ptr [eax + 3], 0x100
        ret    
loc_9_12FC:
        inc    word ptr [ebp + 0x24]
        and    byte ptr [ebp + 0x2c], 0xfe
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ah, 0
        je     loc_9_1320
        cmp    ah, 1
        jne    loc_9_131A
        cmp    al, 2
        ja     loc_9_131A
        cmp    al, 1
        jne    loc_9_1320
loc_9_131A:
        mov    ah, 1
        or     byte ptr [ebp + 0x2c], 1
loc_9_1320:
        ret    
loc_9_1321:
        inc    word ptr [ebp + 0x24]
        jmp    loc_9_134A
loc_9_1327:
        stc    
        ret    
loc_9_1329:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        jmp    loc_9_134A
loc_9_1331:
        mov    eax, 0x67
        VMMCall  VMM.Get_V86_Int_Vector          ; cd2042000100
        shl    ecx, 0x10
        mov    cx, dx
        cmp    ecx, dword ptr [0x18]         ; -> obj6:0x0018
        jne    loc_9_1327
loc_9_134A:
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        movzx  eax, byte ptr [ebp + 0x1d]
        sub    eax, 0x40
        jb     loc_9_13B6
        cmp    eax, 0x1e
        jae    loc_9_13B6
        cmp    dword ptr [0x98], 0           ; -> obj6:0x0098
        jne    loc_9_1370
        test   byte ptr [esi + ebx + 0x2f], 0x80
        je     loc_9_13CD
loc_9_1370:
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        inc    byte ptr [0x2ea]              ; -> obj6:0x02EA
        inc    byte ptr [esi + ebx + 0x27]
        push   ebx
        call   sub_6_001C
        pop    ebx
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        cmp    byte ptr [esi + ebx + 0x27], 0
        je     loc_9_139F
        dec    byte ptr [esi + ebx + 0x27]
loc_9_139F:
        cmp    byte ptr [0x2ea], 0           ; -> obj6:0x02EA
        je     loc_9_13AE
        dec    byte ptr [0x2ea]              ; -> obj6:0x02EA
loc_9_13AE:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
loc_9_13B4:
        clc    
        ret    
loc_9_13B6:
        cmp    byte ptr [ebp + 0x1d], 0xde
        jne    loc_9_13C7
        push   0x20
        push   ebx
        VxDCall  SHELL._SHELL_SuggestSingleMSDOSMode ; cd2019001700
        pop    ecx
        pop    ecx
loc_9_13C7:
        mov    byte ptr [ebp + 0x1d], 0x84
        jmp    loc_9_13B4
loc_9_13CD:
        or     byte ptr [esi + ebx + 0x2f], 0x80
        mov    dword ptr [esi + ebx + 0x50], 0
        mov    dword ptr [esi + ebx + 0x54], 0x200
        jmp    loc_9_1370
loc_9_13E4:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        jmp    loc_9_13B6
sub_9_13F2:
        cmp    byte ptr [0x1215], 2          ; -> obj1:0x1215
        clc    
        jne    loc_9_1404
        pushal 
        VxDCall  PAGESWAP.PageSwap_Test_IO_Valid ; cd2006000700
        popal  
loc_9_1404:
        ret    
V86MMGR_LocalGlobalReg:
        pushal 
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        mov    dword ptr [0], eax            ; -> obj6:0x0000
        cmp    edx, edi
        ja     loc_9_14CE
        cmp    edx, 0x110
        jae    loc_9_14CE
        cmp    edx, dword ptr [0]            ; -> obj6:0x0000
        jb     loc_9_14CE
        cmp    edx, 0x100
        jae    loc_9_14A7
        mov    esi, dword ptr [0x9c]         ; -> obj6:0x009C
        or     esi, esi
        je     loc_9_14CE
        cmp    byte ptr [esi + 0x18b], 0
        je     loc_9_14CE
        mov    ecx, edi
        sub    ecx, edx
        inc    ecx
        mov    eax, edx
        lea    eax, [eax*2 + 0xf28]          ; -> obj1:0x0F28
        xor    esi, esi
loc_9_1461:
        test   word ptr [eax], 0x2000
        je     loc_9_1474
        test   word ptr [eax], 0x4000
        jne    loc_9_1479
        or     esi, 1
        jmp    loc_9_147C
loc_9_1474:
        or     esi, 4
        jmp    loc_9_147C
loc_9_1479:
        or     esi, 2
loc_9_147C:
        add    eax, 2
        loop   loc_9_1461
        or     esi, esi
        je     loc_9_14CE
        cmp    esi, 4
        je     loc_9_14CE
        cmp    esi, 3
        je     loc_9_14C4
        test   si, 4
        jne    loc_9_14C4
        test   si, 1
        jne    loc_9_14BA
loc_9_149D:
        mov    dword ptr [esp + 0x1c], 0
loc_9_14A5:
        popal  
        ret    
loc_9_14A7:
        test   byte ptr [0x1204], 0x20       ; -> obj1:0x1204
        je     loc_9_14B2
        jmp    loc_9_149D
loc_9_14B2:
        cmp    edi, 0x110
        jae    loc_9_14C4
loc_9_14BA:
        mov    dword ptr [esp + 0x1c], 1
        jmp    loc_9_14A5
loc_9_14C4:
        mov    dword ptr [esp + 0x1c], 2
        jmp    loc_9_14A5
loc_9_14CE:
        mov    dword ptr [esp + 0x1c], 3
        jmp    loc_9_14A5
loc_9_14D8:
        mov    byte ptr [ebp + 0x1c], 0x40
loc_9_14DC:
        mov    byte ptr [ebp + 0x1d], 0
        ret    
loc_9_14E1:
        cmp    dword ptr [8], 0              ; -> obj6:0x0008
        je     loc_9_1556
        test   byte ptr [esi + ebx + 0x2c], 2
        jne    loc_9_1556
loc_9_14F1:
        call   sub_9_00A4
        jb     loc_9_2BF4
        call   sub_9_0F77
        jb     loc_9_2BF4
        movzx  eax, word ptr [edi + 0x68a]
        or     eax, eax
        jne    loc_9_156C
        mov    word ptr [ebp + 0x10], 0xb000
        lea    edx, [edi + 4]
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        add    eax, 3
        and    eax, 0xfffffffc
        mov    ecx, 0x100
        sub    ecx, eax
        jbe    loc_9_1551
        shr    ecx, 2
        jecxz  loc_9_1551
        test   eax, 3
        stc    
        jne    loc_9_1542
        shl    eax, 1
        add    eax, edx
loc_9_1542:
        test   byte ptr [eax + 2], 1
        jne    loc_9_1565
        sub    eax, edx
        shl    eax, 7
        mov    word ptr [ebp + 0x10], ax
loc_9_1551:
        mov    byte ptr [ebp + 0x1d], 0x81
        ret    
loc_9_1556:
        cmp    dword ptr [esi + ebx + 0x5c], 0
        jne    loc_9_14F1
        mov    word ptr [ebp + 0x10], 0xb000
        jmp    loc_9_1551
loc_9_1565:
        add    eax, 8
        loop   loc_9_1542
        jmp    loc_9_1551
loc_9_156C:
        mov    word ptr [ebp + 0x10], ax
        jmp    loc_9_14DC
loc_9_1575:
        xor    ecx, ecx
        xor    edx, edx
        cmp    dword ptr [8], 0              ; -> obj6:0x0008
        je     loc_9_15A2
        test   byte ptr [esi + ebx + 0x2c], 2
        jne    loc_9_15A2
        call   sub_9_13F2
        jae    loc_9_1592
        jmp    loc_9_15A2
loc_9_1592:
        call   sub_9_00A4
        jb     loc_9_2BF4
        call   sub_9_15AF
loc_9_15A2:
        mov    word ptr [ebp + 0x14], cx
        mov    word ptr [ebp + 0x10], dx
        jmp    loc_9_14DC
sub_9_15AF:
        push   0
        VMMCall  VMM._PageGetAllocInfo           ; cd2059000100
        pop    ecx
        xchg   ecx, eax
        mov    eax, 0x80
        test   byte ptr [esi + ebx + 0x2c], 0x20
        je     loc_9_15CF
        xor    eax, eax
        cmp    ecx, edx
        jbe    loc_9_15CF
        mov    al, 0x10
        mov    ecx, edx
loc_9_15CF:
        sub    ecx, eax
        jae    loc_9_15D5
        xor    ecx, ecx
loc_9_15D5:
        mov    eax, dword ptr [edi + 0x67c]
        mov    edx, dword ptr [edi + 0x280]
        or     edx, edx
        je     loc_9_15EE
        mov    edx, dword ptr [edx + 0x28]
        sub    eax, edx
        jae    loc_9_15EE
        xor    eax, eax
loc_9_15EE:
        pushal 
        sub    esp, 0x40
        mov    edi, esp
        push   0
        push   edi
        VMMCall  VMM._GetDemandPageInfo          ; cd206f000100
        pop    ecx
        pop    ecx
        mov    edx, dword ptr [edi]
        cmp    dword ptr [esi + ebx + 0x54], -1
        jne    loc_9_1611
        mov    eax, dword ptr [edi + 4]
        cmp    edx, eax
        jb     loc_9_1611
        mov    edx, eax
loc_9_1611:
        add    esp, 0x40
        cmp    edx, dword ptr [esi + ebx + 0x54]
        jbe    loc_9_161E
        mov    edx, dword ptr [esi + ebx + 0x54]
loc_9_161E:
        mov    dword ptr [esp + 0x14], edx
        popal  
        cmp    edx, dword ptr [8]            ; -> obj6:0x0008
        jbe    loc_9_1631
        mov    edx, dword ptr [8]            ; -> obj6:0x0008
loc_9_1631:
        cmp    dword ptr [esi + ebx + 0x48], 0
        je     loc_9_164C
        mov    ecx, edx
        mov    edx, dword ptr [esi + ebx + 0x48]
        shr    edx, 2
        shr    ecx, 2
        cmp    edx, ecx
        jbe    loc_9_1663
        mov    ecx, edx
        jmp    loc_9_1663
loc_9_164C:
        shr    eax, 2
        shr    edx, 2
        shr    ecx, 2
        push   edx
        sub    edx, eax
        jae    loc_9_165C
        xor    edx, edx
loc_9_165C:
        cmp    edx, ecx
        jbe    loc_9_1662
        mov    edx, ecx
loc_9_1662:
        pop    ecx
loc_9_1663:
        cmp    edx, 0x400
        jbe    loc_9_1670
        mov    edx, 0x400
loc_9_1670:
        test   ecx, 0xffff0000
        je     loc_9_167D
        mov    ecx, 0xffff
loc_9_167D:
        ret    
loc_9_167E:
        cmp    dword ptr [8], 0              ; -> obj6:0x0008
        je     loc_9_187F
        test   byte ptr [esi + ebx + 0x2c], 2
        jne    loc_9_187F
        call   sub_9_00A4
        jb     loc_9_187F
        mov    esi, edi
        bts    word ptr [esi + 0x68e], 3
        jb     loc_9_1871
        lea    edi, [edi + 0x280]
        mov    ecx, 0xff
        xor    eax, eax
        cld    
        repne scasd eax, dword ptr es:[edi]
        jne    loc_9_1868
        sub    edi, 4
        mov    eax, edi
        lea    ecx, [esi + 0x280]
        sub    eax, ecx
        shr    eax, 2
        mov    word ptr [ebp + 0x14], ax
        cmp    byte ptr [ebp + 0x1d], 0x43
        jne    loc_9_16EF
        cmp    word ptr [ebp + 0x10], 0
        je     loc_9_185A
        jmp    loc_9_1705
loc_9_16EF:
        cmp    byte ptr [ebp + 0x1c], 1
        ja     loc_9_16F7
        jmp    loc_9_1705
loc_9_16F7:
        btr    word ptr [esi + 0x68e], 3
        jmp    loc_9_2EE9
loc_9_1705:
        push   edi
        mov    edi, esi
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        call   sub_9_15AF
        mov    esi, edi
        pop    edi
        movzx  edx, word ptr [ebp + 0x10]
        cmp    edx, ecx
        ja     loc_9_1876
        or     edx, edx
        je     loc_9_1777
        cmp    edx, 0x4000
        ja     loc_9_1876
        shl    edx, 2
        mov    ecx, edx
        add    ecx, dword ptr [0xc]          ; -> obj6:0x000C
        add    edx, dword ptr [esi + 0x67c]
        mov    eax, dword ptr [esi + 0x280]
        or     eax, eax
        je     loc_9_1750
        mov    eax, dword ptr [eax + 0x28]
loc_9_1750:
        sub    ecx, eax
        jae    loc_9_1756
        xor    ecx, ecx
loc_9_1756:
        sub    edx, eax
        jae    loc_9_175C
        xor    edx, edx
loc_9_175C:
        cmp    ecx, dword ptr [8]            ; -> obj6:0x0008
        ja     loc_9_1939
        mov    eax, dword ptr [0x11fc]       ; -> obj1:0x11FC
        cmp    edx, dword ptr [eax + ebx + 0x54]
        ja     loc_9_1939
loc_9_1777:
        push   0x101
        push   0x36
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_9_1939
        mov    dword ptr [edi], eax
        btr    word ptr [esi + 0x68e], 3
        movzx  edx, word ptr [ebp + 0x10]
        or     edx, edx
        je     loc_9_1848
        shl    edx, 2
        push   edx
        xor    eax, eax
        mov    ecx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        test   byte ptr [ecx + ebx + 0x2c], 0x20
        je     loc_9_17BD
        or     eax, 0x80
loc_9_17BD:
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   edx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        pop    ecx
        or     eax, eax
        je     loc_9_1884
        mov    edi, dword ptr [edi]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi + 0x28], ecx
        add    dword ptr [esi + 0x67c], ecx
        add    dword ptr [0xc], ecx          ; -> obj6:0x000C
        movzx  eax, word ptr [ebp + 0x14]
        mov    byte ptr [edi + 0x2d], al
loc_9_17F9:
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    eax, dword ptr [edx + ebx + 0x48]
        or     eax, eax
        je     loc_9_14DC
        sub    eax, ecx
        jbe    loc_9_182E
        mov    dword ptr [edx + ebx + 0x48], eax
        push   edx
        push   0
        push   eax
        push   dword ptr [edx + ebx + 0x44]
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        pop    edx
        mov    dword ptr [edx + ebx + 0x44], eax
        jmp    loc_9_14DC
loc_9_182E:
        xor    eax, eax
        mov    dword ptr [edx + ebx + 0x48], eax
        xchg   dword ptr [edx + ebx + 0x44], eax
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
        jmp    loc_9_14DC
loc_9_1848:
        mov    byte ptr [eax + 0x2c], 4
        mov    edi, eax
        movzx  eax, word ptr [ebp + 0x14]
        mov    byte ptr [edi + 0x2d], al
        jmp    loc_9_14DC
loc_9_185A:
        btr    word ptr [esi + 0x68e], 3
        mov    byte ptr [ebp + 0x1d], 0x89
        ret    
loc_9_1868:
        btr    word ptr [esi + 0x68e], 3
loc_9_1871:
        mov    byte ptr [ebp + 0x1d], 0x85
        ret    
loc_9_1876:
        btr    word ptr [esi + 0x68e], 3
loc_9_187F:
        mov    byte ptr [ebp + 0x1d], 0x87
        ret    
loc_9_1884:
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    eax, dword ptr [edx + ebx + 0x48]
        or     eax, eax
        je     loc_9_1925
        sub    eax, ecx
        jb     loc_9_1925
        je     loc_9_18BE
        mov    dword ptr [edx + ebx + 0x48], eax
        push   ecx
        push   edx
        push   0
        push   eax
        push   dword ptr [edx + ebx + 0x44]
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        pop    edx
        pop    ecx
        mov    dword ptr [edx + ebx + 0x44], eax
        jmp    loc_9_18D3
loc_9_18BE:
        mov    dword ptr [edx + ebx + 0x48], eax
        xchg   dword ptr [edx + ebx + 0x44], eax
        push   ecx
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
        pop    ecx
loc_9_18D3:
        xor    eax, eax
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        test   byte ptr [edx + ebx + 0x2c], 0x20
        je     loc_9_18E7
        or     eax, 0x80
loc_9_18E7:
        push   ecx
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        pop    ecx
        or     eax, eax
        je     loc_9_1925
        mov    edi, dword ptr [edi]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi + 0x28], ecx
        add    dword ptr [esi + 0x67c], ecx
        add    dword ptr [0xc], ecx          ; -> obj6:0x000C
        movzx  eax, word ptr [ebp + 0x14]
        mov    byte ptr [edi + 0x2d], al
        jmp    loc_9_14DC
loc_9_1925:
        xor    eax, eax
        xchg   dword ptr [edi], eax
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_9_1934:
        mov    byte ptr [ebp + 0x1d], 0x88
        ret    
loc_9_1939:
        btr    word ptr [esi + 0x68e], 3
        jmp    loc_9_1934
loc_9_1944:
        call   sub_9_00A4
        jb     loc_9_1BB8
        cmp    byte ptr [ebp + 0x1c], 4
        jb     loc_9_1962
        call   sub_9_0FE2
        jb     loc_9_2BF4
        jmp    loc_9_196D
loc_9_1962:
        call   sub_9_0F77
        jb     loc_9_2BF4
loc_9_196D:
        movzx  edx, word ptr [ebp + 0x14]
        cmp    edx, 0xff
        jae    loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 0x280]
        or     edx, edx
        je     loc_9_1BB8
        movzx  esi, word ptr [ebp + 0x10]
        cmp    esi, 0xffff
        jne    loc_9_199D
        mov    edx, 0xef2                    ; -> obj1:0x0EF2
loc_9_199D:
        mov    ecx, 1
        movzx  eax, byte ptr [ebp + 0x1c]
        bts    word ptr [edi + 0x68e], 5
        jb     loc_9_2BF4
        call   sub_9_0CAA
        pushfd 
        and    byte ptr [edi + 0x68e], 0xdf
        popfd  
        jae    loc_9_14DC
loc_9_19C9:
        mov    byte ptr [ebp + 0x1d], ah
        ret    
loc_9_19CD:
        call   sub_9_00A4
        jb     loc_9_1BB8
        movzx  edx, word ptr [ebp + 0x14]
        or     edx, edx
        je     loc_9_1BB8
        cmp    edx, 0xff
        jae    loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 0x280]
        or     edx, edx
        je     loc_9_1BB8
        test   byte ptr [edx + 0x2c], 0x80
        jne    loc_9_1BB8
        test   byte ptr [edx + 0x2c], 2
        jne    loc_9_1B0E
        xor    eax, eax
        movzx  edx, word ptr [ebp + 0x14]
        bts    word ptr [edi + 0x68e], 5
        jb     loc_9_1A5D
        lea    esi, [edi + 4]
        mov    ecx, 0x40
loc_9_1A2C:
        test   byte ptr [esi + 2], 3
        jne    loc_9_1A4F
        cmp    dl, byte ptr [esi]
        jne    loc_9_1A4F
        push   esi
        push   ecx
        push   edx
        mov    eax, esi
        xor    esi, esi
        mov    ecx, 1
        mov    edx, 0xef2                    ; -> obj1:0x0EF2
        call   sub_9_0CA2
        pop    edx
        pop    ecx
        pop    esi
loc_9_1A4F:
        add    esi, 8
        loop   loc_9_1A2C
        btr    word ptr [edi + 0x68e], 5
loc_9_1A5D:
        xor    eax, eax
        movzx  edx, word ptr [ebp + 0x14]
        xchg   dword ptr [edi + edx*4 + 0x280], eax
        test   byte ptr [eax + 0x2c], 4
        jne    loc_9_1AFE
        push   eax
        mov    ecx, dword ptr [eax + 0x28]
        sub    dword ptr [edi + 0x67c], ecx
        sub    dword ptr [0xc], ecx          ; -> obj6:0x000C
        mov    eax, dword ptr [eax]
        push   ecx
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
        pop    ecx
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    eax, dword ptr [edx + ebx + 0x4c]
        sub    eax, dword ptr [edx + ebx + 0x48]
        jbe    loc_9_1AFD
        cmp    eax, ecx
        jae    loc_9_1AA9
        mov    ecx, eax
loc_9_1AA9:
        mov    eax, dword ptr [edx + ebx + 0x44]
        add    ecx, dword ptr [edx + ebx + 0x48]
        push   ecx
        test   byte ptr [edx + ebx + 0x2c], 0x20
        je     loc_9_1AC0
        mov    edx, 0x80
        jmp    loc_9_1AC2
loc_9_1AC0:
        xor    edx, edx
loc_9_1AC2:
        or     eax, eax
        je     loc_9_1AD4
        push   edx
        push   ecx
        push   eax
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        jmp    loc_9_1AEA
loc_9_1AD4:
        push   edx
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
loc_9_1AEA:
        pop    ecx
        or     eax, eax
        je     loc_9_1AFD
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    dword ptr [edx + ebx + 0x44], eax
        mov    dword ptr [edx + ebx + 0x48], ecx
loc_9_1AFD:
        pop    eax
loc_9_1AFE:
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        jmp    loc_9_14DC
loc_9_1B0E:
        mov    byte ptr [ebp + 0x1d], 0x86
        ret    
loc_9_1B13:
        call   sub_9_00A4
        jb     loc_9_1BBD
        call   sub_9_0F77
        jb     loc_9_2BF4
        cmp    word ptr [edi + 0x68a], 0
        je     loc_9_1551
        movzx  edx, word ptr [ebp + 0x14]
        or     edx, edx
        je     loc_9_1BB8
        cmp    edx, 0xff
        jae    loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 0x280]
        or     edx, edx
        je     loc_9_1BB8
        bts    word ptr [edx + 0x2c], 3
        jb     loc_9_2BF4
        bts    word ptr [edx + 0x2c], 1
        jb     loc_9_1BA2
        bts    word ptr [edi + 0x68e], 5
        jb     loc_9_1BAD
        lea    eax, [edx + 8]
        lea    esi, [edi + 4]
        push   edi
        mov    edi, eax
loc_9_1B7A:
        test   byte ptr [esi + 2], 0x10
        jne    loc_9_1B85
        add    esi, 8
        jmp    loc_9_1B7A
loc_9_1B85:
        mov    ecx, 8
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        btr    word ptr [edi + 0x68e], 5
        btr    word ptr [edx + 0x2c], 3
        jmp    loc_9_14DC
loc_9_1BA2:
        btr    word ptr [edx + 0x2c], 3
        mov    byte ptr [ebp + 0x1d], 0x8d
        ret    
loc_9_1BAD:
        btr    word ptr [edx + 0x2c], 3
        jmp    loc_9_2BF4
loc_9_1BB8:
        mov    byte ptr [ebp + 0x1d], 0x83
        ret    
loc_9_1BBD:
        mov    byte ptr [ebp + 0x1d], 0x8c
        ret    
loc_9_1BC2:
        call   sub_9_00A4
        jb     loc_9_1BB8
        cmp    word ptr [edi + 0x68a], 0
        je     loc_9_1551
        movzx  edx, word ptr [ebp + 0x14]
        cmp    edx, 0xff
        jae    loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 0x280]
        or     edx, edx
        je     loc_9_1BB8
        bts    word ptr [edx + 0x2c], 3
        jb     loc_9_2BF4
        btr    word ptr [edx + 0x2c], 1
        jae    loc_9_1C4C
        sub    esp, 0x200
        lea    eax, [edi + 4]
        mov    esi, esp
loc_9_1C0D:
        test   byte ptr [eax + 2], 0x10
        jne    loc_9_1C1B
        add    esi, 8
        add    eax, 8
        jmp    loc_9_1C0D
loc_9_1C1B:
        push   edi
        mov    edi, esi
        lea    esi, [edx + 8]
        mov    ecx, 8
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        btr    word ptr [edx + 0x2c], 3
        pop    edi
        mov    eax, esp
        xor    ecx, ecx
        call   sub_11_0074
        lahf   
        add    esp, 0x200
        sahf   
        jb     loc_9_2BF4
        jmp    loc_9_14DC
loc_9_1C4C:
        btr    word ptr [edx + 0x2c], 3
        mov    byte ptr [ebp + 0x1d], 0x8e
        ret    
loc_9_1C57:
        mov    byte ptr [ebp + 0x1d], 0x84
        ret    
loc_9_1C5C:
        call   sub_9_00A4
        jb     loc_9_2BF4
        lea    esi, [edi + 0x280]
        xor    edx, edx
        mov    ecx, 0xff
loc_9_1C74:
        cld    
        lodsd  eax, dword ptr [esi]
        or     eax, eax
        je     loc_9_1C7B
        inc    edx
loc_9_1C7B:
        loop   loc_9_1C74
        mov    word ptr [ebp + 0x10], dx
        jmp    loc_9_14DC
loc_9_1C86:
        call   sub_9_00A4
        jb     loc_9_1BB8
        movzx  edx, word ptr [ebp + 0x14]
        cmp    edx, 0xff
        jae    loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 0x280]
        or     edx, edx
        je     loc_9_1BB8
        mov    eax, dword ptr [edx + 0x28]
        test   byte ptr [edx + 0x2c], 0x40
        je     loc_9_1CBF
        add    eax, dword ptr [0xa4]         ; -> obj6:0x00A4
loc_9_1CBF:
        add    eax, 3
        shr    eax, 2
        mov    word ptr [ebp + 0x10], ax
        jmp    loc_9_14DC
loc_9_1CCE:
        call   sub_9_00A4
        jb     loc_9_2BF4
        lea    esi, [edi + 0x280]
        movzx  edi, word ptr [ebp + 0x38]
        shl    edi, 4
        movzx  ecx, word ptr [ebp]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        xor    edx, edx
        mov    ecx, 0xff
loc_9_1CF6:
        cld    
        lodsd  eax, dword ptr [esi]
        or     eax, eax
        je     loc_9_1D22
        push   ebx
        movzx  ebx, byte ptr [eax + 0x2d]
        mov    word ptr [edi], bx
        mov    ebx, dword ptr [eax + 0x28]
        test   byte ptr [eax + 0x2c], 0x40
        je     loc_9_1D13
        add    ebx, dword ptr [0xa4]         ; -> obj6:0x00A4
loc_9_1D13:
        add    ebx, 3
        shr    ebx, 2
        mov    word ptr [edi + 2], bx
        add    edi, 4
        inc    edx
        pop    ebx
loc_9_1D22:
        loop   loc_9_1CF6
        mov    word ptr [ebp + 0x10], dx
        jmp    loc_9_14DC
loc_9_1D2D:
        call   sub_9_00A4
        jb     loc_9_2BF4
        call   sub_9_0FE2
        jb     loc_9_2BF4
        movzx  eax, word ptr [edx + 0x688]
        test   eax, 0xffffff00
        jne    loc_9_2BF4
        movzx  eax, byte ptr [ebp + 0x1c]
        or     al, al
        jne    loc_9_1D94
sub_9_1D5D:
        bts    word ptr [edi + 0x68e], 5
        jb     loc_9_2BF4
        lea    esi, [edi + 4]
        push   edi
        movzx  edi, word ptr [ebp + 0x38]
        shl    edi, 4
        movzx  ecx, word ptr [ebp]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        call   sub_9_0B04
        pop    edi
        btr    word ptr [edi + 0x68e], 5
        jmp    loc_9_14DC
loc_9_1D94:
        cmp    al, 1
        jne    loc_9_1DD6
loc_9_1D98:
        movzx  esi, word ptr [ebp + 0x3c]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        sub    esp, 0x200
        mov    edx, esp
        call   sub_9_0B8E
        jb     loc_9_1E06
        mov    eax, esp
        mov    ecx, 1
        call   sub_11_0074
        lahf   
        add    esp, 0x200
        sahf   
        jb     loc_9_2BF4
        jmp    loc_9_14DC
loc_9_1DD6:
        cmp    al, 2
        jne    loc_9_1DEA
        push   edi
        push   edx
        call   sub_9_1D5D
        pop    edx
        pop    edi
        cmp    byte ptr [ebp + 0x1d], 0
        je     loc_9_1D98
        ret    
loc_9_1DEA:
        cmp    al, 3
        jne    loc_9_2EE9
        mov    al, byte ptr [edx + 0x688]
        mov    byte ptr [ebp + 0x1c], al
        jmp    loc_9_14DC
loc_9_1E00:
        mov    byte ptr [ebp + 0x1d], 0xa3
        ret    
loc_9_1E05:
        pop    eax
loc_9_1E06:
        add    esp, 0x200
        jmp    loc_9_1E00
loc_9_1E0E:
        call   sub_9_00A4
        jb     loc_9_2BF4
        call   sub_9_0FE2
        jb     loc_9_2BF4
        movzx  eax, byte ptr [ebp + 0x1c]
        cmp    al, 2
        ja     loc_9_2EE9
        je     loc_9_1F7F
        or     al, al
        jne    loc_9_1EE5
        mov    edx, edi
        movzx  edi, word ptr [ebp + 0x38]
        shl    edi, 4
        movzx  ecx, word ptr [ebp]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        movzx  esi, word ptr [ebp + 0x3c]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        movzx  ecx, word ptr [esi]
        lea    esi, [esi + 2]
        test   ch, 0xff
        jne    loc_9_1E00
        cmp    byte ptr [edx + 0x68f], 0
        je     loc_9_1E00
        cmp    cl, byte ptr [edx + 0x68f]
        ja     loc_9_1E00
        bts    word ptr [edx + 0x68e], 5
        jb     loc_9_2BF4
        mov    eax, 0x57333800
        cld    
        stosd  dword ptr es:[edi], eax
        mov    al, cl
        stosb  byte ptr es:[edi], al
loc_9_1E95:
        push   edx
        push   ecx
        cld    
        lodsw  ax, word ptr [esi]
        push   esi
        movzx  eax, ax
        lea    ecx, [edx + 4]
        test   eax, 0x3ff
        stc    
        jne    loc_9_1EAE
        shr    eax, 7
        add    eax, ecx
loc_9_1EAE:
        jb     loc_9_1FB6
        test   byte ptr [eax + 2], 1
        jne    loc_9_1FB6
        mov    edx, eax
        sub    edx, ecx
        shr    edx, 3
        mov    byte ptr [eax + 3], dl
        mov    esi, eax
        mov    ecx, 6
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        pop    ecx
        pop    edx
        loop   loc_9_1E95
        btr    word ptr [edx + 0x68e], 5
        jmp    loc_9_14DC
loc_9_1EE5:
        sub    esp, 0x200
        mov    edx, edi
        lea    esi, [edi + 4]
        mov    edi, esp
        mov    ecx, 0x80
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movzx  esi, word ptr [ebp + 0x3c]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        cld    
        lodsd  eax, dword ptr [esi]
        cmp    eax, 0x57333800
        jne    loc_9_1E06
        lodsb  al, byte ptr [esi]
        movzx  ecx, al
        cmp    byte ptr [edx + 0x68f], 0
        je     loc_9_1E06
        cmp    cl, byte ptr [edx + 0x68f]
        ja     loc_9_1E06
        mov    eax, esp
loc_9_1F36:
        push   ecx
        movzx  ecx, byte ptr [esi + 3]
        cmp    cl, 0x40
        jae    loc_9_1E05
        mov    edi, ecx
        lea    edi, [eax + edi*8]
        test   byte ptr [edi + 2], 1
        jne    loc_9_1E05
        mov    ecx, 6
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        loop   loc_9_1F36
        mov    edi, edx
        mov    eax, esp
        mov    ecx, 1
        call   sub_11_0074
        lahf   
        add    esp, 0x200
        sahf   
        jb     loc_9_2BF4
        jmp    loc_9_14DC
loc_9_1F7F:
        movzx  eax, word ptr [ebp + 0x10]
        test   ah, 0xff
        jne    loc_9_1FC2
        cmp    byte ptr [edi + 0x68f], 0
        je     loc_9_1FC2
        cmp    al, byte ptr [edi + 0x68f]
        ja     loc_9_1FC2
        mov    cl, 6
        mul    cl
        movzx  eax, ax
        add    eax, 5
        cmp    eax, 0xff
        ja     loc_9_2BF4
        mov    byte ptr [ebp + 0x1c], al
        jmp    loc_9_14DC
loc_9_1FB6:
        pop    esi
        pop    ecx
        pop    edx
        btr    word ptr [edx + 0x68e], 5
loc_9_1FC2:
        mov    byte ptr [ebp + 0x1d], 0x8b
        ret    
loc_9_1FC7:
        call   sub_9_00A4
        jb     loc_9_1BB8
        call   sub_9_0FE2
        jb     loc_9_2BF4
        cmp    byte ptr [ebp + 0x1c], 1
        ja     loc_9_2EE9
        movzx  edx, word ptr [ebp + 0x14]
        cmp    edx, 0xff
        jae    loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 0x280]
        or     edx, edx
        je     loc_9_1BB8
        movzx  esi, word ptr [ebp + 0x3c]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        movzx  ecx, word ptr [ebp + 0x18]
        movzx  eax, byte ptr [ebp + 0x1c]
        call   sub_9_0EBC
        jb     loc_9_19C9
        jmp    loc_9_14DC
loc_9_202E:
        call   sub_9_00A4
        jb     loc_9_1BB8
        movzx  edx, word ptr [ebp + 0x14]
        cmp    edx, 0xff
        jae    loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 0x280]
        or     edx, edx
        je     loc_9_1BB8
        test   byte ptr [edx + 0x2c], 0x80
        jne    loc_9_1BB8
        test   byte ptr [edx + 0x2c], 0x20
        jne    loc_9_1BB8
        cmp    dword ptr [8], 0              ; -> obj6:0x0008
        je     loc_9_187F
        test   byte ptr [esi + ebx + 0x2c], 2
        jne    loc_9_187F
        movzx  ecx, word ptr [ebp + 0x10]
        shl    ecx, 2
        test   byte ptr [edx + 0x2c], 0x40
        je     loc_9_2097
        sub    ecx, dword ptr [0xa4]         ; -> obj6:0x00A4
loc_9_2097:
        cmp    ecx, 0x10000
        ja     loc_9_23DD
        cmp    ecx, dword ptr [edx + 0x28]
        je     loc_9_14DC
        jb     loc_9_230C
        test   byte ptr [edx + 0x2c], 4
        jne    loc_9_21C5
        test   byte ptr [edx + 0x2c], 0x40
        je     loc_9_20D1
        mov    eax, dword ptr [edx + 0x28]
        and    eax, 3
        je     loc_9_20D1
        sub    eax, 4
        neg    eax
        add    ecx, eax
loc_9_20D1:
        push   ecx
        push   edx
        call   sub_9_15AF
        mov    eax, ecx
        shl    eax, 2
        pop    edx
        pop    ecx
        cmp    ecx, eax
        ja     loc_9_23DD
        mov    eax, ecx
        sub    eax, dword ptr [edx + 0x28]
        push   ecx
        push   eax
        add    eax, dword ptr [0xc]          ; -> obj6:0x000C
        mov    ecx, dword ptr [edi + 0x280]
        jecxz  loc_9_20FF
        mov    ecx, dword ptr [ecx + 0x28]
loc_9_20FF:
        sub    eax, ecx
        jae    loc_9_2105
        xor    eax, eax
loc_9_2105:
        cmp    eax, dword ptr [8]            ; -> obj6:0x0008
        ja     loc_9_247F
        pop    eax
        push   eax
        add    eax, dword ptr [edi + 0x67c]
        sub    eax, ecx
        jae    loc_9_211F
        xor    eax, eax
loc_9_211F:
        cmp    eax, dword ptr [esi + ebx + 0x54]
        ja     loc_9_247F
        mov    ecx, dword ptr [esp + 4]
        push   edx
        xor    eax, eax
        test   byte ptr [esi + ebx + 0x2c], 0x20
        je     loc_9_213C
        or     eax, 0x80
loc_9_213C:
        push   eax
        push   ecx
        push   dword ptr [edx]
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        mov    esi, edx
        pop    edx
        or     eax, eax
        je     loc_9_23FB
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        and    byte ptr [edx + 0x2c], 0xfb
        pop    ecx
        pop    eax
        mov    dword ptr [edx + 0x28], eax
        push   edx
        push   ecx
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    eax, dword ptr [edx + ebx + 0x48]
        or     eax, eax
        je     loc_9_21A7
        sub    eax, ecx
        jbe    loc_9_2192
        mov    dword ptr [edx + ebx + 0x48], eax
        push   edx
        push   0
        push   eax
        push   dword ptr [edx + ebx + 0x44]
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        pop    edx
        mov    dword ptr [edx + ebx + 0x44], eax
        jmp    loc_9_21A7
loc_9_2192:
        xor    eax, eax
        mov    dword ptr [edx + ebx + 0x48], eax
        xchg   dword ptr [edx + ebx + 0x44], eax
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_9_21A7:
        pop    ecx
        pop    edx
loc_9_21A9:
        call   sub_9_0A13
        jb     loc_9_2BF4
        add    dword ptr [edi + 0x67c], ecx
        add    dword ptr [0xc], ecx          ; -> obj6:0x000C
        jmp    loc_9_14DC
loc_9_21C5:
        push   ecx
        push   edx
        call   sub_9_15AF
        mov    eax, ecx
        shl    eax, 2
        pop    edx
        pop    ecx
        cmp    ecx, eax
        ja     loc_9_23DD
        mov    eax, ecx
        push   ecx
        add    eax, dword ptr [0xc]          ; -> obj6:0x000C
        mov    ecx, dword ptr [edi + 0x280]
        jecxz  loc_9_21EF
        mov    ecx, dword ptr [ecx + 0x28]
loc_9_21EF:
        sub    eax, ecx
        jae    loc_9_21F5
        xor    eax, eax
loc_9_21F5:
        pop    ecx
        cmp    eax, dword ptr [8]            ; -> obj6:0x0008
        ja     loc_9_2482
        mov    eax, ecx
        push   ecx
        add    eax, dword ptr [edi + 0x67c]
        mov    ecx, dword ptr [edi + 0x280]
        jecxz  loc_9_2216
        mov    ecx, dword ptr [ecx + 0x28]
loc_9_2216:
        sub    eax, ecx
        jae    loc_9_221C
        xor    eax, eax
loc_9_221C:
        pop    ecx
        cmp    eax, dword ptr [esi + ebx + 0x54]
        ja     loc_9_2482
        push   ecx
        push   edx
        xor    eax, eax
        test   byte ptr [esi + ebx + 0x2c], 0x20
        je     loc_9_2237
        or     eax, 0x80
loc_9_2237:
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        mov    esi, edx
        pop    edx
        pop    ecx
        or     eax, eax
        je     loc_9_2272
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        and    byte ptr [edx + 0x2c], 0xfb
        mov    dword ptr [edx + 0x28], ecx
        add    dword ptr [edi + 0x67c], ecx
        add    dword ptr [0xc], ecx          ; -> obj6:0x000C
        jmp    loc_9_17F9
loc_9_2272:
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    eax, dword ptr [esi + ebx + 0x48]
        or     eax, eax
        je     loc_9_2482
        sub    eax, ecx
        jb     loc_9_2482
        push   edx
        push   ecx
        je     loc_9_22AA
        mov    dword ptr [esi + ebx + 0x48], eax
        push   0
        push   eax
        push   dword ptr [esi + ebx + 0x44]
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        mov    dword ptr [esi + ebx + 0x44], eax
        jmp    loc_9_22BD
loc_9_22AA:
        mov    dword ptr [esi + ebx + 0x48], eax
        xchg   dword ptr [esi + ebx + 0x44], eax
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_9_22BD:
        pop    ecx
        push   ecx
        xor    eax, eax
        test   byte ptr [esi + ebx + 0x2c], 0x20
        je     loc_9_22CD
        or     eax, 0x80
loc_9_22CD:
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        mov    esi, edx
        pop    ecx
        pop    edx
        or     eax, eax
        je     loc_9_2482
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        and    byte ptr [edx + 0x2c], 0xfb
        mov    dword ptr [edx + 0x28], ecx
        add    dword ptr [edi + 0x67c], ecx
        add    dword ptr [0xc], ecx          ; -> obj6:0x000C
        jmp    loc_9_14DC
loc_9_230C:
        jecxz  loc_9_233B
        mov    eax, dword ptr [edx + 0x28]
        sub    eax, ecx
        push   eax
        push   ecx
        push   edx
        push   0
        push   ecx
        push   dword ptr [edx]
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        mov    esi, edx
        pop    edx
        or     eax, eax
        je     loc_9_23DA
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        pop    eax
        pop    ecx
        mov    dword ptr [edx + 0x28], eax
        jmp    loc_9_2352
loc_9_233B:
        push   edx
        push   0
        push   dword ptr [edx]
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
        pop    edx
        xor    ecx, ecx
        xchg   dword ptr [edx + 0x28], ecx
        or     byte ptr [edx + 0x2c], 4
loc_9_2352:
        sub    dword ptr [edi + 0x67c], ecx
        sub    dword ptr [0xc], ecx          ; -> obj6:0x000C
        push   edx
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    eax, dword ptr [edx + ebx + 0x4c]
        sub    eax, dword ptr [edx + ebx + 0x48]
        jbe    loc_9_23C9
        cmp    eax, ecx
        jae    loc_9_2375
        mov    ecx, eax
loc_9_2375:
        mov    eax, dword ptr [edx + ebx + 0x44]
        add    ecx, dword ptr [edx + ebx + 0x48]
        push   ecx
        test   byte ptr [edx + ebx + 0x2c], 0x20
        je     loc_9_238C
        mov    edx, 0x80
        jmp    loc_9_238E
loc_9_238C:
        xor    edx, edx
loc_9_238E:
        or     eax, eax
        je     loc_9_23A0
        push   edx
        push   ecx
        push   eax
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        jmp    loc_9_23B6
loc_9_23A0:
        push   edx
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
loc_9_23B6:
        pop    ecx
        or     eax, eax
        je     loc_9_23C9
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    dword ptr [edx + ebx + 0x44], eax
        mov    dword ptr [edx + ebx + 0x48], ecx
loc_9_23C9:
        pop    edx
        call   sub_9_0A13
        jb     loc_9_2BF4
        jmp    loc_9_14DC
loc_9_23DA:
        add    esp, 8
loc_9_23DD:
        mov    eax, dword ptr [edx + 0x28]
        test   byte ptr [edx + 0x2c], 0x40
        je     loc_9_23EC
        add    eax, dword ptr [0xa4]         ; -> obj6:0x00A4
loc_9_23EC:
        add    eax, 3
        shr    eax, 2
        mov    word ptr [ebp + 0x10], ax
        jmp    loc_9_187F
loc_9_23FB:
        pop    eax
        pop    ecx
        push   edx
        push   ecx
        push   eax
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    ecx, dword ptr [esi + ebx + 0x48]
        or     ecx, ecx
        je     loc_9_247C
        sub    ecx, eax
        jb     loc_9_247C
        je     loc_9_242E
        mov    dword ptr [esi + ebx + 0x48], ecx
        push   0
        push   ecx
        push   dword ptr [esi + ebx + 0x44]
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        mov    dword ptr [esi + ebx + 0x44], eax
        jmp    loc_9_2441
loc_9_242E:
        mov    dword ptr [esi + ebx + 0x48], ecx
        xchg   dword ptr [esi + ebx + 0x44], ecx
        push   0
        push   ecx
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_9_2441:
        pop    eax
        pop    ecx
        pop    edx
        push   ecx
        push   eax
        push   edx
        xor    eax, eax
        test   byte ptr [esi + ebx + 0x2c], 0x20
        je     loc_9_2455
        or     eax, 0x80
loc_9_2455:
        push   eax
        push   ecx
        push   dword ptr [edx]
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        or     eax, eax
        je     loc_9_247C
        mov    esi, edx
        pop    edx
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        and    byte ptr [edx + 0x2c], 0xfb
        pop    ecx
        pop    eax
        mov    dword ptr [edx + 0x28], eax
        jmp    loc_9_21A9
loc_9_247C:
        add    esp, 4
loc_9_247F:
        add    esp, 8
loc_9_2482:
        mov    eax, dword ptr [edx + 0x28]
        test   byte ptr [edx + 0x2c], 0x40
        je     loc_9_2491
        add    eax, dword ptr [0xa4]         ; -> obj6:0x00A4
loc_9_2491:
        add    eax, 3
        shr    eax, 2
        mov    word ptr [ebp + 0x10], ax
        jmp    loc_9_1934
loc_9_24A0:
        movzx  eax, byte ptr [ebp + 0x1c]
        cmp    al, 1
        jbe    loc_9_24B9
        cmp    al, 2
        jne    loc_9_2EE9
        mov    byte ptr [ebp + 0x1c], 0
        jmp    loc_9_14DC
loc_9_24B9:
        mov    byte ptr [ebp + 0x1d], 0x91
        ret    
loc_9_24BE:
        call   sub_9_00A4
        jb     loc_9_1BB8
        movzx  edx, word ptr [ebp + 0x14]
        cmp    edx, 0xff
        jae    loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 0x280]
        or     edx, edx
        je     loc_9_1BB8
        movzx  eax, byte ptr [ebp + 0x1c]
        cmp    al, 1
        ja     loc_9_2EE9
        or     al, al
        jne    loc_9_2518
        lea    esi, [edx + 0x2e]
        movzx  edi, word ptr [ebp + 0x38]
        shl    edi, 4
        movzx  ecx, word ptr [ebp]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        mov    ecx, 8
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_9_14DC
loc_9_2518:
        test   byte ptr [edx + 0x2c], 0x80
        jne    loc_9_1BB8
        movzx  esi, word ptr [ebp + 0x3c]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        mov    ecx, 8
        push   edi
        mov    edi, esi
        xor    al, al
        cld    
        repe scasb al, byte ptr es:[edi]
        pop    edi
        jne    loc_9_2548
        and    byte ptr [edx + 0x2c], 0xfe
        jmp    loc_9_2580
loc_9_2548:
        mov    ecx, 0xff
        xor    eax, eax
loc_9_254F:
        push   eax
        mov    eax, dword ptr [edi + eax*4 + 0x280]
        or     eax, eax
        je     loc_9_2578
        test   byte ptr [eax + 0x2c], 1
        je     loc_9_2578
        cmp    eax, edx
        je     loc_9_2578
        push   esi
        push   edi
        push   ecx
        lea    edi, [eax + 0x2e]
        mov    ecx, 8
        cld    
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    ecx
        pop    edi
        pop    esi
        je     loc_9_2590
loc_9_2578:
        pop    eax
        inc    eax
        loop   loc_9_254F
        or     byte ptr [edx + 0x2c], 1
loc_9_2580:
        lea    edi, [edx + 0x2e]
        mov    ecx, 8
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_9_14DC
loc_9_2590:
        pop    eax
        mov    byte ptr [ebp + 0x1d], 0xa1
        ret    
loc_9_2596:
        call   sub_9_00A4
        jb     loc_9_2BF4
        movzx  eax, byte ptr [ebp + 0x1c]
        cmp    al, 2
        ja     loc_9_2EE9
        je     loc_9_2650
        or     al, al
        je     loc_9_2602
        lea    esi, [edi + 0x280]
        movzx  edi, word ptr [ebp + 0x3c]
        shl    edi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        mov    ecx, 0xff
loc_9_25D2:
        cld    
        lodsd  eax, dword ptr [esi]
        or     eax, eax
        je     loc_9_25FB
        push   esi
        push   edi
        push   ecx
        lea    esi, [eax + 0x2e]
        mov    ecx, 8
        cld    
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_9_25F8
        movzx  edx, byte ptr [eax + 0x2d]
        mov    word ptr [ebp + 0x14], dx
        add    esp, 0xc
        jmp    loc_9_14DC
loc_9_25F8:
        pop    ecx
        pop    edi
        pop    esi
loc_9_25FB:
        loop   loc_9_25D2
        mov    byte ptr [ebp + 0x1d], 0xa0
        ret    
loc_9_2602:
        mov    edx, edi
        movzx  edi, word ptr [ebp + 0x38]
        shl    edi, 4
        movzx  ecx, word ptr [ebp]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        lea    esi, [edx + 0x280]
        xor    edx, edx
        mov    ecx, 0xff
loc_9_2621:
        cld    
        lodsd  eax, dword ptr [esi]
        or     eax, eax
        je     loc_9_2646
        push   esi
        push   edi
        push   ecx
        movzx  ecx, byte ptr [eax + 0x2d]
        mov    word ptr [edi], cx
        lea    esi, [eax + 0x2e]
        lea    edi, [edi + 2]
        mov    ecx, 8
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        pop    edi
        pop    esi
        add    edi, 0xa
        inc    edx
loc_9_2646:
        loop   loc_9_2621
        mov    byte ptr [ebp + 0x1c], dl
        jmp    loc_9_14DC
loc_9_2650:
        mov    word ptr [ebp + 0x10], 0xff
        jmp    loc_9_14DC
loc_9_265B:
        call   sub_9_00A4
        jb     loc_9_1BB8
        call   sub_9_0FE2
        jb     loc_9_2BF4
        cmp    byte ptr [ebp + 0x1c], 1
        ja     loc_9_2EE9
        movzx  edx, word ptr [ebp + 0x14]
        cmp    edx, 0xff
        jae    loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 0x280]
        or     edx, edx
        je     loc_9_1BB8
        movzx  esi, word ptr [ebp + 0x3c]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        push   esi
        movzx  eax, word ptr [esi + 7]
        shl    eax, 4
        movzx  ecx, word ptr [esi + 5]
        add    eax, ecx
        movzx  ecx, byte ptr [esi + 4]
        mov    esi, eax
        movzx  eax, byte ptr [ebp + 0x1c]
        call   sub_9_0EBC
        pop    esi
        jb     loc_9_19C9
        mov    ecx, dword ptr [esi]
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049000100
        jmp    loc_9_14DC
loc_9_26E1:
        call   sub_9_00A4
        jb     loc_9_1BB8
        call   sub_9_0FE2
        jb     loc_9_2BF4
        cmp    byte ptr [ebp + 0x1c], 2
        ja     loc_9_2EE9
        je     loc_9_2788
        movzx  edx, word ptr [ebp + 0x14]
        cmp    edx, 0xff
        jae    loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 0x280]
        or     edx, edx
        je     loc_9_1BB8
        movzx  esi, word ptr [ebp + 0x3c]
        shl    esi, 4
        movzx  ecx, word ptr [ebp + 4]
        add    esi, ecx
        add    esi, dword ptr [ebx + 4]
        push   esi
        movzx  eax, word ptr [esi + 7]
        shl    eax, 4
        movzx  ecx, word ptr [esi + 5]
        add    eax, ecx
        add    eax, dword ptr [ebx + 4]
        movzx  ecx, byte ptr [esi + 4]
        mov    esi, eax
        movzx  eax, byte ptr [ebp + 0x1c]
        call   sub_9_0EBC
        pop    esi
        jb     loc_9_19C9
        mov    eax, esi
        call   sub_1_05F9
        mov    eax, dword ptr [0x14]         ; -> obj6:0x0014
        call   sub_1_05F9
        mov    eax, dword ptr [esi]
        call   sub_1_05F9
        mov    al, byte ptr [ebp + 0x1c]
        mov    byte ptr [esi + 0x10], al
        movzx  eax, word ptr [ebp + 0x14]
        mov    word ptr [esi + 0xe], ax
        jmp    loc_9_14DC
loc_9_2788:
        mov    word ptr [ebp + 0x10], 0xc
        jmp    loc_9_14DC
loc_9_2793:
        mov    ecx, 5
        VMMCall  VMM.Begin_Critical_Section      ; cd201f000100
        mov    ecx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        inc    byte ptr [0x2ea]              ; -> obj6:0x02EA
        inc    byte ptr [ecx + ebx + 0x27]
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        mov    edx, eax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        shl    eax, 0x10
        mov    ax, dx
        mov    esi, eax
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        mov    ecx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    edi, dword ptr [ecx + ebx + 0x5c]
        or     edi, edi
        je     loc_9_2BF4
        movzx  edx, word ptr [esi + 0xe]
        cmp    edx, 0xff
        jae    loc_9_1BB8
        mov    edx, dword ptr [edi + edx*4 + 0x280]
        or     edx, edx
        je     loc_9_1BB8
        cmp    byte ptr [esi + 0x10], 1
        ja     loc_9_2EE9
        movzx  eax, word ptr [esi + 0xc]
        shl    eax, 4
        movzx  ecx, word ptr [esi + 0xa]
        add    eax, ecx
        add    eax, dword ptr [ebx + 4]
        push   eax
        movzx  ecx, byte ptr [esi + 9]
        movzx  eax, byte ptr [esi + 0x10]
        pop    esi
        call   sub_9_0EBC
        pushfd 
        mov    ecx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        cmp    byte ptr [ecx + ebx + 0x27], 0
        je     loc_9_2836
        dec    byte ptr [ecx + ebx + 0x27]
loc_9_2836:
        cmp    byte ptr [0x2ea], 0           ; -> obj6:0x02EA
        je     loc_9_2845
        dec    byte ptr [0x2ea]              ; -> obj6:0x02EA
loc_9_2845:
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        popfd  
        jb     loc_9_19C9
        jmp    loc_9_14DC
loc_9_2857:
        cmp    byte ptr [ebp + 0x1c], 1
        ja     loc_9_2EE9
        movzx  edx, word ptr [ebp + 0x3c]
        shl    edx, 4
        movzx  ecx, word ptr [ebp + 4]
        add    edx, ecx
        add    edx, dword ptr [ebx + 4]
        mov    ecx, dword ptr [edx]
        or     ecx, ecx
        je     loc_9_14DC
        cmp    ecx, 0x100000
        ja     loc_9_2A2B
        xor    ecx, ecx
        cmp    byte ptr [edx + 4], 1
        ja     loc_9_2A26
        jne    loc_9_28D3
        or     cl, 0x20
        push   ecx
        push   edx
        call   sub_9_00A4
        pop    edx
        pop    ecx
        jb     loc_9_1BB8
        push   edx
        movzx  edx, word ptr [edx + 5]
        or     edx, edx
        je     loc_9_28CB
        cmp    edx, 0xff
        jae    loc_9_28CB
        mov    edx, dword ptr [edi + edx*4 + 0x280]
        or     edx, edx
        je     loc_9_28CB
        bt     word ptr [edx + 0x2c], 7
        jmp    loc_9_28CC
loc_9_28CB:
        stc    
loc_9_28CC:
        pop    edx
        jb     loc_9_1BB8
loc_9_28D3:
        cmp    byte ptr [edx + 0xb], 1
        ja     loc_9_2A26
        jne    loc_9_291D
        or     cl, 0x10
        push   edx
        push   ecx
        call   sub_9_00A4
        pop    ecx
        pop    edx
        jb     loc_9_1BB8
        push   edx
        movzx  edx, word ptr [edx + 0xc]
        or     edx, edx
        je     loc_9_2915
        cmp    edx, 0xff
        jae    loc_9_2915
        mov    edx, dword ptr [edi + edx*4 + 0x280]
        or     edx, edx
        je     loc_9_2915
        bt     word ptr [edx + 0x2c], 7
        jmp    loc_9_2916
loc_9_2915:
        stc    
loc_9_2916:
        pop    edx
        jb     loc_9_1BB8
loc_9_291D:
        test   cl, 0x30
        je     loc_9_2A26
        mov    esi, edx
        mov    al, byte ptr [esi + 4]
        and    al, byte ptr [esi + 0xb]
        je     loc_9_2976
        movzx  eax, word ptr [esi + 5]
        cmp    ax, word ptr [esi + 0xc]
        jne    loc_9_2976
        movzx  eax, word ptr [esi + 9]
        movzx  edx, word ptr [esi + 7]
        shl    eax, 0xe
        add    eax, edx
        push   eax
        movzx  edx, word ptr [esi + 0x10]
        movzx  eax, word ptr [esi + 0xe]
        shl    edx, 0xe
        add    edx, eax
        pop    eax
        sub    edx, eax
        je     loc_9_14DC
        jg     loc_9_2965
        or     cl, 0x80
        neg    edx
loc_9_2965:
        cmp    edx, dword ptr [esi]
        jae    loc_9_2976
        cmp    byte ptr [ebp + 0x1c], 1
        je     loc_9_2A21
        or     cl, 0x40
loc_9_2976:
        push   ecx
        mov    ecx, dword ptr [esi]
        lea    eax, [esi + 4]
        call   sub_9_2A30
        or     ah, ah
        jne    loc_9_2A1B
        push   edx
        lea    eax, [esi + 0xb]
        call   sub_9_2A30
        pop    esi
        or     ah, ah
        jne    loc_9_2A1B
        mov    edi, edx
        cmp    byte ptr [ebp + 0x1c], 1
        je     loc_9_29EA
        test   byte ptr [esp], 0x40
        je     loc_9_29D5
        mov    byte ptr [ebp + 0x1d], 0x92
        test   byte ptr [esp], 0x80
        jne    loc_9_29D9
        add    esi, ecx
        sub    esi, 4
        add    edi, ecx
        sub    edi, 4
        std    
        mov    eax, ecx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        add    esi, 3
        add    edi, 3
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cld    
        jmp    loc_9_29E8
loc_9_29D5:
        mov    byte ptr [ebp + 0x1d], 0
loc_9_29D9:
        cld    
        mov    eax, ecx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_9_29E8:
        pop    eax
        ret    
loc_9_29EA:
        push   ecx
        shr    ecx, 2
        jecxz  loc_9_2A03
        mov    edx, 4
loc_9_29F5:
        mov    eax, dword ptr [esi]
        mov    ebx, dword ptr [edi]
        mov    dword ptr [edi], eax
        mov    dword ptr [esi], ebx
        add    esi, edx
        add    edi, edx
        loop   loc_9_29F5
loc_9_2A03:
        pop    ecx
        and    ecx, 3
        jecxz  loc_9_2A15
loc_9_2A09:
        mov    al, byte ptr [esi]
        mov    bl, byte ptr [edi]
        mov    byte ptr [edi], al
        mov    byte ptr [esi], bl
        inc    esi
        inc    edi
        loop   loc_9_2A09
loc_9_2A15:
        pop    eax
        jmp    loc_9_14DC
loc_9_2A1B:
        pop    ecx
        jmp    loc_9_19C9
loc_9_2A21:
        mov    byte ptr [ebp + 0x1d], 0x97
        ret    
loc_9_2A26:
        mov    byte ptr [ebp + 0x1d], 0x98
        ret    
loc_9_2A2B:
        mov    byte ptr [ebp + 0x1d], 0x96
        ret    
sub_9_2A30:
        push   ecx
        mov    dl, byte ptr [eax]
        or     dl, dl
        je     loc_9_2A9A
        movzx  edx, word ptr [eax + 1]
        mov    edx, dword ptr [edi + edx*4 + 0x280]
        cmp    word ptr [eax + 3], 0x4000
        jae    loc_9_2AE0
        test   byte ptr [edx + 0x2c], 4
        jne    loc_9_2AE4
        mov    ecx, dword ptr [edx + 0x28]
        shr    ecx, 2
        cmp    ecx, 0xffff
        ja     loc_9_2A6C
        cmp    cx, word ptr [eax + 5]
        jbe    loc_9_2AE4
loc_9_2A6C:
        push   eax
        movzx  eax, word ptr [eax + 5]
        sub    ecx, eax
        pop    eax
        shl    ecx, 0xe
        push   eax
        movzx  eax, word ptr [eax + 3]
        sub    ecx, eax
        pop    eax
        cmp    ecx, dword ptr [esp]
        jb     loc_9_2AE4
        movzx  ecx, word ptr [eax + 5]
        shl    ecx, 0xe
        push   eax
        movzx  eax, word ptr [eax + 3]
        add    ecx, eax
        pop    eax
        add    ecx, dword ptr [edx + 4]
        mov    edx, ecx
        jmp    loc_9_2ADC
loc_9_2A9A:
        movzx  edx, word ptr [eax + 5]
        shl    edx, 4
        movzx  ecx, word ptr [eax + 3]
        add    edx, ecx
        mov    ecx, edx
        add    ecx, dword ptr [esp]
        cmp    ecx, 0x100000
        jae    loc_9_2AEC
        cmp    word ptr [edi + 0x68a], 0
        je     loc_9_2AD9
        movzx  eax, word ptr [edi + 0x68a]
        shl    eax, 4
        cmp    ecx, eax
        jbe    loc_9_2AD9
        cmp    edx, eax
        jbe    loc_9_2AE8
        add    eax, 0x10000
        cmp    edx, eax
        jb     loc_9_2AE8
loc_9_2AD9:
        add    edx, dword ptr [ebx + 4]
loc_9_2ADC:
        pop    ecx
        mov    ah, 0
        ret    
loc_9_2AE0:
        pop    ecx
        mov    ah, 0x95
        ret    
loc_9_2AE4:
        pop    ecx
        mov    ah, 0x93
        ret    
loc_9_2AE8:
        pop    ecx
        mov    ah, 0x94
        ret    
loc_9_2AEC:
        pop    ecx
        mov    ah, 0xa2
        ret    
loc_9_2AF0:
        call   sub_9_00A4
        mov    ecx, 0
        jb     loc_9_2B93
        call   sub_9_0FE2
        jb     loc_9_2BF4
        mov    al, byte ptr [ebp + 0x1c]
        cmp    al, 1
        ja     loc_9_2EE9
        or     al, al
        jne    loc_9_2B7D
        movzx  edi, word ptr [ebp + 0x38]
        shl    edi, 4
        movzx  ecx, word ptr [ebp]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        push   edi
        lea    eax, [edx + 0x240]
        lea    edx, [edx + 4]
        mov    esi, edx
        mov    ecx, 0x40
loc_9_2B3B:
        test   byte ptr [edx + 2], 1
        jne    loc_9_2B6E
        push   ecx
        push   edi
        push   eax
        mov    eax, edx
        sub    eax, esi
        shl    eax, 7
        mov    word ptr [edi], ax
        mov    eax, edx
        sub    eax, esi
        shr    eax, 3
        pop    edi
        push   edi
        mov    ecx, 0x40
        cld    
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edi
        pop    eax
        sub    ecx, eax
        pop    edi
        mov    word ptr [edi + 2], cx
        pop    ecx
        add    edi, 4
loc_9_2B6E:
        add    edx, 8
        loop   loc_9_2B3B
        pop    edx
        mov    eax, edi
        sub    eax, edx
        shr    eax, 2
        jmp    loc_9_2B93
loc_9_2B7D:
        lea    edi, [edi + 4]
        mov    ecx, 0x40
        xor    eax, eax
loc_9_2B87:
        test   byte ptr [edi + 2], 1
        jne    loc_9_2B8E
        inc    eax
loc_9_2B8E:
        add    edi, 8
        loop   loc_9_2B87
loc_9_2B93:
        mov    word ptr [ebp + 0x18], ax
        jmp    loc_9_14DC
loc_9_2B9C:
        call   sub_9_00A4
        jb     loc_9_2BF4
        movzx  edi, word ptr [ebp + 0x38]
        shl    edi, 4
        movzx  ecx, word ptr [ebp]
        add    edi, ecx
        add    edi, dword ptr [ebx + 4]
        cmp    byte ptr [edx + 0x691], 2
        jae    loc_9_2BEF
        mov    al, byte ptr [ebp + 0x1c]
        or     al, al
        jne    loc_9_2BE2
        mov    esi, 0xee8                    ; -> obj1:0x0EE8
        mov    ecx, 0xa
        push   edi
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        movzx  eax, word ptr [edx + 0x688]
        mov    word ptr [edi + 4], ax
        jmp    loc_9_14DC
loc_9_2BE2:
        cmp    al, 1
        je     loc_9_1575
        jmp    loc_9_2EE9
loc_9_2BEF:
        mov    byte ptr [ebp + 0x1d], 0xa4
        ret    
loc_9_2BF4:
        mov    byte ptr [ebp + 0x1d], 0x80
        ret    
loc_9_2BF9:
        call   sub_9_00A4
        jb     loc_9_2BEF
        call   sub_9_0FE2
        jb     loc_9_2BF4
        cmp    byte ptr [edi + 0x691], 2
        jae    loc_9_2BEF
        movzx  eax, byte ptr [ebp + 0x1c]
        cmp    al, 8
        ja     loc_9_2EE9
        jmp    dword ptr [eax*4 + 0xec4]     ; -> obj1:0x0EC4
loc_9_2C23:
        mov    byte ptr [ebp + 0x1d], 0x9e
        ret    
loc_9_2C28:
        mov    al, byte ptr [edi + 0x690]
        mov    byte ptr [ebp + 0x10], al
        or     al, al
        jne    loc_9_14DC
        mov    eax, dword ptr [edi + 0x684]
        mov    word ptr [ebp], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x38], ax
        mov    eax, dword ptr [edi + 0x680]
        or     eax, eax
        je     loc_9_14DC
        bts    word ptr [edi + 0x68e], 5
        jb     loc_9_2BF4
        lea    esi, [edi + 4]
        push   edi
        mov    edi, eax
        call   sub_9_0B04
        pop    edi
        btr    word ptr [edi + 0x68e], 5
        jmp    loc_9_14DC
loc_9_2C7D:
        movzx  eax, byte ptr [ebp + 0x10]
        cmp    al, 0x10
        jae    loc_9_2E5F
        or     al, al
        je     loc_9_2CAA
        mov    edx, eax
        dec    edx
        cmp    dword ptr [edi + edx*4 + 0x204], 0
        je     loc_9_2E64
        cmp    al, byte ptr [edi + 0x690]
        je     loc_9_14DC
loc_9_2CAA:
        push   eax
        push   edi
        lea    esi, [edi + 4]
        cmp    byte ptr [edi + 0x690], 0
        je     loc_9_2CE9
        movzx  edx, byte ptr [edi + 0x690]
        dec    edx
        bts    word ptr [edi + 0x68e], 5
        jb     loc_9_2D57
        mov    edi, dword ptr [edi + edx*4 + 0x204]
        mov    ecx, 0x80
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        btr    word ptr [edi + 0x68e], 5
        push   edi
loc_9_2CE9:
        pop    edi
        pop    eax
        mov    byte ptr [edi + 0x690], al
        or     al, al
        jne    loc_9_2D5F
        movzx  edx, word ptr [ebp + 0x38]
        mov    word ptr [edi + 0x686], dx
        shl    edx, 4
        movzx  ecx, word ptr [ebp]
        mov    word ptr [edi + 0x684], cx
        add    edx, ecx
        je     loc_9_2D15
        add    edx, dword ptr [ebx + 4]
loc_9_2D15:
        mov    dword ptr [edi + 0x680], edx
        or     edx, edx
        je     loc_9_14DC
        mov    esi, edx
        sub    esp, 0x200
        mov    edx, esp
        call   sub_9_0B8E
        jb     loc_9_1E06
        mov    eax, esp
        mov    ecx, 1
        call   sub_11_0074
        lahf   
        add    esp, 0x200
        sahf   
        jb     loc_9_2BF4
        jmp    loc_9_14DC
loc_9_2D57:
        add    esp, 8
        jmp    loc_9_2BF4
loc_9_2D5F:
        movzx  eax, byte ptr [edi + 0x690]
        dec    eax
        mov    eax, dword ptr [edi + eax*4 + 0x204]
        or     eax, eax
        je     loc_9_2BF4
        mov    ecx, 1
        call   sub_11_0074
        jb     loc_9_2BF4
        jmp    loc_9_14DC
loc_9_2D8B:
        movzx  eax, word ptr [edi + 0x688]
        mov    word ptr [ebp + 0x14], ax
        jmp    loc_9_14DC
loc_9_2D9B:
        xor    eax, eax
        mov    ecx, 0xf
        bts    word ptr [edi + 0x68e], 4
        jb     loc_9_2DC3
loc_9_2DAD:
        cmp    dword ptr [edi + eax*4 + 0x204], 0
        je     loc_9_2DD3
        inc    eax
        loop   loc_9_2DAD
loc_9_2DBA:
        btr    word ptr [edi + 0x68e], 4
loc_9_2DC3:
        mov    byte ptr [ebp + 0x1d], 0x9b
        ret    
loc_9_2DC8:
        btr    word ptr [edi + 0x68e], 5
        jmp    loc_9_2DBA
loc_9_2DD3:
        bts    word ptr [edi + 0x68e], 5
        jb     loc_9_2DBA
        push   eax
        push   0x101
        push   0x200
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_9_2DC8
        mov    dword ptr [edi + ecx*4 + 0x204], eax
        btr    word ptr [edi + 0x68e], 4
        lea    esi, [edi + 4]
        push   edi
        mov    edi, eax
        push   ecx
        mov    ecx, 0x80
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    eax
        pop    edi
        btr    word ptr [edi + 0x68e], 5
        inc    eax
        mov    byte ptr [ebp + 0x10], al
        jmp    loc_9_14DC
loc_9_2E29:
        movzx  eax, byte ptr [ebp + 0x10]
        or     al, al
        je     loc_9_14DC
        cmp    al, byte ptr [edi + 0x690]
        je     loc_9_2E64
        cmp    al, 0x10
        jae    loc_9_2E5F
        dec    eax
        xor    edx, edx
        xchg   dword ptr [edi + eax*4 + 0x204], edx
        or     edx, edx
        je     loc_9_2E64
        push   0
        push   edx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        jmp    loc_9_14DC
loc_9_2E5F:
        mov    byte ptr [ebp + 0x1d], 0x9a
        ret    
loc_9_2E64:
        mov    byte ptr [ebp + 0x1d], 0x9d
        ret    
loc_9_2E69:
        jmp    loc_9_14DC
loc_9_2E6E:
        call   sub_9_00A4
        jb     loc_9_2BF4
        mov    al, byte ptr [ebp + 0x1c]
        cmp    al, 2
        ja     loc_9_2EE9
        cmp    dword ptr [edx], 0
        jne    loc_9_2EA1
        cmp    al, 2
        je     loc_9_2BEF
        mov    eax, 0x44554d42
        mov    dword ptr [edx], eax
        mov    word ptr [ebp + 0x10], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x18], ax
        jmp    loc_9_2EB4
loc_9_2EA1:
        movzx  eax, word ptr [ebp + 0x18]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x10]
        cmp    eax, dword ptr [edx]
        jne    loc_9_2BEF
loc_9_2EB4:
        mov    al, byte ptr [ebp + 0x1c]
        or     al, al
        jne    loc_9_2EC7
        mov    byte ptr [edx + 0x691], 1
        jmp    loc_9_14DC
loc_9_2EC7:
        cmp    al, 1
        jne    loc_9_2ED7
        mov    byte ptr [edx + 0x691], 2
        jmp    loc_9_14DC
loc_9_2ED7:
        mov    byte ptr [edx + 0x691], 0
        mov    dword ptr [edx], 0
        jmp    loc_9_14DC
loc_9_2EE9:
        mov    byte ptr [ebp + 0x1d], 0x8f
        ret    
        db  000h, 000h
sub_9_2EF0:
        mov    edi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        bts    dword ptr [edi + ebx + 0x2c], 3
        jb     loc_9_2F14
        lea    edi, [edi + ebx + 0x64]
        mov    esi, 0xf28                    ; -> obj1:0x0F28
        mov    ecx, 0x88
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        jmp    loc_11_03CC
loc_9_2F14:
        clc    
        ret    
loc_9_2F16:
        call   sub_9_118C
        call   sub_8_01A1
        clc    
        ret    
V86MMGR_Get_EMS_XMS_Limits:
        push   ebx
        add    ebx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    edx, dword ptr [ebx + 0x54]
        mov    eax, dword ptr [ebx + 0x50]
        mov    edi, dword ptr [ebx + 0x40]
        mov    esi, dword ptr [ebx + 0x3c]
        shl    edx, 2
        shl    eax, 2
        shl    edi, 2
        shl    esi, 2
        test   byte ptr [ebx + 0x2f], 0x40
        jne    loc_9_2F4A
        or     edi, 0xffffffff
loc_9_2F4A:
        test   byte ptr [ebx + 0x2f], 0x80
        jne    loc_9_2F53
        or     edx, 0xffffffff
loc_9_2F53:
        xor    ecx, ecx
        test   byte ptr [ebx + 0x2c], 4
        jne    loc_9_2F5C
        inc    ecx
loc_9_2F5C:
        pop    ebx
        ret    
V86MMGR_Get_EMS_XMS_Avail:
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        xor    ecx, ecx
        xor    edx, edx
        cmp    dword ptr [8], 0              ; -> obj6:0x0008
        je     loc_9_2F8E
        test   byte ptr [esi + ebx + 0x2c], 2
        jne    loc_9_2F8E
        mov    edx, dword ptr [esi + ebx + 0x54]
        shr    edx, 2
        mov    ecx, edx
        mov    edi, dword ptr [esi + ebx + 0x5c]
        or     edi, edi
        je     loc_9_2F8E
        call   sub_9_15AF
loc_9_2F8E:
        xchg   edx, ecx
        push   ecx
        push   edx
        mov    eax, dword ptr [esi + ebx + 0x40]
        shl    eax, 2
        mov    edi, eax
        cmp    dword ptr [esi + ebx + 0x60], 0
        je     loc_9_2FCD
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x14]
        push   esi
        push   ebx
        call   sub_15_0568
        movzx  eax, word ptr [ebp + 0x1c]
        pop    ebx
        pop    esi
        mov    edi, dword ptr [esi + ebx + 0x40]
        shl    edi, 2
        pop    dword ptr [ebp + 0x14]
        pop    dword ptr [ebp + 0x10]
        pop    dword ptr [ebp + 0x1c]
        pop    ebp
loc_9_2FCD:
        pop    edx
        pop    ecx
        xor    esi, esi
        dec    esi
        ret    
V86MMGR_GetPgStatus:
        push   edx
        push   ecx
        xor    eax, eax
        cmp    byte ptr [0x1214], al         ; -> obj1:0x1214
        je     loc_9_3015
        cmp    ecx, 0x110
        jae    loc_9_3015
        or     ebx, ebx
        je     loc_9_3004
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        test   byte ptr [edx + ebx + 0x2c], 8
        je     loc_9_3015
        lea    ecx, [ebx + ecx*2 + 0x64]
        add    ecx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        jmp    loc_9_300B
loc_9_3004:
        lea    ecx, [ecx*2 + 0xf28]          ; -> obj1:0x0F28
loc_9_300B:
        mov    ax, word ptr [ecx]
        and    eax, 0x2706
        shr    eax, 1
loc_9_3015:
        pop    ecx
        pop    edx
        ret    
sub_9_3018:
        pushal 
        pushfd 
        xor    al, al
        xchg   byte ptr [0x1327], al         ; -> obj1:0x1327
        or     al, al
        je     loc_9_3031
        mov    eax, dword ptr [0x3298]       ; -> loc_4_3298
        VMMCall  VMM.Remove_V86_Break_Point      ; cd200a000100
loc_9_3031:
        call   sub_8_01A1
        popfd  
        popal  
        ret    
        db  000h, 000h, 000h, 0E9h
        dd  loc_15_0325              ; fixup
        db  0A1h
        dd  obj1:0x132C              ; fixup
        db  0B9h, 005h, 000h, 000h, 000h, 0CDh, 020h, 027h, 000h, 001h, 000h, 0E8h
        dd  sub_15_0097              ; fixup
        db  0A1h
        dd  obj1:0x132C              ; fixup
        db  0CDh, 020h, 028h, 000h, 001h, 000h, 0E9h
        dd  loc_15_0319              ; fixup
loc_9_3066:
        push   2
        push   ebx
        VMMCall  VMM._MMGR_Toggle_HMA            ; cd207d000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_9_3099
        mov    eax, dword ptr [edi]
        test   byte ptr [edi + 0x14], 2
        je     loc_9_3081
        or     byte ptr [edi + 0x14], 4
loc_9_3081:
        push   0
        push   0
        push   0x10
        push   0x100
        push   ebx
        push   eax
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        or     eax, eax
loc_9_3099:
        ret    
        db  0B0h, 093h, 0E9h
        dd  loc_15_0050              ; fixup
        db  0F6h, 044h, 01Eh, 02Ch, 004h, 0B0h, 090h, 00Fh, 085h
        dd  loc_15_0050              ; fixup
        db  083h, 07Ch, 01Eh, 060h, 000h, 074h, 0E5h, 08Bh, 07Ch, 01Eh, 060h, 066h, 00Fh, 0BAh, 077h, 014h
        db  001h, 073h, 0D9h, 0A1h
        dd  obj1:0x1200              ; fixup
        db  087h, 007h, 06Ah, 000h, 050h, 0CDh, 020h, 055h, 000h, 001h, 000h, 059h, 059h, 066h, 00Fh, 0BAh
        db  077h, 014h, 002h, 06Ah, 008h, 053h, 0CDh, 020h, 07Dh, 000h, 001h, 000h, 059h, 059h, 00Bh, 0C0h
        db  00Fh, 084h
        dd  loc_15_0319              ; fixup
        db  06Ah, 000h, 06Ah, 000h, 06Ah, 010h, 068h, 000h, 001h, 000h, 000h, 053h, 0FFh, 035h
        dd  obj1:0x1200              ; fixup
        db  0CDh, 020h, 05Dh, 000h, 001h, 000h, 083h, 0C4h, 018h, 0E9h
        dd  loc_15_0319              ; fixup
        db  00Fh, 0B6h, 00Dh
        dd  obj1:0x1320              ; fixup
        db  00Bh, 0C9h, 00Fh, 084h, 094h, 000h, 000h, 000h, 08Bh, 07Ch, 01Eh, 060h, 0E8h, 0DFh, 001h, 000h
        db  000h, 00Fh, 082h, 085h, 000h, 000h, 000h, 033h, 0C0h, 00Fh, 0BFh, 055h, 014h, 00Bh, 0D2h, 079h
        db  005h, 0BAh, 0FFh, 07Fh, 000h, 000h, 083h, 0FAh, 010h, 077h, 002h, 0B2h, 010h, 08Bh, 036h, 00Fh
        db  0BFh, 04Eh, 006h, 03Bh, 0CAh, 07Dh, 008h, 03Bh, 0C8h, 07Ch, 0F2h, 08Bh, 0C1h, 0EBh, 0EEh, 066h
        db  083h, 07Eh, 004h, 0FFh, 074h, 052h, 02Bh, 0CAh, 083h, 0F9h, 010h, 072h, 02Fh, 051h, 052h, 068h
        db  001h, 001h, 000h, 000h, 06Ah, 008h, 0CDh, 020h, 04Fh, 000h, 001h, 000h, 059h, 059h, 05Ah, 059h
        db  00Bh, 0C0h, 074h, 018h, 066h, 089h, 048h, 006h, 066h, 089h, 056h, 006h, 08Bh, 00Eh, 089h, 008h
        db  089h, 006h, 00Fh, 0B7h, 04Eh, 004h, 003h, 0CAh, 066h, 089h, 048h, 004h, 00Fh, 0B7h, 046h, 004h
        db  066h, 089h, 045h, 010h, 00Fh, 0B7h, 046h, 006h, 066h, 089h, 045h, 014h, 080h, 04Eh, 007h, 080h
        db  066h, 0C7h, 045h, 01Ch, 001h, 000h, 0F8h, 0C3h, 00Bh, 0C0h, 075h, 007h, 0B0h, 0B1h, 0E9h
        dd  loc_15_0050              ; fixup
        db  066h, 089h, 045h, 014h, 0B0h, 0B0h, 0E9h
        dd  loc_15_0050              ; fixup
        db  00Fh, 0B7h, 055h, 014h, 0E8h, 0C9h, 002h, 000h, 000h, 075h, 045h, 066h, 00Fh, 0BAh, 076h, 006h
        db  00Fh, 073h, 03Dh, 066h, 08Bh, 041h, 004h, 066h, 003h, 041h, 006h, 066h, 039h, 046h, 004h, 075h
        db  019h, 066h, 08Bh, 046h, 006h, 066h, 001h, 041h, 006h, 08Bh, 006h, 089h, 001h, 087h, 0F1h, 06Ah
        db  000h, 051h, 0CDh, 020h, 051h, 000h, 001h, 000h, 059h, 059h, 08Bh, 0CEh, 08Bh, 031h, 066h, 083h
        db  07Eh, 004h, 0FFh, 074h, 006h, 0F6h, 046h, 007h, 080h, 074h, 0C8h, 0E9h
        dd  loc_15_0319              ; fixup
        db  0B0h, 0B2h, 0E9h
        dd  loc_15_0050              ; fixup
        db  00Fh, 0B7h, 055h, 014h, 0E8h, 072h, 002h, 000h, 000h, 075h, 0EEh, 0F6h, 046h, 007h, 080h, 074h
        db  0E8h, 00Fh, 0BFh, 055h, 010h, 00Bh, 0D2h, 079h, 005h, 0BAh, 0FFh, 07Fh, 000h, 000h, 083h, 0FAh
        db  010h, 077h, 002h, 0B2h, 010h, 08Bh, 00Eh, 080h, 0C6h, 080h, 066h, 02Bh, 056h, 006h, 074h, 051h
        db  072h, 054h, 08Bh, 0C2h, 0F7h, 0D8h, 050h, 066h, 08Bh, 046h, 004h, 066h, 003h, 046h, 006h, 080h
        db  0ECh, 080h, 066h, 039h, 041h, 004h, 058h, 00Fh, 085h, 08Ch, 000h, 000h, 000h, 00Fh, 0BFh, 041h
        db  006h, 02Bh, 0C2h, 00Fh, 08Ch, 080h, 000h, 000h, 000h, 083h, 0F8h, 010h, 072h, 00Eh, 066h, 029h
        db  051h, 006h, 066h, 001h, 051h, 004h, 066h, 001h, 056h, 006h, 0EBh, 015h, 003h, 0C2h, 066h, 001h
        db  046h, 006h, 08Bh, 001h, 089h, 006h, 06Ah, 000h, 051h, 0CDh, 020h, 051h, 000h, 001h, 000h, 059h
        db  059h, 0E9h
        dd  loc_15_0319              ; fixup
        db  0F6h, 041h, 007h, 080h, 075h, 011h, 066h, 08Bh, 046h, 004h, 066h, 003h, 046h, 006h, 080h, 0ECh
        db  080h, 066h, 039h, 041h, 004h, 074h, 0C1h, 066h, 083h, 0FAh, 0F0h, 077h, 0DEh, 052h, 068h, 001h
        db  001h, 000h, 000h, 06Ah, 008h, 0CDh, 020h, 04Fh, 000h, 001h, 000h, 059h, 059h, 05Ah, 00Bh, 0C0h
        db  074h, 0C9h, 0F7h, 0DAh, 066h, 029h, 056h, 006h, 066h, 089h, 050h, 006h, 00Fh, 0B7h, 04Eh, 004h
        db  066h, 003h, 04Eh, 006h, 080h, 0EDh, 080h, 066h, 089h, 048h, 004h, 08Bh, 00Eh, 089h, 008h, 089h
        db  006h, 0EBh, 0A8h, 003h, 0C2h, 07Dh, 002h, 033h, 0C0h, 066h, 003h, 046h, 006h, 080h, 0ECh, 080h
        db  0E9h, 0B3h, 0FEh, 0FFh, 0FFh, 08Dh, 077h, 00Ch, 083h, 07Fh, 018h, 000h, 074h, 001h, 0C3h, 0C8h
        db  00Ch, 000h, 000h, 0CDh, 020h, 0D5h, 000h, 001h, 000h, 040h, 08Bh, 0F0h, 0C1h, 0E6h, 008h, 08Dh
        db  044h, 043h, 064h, 003h, 005h
        dd  obj1:0x11FC              ; fixup
        db  033h, 0D2h, 089h, 055h, 0FCh, 089h, 055h, 0F4h, 08Dh, 057h, 00Ch, 033h, 0C9h, 0F6h, 040h, 001h
        db  040h, 074h, 066h, 0F6h, 040h, 001h, 005h, 075h, 060h, 0F6h, 040h, 001h, 010h, 075h, 01Ah, 050h
        db  052h, 051h, 08Bh, 0C6h, 0C1h, 0E8h, 008h, 06Ah, 000h, 053h, 06Ah, 001h, 050h, 0CDh, 020h, 072h
        db  000h, 001h, 000h, 083h, 0C4h, 010h, 059h, 05Ah, 058h, 080h, 048h, 001h, 012h, 0FFh, 045h, 0FCh
        db  03Bh, 0CEh, 075h, 008h, 066h, 0FFh, 042h, 007h, 0FEh, 0C5h, 0EBh, 02Dh, 050h, 052h, 068h, 001h
        db  001h, 000h, 000h, 06Ah, 008h, 0CDh, 020h, 04Fh, 000h, 001h, 000h, 059h, 059h, 05Ah, 089h, 002h
        db  08Bh, 0D0h, 058h, 00Bh, 0D2h, 00Fh, 084h, 0C6h, 000h, 000h, 000h, 066h, 089h, 072h, 004h, 066h
        db  0C7h, 042h, 006h, 000h, 001h, 08Bh, 0CEh, 0FEh, 0C5h, 083h, 0C0h, 002h, 066h, 081h, 0C6h, 000h
        db  001h, 073h, 08Ah, 08Dh, 04Fh, 00Ch, 089h, 00Ah, 083h, 07Dh, 0FCh, 000h, 00Fh, 084h, 09Fh, 000h
        db  000h, 000h, 068h, 080h, 000h, 000h, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 06Ah, 000h, 053h
        db  06Ah, 000h, 0FFh, 075h, 0FCh, 0CDh, 020h, 053h, 000h, 001h, 000h, 083h, 0C4h, 020h, 089h, 055h
        db  0F8h, 00Bh, 0C0h, 074h, 07Ch, 089h, 047h, 018h, 08Bh, 077h, 00Ch, 056h, 057h, 00Fh, 0B7h, 046h
        db  004h, 00Fh, 0B7h, 04Eh, 006h, 0C1h, 0E0h, 004h, 050h, 003h, 043h, 004h, 08Bh, 0F0h, 08Bh, 07Dh
        db  0F4h, 0C1h, 0E7h, 00Ch, 003h, 07Dh, 0F8h, 0C1h, 0E1h, 002h, 0FCh, 0F3h, 0A5h, 058h, 05Fh, 05Eh
        db  0C1h, 0E8h, 00Ch, 06Ah, 000h, 0FFh, 075h, 0F4h, 06Ah, 001h, 050h, 053h, 0FFh, 077h, 018h, 0CDh
        db  020h, 05Dh, 000h, 001h, 000h, 083h, 0C4h, 018h, 00Fh, 0B7h, 046h, 006h, 0C1h, 0E8h, 008h, 001h
        db  045h, 0F4h, 08Bh, 036h, 066h, 083h, 07Eh, 004h, 0FFh, 075h, 0B0h, 08Bh, 035h
        dd  obj1:0x11FC              ; fixup
        db  0F6h, 044h, 01Eh, 02Eh, 001h, 075h, 013h, 06Ah, 000h, 06Ah, 000h, 0FFh, 075h, 0FCh, 0FFh, 077h
        db  018h, 0CDh, 020h, 057h, 000h, 001h, 000h, 083h, 0C4h, 010h, 08Dh, 077h, 00Ch, 0F8h, 0C9h, 0C3h
        db  033h, 0F6h, 087h, 077h, 018h, 00Bh, 0F6h, 074h, 00Bh, 06Ah, 000h, 056h, 0CDh, 020h, 055h, 000h
        db  001h, 000h, 059h, 059h, 08Bh, 077h, 00Ch, 00Bh, 0F6h, 074h, 017h, 066h, 083h, 07Eh, 004h, 0FFh
        db  074h, 010h, 0FFh, 036h, 06Ah, 000h, 056h, 0CDh, 020h, 051h, 000h, 001h, 000h, 059h, 059h, 05Eh
        db  0EBh, 0E5h, 08Dh, 077h, 00Ch, 089h, 077h, 00Ch, 0F9h, 0C9h, 0C3h, 08Bh, 07Ch, 01Eh, 060h, 00Bh
        db  0FFh, 074h, 014h, 08Dh, 077h, 00Ch, 066h, 083h, 0FAh, 0FFh, 074h, 00Bh, 08Bh, 0CEh, 08Bh, 036h
        db  066h, 039h, 056h, 004h, 072h, 0F6h, 0C3h, 083h, 0C9h, 001h, 0C3h, 000h
V86MMGR_PM_API:
        or     byte ptr [ebp + 0x2c], 1
        movzx  eax, word ptr [ebp + 0x1c]
        sub    eax, 1
        jb     loc_9_34FA
        ja     loc_9_3504
        mov    ecx, 0x1204                   ; -> obj1:0x1204
        cmp    dword ptr [0x9c], eax         ; -> obj6:0x009C
        je     loc_9_34D2
        or     al, 2
loc_9_34D2:
        test   byte ptr [ecx + 1], 9
        je     loc_9_34D9
        inc    eax
loc_9_34D9:
        test   byte ptr [ecx + 1], 4
        je     loc_9_34E1
        or     al, 4
loc_9_34E1:
        cmp    word ptr [0x2e8], 0           ; -> obj6:0x02E8
        je     loc_9_34ED
        or     al, 8
loc_9_34ED:
        test   byte ptr [ecx + 1], 8
        je     loc_9_34F5
        or     al, 0x10
loc_9_34F5:
        mov    dword ptr [ebp + 0x1c], eax
        jmp    loc_9_3500
loc_9_34FA:
        mov    word ptr [ebp + 0x1c], 0x400
loc_9_3500:
        and    byte ptr [ebp + 0x2c], 0xfe
loc_9_3504:
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 10  vsize=0x84  R-X
loc_10_0000:
        push   eax
        push   esi
        movzx  eax, word ptr [edx]
        movzx  esi, ah
        push   dword ptr [esi + ebp + 0x24]
        add    edx, 2
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006000600
        pop    edx
        xchg   word ptr [esi + ebp + 0x24], dx
        movzx  edx, dx
        shl    edx, 4
        pop    esi
        test   byte ptr [ebx + 1], 1
        jne    loc_10_005E
        test   byte ptr [ebx], 0x80
        jne    loc_10_003D
        push   eax
        call   sub_10_006E
        pop    edx
        movzx  edx, dh
        mov    word ptr [edx + ebp], ax
        jmp    loc_10_006C
loc_10_003D:
        push   eax
        push   ebp
        movzx  eax, al
        add    ebp, eax
        movzx  eax, word ptr [ebp]
        add    edx, eax
        mov    dword ptr [ebp], edx
        pop    ebp
        VxDCall  V86MMGR.V86MMGR_Get_VM_Flat_Sel ; cd200c000600
        pop    edx
        movzx  edx, dh
        mov    word ptr [edx + ebp], ax
        jmp    loc_10_006C
loc_10_005E:
        movzx  eax, al
        push   eax
        movzx  eax, word ptr [eax + ebp]
        add    edx, eax
        pop    eax
        mov    dword ptr [eax + ebp], edx
loc_10_006C:
        pop    eax
        ret    
sub_10_006E:
        push   ecx
        push   edx
        mov    eax, edx
        mov    ecx, 0xffff
        VMMCall  VMM.Map_Lin_To_VM_Addr          ; cd201d000100
        movzx  eax, cx
        pop    edx
        pop    ecx
        ret    
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 11  vsize=0xd5c  R-X
loc_11_0000:
        pushal 
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        movzx  edi, byte ptr [0x68b]         ; -> loc_1_068B
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   ebx
        push   0
        push   edi
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        test   eax, eax
        je     loc_11_0070
        mov    dword ptr [esi + ebx + 0x10], eax
        movzx  ecx, byte ptr [0x68a]         ; -> obj1:0x068A
        push   0
        push   0
        push   edi
        push   ecx
        push   ebx
        push   eax
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        movzx  eax, byte ptr [0x68a]         ; -> obj1:0x068A
        shl    eax, 8
        mov    word ptr [esi + ebx + 0x1e], ax
        shl    eax, 4
        add    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + ebx + 0x20], eax
        movzx  eax, byte ptr [0x68b]         ; -> loc_1_068B
        shl    eax, 0xc
        mov    dword ptr [esi + ebx + 0x14], eax
        mov    dword ptr [esi + ebx + 0x18], eax
        popal  
        clc    
        ret    
loc_11_0070:
        stc    
        popal  
        ret    
        db  000h
sub_11_0074:
        enter  0, 0
        pushal 
        bts    word ptr [edi + 0x68e], 5
        jb     loc_11_019A
        mov    esi, eax
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        inc    eax
        mov    dword ptr [0], eax            ; -> obj6:0x0000
        lea    edx, [edi + 4]
        mov    ecx, 0x40
        xor    eax, eax
loc_11_00A0:
        test   byte ptr [edx + 2], 3
        jne    loc_11_0180
        test   byte ptr [edi + 0x68e], 4
        jne    loc_11_00BF
        cmp    eax, dword ptr [0]            ; -> obj6:0x0000
        jb     loc_11_0180
loc_11_00BF:
        cmp    byte ptr [ebp - 8], 0
        jne    loc_11_00CF
        test   byte ptr [edx + 2], 0x10
        je     loc_11_0180
loc_11_00CF:
        push   eax
        test   byte ptr [ebp - 5], 0x80
        jne    loc_11_00E8
        mov    eax, dword ptr [edx]
        cmp    eax, dword ptr [esi]
        jne    loc_11_00E8
        mov    eax, dword ptr [edx + 4]
        cmp    eax, dword ptr [esi + 4]
        je     loc_11_017F
loc_11_00E8:
        push   ecx
        push   edx
        push   esi
        mov    eax, dword ptr [esi]
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edx + 4], eax
        mov    esi, edx
        test   byte ptr [esi + 2], 0x20
        jne    loc_11_0120
        movzx  eax, byte ptr [esi]
        mov    eax, dword ptr [edi + eax*4 + 0x280]
        or     eax, eax
        je     loc_11_0120
        test   byte ptr [eax + 0x2c], 4
        jne    loc_11_0120
        movzx  ecx, byte ptr [esi + 1]
        movzx  edx, word ptr [esi + 4]
        add    edx, ecx
        cmp    edx, dword ptr [eax + 0x28]
        jbe    loc_11_013A
loc_11_0120:
        and    byte ptr [esi + 2], 0x13
        or     byte ptr [esi + 2], 0x20
        mov    byte ptr [esi], 0xff
        mov    byte ptr [esi + 1], 4
        mov    word ptr [esi + 4], 0
        mov    eax, 0xef2                    ; -> obj1:0x0EF2
loc_11_013A:
        movzx  ecx, byte ptr [esi + 1]
        movzx  edx, word ptr [esi + 4]
        mov    esi, dword ptr [esp + 0xc]
        test   byte ptr [eax + 0x2c], 0x80
        jne    loc_11_01AA
        test   byte ptr [eax + 0x2c], 0x20
        jne    loc_11_0166
        cmp    dword ptr [0x98], 0           ; -> obj6:0x0098
        jne    loc_11_0166
        call   sub_9_0EA5
        jae    loc_11_0166
        xor    eax, eax
        jmp    loc_11_0177
loc_11_0166:
        push   0x10
        push   edx
        push   ecx
        push   esi
        push   ebx
        push   dword ptr [eax]
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
loc_11_0177:
        or     eax, eax
        pop    esi
        pop    edx
        pop    ecx
        stc    
        je     loc_11_019D
loc_11_017F:
        pop    eax
loc_11_0180:
        add    esi, 8
        add    edx, 8
        add    eax, 4
        dec    ecx
        jne    loc_11_00A0
        btr    word ptr [edi + 0x68e], 5
        clc    
loc_11_019A:
        popal  
        leave  
        ret    
loc_11_019D:
        pop    eax
        btr    word ptr [edi + 0x68e], 5
        stc    
        jmp    loc_11_019A
loc_11_01AA:
        push   0
        push   edx
        push   ecx
        push   esi
        push   ebx
        push   dword ptr [eax]
        call   sub_9_10D3
        add    esp, 0x18
        jmp    loc_11_0177
sub_11_01BC:
        enter  0xc, 0
        cmp    dword ptr [0x98], 0           ; -> obj6:0x0098
        je     loc_11_03C7
        mov    dword ptr [ebp - 0xc], eax
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    edi, dword ptr [edx + ebx + 0x5c]
        mov    edi, dword ptr [edi + 0x280]
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        push   0
        push   ebx
        push   dword ptr [ebp + 8]
        push   dword ptr [0x1208]            ; -> obj1:0x1208
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        mov    ecx, dword ptr [0x1208]       ; -> obj1:0x1208
        and    ecx, 3
        je     loc_11_0260
        sub    ecx, 4
        neg    ecx
        mov    dword ptr [ebp - 4], ecx
        push   dword ptr [ebp - 0xc]
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        test   eax, eax
        je     loc_11_03C9
        mov    dword ptr [esi + ebx + 4], eax
        push   0
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [0x1208]            ; -> obj1:0x1208
        push   ebx
        push   eax
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        test   eax, eax
        je     loc_11_03C9
loc_11_0260:
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, dword ptr [0x1208]       ; -> obj1:0x1208
        and    ecx, 3
        je     loc_11_02DF
        mov    eax, dword ptr [0x1208]       ; -> obj1:0x1208
        add    eax, dword ptr [ebp + 8]
        sub    eax, ecx
        mov    edx, dword ptr [esi + ebx + 0x5c]
        lea    edx, [edx + 4]
        test   eax, 3
        stc    
        jne    loc_11_028B
        shl    eax, 1
        add    eax, edx
loc_11_028B:
        test   byte ptr [eax + 2], 1
        je     loc_11_02DF
        mov    dword ptr [ebp - 8], ecx
        push   dword ptr [ebp - 0xc]
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        test   eax, eax
        je     loc_11_03C9
        mov    dword ptr [esi + ebx + 8], eax
        mov    ecx, dword ptr [0x1208]       ; -> obj1:0x1208
        add    ecx, dword ptr [ebp + 8]
        sub    ecx, dword ptr [ebp - 8]
        push   0
        push   0
        push   dword ptr [ebp - 8]
        push   ecx
        push   ebx
        push   eax
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        test   eax, eax
        je     loc_11_03C9
loc_11_02DF:
        mov    ecx, dword ptr [ebp + 8]
        sub    ecx, dword ptr [ebp - 4]
        jbe    loc_11_03C4
        sub    ecx, dword ptr [ebp - 8]
        jbe    loc_11_03C4
        push   ecx
        push   dword ptr [ebp - 0xc]
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        pop    ecx
        test   eax, eax
        je     loc_11_03C9
        mov    dword ptr [esi + ebx], eax
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], edx
        mov    dword ptr [edi + 0x28], ecx
        mov    edx, dword ptr [esi + ebx + 0x5c]
        mov    dword ptr [edx + 0x67c], ecx
        add    dword ptr [0xc], ecx          ; -> obj6:0x000C
        and    byte ptr [edi + 0x2c], 0xfb
        or     byte ptr [edi + 0x2c], 0x40
        mov    edx, dword ptr [0x1208]       ; -> obj1:0x1208
        add    edx, dword ptr [ebp - 4]
        push   0
        push   0
        push   ecx
        push   edx
        push   ebx
        push   eax
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        or     eax, eax
        je     loc_11_03C9
        mov    edx, dword ptr [0x1208]       ; -> obj1:0x1208
        add    edx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 8]
        sub    ecx, dword ptr [ebp - 4]
        sub    ecx, dword ptr [ebp - 8]
        push   ebx
        mov    edi, dword ptr [esi + ebx + 0x5c]
        mov    ebx, ecx
        mov    bh, cl
        mov    bl, 4
        cmp    bh, bl
        jae    loc_11_037A
        mov    bl, bh
loc_11_037A:
        add    ecx, 3
        shr    ecx, 2
        lea    edi, [edi + 4]
        test   edx, 3
        stc    
        jne    loc_11_0390
        shl    edx, 1
        add    edx, edi
loc_11_0390:
        xor    eax, eax
loc_11_0392:
        test   byte ptr [edx + 2], 3
        jne    loc_11_03A6
        mov    byte ptr [edx], 0
        mov    byte ptr [edx + 1], bl
        or     byte ptr [edx + 2], 0x40
        mov    word ptr [edx + 4], ax
loc_11_03A6:
        add    edx, 8
        add    eax, 4
        sub    bh, 4
        jae    loc_11_03B6
        add    bh, 4
        mov    bl, bh
loc_11_03B6:
        loop   loc_11_0392
        pop    ebx
        mov    edi, dword ptr [esi + ebx + 0x5c]
        or     byte ptr [edi + 0x68e], 4
loc_11_03C4:
        xor    eax, eax
        inc    eax
loc_11_03C7:
        leave  
        ret    
loc_11_03C9:
        stc    
        jmp    loc_11_03C7
loc_11_03CC:
        mov    edi, dword ptr [0x98]         ; -> obj6:0x0098
        or     edi, edi
        je     loc_11_0475
        test   byte ptr [0x1204], 4          ; -> obj1:0x1204
        jne    loc_11_041A
        lea    edx, [edi + 4]
        mov    ecx, 0x40
loc_11_03EB:
        test   byte ptr [edx + 2], 1
        je     loc_11_03F8
        add    edx, 8
        loop   loc_11_03EB
        jmp    loc_11_041A
loc_11_03F8:
        lea    ecx, [edi + 4]
        sub    edx, ecx
        shr    edx, 1
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        mov    ecx, eax
        and    ecx, 3
        je     loc_11_0413
        sub    ecx, 4
        neg    ecx
        add    eax, ecx
loc_11_0413:
        sub    eax, edx
        mov    dword ptr [0xa4], eax         ; -> obj6:0x00A4
loc_11_041A:
        mov    esi, 0xa8                     ; -> obj6:0x00A8
        lea    edx, [edi + 4]
        mov    ecx, 0x40
        xor    eax, eax
loc_11_0429:
        test   byte ptr [edx + 2], 1
        jne    loc_11_0457
        test   byte ptr [edx + 2], 2
        jne    loc_11_0456
        push   eax
        push   ecx
        push   edx
        mov    ecx, edx
        lea    edx, [edi + 4]
        sub    ecx, edx
        shr    ecx, 1
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        cmp    ecx, eax
        pop    edx
        pop    ecx
        pop    eax
        jae    loc_11_0456
        or     byte ptr [edx + 2], 2
        or     byte ptr [esi + 2], 2
loc_11_0456:
        inc    eax
loc_11_0457:
        add    edx, 8
        add    esi, 8
        loop   loc_11_0429
        mov    byte ptr [edi + 0x68f], al
        mov    edx, eax
        shl    eax, 1
        add    eax, edx
        add    eax, 4
        mov    word ptr [edi + 0x688], ax
loc_11_0475:
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    edi, dword ptr [esi + ebx + 0x5c]
        or     edi, edi
        je     loc_11_04CB
        lea    edx, [esi + ebx + 0x64]
        lea    eax, [edi + 4]
        mov    ecx, 0x40
loc_11_048F:
        test   byte ptr [eax + 2], 3
        jne    loc_11_04C3
        test   byte ptr [edx + 1], 6
        jne    loc_11_04BF
        test   byte ptr [edx + 3], 6
        jne    loc_11_04BF
        test   byte ptr [edx + 5], 6
        jne    loc_11_04BF
        test   byte ptr [edx + 7], 6
        jne    loc_11_04BF
        or     byte ptr [edx + 1], 1
        or     byte ptr [edx + 3], 1
        or     byte ptr [edx + 5], 1
        or     byte ptr [edx + 7], 1
        jmp    loc_11_04C3
loc_11_04BF:
        or     byte ptr [eax + 2], 2
loc_11_04C3:
        add    eax, 8
        add    edx, 8
        loop   loc_11_048F
loc_11_04CB:
        clc    
        ret    
sub_11_04CD:
        mov    esi, dword ptr [0x98]         ; -> obj6:0x0098
        or     esi, esi
        je     loc_11_05BA
        mov    edi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        push   0x101
        push   0x692
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        stc    
        je     loc_11_05BA
        mov    dword ptr [edi + ebx + 0x5c], eax
        push   esi
        lea    edi, [eax + 0x240]
        mov    esi, 0x2a8                    ; -> obj6:0x02A8
        mov    ecx, 0x10
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        lea    edi, [eax + 4]
        mov    esi, 0xa8                     ; -> obj6:0x00A8
        mov    ecx, 0x80
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, eax
        mov    ax, word ptr [0x2e8]          ; -> obj6:0x02E8
        mov    word ptr [edi + 0x68a], ax
        pop    esi
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi], eax
        mov    al, byte ptr [esi + 0x691]
        mov    byte ptr [edi + 0x691], al
        mov    ax, word ptr [esi + 0x688]
        mov    word ptr [edi + 0x688], ax
        mov    al, byte ptr [esi + 0x68f]
        mov    byte ptr [edi + 0x68f], al
        push   edi
        lea    esi, [esi + 0x280]
        lea    edi, [edi + 0x280]
        mov    ecx, 0xff
loc_11_0570:
        cld    
        lodsd  eax, dword ptr [esi]
        or     eax, eax
        je     loc_11_057F
        test   byte ptr [eax + 0x2c], 0x80
        je     loc_11_057F
        stosd  dword ptr es:[edi], eax
        jmp    loc_11_0582
loc_11_057F:
        add    edi, 4
loc_11_0582:
        loop   loc_11_0570
        pop    edi
        push   0x101
        push   0x36
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_11_05BB
        mov    dword ptr [edi + 0x280], eax
        xor    ecx, ecx
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 0x28], ecx
        dec    ecx
        mov    dword ptr [eax + 4], ecx
        mov    byte ptr [eax + 0x2c], 0x44
        lea    eax, [edi + 4]
        mov    ecx, 0x80000001
        call   sub_11_0074
loc_11_05BA:
        ret    
loc_11_05BB:
        mov    edi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        xor    eax, eax
        xchg   dword ptr [edi + ebx + 0x5c], eax
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        stc    
        jmp    loc_11_05BA
        db  000h, 000h, 000h
sub_11_05D8:
        VMMCall  VMM._GetNulPageHandle           ; cd206a000100
        mov    edi, eax
        mov    esi, dword ptr [0xeb4]        ; -> obj1:0x0EB4
loc_11_05E6:
        movzx  ecx, word ptr [esi*2 + 0xf28] ; -> obj1:0x0F28
        test   ch, 4
        je     loc_11_060A
        test   ch, 0x80
        jne    loc_11_060A
        push   0
        push   0
        push   1
        push   esi
        push   ebx
        push   edi
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
loc_11_060A:
        inc    esi
        cmp    esi, dword ptr [0xeb8]        ; -> obj1:0x0EB8
        jbe    loc_11_05E6
        clc    
        ret    
        db  000h, 000h, 000h
loc_11_0618:
        mov    edi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        lea    edi, [edi + ebx + 0x64]
        mov    esi, 0xf28                    ; -> obj1:0x0F28
        mov    ecx, 0x88
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        or     byte ptr [edi + ebx + 0x2c], 8
        call   sub_11_05D8
        jb     loc_11_065E
        call   0
        jb     loc_11_065E
        call   sub_11_04CD
        jb     loc_11_065E
        call   sub_11_0C9C
        jb     loc_11_065D
        call   sub_11_0CE9
        jb     loc_11_065E
loc_11_065D:
        ret    
loc_11_065E:
        xor    ebx, ebx
        mov    eax, 0x20008
        lea    edx, [0x11b8]                 ; -> obj1:0x11B8
        xor    ecx, ecx
        inc    ecx
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        stc    
        jmp    loc_11_065D
V86MMGR_Allocate_V86_Pages:
        enter  0x2c, 0
        pushal 
        mov    dword ptr [ebp - 4], 0
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_11_069A
        pushal 
        call   sub_9_2EF0
        popal  
        jae    loc_11_069A
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
loc_11_069A:
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        mov    dword ptr [0x1208], eax       ; -> obj1:0x1208
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        mov    dword ptr [0x120c], eax       ; -> obj1:0x120C
        inc    eax
        shl    eax, 2
        mov    dword ptr [0x1210], eax       ; -> obj1:0x1210
        mov    ecx, esi
        xor    eax, eax
        call   sub_1_060C
        mov    esi, ecx
        mov    eax, dword ptr [0x1210]       ; -> obj1:0x1210
        mov    ecx, edi
        cmp    ecx, eax
        ja     loc_11_091A
        mov    eax, 1
        call   sub_1_060C
        mov    edi, ecx
        lea    edx, [ebp - 0x2c]
        push   0
        push   edx
        push   ebx
        VMMCall  VMM._Get_Device_V86_Pages_Array ; cd2074000100
        add    esp, 0xc
        or     eax, eax
        je     loc_11_0906
        push   edi
        push   esi
        lea    edx, [ebp - 0x2c]
        mov    esi, dword ptr [0x1208]       ; -> obj1:0x1208
        mov    edi, esi
        mov    eax, esi
        and    esi, 0x1f
        shr    edi, 5
        or     esi, esi
        je     loc_11_0722
        mov    ecx, esi
        sub    ecx, 0x20
        neg    ecx
loc_11_0717:
        bt     dword ptr [edx + edi*4], esi
        jb     loc_11_0738
        inc    esi
        inc    eax
        loop   loc_11_0717
        inc    edi
loc_11_0722:
        cmp    eax, dword ptr [0x120c]       ; -> obj1:0x120C
        ja     loc_11_0738
        bsf    esi, dword ptr [edx + edi*4]
        jne    loc_11_0736
        add    eax, 0x20
        inc    edi
        jmp    loc_11_0722
loc_11_0736:
        add    eax, esi
loc_11_0738:
        mov    edx, dword ptr [0x120c]       ; -> obj1:0x120C
        inc    edx
        cmp    eax, edx
        jbe    loc_11_0745
        mov    eax, edx
loc_11_0745:
        mov    dword ptr [ebp - 8], eax
        sub    eax, dword ptr [0x1208]       ; -> obj1:0x1208
        pop    esi
        pop    edi
        cmp    eax, edi
        jb     loc_11_091A
        cmp    eax, esi
        jae    loc_11_075E
        mov    esi, eax
loc_11_075E:
        push   0
        VMMCall  VMM._PageGetAllocInfo           ; cd2059000100
        pop    ecx
        sub    edx, 0x14
        jae    loc_11_076E
        xor    edx, edx
loc_11_076E:
        mov    ecx, esi
        test   byte ptr [esp + 0x18], 1
        jne    loc_11_0782
        cmp    byte ptr [0x1215], 3          ; -> obj1:0x1215
        jne    loc_11_0782
        mov    edx, eax
loc_11_0782:
        cmp    ecx, edx
        jbe    loc_11_0790
        cmp    edi, edx
        ja     loc_11_0913
        mov    ecx, edx
loc_11_0790:
        xor    eax, eax
        test   byte ptr [esp + 0x18], 1
        je     loc_11_07A4
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        or     byte ptr [edx + ebx + 0x2e], 1
loc_11_07A4:
        mov    dword ptr [ebp - 4], ecx
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    dword ptr [edx + ebx + 0xc], ecx
        push   ecx
        call   sub_11_01BC
        pop    edx
        jb     loc_11_0913
        jne    loc_11_081E
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        test   eax, eax
        je     loc_11_0913
        mov    ecx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    dword ptr [ecx + ebx], eax
        mov    edi, eax
        push   0
        push   ebx
        push   dword ptr [ebp - 4]
        push   dword ptr [0x1208]            ; -> obj1:0x1208
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        push   0
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [0x1208]            ; -> obj1:0x1208
        push   ebx
        push   edi
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        or     eax, eax
        je     loc_11_0913
loc_11_081E:
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        test   byte ptr [edx + ebx + 0x2e], 1
        je     loc_11_083E
        push   0x800
        push   0
        push   0
        push   ebx
        VMMCall  VMM._SetResetV86Pageable        ; cd2065000100
        add    esp, 0x10
loc_11_083E:
        mov    eax, dword ptr [0x1208]       ; -> obj1:0x1208
        mov    ecx, dword ptr [ebp - 4]
        lea    eax, [ebx + eax*2 + 0x64]
        add    eax, dword ptr [0x11fc]       ; -> obj1:0x11FC
loc_11_0850:
        or     word ptr [eax], 0x1010
        add    eax, 2
        loop   loc_11_0850
        mov    esi, dword ptr [ebp - 4]
        add    esi, dword ptr [0x1208]       ; -> obj1:0x1208
        mov    edi, dword ptr [ebp - 8]
        sub    edi, esi
        jbe    loc_11_08DA
        push   0
        push   0
        push   edi
        push   esi
        push   ebx
        push   dword ptr [0x1200]            ; -> obj1:0x1200
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        cmp    dword ptr [0x98], 0           ; -> obj6:0x0098
        je     loc_11_08DA
        mov    edx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    edx, dword ptr [edx + ebx + 0x5c]
        lea    edx, [edx + 4]
        mov    eax, esi
        and    eax, 3
        je     loc_11_08A2
        sub    eax, 4
        neg    eax
loc_11_08A2:
        add    esi, eax
        sub    edi, eax
        jbe    loc_11_08DA
        shr    edi, 2
        or     edi, edi
        je     loc_11_08DA
        mov    ecx, edi
        test   esi, 3
        stc    
        jne    loc_11_08BE
        shl    esi, 1
        add    esi, edx
loc_11_08BE:
        test   byte ptr [esi + 2], 3
        jne    loc_11_08D5
        mov    byte ptr [esi], 0xff
        mov    byte ptr [esi + 1], 4
        or     byte ptr [esi + 2], 0x20
        mov    word ptr [esi + 4], 0
loc_11_08D5:
        add    esi, 8
        loop   loc_11_08BE
loc_11_08DA:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [0x120c]       ; -> obj1:0x120C
        inc    edx
        cmp    eax, edx
        jae    loc_11_08FF
        mov    ecx, edx
        sub    ecx, eax
        lea    eax, [ebx + eax*2 + 0x64]
        add    eax, dword ptr [0x11fc]       ; -> obj1:0x11FC
loc_11_08F6:
        or     byte ptr [eax + 1], 8
        add    eax, 2
        loop   loc_11_08F6
loc_11_08FF:
        clc    
loc_11_0900:
        popal  
        mov    eax, dword ptr [ebp - 4]
        leave  
        ret    
loc_11_0906:
        mov    eax, 0x20008
        lea    edx, [0x11b8]                 ; -> obj1:0x11B8
        jmp    loc_11_0921
loc_11_0913:
        mov    eax, 0x20001
        jmp    loc_11_091F
loc_11_091A:
        mov    eax, 0x20002
loc_11_091F:
        xor    edx, edx
loc_11_0921:
        push   ebx
        test   byte ptr [ebx], 4
        je     loc_11_0929
        xor    ebx, ebx
loc_11_0929:
        xor    ecx, ecx
        inc    ecx
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        pop    ebx
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        xor    ecx, ecx
        xchg   dword ptr [esi + ebx + 4], ecx
        jecxz  loc_11_094C
        push   0
        push   ecx
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_11_094C:
        xor    ecx, ecx
        xchg   dword ptr [esi + ebx], ecx
        jecxz  loc_11_095E
        push   0
        push   ecx
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_11_095E:
        xor    ecx, ecx
        xchg   dword ptr [esi + ebx + 8], ecx
        jecxz  loc_11_0971
        push   0
        push   ecx
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_11_0971:
        and    dword ptr [ebp - 4], 0
        stc    
        jmp    loc_11_0900
V86MMGR_ResetBasePages:
        pushal 
        or     ecx, ecx
        je     loc_11_0AAE
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        mov    edx, dword ptr [esi + ebx]
        or     edx, dword ptr [esi + ebx + 4]
        or     edx, dword ptr [esi + ebx + 8]
        je     loc_11_0AAE
        cmp    dword ptr [esi + ebx + 0x5c], 0
        je     loc_11_0AAE
        mov    edx, eax
        add    ecx, edx
        dec    ecx
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        cmp    edx, eax
        ja     loc_11_0AAE
        cmp    ecx, eax
        ja     loc_11_0AAE
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        cmp    edx, eax
        jb     loc_11_0AAE
        mov    edi, edx
        sub    edx, eax
        mov    ecx, dword ptr [esp + 0x18]
        add    ecx, edx
        dec    ecx
        cmp    edx, dword ptr [esi + ebx + 0xc]
        jae    loc_11_0AAE
        cmp    ecx, dword ptr [esi + ebx + 0xc]
        jae    loc_11_0AAE
        mov    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esi + ebx + 4]
        or     eax, eax
        je     loc_11_0A34
        push   edx
        push   ecx
        push   0
        push   eax
        VMMCall  VMM._PageGetSizeAddr            ; cd2058000100
        pop    ecx
        pop    ecx
        pop    ecx
        pop    edx
        cmp    eax, edx
        jbe    loc_11_0A32
        sub    eax, edx
        cmp    eax, ecx
        jbe    loc_11_0A12
        mov    eax, ecx
loc_11_0A12:
        push   ecx
        push   eax
        push   0
        push   edi
        push   eax
        push   edx
        push   dword ptr [esi + ebx + 4]
        VMMCall  VMM._PageResetHandlePAddr       ; cd20d3000100
        add    esp, 0x14
        pop    eax
        pop    ecx
        sub    ecx, eax
        jbe    loc_11_0AAB
        xor    edx, edx
        add    edi, eax
        jmp    loc_11_0A34
loc_11_0A32:
        sub    edx, eax
loc_11_0A34:
        mov    eax, dword ptr [esi + ebx]
        or     eax, eax
        je     loc_11_0A77
        push   edx
        push   ecx
        push   0
        push   eax
        VMMCall  VMM._PageGetSizeAddr            ; cd2058000100
        pop    ecx
        pop    ecx
        pop    ecx
        pop    edx
        cmp    eax, edx
        jbe    loc_11_0A75
        sub    eax, edx
        cmp    eax, ecx
        jbe    loc_11_0A56
        mov    eax, ecx
loc_11_0A56:
        push   ecx
        push   eax
        push   0
        push   edi
        push   eax
        push   edx
        push   dword ptr [esi + ebx]
        VMMCall  VMM._PageResetHandlePAddr       ; cd20d3000100
        add    esp, 0x14
        pop    eax
        pop    ecx
        sub    ecx, eax
        jbe    loc_11_0AAB
        xor    edx, edx
        add    edi, eax
        jmp    loc_11_0A77
loc_11_0A75:
        sub    edx, eax
loc_11_0A77:
        mov    eax, dword ptr [esi + ebx + 8]
        or     eax, eax
        je     loc_11_0AAB
        push   edx
        push   ecx
        push   0
        push   eax
        VMMCall  VMM._PageGetSizeAddr            ; cd2058000100
        pop    ecx
        pop    ecx
        pop    ecx
        pop    edx
        cmp    eax, edx
        jbe    loc_11_0AAB
        sub    eax, edx
        cmp    eax, ecx
        jbe    loc_11_0A9A
        mov    eax, ecx
loc_11_0A9A:
        push   0
        push   edi
        push   eax
        push   edx
        push   dword ptr [esi + ebx]
        VMMCall  VMM._PageResetHandlePAddr       ; cd20d3000100
        add    esp, 0x14
loc_11_0AAB:
        clc    
loc_11_0AAC:
        popal  
        ret    
loc_11_0AAE:
        stc    
        jmp    loc_11_0AAC
V86MMGR_Set_EMS_XMS_Limits:
        enter  0, 0
        pushal 
        pushfd 
        and    byte ptr [ebp - 0x24], 0xfe
        mov    ecx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        cmp    eax, edx
        jne    loc_11_0ACF
        inc    eax
        je     loc_11_0B65
        dec    eax
        je     loc_11_0B29
loc_11_0ACF:
        add    eax, 3
        shr    eax, 2
        cmp    edx, -1
        je     loc_11_0AE0
        add    edx, 3
        shr    edx, 2
loc_11_0AE0:
        test   byte ptr [ecx + ebx + 0x2f], 0x80
        jne    loc_11_0B29
        or     eax, eax
        je     loc_11_0B29
        push   eax
        push   edx
        push   ecx
        mov    dword ptr [ecx + ebx + 0x48], eax
        mov    dword ptr [ecx + ebx + 0x4c], eax
        xor    ecx, ecx
        test   byte ptr [ebp - 8], 8
        je     loc_11_0B04
        or     ecx, 0x80
loc_11_0B04:
        push   ecx
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   eax
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        or     eax, eax
        pop    ecx
        mov    dword ptr [ecx + ebx + 0x44], eax
        pop    edx
        pop    eax
        je     loc_11_0C7D
loc_11_0B29:
        test   byte ptr [ecx + ebx + 0x2f], 0x80
        jne    loc_11_0B65
        mov    dword ptr [ecx + ebx + 0x50], eax
        mov    dword ptr [ecx + ebx + 0x54], edx
        mov    edx, dword ptr [ecx + ebx + 0x54]
        or     edx, dword ptr [ecx + ebx + 0x50]
        mov    eax, 0x80000000
        jne    loc_11_0B4A
        or     eax, 2
loc_11_0B4A:
        test   byte ptr [0x1204], 8          ; -> obj1:0x1204
        jne    loc_11_0B59
        test   byte ptr [ebp - 8], 8
        je     loc_11_0B5C
loc_11_0B59:
        or     eax, 0x20
loc_11_0B5C:
        and    byte ptr [ecx + ebx + 0x2c], 0xdd
        or     dword ptr [ecx + ebx + 0x2c], eax
loc_11_0B65:
        cmp    esi, edi
        jne    loc_11_0B73
        inc    esi
        je     loc_11_0C05
        dec    esi
        je     loc_11_0BC9
loc_11_0B73:
        add    esi, 3
        shr    esi, 2
        cmp    edi, -1
        je     loc_11_0B84
        add    edi, 3
        shr    edi, 2
loc_11_0B84:
        test   byte ptr [ecx + ebx + 0x2f], 0x40
        jne    loc_11_0BC9
        or     esi, esi
        je     loc_11_0BC9
        push   ecx
        mov    dword ptr [ecx + ebx + 0x34], esi
        mov    dword ptr [ecx + ebx + 0x38], esi
        xor    ecx, ecx
        test   byte ptr [ebp - 8], 4
        je     loc_11_0BA6
        or     ecx, 0x80
loc_11_0BA6:
        push   ecx
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   esi
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        or     eax, eax
        pop    ecx
        mov    dword ptr [ecx + ebx + 0x30], eax
        je     loc_11_0C76
loc_11_0BC9:
        test   byte ptr [ecx + ebx + 0x2f], 0x40
        jne    loc_11_0C05
        mov    dword ptr [ecx + ebx + 0x3c], esi
        mov    dword ptr [ecx + ebx + 0x40], edi
        mov    edx, dword ptr [ecx + ebx + 0x40]
        or     edx, dword ptr [ecx + ebx + 0x3c]
        mov    eax, 0x40000000
        jne    loc_11_0BEA
        or     eax, 1
loc_11_0BEA:
        test   byte ptr [0x1204], 0x10       ; -> obj1:0x1204
        jne    loc_11_0BF9
        test   byte ptr [ebp - 8], 4
        je     loc_11_0BFC
loc_11_0BF9:
        or     eax, 0x10
loc_11_0BFC:
        and    byte ptr [ecx + ebx + 0x2c], 0xee
        or     dword ptr [ecx + ebx + 0x2c], eax
loc_11_0C05:
        test   byte ptr [ebp - 8], 3
        je     loc_11_0C19
        xor    eax, eax
        test   byte ptr [ebp - 8], 1
        jne    loc_11_0C14
        inc    eax
loc_11_0C14:
        call   sub_15_0137
loc_11_0C19:
        test   byte ptr [ebx], 8
        je     loc_11_0C72
        mov    edi, dword ptr [ecx + ebx + 0x44]
        or     edi, edi
        je     loc_11_0C48
        test   byte ptr [ebp - 8], 8
        je     loc_11_0C48
        push   ecx
        push   0
        push   edi
        VMMCall  VMM._PageGetSizeAddr            ; cd2058000100
        pop    ecx
        pop    ecx
        push   0
        push   0
        push   eax
        push   edi
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
        pop    ecx
loc_11_0C48:
        mov    edi, dword ptr [ecx + ebx + 0x30]
        or     edi, edi
        je     loc_11_0C72
        test   byte ptr [ebp - 8], 4
        je     loc_11_0C72
        push   ecx
        push   0
        push   edi
        VMMCall  VMM._PageGetSizeAddr            ; cd2058000100
        pop    ecx
        pop    ecx
        push   0
        push   0
        push   eax
        push   edi
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
        pop    ecx
loc_11_0C72:
        popfd  
        popal  
        leave  
        ret    
loc_11_0C76:
        mov    eax, 0x20003
        jmp    loc_11_0C82
loc_11_0C7D:
        mov    eax, 0x20004
loc_11_0C82:
        test   byte ptr [ebx], 4
        je     loc_11_0C89
        xor    ebx, ebx
loc_11_0C89:
        xor    edx, edx
        xor    ecx, ecx
        inc    ecx
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        or     byte ptr [ebp - 0x24], 1
        jmp    loc_11_0C72
        db  000h, 000h
sub_11_0C9C:
        pushal 
        cmp    dword ptr [0x1330], -1        ; -> obj1:0x1330
        je     loc_11_0CD3
        cmp    dword ptr [0x1330], 0         ; -> obj1:0x1330
        mov    eax, 4
        je     loc_11_0CC5
        mov    eax, 2
        test   byte ptr [0x1204], 0x20       ; -> obj1:0x1204
        jne    loc_11_0CC5
        or     eax, 1
loc_11_0CC5:
        push   eax
        push   ebx
        VMMCall  VMM._MMGR_Toggle_HMA            ; cd207d000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_11_0CD6
loc_11_0CD3:
        clc    
loc_11_0CD4:
        popal  
        ret    
loc_11_0CD6:
        mov    eax, 0x20005
        xor    edx, edx
        xor    ecx, ecx
        inc    ecx
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        stc    
        jmp    loc_11_0CD4
sub_11_0CE9:
        mov    esi, dword ptr [0x12b0]       ; -> obj1:0x12B0
        VMMCall  VMM.List_Allocate               ; cd209d000100
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        jb     loc_15_00DD
        xchg   edi, eax
        push   edi
        mov    ecx, dword ptr [0x12ac]       ; -> obj1:0x12AC
        add    ecx, 7
        xor    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        pop    edi
        mov    dword ptr [esi + ebx + 0x60], edi
        mov    edx, dword ptr [0x1330]       ; -> obj1:0x1330
        mov    word ptr [edi + 0x16], dx
        mov    eax, dword ptr [0x1200]       ; -> obj1:0x1200
        mov    dword ptr [edi], eax
        lea    esi, [edi + 0xc]
        mov    dword ptr [esi + 4], 0x7fffffff
        mov    dword ptr [esi], esi
        push   edi
        lea    edi, [edi + 0x1c]
        mov    esi, dword ptr [0x12bc]       ; -> obj1:0x12BC
        mov    ecx, dword ptr [0x12ac]       ; -> obj1:0x12AC
        jecxz  loc_11_0D53
loc_11_0D45:
        lodsw  ax, word ptr [esi]
        cmp    ax, -1
        je     loc_11_0D51
        movzx  eax, ax
        stosd  dword ptr es:[edi], eax
loc_11_0D51:
        loop   loc_11_0D45
loc_11_0D53:
        pop    edi
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 12  vsize=0x234  R-X
        db  060h, 033h, 0C9h, 087h, 04Ch, 01Eh, 010h, 0E3h, 00Bh, 06Ah, 000h, 051h, 0CDh, 020h, 055h, 000h
        db  001h, 000h, 059h, 059h, 061h, 0C3h, 000h, 000h
sub_12_0018:
        pushal 
        xor    eax, eax
        mov    dword ptr [esi + ebx + 0x48], eax
        mov    dword ptr [esi + ebx + 0x4c], eax
        xchg   dword ptr [esi + ebx + 0x44], eax
        or     eax, eax
        je     loc_12_0036
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_12_0036:
        xor    al, al
        xchg   byte ptr [esi + ebx + 0x27], al
        or     al, al
        je     loc_12_0052
        mov    ah, byte ptr [0x2ea]          ; -> obj6:0x02EA
        cmp    al, ah
        ja     loc_12_004C
        mov    ah, al
loc_12_004C:
        sub    byte ptr [0x2ea], ah          ; -> obj6:0x02EA
loc_12_0052:
        xor    edi, edi
        xchg   dword ptr [esi + ebx + 0x5c], edi
        or     edi, edi
        je     loc_12_00E8
        mov    ecx, dword ptr [edi + 0x67c]
        sub    dword ptr [0xc], ecx          ; -> obj6:0x000C
        jae    loc_12_0078
        mov    dword ptr [0xc], 0            ; -> obj6:0x000C
loc_12_0078:
        mov    ecx, 0xff
        lea    esi, [edi + 0x280]
loc_12_0083:
        cld    
        lodsd  eax, dword ptr [esi]
        or     eax, eax
        je     loc_12_00BB
        cmp    ecx, 0xff
        je     loc_12_00AE
        test   byte ptr [eax + 0x2c], 0x80
        jne    loc_12_00BB
        test   byte ptr [eax + 0x2c], 4
        jne    loc_12_00AE
        push   ecx
        push   eax
        mov    eax, dword ptr [eax]
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
        pop    eax
        pop    ecx
loc_12_00AE:
        push   ecx
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    ecx
loc_12_00BB:
        loop   loc_12_0083
        mov    ecx, 0xf
        lea    esi, [edi + 0x204]
loc_12_00C8:
        cld    
        lodsd  eax, dword ptr [esi]
        or     eax, eax
        je     loc_12_00DB
        push   ecx
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    ecx
loc_12_00DB:
        loop   loc_12_00C8
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_12_00E8:
        popal  
        ret    
        db  000h, 000h
loc_12_00EC:
        mov    eax, dword ptr [0x11fc]       ; -> obj1:0x11FC
        xor    ecx, ecx
        mov    word ptr [eax + ebx + 0x24], cx
        ret    
loc_12_00F9:
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        call   0
        call   sub_12_0018
        call   sub_12_0170
        xor    eax, eax
        xchg   dword ptr [esi + ebx + 4], eax
        or     eax, eax
        je     loc_12_0123
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_12_0123:
        xor    eax, eax
        xchg   dword ptr [esi + ebx + 8], eax
        or     eax, eax
        je     loc_12_0138
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_12_0138:
        xor    eax, eax
        xchg   dword ptr [esi + ebx], eax
        or     eax, eax
        je     loc_12_016B
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        push   0
        push   0
        push   dword ptr [esi + ebx + 0xc]
        push   eax
        push   ebx
        push   dword ptr [0x1200]            ; -> obj1:0x1200
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
loc_12_016B:
        clc    
        ret    
        db  000h, 000h, 000h
sub_12_0170:
        pushal 
        xor    ecx, ecx
        mov    dword ptr [esi + ebx + 0x34], ecx
        mov    dword ptr [esi + ebx + 0x38], ecx
        xchg   dword ptr [esi + ebx + 0x30], ecx
        jecxz  loc_12_018C
        push   0
        push   ecx
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_12_018C:
        xor    edi, edi
        xchg   dword ptr [esi + ebx + 0x60], edi
        or     edi, edi
        je     loc_12_022F
        btr    word ptr [edi + 0x14], 1
        jae    loc_12_01AE
        push   0
        push   dword ptr [edi]
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_12_01AE:
        test   byte ptr [0x1204], 0x20       ; -> obj1:0x1204
        je     loc_12_01C2
        push   4
        push   ebx
        VMMCall  VMM._MMGR_Toggle_HMA            ; cd207d000100
        pop    ecx
        pop    ecx
loc_12_01C2:
        lea    esi, [edi + 0x1c]
        mov    ecx, dword ptr [0x12ac]       ; -> obj1:0x12AC
        jecxz  loc_12_01F5
loc_12_01CD:
        cld    
        lodsd  eax, dword ptr [esi]
        or     eax, eax
        jns    loc_12_01F3
        push   ecx
        cmp    dword ptr [eax], 0
        je     loc_12_01E7
        push   eax
        push   0
        push   dword ptr [eax]
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
        pop    eax
loc_12_01E7:
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    ecx
loc_12_01F3:
        loop   loc_12_01CD
loc_12_01F5:
        xor    ecx, ecx
        xchg   dword ptr [edi + 0x18], ecx
        jecxz  loc_12_0207
        push   0
        push   ecx
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_12_0207:
        mov    esi, dword ptr [edi + 0xc]
loc_12_020A:
        cmp    word ptr [esi + 4], -1
        je     loc_12_0221
        push   dword ptr [esi]
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    esi
        jmp    loc_12_020A
loc_12_0221:
        mov    eax, edi
        mov    esi, dword ptr [0x12b0]       ; -> obj1:0x12B0
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_12_022F:
        popal  
        ret    
        db  000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 13  vsize=0x12c  R-X
        db  060h, 08Bh, 035h
        dd  obj1:0x11FC              ; fixup
        db  0F6h, 044h, 01Eh, 02Ch, 020h, 074h, 05Fh, 08Bh, 07Ch, 01Eh, 044h, 00Bh, 0FFh, 074h, 01Ah, 06Ah
        db  000h, 057h, 0CDh, 020h, 058h, 000h, 001h, 000h, 059h, 059h, 06Ah, 000h, 06Ah, 000h, 050h, 057h
        db  0CDh, 020h, 057h, 000h, 001h, 000h, 083h, 0C4h, 010h, 08Bh, 07Ch, 01Eh, 05Ch, 00Bh, 0FFh, 074h
        db  035h, 0B9h, 0FFh, 000h, 000h, 000h, 08Dh, 0B7h, 080h, 002h, 000h, 000h, 0FCh, 0ADh, 00Bh, 0C0h
        db  074h, 022h, 081h, 0F9h, 0FFh, 000h, 000h, 000h, 074h, 01Ah, 0F6h, 040h, 02Ch, 084h, 075h, 014h
        db  051h, 06Ah, 000h, 06Ah, 000h, 0FFh, 070h, 028h, 0FFh, 030h, 0CDh, 020h, 057h, 000h, 001h, 000h
        db  083h, 0C4h, 010h, 059h, 0E2h, 0D6h, 061h, 0F8h, 0C3h
loc_13_0070:
        call   sub_13_007C
        call   0
        clc    
        ret    
sub_13_007C:
        pushal 
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        test   byte ptr [esi + ebx + 0x2c], 0x10
        je     loc_13_00AC
        mov    edi, dword ptr [esi + ebx + 0x30]
        or     edi, edi
        je     loc_13_00AC
        push   0
        push   edi
        VMMCall  VMM._PageGetSizeAddr            ; cd2058000100
        pop    ecx
        pop    ecx
        push   0
        push   0
        push   eax
        push   edi
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_13_00AC:
        mov    edi, dword ptr [esi + ebx + 0x60]
        or     edi, edi
        je     loc_13_0129
        mov    ecx, dword ptr [edi + 0x18]
        jecxz  loc_13_00DC
        test   byte ptr [esi + ebx + 0x2e], 1
        je     loc_13_00DC
        push   0
        push   ecx
        VMMCall  VMM._PageGetSizeAddr            ; cd2058000100
        pop    ecx
        pop    ecx
        push   0
        push   0
        push   eax
        push   dword ptr [edi + 0x18]
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_13_00DC:
        test   byte ptr [edi + 0x14], 2
        je     loc_13_00F6
        push   0x100
        push   0
        push   0x10
        push   dword ptr [edi]
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_13_00F6:
        test   byte ptr [esi + ebx + 0x2c], 0x10
        je     loc_13_0129
        lea    esi, [edi + 0x1c]
        mov    ecx, dword ptr [0x12ac]       ; -> obj1:0x12AC
        jecxz  loc_13_0129
loc_13_0108:
        cld    
        lodsd  eax, dword ptr [esi]
        or     eax, eax
        jns    loc_13_0127
        cmp    dword ptr [eax], 0
        je     loc_13_0127
        push   ecx
        push   0
        push   0
        push   dword ptr [eax + 8]
        push   dword ptr [eax]
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
        pop    ecx
loc_13_0127:
        loop   loc_13_0108
loc_13_0129:
        popal  
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 14  vsize=0x280  R-X
        db  060h, 08Bh, 035h
        dd  obj1:0x11FC              ; fixup
        db  0F6h, 044h, 01Eh, 02Ch, 020h, 00Fh, 084h, 0CFh, 000h, 000h, 000h, 08Bh, 07Ch, 01Eh, 044h, 00Bh
        db  0FFh, 074h, 022h, 06Ah, 000h, 057h, 0CDh, 020h, 058h, 000h, 001h, 000h, 059h, 059h, 06Ah, 000h
        db  06Ah, 000h, 050h, 057h, 0CDh, 020h, 056h, 000h, 001h, 000h, 083h, 0C4h, 010h, 00Bh, 0C0h, 00Fh
        db  084h, 0A4h, 000h, 000h, 000h, 08Bh, 07Ch, 01Eh, 05Ch, 00Bh, 0FFh, 00Fh, 084h, 099h, 000h, 000h
        db  000h, 08Dh, 0B7h, 080h, 002h, 000h, 000h, 0B9h, 0FFh, 000h, 000h, 000h, 0FCh, 0ADh, 00Bh, 0C0h
        db  074h, 026h, 081h, 0F9h, 0FFh, 000h, 000h, 000h, 074h, 01Eh, 0F6h, 040h, 02Ch, 084h, 075h, 018h
        db  051h, 06Ah, 000h, 06Ah, 000h, 0FFh, 070h, 028h, 0FFh, 030h, 0CDh, 020h, 056h, 000h, 001h, 000h
        db  083h, 0C4h, 010h, 059h, 00Bh, 0C0h, 074h, 005h, 0E2h, 0D2h, 0F8h, 0EBh, 05Dh, 081h, 0E9h, 0FFh
        db  000h, 000h, 000h, 0F7h, 0D9h, 0E3h, 02Ah, 08Dh, 0B7h, 084h, 002h, 000h, 000h, 0EBh, 020h, 0FCh
        db  0ADh, 00Bh, 0C0h, 074h, 01Ah, 0F6h, 040h, 02Ch, 084h, 075h, 014h, 051h, 06Ah, 000h, 06Ah, 000h
        db  0FFh, 070h, 028h, 0FFh, 030h, 0CDh, 020h, 057h, 000h, 001h, 000h, 083h, 0C4h, 010h, 059h, 0E2h
        db  0DEh, 08Bh, 035h
        dd  obj1:0x11FC              ; fixup
        db  08Bh, 07Ch, 01Eh, 044h, 00Bh, 0FFh, 074h, 01Ah, 06Ah, 000h, 057h, 0CDh, 020h, 058h, 000h, 001h
        db  000h, 059h, 059h, 06Ah, 000h, 06Ah, 000h, 050h, 057h, 0CDh, 020h, 057h, 000h, 001h, 000h, 083h
        db  0C4h, 010h, 0F9h, 061h, 0C3h, 000h
loc_14_00E4:
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        call   sub_14_0100
        jb     loc_14_00FE
        call   0
        jae    loc_14_00FE
        call   sub_13_007C
        stc    
loc_14_00FE:
        ret    
        db  000h
sub_14_0100:
        pushal 
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        test   byte ptr [esi + ebx + 0x2c], 0x10
        je     loc_14_0138
        mov    edi, dword ptr [esi + ebx + 0x30]
        or     edi, edi
        je     loc_14_0138
        push   0
        push   edi
        VMMCall  VMM._PageGetSizeAddr            ; cd2058000100
        pop    ecx
        pop    ecx
        push   0
        push   0
        push   eax
        push   edi
        VMMCall  VMM._PageLock                   ; cd2056000100
        add    esp, 0x10
        or     eax, eax
        je     loc_14_024E
loc_14_0138:
        mov    edi, dword ptr [esi + ebx + 0x60]
        or     edi, edi
        je     loc_14_027B
        mov    ecx, dword ptr [edi + 0x18]
        jecxz  loc_14_0174
        test   byte ptr [esi + ebx + 0x2e], 1
        je     loc_14_0174
        push   0
        push   ecx
        VMMCall  VMM._PageGetSizeAddr            ; cd2058000100
        pop    ecx
        pop    ecx
        push   0
        push   0
        push   eax
        push   dword ptr [edi + 0x18]
        VMMCall  VMM._PageLock                   ; cd2056000100
        add    esp, 0x10
        or     eax, eax
        je     loc_14_027A
loc_14_0174:
        test   byte ptr [edi + 0x14], 2
        je     loc_14_0196
        push   0x100
        push   0
        push   0x10
        push   dword ptr [edi]
        VMMCall  VMM._PageLock                   ; cd2056000100
        add    esp, 0x10
        or     eax, eax
        je     loc_14_0225
loc_14_0196:
        test   byte ptr [esi + ebx + 0x2c], 0x10
        je     loc_14_027B
        lea    esi, [edi + 0x1c]
        mov    ecx, dword ptr [0x12ac]       ; -> obj1:0x12AC
        jecxz  loc_14_01D1
loc_14_01AC:
        cld    
        lodsd  eax, dword ptr [esi]
        or     eax, eax
        jns    loc_14_01CF
        cmp    dword ptr [eax], 0
        je     loc_14_01CF
        push   ecx
        push   0
        push   0
        push   dword ptr [eax + 8]
        push   dword ptr [eax]
        VMMCall  VMM._PageLock                   ; cd2056000100
        add    esp, 0x10
        pop    ecx
        or     eax, eax
        je     loc_14_01D7
loc_14_01CF:
        loop   loc_14_01AC
loc_14_01D1:
        clc    
        jmp    loc_14_027B
loc_14_01D7:
        sub    ecx, dword ptr [0x12ac]       ; -> obj1:0x12AC
        neg    ecx
        jecxz  loc_14_0205
        lea    esi, [edi + 0x1c]
loc_14_01E4:
        cld    
        lodsd  eax, dword ptr [esi]
        or     eax, eax
        jns    loc_14_0203
        cmp    dword ptr [eax], 0
        je     loc_14_0203
        push   ecx
        push   0
        push   0
        push   dword ptr [eax + 8]
        push   dword ptr [eax]
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
        pop    ecx
loc_14_0203:
        loop   loc_14_01E4
loc_14_0205:
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        test   byte ptr [edi + 0x14], 2
        je     loc_14_0225
        push   0x100
        push   0
        push   0x10
        push   dword ptr [edi]
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_14_0225:
        test   byte ptr [esi + ebx + 0x2c], 0x10
        je     loc_14_024E
        mov    edi, dword ptr [esi + ebx + 0x30]
        or     edi, edi
        je     loc_14_024E
        push   0
        push   edi
        VMMCall  VMM._PageGetSizeAddr            ; cd2058000100
        pop    ecx
        pop    ecx
        push   0
        push   0
        push   eax
        push   edi
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_14_024E:
        mov    edi, dword ptr [esi + ebx + 0x60]
        or     edi, edi
        je     loc_14_027A
        mov    edi, dword ptr [edi + 0x18]
        test   byte ptr [esi + ebx + 0x2e], 1
        je     loc_14_027A
        push   0
        push   edi
        VMMCall  VMM._PageGetSizeAddr            ; cd2058000100
        pop    ecx
        pop    ecx
        push   0
        push   0
        push   eax
        push   edi
        VMMCall  VMM._PageUnLock                 ; cd2057000100
        add    esp, 0x10
loc_14_027A:
        stc    
loc_14_027B:
        popal  
        ret    
        db  000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 15  vsize=0xc78  R-X
loc_15_0000:
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        movzx  eax, byte ptr [ebp + 0x1d]
        shl    eax, 1
        cmp    al, 0x26
        jae    loc_15_0049
        mov    dl, ah
        xor    ah, ah
        mov    edi, dword ptr [eax*2 + 0x1218] ; -> loc_1_1218
        shl    edi, 1
        sbb    dl, 0
        ja     loc_15_0049
        stc    
        rcr    edi, 1
        test   byte ptr [esi + ebx + 0x2f], 0x40
        je     loc_15_005B
loc_15_002C:
        call   edi
loc_15_002E:
        jb     loc_15_0037
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        ret    
loc_15_0037:
        mov    ecx, dword ptr [0x129c]       ; -> obj1:0x129C
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_15_0049:
        push   0x2e                          ; -> loc_15_002E
        mov    al, 0x80
loc_15_0050:
        mov    byte ptr [ebp + 0x10], al
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_15_005B:
        or     byte ptr [esi + ebx + 0x2f], 0x40
        mov    dword ptr [esi + ebx + 0x3c], 0
        mov    dword ptr [esi + ebx + 0x40], 0x200
        jmp    loc_15_002C
        db  066h, 0C7h, 045h, 01Ch, 000h, 003h, 066h, 0C7h, 045h, 010h, 05Fh, 003h, 033h, 0D2h, 0F6h, 005h
        dd  obj1:0x1204              ; fixup
        db  080h, 075h, 008h, 0F6h, 044h, 01Eh, 02Ch, 004h, 075h, 001h, 042h, 066h, 089h, 055h, 014h, 0F8h
        db  0C3h
sub_15_0097:
        mov    edx, dword ptr [0x1330]       ; -> obj1:0x1330
        cmp    edx, -1
        je     loc_15_00BA
        mov    ecx, ebx
loc_15_00A4:
        mov    edi, dword ptr [esi + ebx + 0x60]
        or     edi, edi
        je     loc_15_00B0
        mov    word ptr [edi + 0x16], dx
loc_15_00B0:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, ecx
        jne    loc_15_00A4
loc_15_00BA:
        mov    esi, 0x1264                   ; -> loc_1_1264
        mov    edi, 0x121c                   ; -> loc_1_121C
        mov    ecx, 6
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [0x1328], 0x3066    ; -> sub_1_1328
        or     byte ptr [0x1204], 0x60       ; -> obj1:0x1204
loc_15_00DD:
        ret    
sub_15_00DE:
        test   byte ptr [0x1204], 0x20       ; -> obj1:0x1204
        stc    
        je     loc_15_011C
        test   byte ptr [0x1204], 0x80       ; -> obj1:0x1204
        stc    
        jne    loc_15_011C
        push   esi
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        test   byte ptr [esi + ebx + 0x2c], 4
        jne    loc_15_011B
        cmp    dword ptr [esi + ebx + 0x60], 0
        je     loc_15_0118
        mov    esi, dword ptr [esi + ebx + 0x60]
        test   byte ptr [esi + 0x14], 2
        je     loc_15_0118
        mov    eax, dword ptr [esi + 4]
        xor    esi, esi
        jmp    loc_15_011B
loc_15_0118:
        xor    esi, esi
        inc    esi
loc_15_011B:
        pop    esi
loc_15_011C:
        ret    
sub_15_011D:
        pushfd 
        sti    
        cld    
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        VMMCall  VMM._lmemcpy                    ; cd207d010100
        add    esp, 0xc
        popfd  
        ret    
sub_15_0137:
        pushal 
        mov    ecx, dword ptr [0x11fc]       ; -> obj1:0x11FC
        or     eax, eax
        je     loc_15_014A
        btr    dword ptr [ecx + ebx + 0x2c], 2
        jmp    loc_15_0167
loc_15_014A:
        test   byte ptr [0x1204], 0x20       ; -> obj1:0x1204
        je     loc_15_0167
        mov    edi, dword ptr [ecx + ebx + 0x60]
        or     edi, edi
        je     loc_15_0161
        test   byte ptr [edi + 0x14], 2
        jne    loc_15_0167
loc_15_0161:
        bts    dword ptr [ecx + ebx + 0x2c], 2
loc_15_0167:
        popal  
        ret    
        db  000h, 000h, 000h
loc_15_016C:
        push   3
        push   ebx
        VMMCall  VMM._MMGR_Toggle_HMA            ; cd207d000100
        pop    ecx
        pop    ecx
        or     eax, eax
        ret    
        db  0A1h
        dd  obj1:0x132C              ; fixup
        db  0B9h, 005h, 000h, 000h, 000h, 0CDh, 020h, 027h, 000h, 001h, 000h, 00Fh, 0BAh, 02Dh
        dd  obj1:0x1204              ; fixup
        db  009h, 0F5h, 073h, 005h, 0E8h, 08Eh, 000h, 000h, 000h, 09Ch, 0A1h
        dd  obj1:0x132C              ; fixup
        db  0CDh, 020h, 028h, 000h, 001h, 000h, 09Dh, 0B0h, 082h, 00Fh, 082h, 090h, 001h, 000h, 000h, 033h
        db  0C0h, 088h, 045h, 010h, 00Ah, 0C0h, 066h, 0C7h, 045h, 01Ch, 001h, 000h, 0F8h, 0C3h, 0A1h
        dd  obj1:0x132C              ; fixup
        db  0B9h, 005h, 000h, 000h, 000h, 0CDh, 020h, 027h, 000h, 001h, 000h, 00Fh, 0BAh, 035h
        dd  obj1:0x1204              ; fixup
        db  009h, 073h, 005h, 0E8h, 0B2h, 000h, 000h, 000h, 09Ch, 0A1h
        dd  obj1:0x132C              ; fixup
        db  0CDh, 020h, 028h, 000h, 001h, 000h, 09Dh, 00Fh, 082h, 04Fh, 001h, 000h, 000h, 0E9h, 024h, 001h
        db  000h, 000h, 0E8h, 00Bh, 000h, 000h, 000h, 00Fh, 082h, 03Fh, 001h, 000h, 000h, 0E9h, 014h, 001h
        db  000h, 000h
loc_15_0205:
        mov    eax, dword ptr [0x132c]       ; -> obj1:0x132C
        mov    ecx, 5
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        call   sub_15_0228
        pushfd 
        mov    eax, dword ptr [0x132c]       ; -> obj1:0x132C
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        popfd  
        ret    
sub_15_0228:
        cmp    dword ptr [0x1330], -2        ; -> obj1:0x1330
        jae    loc_15_025A
        inc    dword ptr [0x1330]            ; -> obj1:0x1330
        cmp    dword ptr [0x1330], 1         ; -> obj1:0x1330
        jne    loc_15_0258
        push   ebx
loc_15_0241:
        push   3
        push   ebx
        VMMCall  VMM._MMGR_Toggle_HMA            ; cd207d000100
        pop    ecx
        pop    ecx
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_15_0241
        pop    ebx
loc_15_0258:
        clc    
        ret    
loc_15_025A:
        stc    
        ret    
        db  0E8h, 00Bh, 000h, 000h, 000h, 00Fh, 082h, 0D8h, 000h, 000h, 000h, 0E9h, 0ADh, 000h, 000h, 000h
loc_15_026C:
        mov    eax, dword ptr [0x132c]       ; -> obj1:0x132C
        mov    ecx, 5
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        call   sub_15_028F
        pushfd 
        mov    eax, dword ptr [0x132c]       ; -> obj1:0x132C
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        popfd  
        ret    
sub_15_028F:
        cmp    dword ptr [0x1330], -1        ; -> obj1:0x1330
        je     loc_15_02C3
        cmp    dword ptr [0x1330], 0         ; -> obj1:0x1330
        je     loc_15_02C3
        dec    dword ptr [0x1330]            ; -> obj1:0x1330
        jne    loc_15_02C1
        push   ebx
loc_15_02AA:
        push   4
        push   ebx
        VMMCall  VMM._MMGR_Toggle_HMA            ; cd207d000100
        pop    ecx
        pop    ecx
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_15_02AA
        pop    ebx
loc_15_02C1:
        clc    
        ret    
loc_15_02C3:
        stc    
        ret    
        db  0B0h, 090h, 0F6h, 044h, 01Eh, 02Ch, 004h, 00Fh, 085h, 07Eh, 0FDh, 0FFh, 0FFh, 08Bh, 07Ch, 01Eh
        db  060h, 0F6h, 047h, 014h, 002h, 075h, 049h, 068h, 000h, 001h, 000h, 000h, 06Ah, 000h, 06Ah, 000h
        db  06Ah, 000h, 06Ah, 000h, 053h, 06Ah, 000h, 06Ah, 010h, 0CDh, 020h, 053h, 000h, 001h, 000h, 083h
        db  0C4h, 020h, 00Bh, 0C0h, 074h, 02Ah, 089h, 007h, 089h, 057h, 004h, 080h, 04Fh, 014h, 002h, 06Ah
        db  008h, 053h, 0CDh, 020h, 07Dh, 000h, 001h, 000h, 059h, 059h, 00Bh, 0C0h, 074h, 006h, 0FFh, 015h
        dd  sub_1_1328               ; fixup
loc_15_0319:
        mov    byte ptr [ebp + 0x10], 0
        mov    word ptr [ebp + 0x1c], 1
        clc    
        ret    
loc_15_0325:
        mov    al, 0x91
        jmp    loc_15_0050
        db  08Bh, 07Ch, 01Eh, 060h, 066h, 00Fh, 0BAh, 06Fh, 014h, 000h, 072h, 0E1h, 0E8h, 033h, 000h, 000h
        db  000h, 073h, 0DAh, 0B0h, 082h, 0E9h, 00Ah, 0FDh, 0FFh, 0FFh, 08Bh, 07Ch, 01Eh, 060h, 066h, 00Fh
        db  0BAh, 077h, 014h, 000h, 073h, 0C7h, 0E8h, 045h, 000h, 000h, 000h, 072h, 0E6h, 0EBh, 0BEh, 0E8h
        db  004h, 000h, 000h, 000h, 072h, 0DDh, 0EBh, 0B5h
loc_15_0364:
        push   edi
        mov    edi, dword ptr [esi + ebx + 0x60]
        call   sub_15_0370
        pop    edi
        ret    
sub_15_0370:
        cmp    word ptr [edi + 0x16], 0
        jne    loc_15_037F
        call   sub_1_1328
        je     loc_15_0385
loc_15_037F:
        inc    word ptr [edi + 0x16]
        clc    
        ret    
loc_15_0385:
        stc    
        ret    
        db  0E8h, 004h, 000h, 000h, 000h, 072h, 0B1h, 0EBh, 089h
loc_15_0390:
        push   edi
        mov    edi, dword ptr [esi + ebx + 0x60]
        call   sub_15_039C
        pop    edi
        ret    
sub_15_039C:
        cmp    word ptr [edi + 0x16], 0
        je     loc_15_03C0
        dec    word ptr [edi + 0x16]
        jne    loc_15_03BE
        push   4
        push   ebx
        VMMCall  VMM._MMGR_Toggle_HMA            ; cd207d000100
        pop    ecx
        pop    ecx
        test   byte ptr [edi + 0x14], 2
        je     loc_15_03BE
        and    byte ptr [edi + 0x14], 0xfb
loc_15_03BE:
        clc    
        ret    
loc_15_03C0:
        stc    
        ret    
        db  0C6h, 045h, 010h, 000h, 0C6h, 045h, 01Dh, 000h, 06Ah, 008h, 053h, 0CDh, 020h, 07Dh, 000h, 001h
        db  000h, 059h, 059h, 00Bh, 0C0h, 00Fh, 095h, 045h, 01Ch, 0C3h
sub_15_03DC:
        add    eax, 0x6c000002               ; -> loc_15_026C
        add    al, byte ptr [eax]
        add    byte ptr [ebx + eax], ah      ; -> loc_15_0364
        db  000h
        dd  loc_15_0390              ; fixup
V86MMGR_Toggle_HMA:
        push   esi
        test   byte ptr [0x1204], 0x80       ; -> obj1:0x1204
        jne    loc_15_041A
        bsf    eax, eax
        stc    
        je     loc_15_041A
        dec    eax
        cmp    al, 2
        cmc    
        jb     loc_15_041A
        test   byte ptr [0x1204], 0x40       ; -> obj1:0x1204
        je     loc_15_040D
        add    al, 2
loc_15_040D:
        mov    esi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        call   sub_15_03DC
loc_15_041A:
        sbb    eax, eax
        pop    esi
        inc    eax
        ret    
        db  000h
sub_15_0420:
        push   esi
        push   edi
        mov    edi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        xor    edx, edx
        bt     dword ptr [edi + ebx + 0x2c], 4
        rcl    edx, 8
        mov    eax, dword ptr [edi + ebx + 0x60]
        mov    eax, dword ptr [eax + 8]
        add    eax, ecx
        mov    ecx, dword ptr [edi + ebx + 0x30]
        mov    esi, dword ptr [edi + ebx + 0x38]
        sub    esi, eax
        jbe    loc_15_047C
        cmp    esi, dword ptr [edi + ebx + 0x34]
        stc    
        je     loc_15_0496
        jecxz  loc_15_045F
        push   edx
        push   esi
        push   ecx
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        jmp    loc_15_0475
loc_15_045F:
        push   edx
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   esi
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
loc_15_0475:
        or     eax, eax
        jne    loc_15_048E
        stc    
        jmp    loc_15_0496
loc_15_047C:
        stc    
        jecxz  loc_15_0496
        push   0
        push   ecx
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
        xor    eax, eax
        xor    esi, esi
loc_15_048E:
        mov    dword ptr [edi + ebx + 0x30], eax
        mov    dword ptr [edi + ebx + 0x34], esi
loc_15_0496:
        pop    edi
        pop    esi
        ret    
sub_15_0499:
        push   edi
        push   eax
        push   -1
        mov    edi, dword ptr [0x11fc]       ; -> obj1:0x11FC
        xor    ecx, ecx
        bt     dword ptr [edi + ebx + 0x2c], 4
        rcl    ecx, 8
        push   ecx
        mov    ecx, edi
        mov    edx, dword ptr [edi + ebx + 0x60]
        push   edx
        mov    edi, dword ptr [esi + 0xc]
        or     edi, dword ptr [esp + 0xc]
        je     loc_15_0562
        mov    edi, dword ptr [esp + 0xc]
        add    edi, 3
        rcr    edi, 1
        shr    edi, 1
        mov    eax, edi
        sub    eax, dword ptr [esi + 8]
        je     loc_15_0545
        add    eax, dword ptr [edx + 8]
        cmp    eax, dword ptr [ecx + ebx + 0x40]
        ja     loc_15_053D
loc_15_04DE:
        cmp    dword ptr [esi + 0xc], 0
        je     loc_15_050C
        or     edi, edi
        jne    loc_15_04FA
        push   0
        push   dword ptr [esi]
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
        xor    eax, eax
        xor    edx, edx
        jmp    loc_15_0540
loc_15_04FA:
        push   dword ptr [esp + 4]
        push   edi
        push   dword ptr [esi]
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        jmp    loc_15_0525
loc_15_050C:
        push   dword ptr [esp + 4]
        push   0
        push   0
        push   0
        push   0
        push   ebx
        push   0
        push   edi
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
loc_15_0525:
        or     eax, eax
        jne    loc_15_0540
        mov    ecx, edi
        sub    ecx, dword ptr [esi + 8]
        jbe    loc_15_053D
        inc    dword ptr [esp + 8]
        jne    loc_15_053D
        call   sub_15_0420
        jae    loc_15_04DE
loc_15_053D:
        stc    
        jmp    loc_15_0562
loc_15_0540:
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], edx
loc_15_0545:
        xchg   dword ptr [esi + 8], edi
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esi + 0xc], eax
        mov    ecx, dword ptr [esi + 8]
        sub    ecx, edi
        mov    edi, dword ptr [esp]
        add    dword ptr [edi + 8], ecx
        xor    ecx, ecx
        call   sub_15_0420
        clc    
loc_15_0562:
        lea    esp, [esp + 0x10]
        pop    edi
        ret    
sub_15_0568:
        mov    edi, dword ptr [esi + ebx + 0x60]
        pushal 
        VxDCall  PAGESWAP.PageSwap_Get_Version   ; cd2000000700
        test   eax, eax
        je     loc_15_0585
        VxDCall  PAGESWAP.PageSwap_Test_IO_Valid ; cd2006000700
        jae    loc_15_0585
        popal  
        jmp    loc_15_062A
loc_15_0585:
        popal  
        push   0
        VMMCall  VMM._PageGetAllocInfo           ; cd2059000100
        add    esp, 4
        sub    ecx, 0x30
        cmp    eax, ecx
        ja     loc_15_059D
        mov    ecx, eax
loc_15_059D:
        mov    eax, 0x80
        test   byte ptr [esi + ebx + 0x2c], 0x10
        je     loc_15_05B6
        xor    eax, eax
        cmp    ecx, edx
        jbe    loc_15_05B6
        mov    eax, 0x10
        mov    ecx, edx
loc_15_05B6:
        sub    ecx, eax
        jae    loc_15_05BC
        xor    ecx, ecx
loc_15_05BC:
        mov    edx, dword ptr [esi + ebx + 0x40]
        sub    edx, dword ptr [edi + 8]
        jb     loc_15_062A
        shl    edx, 2
        shl    ecx, 2
        cmp    edx, ecx
        jbe    loc_15_05D1
        mov    edx, ecx
loc_15_05D1:
        mov    eax, dword ptr [esi + ebx + 0x34]
        or     eax, eax
        je     loc_15_05E2
        shl    eax, 2
        cmp    eax, edx
        jbe    loc_15_05E2
        mov    edx, eax
loc_15_05E2:
        mov    eax, edx
        cmp    eax, 0x800
        jbe    loc_15_05F0
        mov    eax, 0x800
loc_15_05F0:
        test   byte ptr [ebp + 0x1d], 0x80
        je     loc_15_060F
        mov    dword ptr [ebp + 0x14], edx
        mov    dword ptr [ebp + 0x1c], eax
        mov    edx, dword ptr [esi + ebx + 0x40]
        add    edx, 0x110
        shl    edx, 0xc
        dec    edx
        mov    dword ptr [ebp + 0x18], edx
        jmp    loc_15_0624
loc_15_060F:
        test   edx, 0xffff0000
        je     loc_15_061C
        mov    edx, 0xffff
loc_15_061C:
        mov    word ptr [ebp + 0x14], dx
        mov    word ptr [ebp + 0x1c], ax
loc_15_0624:
        mov    byte ptr [ebp + 0x10], 0
        clc    
        ret    
loc_15_062A:
        xor    edx, edx
        mov    eax, edx
        jmp    loc_15_05E2
loc_15_0630:
        mov    edi, dword ptr [esi + ebx + 0x60]
        mov    eax, dword ptr [ebp + 0x14]
        test   byte ptr [ebp + 0x1d], 0x80
        jne    loc_15_0642
        and    eax, 0xffff
loc_15_0642:
        add    eax, 3
        rcr    eax, 1
        shr    eax, 1
        add    eax, dword ptr [edi + 8]
        cmp    eax, dword ptr [esi + ebx + 0x40]
        ja     loc_15_06BA
        lea    esi, [edi + 0x1c]
        mov    ecx, dword ptr [0x12ac]       ; -> obj1:0x12AC
        jecxz  loc_15_0665
        cld    
loc_15_065E:
        lodsd  eax, dword ptr [esi]
        or     eax, eax
        jg     loc_15_066C
        loop   loc_15_065E
loc_15_0665:
        mov    al, 0xa1
        jmp    loc_15_0050
loc_15_066C:
        sub    esi, 4
        push   0x101
        push   0x14
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_15_06BA
        mov    edi, esi
        mov    esi, eax
        xchg   dword ptr [edi], eax
        mov    dword ptr [esi + 0x10], eax
        mov    ecx, dword ptr [ebp + 0x14]
        mov    word ptr [ebp + 0x14], ax
        mov    eax, ecx
        test   byte ptr [ebp + 0x1d], 0x80
        jne    loc_15_069F
        and    eax, 0xffff
loc_15_069F:
        call   sub_15_0499
        jae    loc_15_0319
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [edi], eax
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_15_06BA:
        mov    word ptr [ebp + 0x14], 0
loc_15_06C0:
        mov    al, 0xa0
        jmp    loc_15_0050
        db  08Bh, 07Ch, 01Eh, 060h, 00Fh, 0B7h, 055h, 014h, 0E8h, 06Bh, 004h, 000h, 000h, 00Fh, 082h, 09Ah
        db  000h, 000h, 000h, 075h, 032h, 08Bh, 0F2h, 033h, 0C0h, 0E8h, 0B4h, 0FDh, 0FFh, 0FFh, 06Ah, 000h
        db  056h, 0CDh, 020h, 051h, 000h, 001h, 000h, 059h, 059h, 08Bh, 0C6h, 08Dh, 07Fh, 01Ch, 08Bh, 00Dh
        dd  obj1:0x12AC              ; fixup
        db  0E3h, 00Ch, 0FCh, 0F2h, 0AFh, 075h, 007h, 00Fh, 0B7h, 055h, 014h, 089h, 057h, 0FCh, 0E9h, 00Bh
        db  0FCh, 0FFh, 0FFh
loc_15_070E:
        movzx  ecx, word ptr [edx + 0xa]
        jecxz  loc_15_075E
        sub    esp, 0x6c
        mov    edi, esp
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    esi, dword ptr [ebx + 8]
loc_15_0728:
        mov    ax, word ptr [edx + 8]
        mov    word ptr [esi + 0x14], ax
        mov    byte ptr [esi + 0x1d], 0xd
        mov    eax, dword ptr [0x129c]       ; -> obj1:0x129C
        call   sub_1_05F9
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cmp    word ptr [esi + 0x1c], 1
        jne    loc_15_074D
        loop   loc_15_0728
loc_15_074D:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    esi, esp
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        add    esp, 0x6c
loc_15_075E:
        btr    word ptr [edx + 0xc], 1
        jae    loc_15_076C
        or     byte ptr [edx + 0xc], 4
        jmp    loc_15_0772
loc_15_076C:
        mov    word ptr [edx + 0xc], 0
loc_15_0772:
        stc    
        ret    
loc_15_0774:
        mov    al, 0xa2
        jmp    loc_15_0050
        db  083h, 0ECh, 070h, 056h, 057h, 083h, 07Ch, 024h, 07Ch, 000h, 075h, 029h, 00Fh, 0B7h, 094h, 024h
        db  080h, 000h, 000h, 000h, 00Fh, 0B7h, 084h, 024h, 082h, 000h, 000h, 000h, 0C1h, 0E0h, 004h, 003h
        db  0C2h, 089h, 084h, 024h, 080h, 000h, 000h, 000h, 0BEh, 0F0h, 0FFh, 010h, 000h, 08Bh, 043h, 004h
        db  0E9h, 0ACh, 000h, 000h, 000h, 08Bh, 054h, 024h, 07Ch, 0E8h, 086h, 003h, 000h, 000h, 00Fh, 082h
        db  0B3h, 000h, 000h, 000h, 00Fh, 084h, 08Eh, 000h, 000h, 000h, 066h, 083h, 07Ah, 00Ah, 000h, 075h
        db  07Dh, 066h, 00Fh, 0BAh, 072h, 00Ch, 004h, 072h, 006h, 0F6h, 042h, 00Ch, 002h, 075h, 06Fh, 08Dh
        db  07Ch, 024h, 008h, 08Bh, 044h, 024h, 07Ch, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 0CDh, 020h, 082h
        db  000h, 001h, 000h, 066h, 089h, 045h, 014h, 0C6h, 045h, 01Dh, 00Ch, 0A1h
        dd  obj1:0x129C              ; fixup
        db  0E8h
        dd  sub_1_05F9               ; fixup
        db  0CDh, 020h, 085h, 000h, 001h, 000h, 08Bh, 07Dh, 014h, 0C1h, 0E7h, 010h, 066h, 08Bh, 07Dh, 010h
        db  00Fh, 0B7h, 045h, 01Ch, 0CDh, 020h, 086h, 000h, 001h, 000h, 08Dh, 074h, 024h, 008h, 0CDh, 020h
        db  08Eh, 000h, 001h, 000h, 083h, 0F8h, 001h, 075h, 049h, 08Bh, 0F2h, 08Bh, 046h, 004h, 0C1h, 0E0h
        db  00Ah, 06Ah, 000h, 050h, 057h, 0CDh, 020h, 06Ch, 000h, 001h, 000h, 083h, 0C4h, 00Ch, 083h, 0F8h
        db  0FFh, 089h, 006h, 066h, 0FFh, 046h, 00Ah, 08Bh, 0D6h, 08Bh, 002h, 08Bh, 072h, 004h, 0C1h, 0E6h
        db  00Ah, 0EBh, 009h, 08Bh, 042h, 004h, 08Bh, 072h, 008h, 0C1h, 0E6h, 00Ch, 08Bh, 094h, 024h, 080h
        db  000h, 000h, 000h, 003h, 0C2h, 02Bh, 0F2h, 076h, 00Dh, 02Bh, 0B4h, 024h, 084h, 000h, 000h, 000h
        db  073h, 00Bh, 0B0h, 0A3h, 0EBh, 006h, 0B0h, 0A4h, 0EBh, 002h, 0B0h, 0A7h, 0F9h, 05Fh, 05Eh, 08Dh
        db  064h, 024h, 070h, 0C2h, 00Ch, 000h, 083h, 0ECh, 00Ch, 08Bh, 07Ch, 01Eh, 060h, 066h, 0B8h, 004h
        db  03Ch, 0CDh, 020h, 01Ch, 000h, 001h, 000h, 096h, 08Bh, 00Eh, 089h, 04Ch, 024h, 008h, 00Fh, 0B7h
        db  056h, 004h, 051h, 0FFh, 076h, 006h, 052h, 0E8h, 0CFh, 0FEh, 0FFh, 0FFh, 00Fh, 082h, 0A4h, 000h
        db  000h, 000h, 089h, 044h, 024h, 004h, 00Fh, 0B7h, 056h, 00Ah, 0FFh, 074h, 024h, 008h, 0FFh, 076h
        db  00Ch, 052h, 0E8h, 0B4h, 0FEh, 0FFh, 0FFh, 00Fh, 082h, 083h, 000h, 000h, 000h, 089h, 004h, 024h
        db  08Bh, 035h
        dd  obj1:0x11FC              ; fixup
        db  08Bh, 07Ch, 01Eh, 060h, 0F6h, 005h
        dd  obj1:0x1204              ; fixup
        db  040h, 075h, 010h, 0A1h
        dd  obj1:0x132C              ; fixup
        db  0B9h, 005h, 000h, 000h, 000h, 0CDh, 020h, 027h, 000h, 001h, 000h, 06Ah, 008h, 053h, 0CDh, 020h
        db  07Dh, 000h, 001h, 000h, 059h, 059h, 00Bh, 0C0h, 09Ch, 057h, 075h, 00Ah, 0FFh, 015h
        dd  sub_1_1328               ; fixup
        db  0B0h, 082h, 074h, 048h, 0FFh, 074h, 024h, 010h, 0FFh, 074h, 024h, 010h, 0FFh, 074h, 024h, 010h
        db  0E8h, 0FEh, 0F7h, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 05Fh, 09Dh, 075h, 00Bh, 06Ah, 004h, 053h, 0CDh
        db  020h, 07Dh, 000h, 001h, 000h, 059h, 059h, 0F6h, 005h
        dd  obj1:0x1204              ; fixup
        db  040h, 075h, 00Bh, 0A1h
        dd  obj1:0x132C              ; fixup
        db  0CDh, 020h, 028h, 000h, 001h, 000h, 066h, 0C7h, 045h, 01Ch, 001h, 000h, 083h, 0C4h, 00Ch, 0F8h
        db  0C3h, 03Ch, 0A7h, 074h, 002h, 004h, 002h, 088h, 045h, 010h, 066h, 083h, 065h, 01Ch, 000h, 0EBh
        db  0EBh, 08Bh, 07Ch, 01Eh, 060h, 00Fh, 0B7h, 055h, 014h, 0E8h, 0D2h, 001h, 000h, 000h, 00Fh, 082h
        db  001h, 0FEh, 0FFh, 0FFh, 00Fh, 085h, 0F9h, 0FDh, 0FFh, 0FFh, 06Ah, 020h, 053h, 0CDh, 020h, 019h
        db  000h, 017h, 000h, 059h, 059h, 0B0h, 0ADh, 0E9h, 0C5h, 0F6h, 0FFh, 0FFh, 08Bh, 07Ch, 01Eh, 060h
        db  00Fh, 0B7h, 055h, 014h, 0E8h, 0A7h, 001h, 000h, 000h, 00Fh, 082h, 0D6h, 0FDh, 0FFh, 0FFh, 075h
        db  007h, 0B0h, 0AAh, 0E9h, 0A9h, 0F6h, 0FFh, 0FFh, 066h, 083h, 07Ah, 00Ah, 000h, 00Fh, 084h, 0C0h
        db  0FDh, 0FFh, 0FFh, 083h, 0ECh, 06Ch, 08Bh, 0FCh, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 0CDh, 020h
        db  082h, 000h, 001h, 000h, 08Bh, 04Bh, 008h, 00Fh, 0B7h, 042h, 008h, 066h, 089h, 041h, 014h, 0C6h
        db  041h, 01Dh, 00Eh, 0A1h
        dd  obj1:0x129C              ; fixup
        db  0E8h
        dd  sub_1_05F9               ; fixup
        db  0CDh, 020h, 085h, 000h, 001h, 000h, 00Fh, 0B7h, 041h, 010h, 00Fh, 0B7h, 049h, 01Ch, 0CDh, 020h
        db  086h, 000h, 001h, 000h, 08Bh, 0F4h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 083h, 0C4h, 06Ch, 083h
        db  0F9h, 001h, 00Fh, 085h, 04Ch, 0F6h, 0FFh, 0FFh, 00Fh, 0B6h, 0C4h, 066h, 03Bh, 042h, 00Ah, 076h
        db  093h, 0E9h, 060h, 0FDh, 0FFh, 0FFh
loc_15_0A12:
        mov    edi, dword ptr [esi + ebx + 0x60]
        movzx  edx, word ptr [ebp + 0x14]
        call   sub_15_0B3F
        jb     loc_15_0774
        jne    loc_15_0A7B
        mov    byte ptr [ebp + 0x11], 0
        mov    eax, dword ptr [edx + 0xc]
        test   byte ptr [ebp + 0x1d], 0x80
        je     loc_15_0A39
        mov    dword ptr [ebp + 0x14], eax
        jmp    loc_15_0A48
loc_15_0A39:
        cmp    eax, 0x10000
        jae    loc_15_0774
        mov    word ptr [ebp + 0x14], ax
loc_15_0A48:
        xor    edx, edx
        lea    esi, [edi + 0x1c]
        mov    ecx, dword ptr [0x12ac]       ; -> obj1:0x12AC
        jecxz  loc_15_0A5E
loc_15_0A55:
        cld    
        lodsd  eax, dword ptr [esi]
        or     eax, eax
        jle    loc_15_0A5C
        inc    edx
loc_15_0A5C:
        loop   loc_15_0A55
loc_15_0A5E:
        test   byte ptr [ebp + 0x1d], 0x80
        je     loc_15_0A6A
        mov    word ptr [ebp + 0x18], dx
        jmp    loc_15_0A73
loc_15_0A6A:
        or     dh, dh
        je     loc_15_0A70
        mov    dl, 0xff
loc_15_0A70:
        mov    byte ptr [ebp + 0x10], dl
loc_15_0A73:
        mov    word ptr [ebp + 0x1c], 1
        clc    
        ret    
loc_15_0A7B:
        cmp    word ptr [edx + 0xa], 0
        je     loc_15_0772
        mov    eax, dword ptr [ebp + 0x1c]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        mov    esi, dword ptr [ebp + 0x14]
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    ecx, dword ptr [ebx + 8]
        mov    si, word ptr [edx + 8]
        mov    dword ptr [ecx + 0x14], esi
        mov    dword ptr [ecx + 0x1c], eax
        mov    eax, dword ptr [0x129c]       ; -> obj1:0x129C
        call   sub_1_05F9
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cmp    word ptr [ecx + 0x1c], 1
        lahf   
        movzx  esi, word ptr [ecx + 0x10]
        mov    ecx, dword ptr [ecx + 0x14]
        mov    di, word ptr [ecx + 0x18]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        sahf   
        mov    eax, esi
        jne    loc_15_0050
        mov    dword ptr [ebp + 0x14], ecx
        movzx  eax, ah
        sub    ax, word ptr [edx + 0xa]
        jae    loc_15_0AFB
        xor    al, al
loc_15_0AFB:
        mov    byte ptr [ebp + 0x11], al
        mov    word ptr [ebp + 0x1c], 1
        clc    
        ret    
loc_15_0B06:
        mov    edi, dword ptr [esi + ebx + 0x60]
        movzx  edx, word ptr [ebp + 0x14]
        call   sub_15_0B3F
        jb     loc_15_0774
        jne    loc_15_070E
        mov    esi, edx
        mov    eax, dword ptr [ebp + 0x10]
        test   byte ptr [ebp + 0x1d], 0x80
        jne    loc_15_0B2F
        and    eax, 0xffff
loc_15_0B2F:
        call   sub_15_0499
        jae    loc_15_0319
        jmp    loc_15_06C0
sub_15_0B3F:
        push   eax
        push   ecx
        push   esi
        lea    esi, [edi + 0x1c]
        mov    ecx, dword ptr [0x12ac]       ; -> obj1:0x12AC
        jecxz  loc_15_0B69
loc_15_0B4D:
        cld    
        lodsd  eax, dword ptr [esi]
        cmp    eax, edx
        je     loc_15_0C04
        or     eax, eax
        jns    loc_15_0B67
        cmp    edx, dword ptr [eax + 0x10]
        jne    loc_15_0B67
        mov    edx, eax
        jmp    loc_15_0C71
loc_15_0B67:
        loop   loc_15_0B4D
loc_15_0B69:
        mov    eax, dword ptr [0x12b4]       ; -> obj1:0x12B4
        mov    ecx, dword ptr [0x12b8]       ; -> obj1:0x12B8
        xor    esi, esi
        jecxz  loc_15_0B9B
loc_15_0B78:
        test   byte ptr [eax + 0xc], 1
        jne    loc_15_0B82
        mov    esi, eax
        jmp    loc_15_0B88
loc_15_0B82:
        cmp    dx, word ptr [eax + 8]
        je     loc_15_0B8F
loc_15_0B88:
        add    eax, 0x10
        loop   loc_15_0B78
        jmp    loc_15_0B9B
loc_15_0B8F:
        test   byte ptr [eax + 0xc], 4
        je     loc_15_0C6C
        mov    esi, eax
loc_15_0B9B:
        or     esi, esi
        je     loc_15_0C04
        cmp    dword ptr [0x127c], 0         ; -> obj1:0x127C
        je     loc_15_0C07
        test   byte ptr [esi + 0xc], 4
        jne    loc_15_0C07
        mov    eax, esi
        mov    ecx, dword ptr [0x127c]       ; -> obj1:0x127C
        mov    esi, dword ptr [0x9c]         ; -> obj6:0x009C
        add    esi, dword ptr [0x1280]       ; -> obj1:0x1280
loc_15_0BC2:
        cmp    dx, word ptr [esi]
        jne    loc_15_0BFF
        test   byte ptr [esi + 2], 1
        jne    loc_15_0BFF
        or     byte ptr [eax + 0xc], 3
        mov    word ptr [eax + 8], dx
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [eax + 4], ecx
        shl    ecx, 0xa
        mov    edx, dword ptr [esi + 8]
        xchg   esi, eax
        push   0
        push   ecx
        push   edx
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_15_0C04
        mov    dword ptr [esi], eax
        mov    word ptr [esi + 0xa], 0
        xchg   esi, eax
        jmp    loc_15_0C6C
loc_15_0BFF:
        add    esi, 0xc
        loop   loc_15_0BC2
loc_15_0C04:
        stc    
        jmp    loc_15_0C71
loc_15_0C07:
        push   esi
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    esi, dword ptr [ebx + 8]
        mov    word ptr [esi + 0x14], dx
        mov    byte ptr [esi + 0x1d], 0xe
        mov    eax, dword ptr [0x129c]       ; -> obj1:0x129C
        call   sub_1_05F9
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        movzx  ecx, word ptr [esi + 0x14]
        movzx  esi, word ptr [esi + 0x1c]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    eax
        cmp    esi, 1
        jne    loc_15_0C04
        or     byte ptr [eax + 0xc], 1
        mov    word ptr [eax + 8], dx
        mov    dword ptr [eax + 4], ecx
        mov    word ptr [eax + 0xa], 0
loc_15_0C6C:
        mov    edx, eax
        or     eax, 1
loc_15_0C71:
        pop    esi
        pop    ecx
        pop    eax
        ret    
        db  024h, 056h, 038h
VxD__LTEXT ENDS
