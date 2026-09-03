; VDMAD -- reconstructed 32-bit disassembly (VxD / LE format)
; 8 objects, 4941 instructions recovered by recursive descent

Declare_Virtual_Device VDMAD, 4, 0, VDMAD_Control, 00004h, Undefined_Init_Order
;   DDK version 0400h   init_order 090000000h   flags 00000h
;   service table: obj1:0x1d80, 34 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2164  R-X
loc_1_0000:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_0011
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0011:
        je     loc_1_002C
        xor    al, al
        ret    
loc_1_0016:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_0027
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0027:
        je     loc_1_002C
        xor    al, al
loc_1_002B:
        ret    
loc_1_002C:
        mov    cl, al
        and    cl, 0xc
        je     loc_1_002B
        VMMCall  VMM.Crash_Cur_VM                ; cd202f000100
loc_1_0039:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_004A
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_004A:
        je     loc_1_004E
        in     al, dx
        ret    
loc_1_004E:
        out    dx, al
        mov    esi, 0x200c                   ; -> obj1:0x200C
        cmp    dx, 0x40b
        mov    edx, 0x1d78                   ; -> obj1:0x1D78
        je     loc_1_0066
        add    esi, 8
        add    edx, 4
loc_1_0066:
        movzx  ecx, al
        and    cl, 3
        mov    byte ptr [edx + ecx], al
        and    al, 0xc
        btr    dword ptr [esi], ecx
        cmp    al, 4
        jne    loc_1_007B
        bts    dword ptr [esi], ecx
loc_1_007B:
        ret    
loc_1_007C:
        call   sub_1_2104
        sub    edx, 0x400
        cmp    ecx, 4
        jbe    loc_1_0093
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0093:
        je     loc_1_00A4
        call   sub_1_1519
        call   sub_1_02DA
        movzx  eax, word ptr [eax + 0x23]
        ret    
loc_1_00A4:
        call   sub_1_1519
        mov    ah, 3
        call   sub_1_175F
        xor    edx, edx
        jmp    sub_1_1932
loc_1_00B7:
        call   sub_1_2104
        mov    esi, 0x200c                   ; -> obj1:0x200C
        cmp    ecx, 4
        jbe    loc_1_00CD
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_00CD:
        jb     loc_1_00E8
        call   sub_1_00F6
        mov    edx, eax
        call   sub_1_02DA
        mov    byte ptr [eax + 0x2a], dl
        mov    byte ptr [eax + 0x2e], dl
        xor    edx, edx
        jmp    sub_1_1932
loc_1_00E8:
        call   sub_1_00F6
        call   sub_1_02DA
        mov    al, byte ptr [eax + 0x2a]
        ret    
sub_1_00F6:
        movzx  ecx, dl
        and    cl, 7
        cmp    dx, 0x4c6
        jb     loc_1_0108
        add    esi, 8
        shr    cl, 1
loc_1_0108:
        dec    cl
        shr    cl, 1
        ret    
loc_1_010D:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_011E
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_011E:
        je     loc_1_0123
        xor    al, al
loc_1_0122:
        ret    
loc_1_0123:
        or     al, al
        je     loc_1_0122
        VMMCall  VMM.Crash_Cur_VM                ; cd202f000100
loc_1_012D:
        pushal 
        mov    edi, eax
        mov    ebx, edx
        movzx  eax, word ptr [edi + 0x18]
        mov    edx, dword ptr [edi]
        movzx  edx, byte ptr [edx + 0x2134]  ; -> obj1:0x2134
        call   sub_1_06C6
        movzx  eax, word ptr [edi + 0x16]
        mov    edx, dword ptr [edi]
        movzx  edx, byte ptr [edx + 0x212c]  ; -> loc_1_212C
        out    dx, al
        add    dx, 0x400
        mov    al, ah
        out    dx, al
        mov    eax, dword ptr [edi + 0x10]
        dec    eax
        mov    edx, dword ptr [edi]
        mov    cl, byte ptr [edx + 0x1d78]   ; -> obj1:0x1D78
        and    cl, 0xc
        cmp    cl, 4
        jne    loc_1_0170
        shr    eax, 1
loc_1_0170:
        movzx  edx, byte ptr [edx + 0x213c]  ; -> loc_1_213C
        call   sub_1_06C6
        add    dx, 0x400
        shr    eax, 0x10
        out    dx, al
        mov    eax, dword ptr [edi]
        mov    edx, eax
        and    al, 3
        and    bl, 0xfc
        or     al, bl
        and    edx, 4
        shl    edx, 2
        movzx  edx, byte ptr [edx + 0x2020]  ; -> obj1:0x2020
        out    dx, al
        mov    eax, dword ptr [edi]
        mov    edx, eax
        and    al, 3
        or     al, byte ptr [edx + 0x1d78]   ; -> obj1:0x1D78
        and    bh, 0xf0
        or     al, bh
        and    edx, 0xfffffffc
        mov    dx, 0x40b
        je     loc_1_01BC
        mov    dx, 0x4d6
loc_1_01BC:
        out    dx, al
        popal  
        ret    
        db  000h
VDMAD_Control:
        cmp    eax, 0
        je     loc_3_009C
        cmp    eax, 7
        je     loc_1_0205
        cmp    eax, 0xb
        je     loc_1_024F
        cmp    eax, 6
        je     loc_1_1058
        cmp    eax, 0x22
        je     loc_8_0000
        cmp    eax, 2
        je     loc_3_031C
        test   dword ptr [0x1f4], 0x400000   ; -> obj1:0x01F4
        je     loc_1_0203
        cmp    eax, 0x14
        je     loc_6_0000
loc_1_0203:
        clc    
        ret    
loc_1_0205:
        mov    esi, ebx
        add    esi, dword ptr [0x1e68]       ; -> obj1:0x1E68
        xor    eax, eax
        mov    word ptr [esi + 8], ax
        clc    
        ret    
sub_1_0215:
        push   eax
        push   esi
        mov    esi, 0x200c                   ; -> obj1:0x200C
        mov    eax, dword ptr [eax]
        btr    eax, 2
        jae    loc_1_0229
        mov    esi, 0x2014                   ; -> obj1:0x2014
loc_1_0229:
        bts    dword ptr [esi + 5], eax
        pop    esi
        pop    eax
        VxDCall  VDMAD.VDMAD_Mask_Channel        ; cd2007000400
        cmp    byte ptr [eax + 0x15], 0
        je     loc_1_0242
        call   sub_1_0FCC
        jmp    loc_1_024E
loc_1_0242:
        cmp    byte ptr [eax + 0x14], 0
        je     loc_1_024E
        stc    
        call   sub_1_0F64
loc_1_024E:
        ret    
loc_1_024F:
        mov    eax, 0x1e6c                   ; -> obj1:0x1E6C
        mov    ecx, 8
loc_1_0259:
        cmp    dword ptr [eax + 8], ebx
        jne    loc_1_029F
        call   sub_1_02EF
        jae    loc_1_029A
        mov    edx, ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xchg   ebx, edx
        mov    dword ptr [eax + 8], edx
        cmp    byte ptr [eax + 0x15], 0
        je     loc_1_0284
        push   eax
        movzx  eax, byte ptr [eax + 0x15]
        mov    dword ptr [eax + 0x204b], edx ; -> loc_1_204B
        pop    eax
loc_1_0284:
        cmp    dword ptr [eax + 0x1c], 0
        jne    loc_1_029F
        push   esi
        mov    esi, dword ptr [eax + 0xc]
        sub    esi, dword ptr [ebx + 4]
        add    esi, dword ptr [edx + 4]
        mov    dword ptr [eax + 0xc], esi
        pop    esi
        jmp    loc_1_029F
loc_1_029A:
        call   sub_1_0215
loc_1_029F:
        add    eax, 0x34
        loop   loc_1_0259
        mov    eax, 0x203c                   ; -> obj1:0x203C
loc_1_02A9:
        cmp    dword ptr [eax + 0x10], ebx
        jne    loc_1_02BE
        push   ebx
        mov    ebx, eax
        sub    ebx, 0x203b                   ; -> loc_1_203B
        VxDCall  VDMAD.VDMAD_Release_Buffer      ; cd200f000400
        pop    ebx
loc_1_02BE:
        add    eax, 0x18
        cmp    eax, 0x20fc                   ; -> obj1:0x20FC
        jb     loc_1_02A9
        clc    
        ret    
sub_1_02CA:
        cmp    eax, 8
        jae    loc_1_02D8
        imul   eax, eax, 0x34
        add    eax, 0x1e6c                   ; -> obj1:0x1E6C
        stc    
loc_1_02D8:
        cmc    
        ret    
sub_1_02DA:
        mov    eax, ecx
        cmp    esi, 0x200c                   ; -> obj1:0x200C
        jbe    loc_1_02E6
        add    al, 4
loc_1_02E6:
        imul   eax, eax, 0x34
        add    eax, 0x1e6c                   ; -> obj1:0x1E6C
        ret    
sub_1_02EF:
        push   eax
        push   ecx
        push   edx
        cmp    dword ptr [eax + 0x24], 0xa0000
        jae    loc_1_031F
        mov    edx, dword ptr [eax + 0x2c]
        test   byte ptr [eax + 0x31], 0x40
        je     loc_1_0307
        shl    edx, 1
        inc    edx
loc_1_0307:
        inc    edx
        push   0
        push   edx
        push   dword ptr [eax + 0x24]
        VMMCall  VMM._TestGlobalV86Mem           ; cd205f000100
        add    esp, 0xc
        cmp    eax, 1
        je     loc_1_031F
        clc    
        jmp    loc_1_0320
loc_1_031F:
        stc    
loc_1_0320:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_1_0324:
        push   ecx
        in     al, 8
        or     byte ptr [0x2118], al         ; -> loc_1_2118
        mov    cl, byte ptr [0x2010]         ; -> obj1:0x2010
        and    cl, 0xf
        or     cl, al
        mov    byte ptr [0x2010], cl         ; -> obj1:0x2010
        in     al, 0xd0
        or     byte ptr [0x2119], al         ; -> obj1:0x2119
        mov    cl, byte ptr [0x2018]         ; -> obj1:0x2018
        and    cl, 0xf
        or     cl, al
        mov    byte ptr [0x2018], cl         ; -> obj1:0x2018
        movzx  eax, word ptr [0x2118]        ; -> loc_1_2118
        pop    ecx
        ret    
VDMAD_Get_Version:
        mov    eax, 0x400
        clc    
        ret    
VDMAD_Get_Max_Phys_Page:
        mov    eax, dword ptr [0x2100]       ; -> obj1:0x2100
        ret    
VDMAD_Virtualize_Channel:
        call   sub_1_02CA
        cmp    dword ptr [eax + 4], 0xe2d    ; -> loc_1_0E2D
        je     loc_1_037E
        stc    
        jmp    loc_1_038B
loc_1_037E:
        push   eax
        mov    eax, dword ptr [eax]
        call   sub_8_01D6
        pop    eax
        mov    dword ptr [eax + 4], esi
        clc    
loc_1_038B:
        ret    
VDMAD_Unvirtualize_Channel:
        mov    edx, eax
        mov    eax, dword ptr [edx]
        call   sub_1_02CA
        jb     loc_1_039B
        cmp    eax, edx
        je     loc_1_039D
loc_1_039B:
        stc    
        ret    
loc_1_039D:
        mov    eax, dword ptr [edx]
        call   sub_8_01EC
        mov    dword ptr [edx + 4], 0xe2d    ; -> loc_1_0E2D
        mov    word ptr [edx + 0x1a], 0
        clc    
        ret    
VDMAD_Set_Channel_Callbacks:
        and    word ptr [eax + 0x1a], bx
        or     word ptr [eax + 0x1a], cx
        ret    
VDMAD_Disable_Translation:
        push   ecx
        mov    ecx, dword ptr [eax]
        bts    dword ptr [0x2110], ecx       ; -> loc_1_2110
        mov    ecx, dword ptr [eax + 0x1c]
        add    ecx, 1
        jb     loc_1_03D1
        mov    dword ptr [eax + 0x1c], ecx
loc_1_03D1:
        pop    ecx
        ret    
VDMAD_Enable_Translation:
        push   ecx
        mov    ecx, dword ptr [eax + 0x1c]
        sub    ecx, 1
        jb     loc_1_03EB
        mov    dword ptr [eax + 0x1c], ecx
        jne    loc_1_03EB
        mov    ecx, dword ptr [eax]
        btr    dword ptr [0x2110], ecx       ; -> loc_1_2110
        clc    
loc_1_03EB:
        pop    ecx
        ret    
VDMAD_Get_Region_Info:
        movzx  ebx, byte ptr [eax + 0x15]
        mov    bh, byte ptr [eax + 0x14]
        mov    esi, dword ptr [eax + 0xc]
        mov    ecx, dword ptr [eax + 0x10]
        ret    
VDMAD_Set_Region_Info:
        mov    byte ptr [eax + 0x15], bl
        mov    byte ptr [eax + 0x14], bh
        mov    dword ptr [eax + 0xc], esi
        mov    dword ptr [eax + 0x10], ecx
        push   edx
        cmp    byte ptr [0x211a], 1          ; -> loc_1_211A
        je     loc_1_0422
        test   byte ptr [eax + 0x31], 0x40
        je     loc_1_0422
        shr    edx, 1
        ror    edx, 0x10
        shl    dx, 1
        ror    edx, 0x10
loc_1_0422:
        mov    word ptr [eax + 0x18], dx
        shr    edx, 0x10
        mov    word ptr [eax + 0x16], dx
        pop    edx
        ret    
VDMAD_Get_Virt_State:
        mov    esi, 0x200c                   ; -> obj1:0x200C
        mov    ecx, dword ptr [eax]
        push   eax
        cmp    cl, 4
        jb     loc_1_0442
        sub    cl, 4
        add    esi, 8
loc_1_0442:
        movzx  edx, byte ptr [eax + 0x30]
        and    dl, 0xfc
        mov    dh, byte ptr [eax + 0x31]
        movzx  eax, byte ptr [esi + 5]
        bt     eax, ecx
        jae    loc_1_0458
        or     dl, 1
loc_1_0458:
        movzx  eax, byte ptr [esi + 6]
        bt     eax, ecx
        jae    loc_1_0464
        or     dl, 2
loc_1_0464:
        pop    eax
        mov    esi, dword ptr [eax + 0x20]
        mov    ecx, dword ptr [eax + 0x28]
        cmp    dword ptr [eax + 0x1c], 0
        jne    loc_1_0474
        add    esi, dword ptr [ebx + 4]
loc_1_0474:
        test   dh, 0x40
        je     loc_1_047C
        shl    ecx, 1
        inc    ecx
loc_1_047C:
        inc    ecx
        ret    
VDMAD_Set_Virt_State:
        pushal 
        mov    edi, 0x200c                   ; -> obj1:0x200C
        mov    ecx, dword ptr [eax]
        cmp    cl, 4
        jb     loc_1_0491
        sub    cl, 4
        add    edi, 8
loc_1_0491:
        movzx  eax, byte ptr [edi + 5]
        test   dl, 1
        je     loc_1_049F
        bts    eax, ecx
        jmp    loc_1_04A2
loc_1_049F:
        btr    eax, ecx
loc_1_04A2:
        mov    byte ptr [edi + 5], al
        movzx  eax, byte ptr [edi + 6]
        test   dl, 2
        je     loc_1_04B3
        bts    eax, ecx
        jmp    loc_1_04B6
loc_1_04B3:
        btr    eax, ecx
loc_1_04B6:
        mov    byte ptr [edi + 6], al
        mov    edi, dword ptr [esp + 0x1c]
        and    dl, 0xfc
        mov    byte ptr [edi + 0x30], dl
        mov    byte ptr [edi + 0x31], dh
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [edi + 0x1c], 0
        jne    loc_1_04D3
        sub    eax, dword ptr [ebx + 4]
loc_1_04D3:
        mov    dword ptr [edi + 0x20], eax
        mov    eax, dword ptr [esp + 0x18]
        dec    eax
        test   dh, 0x40
        je     loc_1_04E2
        shr    eax, 1
loc_1_04E2:
        mov    dword ptr [edi + 0x28], eax
        mov    dword ptr [edi + 0x2c], eax
        popal  
        ret    
VDMAD_Get_Virt_Count:
        mov    ecx, dword ptr [eax + 0x28]
        test   byte ptr [eax + 0x31], 0x40
        je     loc_1_04F6
        shl    ecx, 1
        inc    ecx
loc_1_04F6:
        inc    ecx
        ret    
VDMAD_Set_Virt_Count:
        push   ecx
        jecxz  loc_1_0504
        dec    ecx
        test   byte ptr [eax + 0x31], 0x40
        je     loc_1_0504
        shr    ecx, 1
loc_1_0504:
        mov    dword ptr [eax + 0x28], ecx
        pop    ecx
        ret    
VDMAD_Set_Phys_State:
        pushal 
        mov    edi, eax
        mov    ebx, edx
        movzx  eax, word ptr [edi + 0x18]
        mov    edx, dword ptr [edi]
        movzx  edx, byte ptr [edx + 0x2134]  ; -> obj1:0x2134
        call   sub_1_06C6
        movzx  eax, word ptr [edi + 0x16]
        mov    edx, dword ptr [edi]
        movzx  edx, byte ptr [edx + 0x212c]  ; -> loc_1_212C
        out    dx, al
        mov    eax, dword ptr [edi + 0x10]
        dec    eax
        mov    edx, dword ptr [edi]
        cmp    edx, 4
        jb     loc_1_053B
        shr    eax, 1
loc_1_053B:
        movzx  edx, byte ptr [edx + 0x213c]  ; -> loc_1_213C
        call   sub_1_06C6
        mov    eax, dword ptr [edi]
        mov    edx, eax
        and    al, 3
        and    bl, 0xfc
        or     al, bl
        and    edx, 4
        shl    edx, 2
        movzx  edx, byte ptr [edx + 0x2020]  ; -> obj1:0x2020
        out    dx, al
        popal  
        ret    
VDMAD_Get_Phys_Count:
        push   edx
        push   edi
        mov    edi, eax
        mov    eax, dword ptr [edi]
        movzx  edx, byte ptr [eax + 0x213c]  ; -> loc_1_213C
        call   sub_1_067C
        mov    ecx, eax
        mov    eax, edi
        pop    edi
        pop    edx
        ret    
VDMAD_Get_Phys_Status:
        pushfd 
        cli    
        call   sub_1_0324
        popfd  
        ret    
VDMAD_Mask_Channel:
        pushal 
        pushfd 
        cli    
        movzx  ecx, byte ptr [0x2013]        ; -> loc_1_2013
        mov    ch, byte ptr [0x201b]         ; -> loc_1_201B
        jecxz  loc_1_05DB
        push   ebx
        xor    ebx, ebx
        mov    edx, dword ptr [eax]
        bts    ebx, edx
        shl    ebx, 4
        shr    bl, 4
        not    ebx
        and    ecx, ebx
        pop    ebx
        mov    byte ptr [0x2013], cl         ; -> loc_1_2013
        mov    byte ptr [0x201b], ch         ; -> loc_1_201B
        jne    loc_1_05DB
        push   eax
        mov    eax, dword ptr [0x2108]       ; -> loc_1_2108
        mov    dword ptr [0x210c], eax       ; -> loc_1_210C
        pop    eax
        mov    dword ptr [0x2104], 0x1952    ; -> sub_1_2104
        xor    esi, esi
        xchg   dword ptr [0x2114], esi       ; -> loc_1_2114
        VMMCall  VMM.Cancel_Global_Event         ; cd2012000100
loc_1_05DB:
        popfd  
loc_1_05DC:
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [eax]
        mov    edx, eax
        and    edx, 4
        shl    edx, 2
        and    al, 3
        or     al, 4
        movzx  edx, byte ptr [edx + 0x2021]  ; -> loc_1_2021
        out    dx, al
        jmp    loc_1_05FB
loc_1_05FB:
        popal  
        ret    
VDMAD_Phys_Mask_Channel:
        pushal 
        jmp    loc_1_05DC
VDMAD_UnMask_Channel:
        pushal 
        mov    edx, eax
        pushfd 
        cli    
        call   sub_1_0324
        xor    ecx, ecx
        mov    eax, dword ptr [edx]
        bts    ecx, eax
        shl    ecx, 4
        shr    cl, 4
        not    ecx
        and    byte ptr [0x2013], cl         ; -> loc_1_2013
        and    byte ptr [0x201b], ch         ; -> loc_1_201B
        not    ecx
        test   byte ptr [edx + 0x30], 0x10
        jne    loc_1_0658
        or     byte ptr [0x2013], cl         ; -> loc_1_2013
        or     byte ptr [0x201b], ch         ; -> loc_1_201B
        not    ecx
        and    word ptr [0x2118], cx         ; -> loc_1_2118
        mov    esi, 0xd7e                    ; -> loc_1_0D7E
        mov    dword ptr [0x210c], esi       ; -> loc_1_210C
        mov    esi, 0x195d                   ; -> loc_1_195D
        mov    dword ptr [0x2104], esi       ; -> sub_1_2104
loc_1_0658:
        popfd  
loc_1_0659:
        mov    dword ptr [edx + 8], ebx
        mov    eax, dword ptr [edx]
        call   sub_1_1D68
        mov    edx, eax
        and    edx, 4
        shl    edx, 2
        and    al, 3
        movzx  edx, byte ptr [edx + 0x2021]  ; -> loc_1_2021
        out    dx, al
        jmp    loc_1_0677
loc_1_0677:
        popal  
        ret    
VDMAD_Phys_Unmask_Channel:
        pushal 
        jmp    loc_1_0659
sub_1_067C:
        pushfd 
        cli    
        push   edx
        mov    eax, dword ptr [edi]
        and    eax, 4
        shl    eax, 2
        movzx  edx, byte ptr [eax + 0x2023]  ; -> loc_1_2023
        out    dx, al
        jmp    loc_1_0691
loc_1_0691:
        pop    edx
        push   ecx
        call   sub_1_06A6
loc_1_0698:
        mov    ecx, eax
        call   sub_1_06A6
        cmp    ah, ch
        jne    loc_1_0698
        pop    ecx
        popfd  
        ret    
sub_1_06A6:
        push   edx
        in     al, dx
        jmp    loc_1_06AA
loc_1_06AA:
        xchg   ah, al
        in     al, dx
        xchg   ah, al
        cmp    byte ptr [0x211a], 2          ; -> loc_1_211A
        jne    loc_1_06C4
        add    dx, 0x400
        shl    eax, 0x10
        in     al, dx
        rol    eax, 0x10
loc_1_06C4:
        pop    edx
        ret    
sub_1_06C6:
        pushfd 
        cli    
        push   eax
        push   edx
        mov    eax, dword ptr [edi]
        and    eax, 4
        shl    eax, 2
        movzx  edx, byte ptr [eax + 0x2023]  ; -> loc_1_2023
        out    dx, al
        jmp    loc_1_06DC
loc_1_06DC:
        pop    edx
        pop    eax
        out    dx, al
        jmp    loc_1_06E1
loc_1_06E1:
        xchg   ah, al
        out    dx, al
        xchg   ah, al
        popfd  
        ret    
sub_1_06E8:
        or     al, 0xff
        push   esi
        push   ecx
        shl    esi, 0xc
loc_1_06EF:
        and    byte ptr [esi], al
        add    esi, 0x1000
        loop   loc_1_06EF
        pop    ecx
        pop    esi
        ret    
VDMAD_Lock_DMA_Region:
        pushfd 
        pushal 
        mov    ebp, esp
        sub    esp, 0x18
        mov    edi, esp
        mov    dword ptr [edi], ecx
        mov    dword ptr [edi + 4], esi
        xor    eax, eax
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 0xc], eax
        mov    byte ptr [edi + 0xc], 1
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VxDCall  VDMAD.VDMAD_Scatter_Lock        ; cd200b000400
        jb     loc_1_078E
        mov    edx, dword ptr [edi + 0x10]
        mov    ecx, dword ptr [edi + 0x14]
        pushfd 
        test   byte ptr [ebp + 0x14], 3
        je     loc_1_0748
        mov    esi, 0x10000
        test   byte ptr [ebp + 0x14], 2
        je     loc_1_073E
        shl    esi, 1
loc_1_073E:
        movzx  eax, dx
        add    eax, ecx
        dec    eax
        cmp    eax, esi
        jae    loc_1_0781
loc_1_0748:
        popfd  
        je     loc_1_079B
        mov    dword ptr [ebp + 0x18], ecx
        mov    al, 1
        cmp    word ptr [edi + 0xe], 1
        jg     loc_1_0759
        mov    al, 3
loc_1_0759:
        movzx  eax, al
        mov    dword ptr [ebp + 0x1c], eax
        mov    esi, dword ptr [edi + 4]
        lea    eax, [ecx + esi + 0xfff]
        shr    esi, 0xc
        shr    eax, 0xc
        sub    eax, esi
        push   0
        push   eax
        push   esi
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        stc    
        jmp    loc_1_079F
loc_1_0781:
        popfd  
        sub    esi, eax
        dec    esi
        add    esi, ecx
        mov    dword ptr [ebp + 0x18], esi
        mov    al, 2
        jmp    loc_1_0759
loc_1_078E:
        xor    eax, eax
        mov    dword ptr [ebp + 0x18], eax
        mov    al, 3
        mov    dword ptr [ebp + 0x1c], eax
        stc    
        jmp    loc_1_079F
loc_1_079B:
        mov    dword ptr [ebp + 0x14], edx
        clc    
loc_1_079F:
        pushfd 
        pop    eax
        mov    dword ptr [ebp + 0x20], eax
        mov    esp, ebp
        popal  
        popfd  
        ret    
VDMAD_Unlock_DMA_Region:
        clc    
        jecxz  loc_1_07D3
        pushal 
loc_1_07AD:
        pushfd 
        dec    ecx
        add    ecx, esi
        shr    esi, 0xc
        shr    ecx, 0xc
        sub    ecx, esi
        inc    ecx
        popfd  
        jb     loc_1_07C2
        call   sub_1_06E8
loc_1_07C2:
        push   0
        push   ecx
        push   esi
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        sub    eax, 1
        popal  
loc_1_07D3:
        ret    
VDMAD_Unlock_DMA_Region_No_Dirty:
        pushal 
        stc    
        jmp    loc_1_07AD
loc_1_07D8:
        push   0
        push   1
        push   esi
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_0804
        push   0
        push   ebp
        push   1
        push   esi
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        mov    eax, dword ptr [ebp]
        and    eax, 0xfffff000
        or     al, 1
        ret    
loc_1_0804:
        xor    eax, eax
        stc    
        ret    
loc_1_0808:
        push   0
        push   ebp
        push   1
        push   esi
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        mov    eax, dword ptr [ebp]
        test   al, 1
        je     loc_1_084A
        push   0
        push   1
        push   esi
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_084D
        push   0
        push   ebp
        push   1
        push   esi
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        mov    eax, dword ptr [ebp]
        and    eax, 0xfffff000
        or     al, 1
        ret    
loc_1_084A:
        xor    eax, eax
        ret    
loc_1_084D:
        xor    eax, eax
        stc    
        ret    
VDMAD_Scatter_Lock:
        pushal 
        movzx  eax, word ptr [edi + 8]
        or     eax, eax
        je     loc_1_0870
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_0A0B
loc_1_0870:
        mov    esi, dword ptr [edi + 4]
        add    esi, eax
        mov    edx, esi
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        jne    loc_1_0882
        jmp    loc_1_0A0D
loc_1_0882:
        dec    ecx
        add    ecx, esi
        jb     loc_1_0A0F
        shr    esi, 0xc
        shr    ecx, 0xc
        sub    ecx, esi
        inc    ecx
        test   byte ptr [esp + 0x1c], 1
        je     loc_1_092B
        mov    eax, edx
        and    eax, 0xfff
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp + 0x14], ecx
        mov    word ptr [edi + 0xe], 0
        movzx  ebx, word ptr [edi + 0xc]
        or     ebx, ebx
        je     loc_1_0A11
        mov    dword ptr [0x211c], 0x7d8     ; -> sub_1_211C
        test   byte ptr [esp + 0x1c], 2
        jne    loc_1_08DB
        mov    dword ptr [0x211c], 0x808     ; -> sub_1_211C
loc_1_08DB:
        push   edi
        push   ecx
        push   ebp
        sub    esp, 4
        mov    ebp, esp
        add    edi, 0x10
loc_1_08E6:
        push   ecx
        call   sub_1_211C
        jb     loc_1_0928
        dec    ebx
        js     loc_1_091A
        cld    
        stosd  dword ptr es:[edi], eax
        pop    ecx
        inc    esi
        loop   loc_1_08E6
loc_1_08F8:
        add    esp, 4
        pop    ebp
        pop    edx
        pop    edi
        jecxz  loc_1_0915
        sub    edx, ecx
        jbe    loc_1_0A11
        shl    edx, 0xc
        mov    eax, edx
        mov    edx, dword ptr [esp + 4]
        sub    eax, edx
        mov    dword ptr [edi], eax
loc_1_0915:
        jmp    loc_1_09E8
loc_1_091A:
        push   0
        push   1
        push   esi
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
loc_1_0928:
        pop    ecx
        jmp    loc_1_08F8
loc_1_092B:
        mov    dword ptr [esp + 4], 0
        mov    dword ptr [esp + 0x14], ecx
        mov    word ptr [edi + 0xe], 0
        movzx  ebx, word ptr [edi + 0xc]
        or     ebx, ebx
        je     loc_1_0A11
        push   edi
        push   ecx
        push   ebp
        push   dword ptr [edi]
        sub    esp, 0xc
        mov    ebp, esp
        add    edi, 0xc
        mov    eax, edx
        and    eax, 0xfff
        neg    eax
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [ebp + 8], eax
        mov    dword ptr [ebp + 4], 0xffffffff
loc_1_096C:
        push   ecx
        push   0
        push   1
        push   esi
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_0A08
        push   0
        push   ebp
        push   1
        push   esi
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        mov    eax, dword ptr [ebp]
        and    eax, 0xfffff000
        cmp    eax, dword ptr [ebp + 4]
        je     loc_1_09BC
        add    edi, 4
        dec    ebx
        js     loc_1_09FA
        cld    
        stosd  dword ptr es:[edi], eax
        cmp    dword ptr [ebp + 4], -1
        je     loc_1_09B5
        mov    dword ptr [edi], 0
        jmp    loc_1_09BC
loc_1_09B5:
        mov    edx, dword ptr [edi]
        neg    edx
        add    dword ptr [edi - 4], edx
loc_1_09BC:
        mov    edx, 0x1000
        add    dword ptr [edi], edx
        add    eax, edx
        add    dword ptr [ebp + 8], edx
        mov    dword ptr [ebp + 4], eax
        pop    ecx
        inc    esi
        loop   loc_1_096C
        mov    edx, dword ptr [ebp + 8]
        sub    edx, dword ptr [ebp + 0xc]
        sub    dword ptr [edi], edx
loc_1_09D7:
        mov    eax, dword ptr [ebp + 8]
        add    esp, 0x10
        pop    ebp
        pop    edx
        pop    edi
        jecxz  loc_1_09E8
        sub    edx, ecx
        jbe    loc_1_0A11
        mov    dword ptr [edi], eax
loc_1_09E8:
        movzx  eax, word ptr [edi + 0xc]
        sub    eax, ebx
        mov    word ptr [edi + 0xe], ax
        mov    dword ptr [esp + 0x14], eax
        or     ecx, ecx
        jmp    loc_1_0A18
loc_1_09FA:
        push   0
        push   1
        push   esi
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
loc_1_0A08:
        pop    ecx
        jmp    loc_1_09D7
loc_1_0A0B:
        jmp    loc_1_0A11
loc_1_0A0D:
        jmp    loc_1_0A11
loc_1_0A0F:
        jmp    loc_1_0A11
loc_1_0A11:
        mov    dword ptr [edi], 0
        stc    
loc_1_0A18:
        popal  
        ret    
VDMAD_Scatter_Unlock:
        pushal 
        movzx  eax, word ptr [edi + 8]
        or     eax, eax
        je     loc_1_0A30
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
loc_1_0A30:
        mov    esi, dword ptr [edi + 4]
        add    esi, eax
        mov    eax, dword ptr [esp + 0x1c]
        and    al, 3
        cmp    al, 3
        je     loc_1_0A6B
        mov    ecx, dword ptr [edi]
        dec    ecx
        add    ecx, esi
        shr    esi, 0xc
        shr    ecx, 0xc
        sub    ecx, esi
        inc    ecx
        test   byte ptr [esp + 0x1c], 4
        jne    loc_1_0A59
        call   sub_1_06E8
loc_1_0A59:
        push   0
        push   ecx
        push   esi
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        sub    eax, 1
loc_1_0A69:
        popal  
        ret    
loc_1_0A6B:
        mov    ebp, esi
        and    ebp, 0xfffff000
        shr    esi, 0xc
        movzx  ecx, word ptr [edi + 0xe]
        lea    edi, [edi + 0x10]
loc_1_0A7D:
        test   byte ptr [edi], 1
        je     loc_1_0A9D
        test   byte ptr [esp + 0x1c], 4
        jne    loc_1_0A8D
        and    byte ptr [ebp], 0xff
loc_1_0A8D:
        push   ecx
        push   0
        push   1
        push   esi
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        pop    ecx
loc_1_0A9D:
        inc    esi
        add    edi, 4
        add    ebp, 0x1000
        loop   loc_1_0A7D
        clc    
        jmp    loc_1_0A69
VDMAD_Request_Buffer:
        push   ecx
        push   esi
        add    ecx, 0xfff
        shr    ecx, 0xc
        mov    dl, 5
        xor    esi, esi
        mov    eax, 0x203c                   ; -> obj1:0x203C
loc_1_0AC0:
        mov    ebx, dword ptr [eax]
        cmp    ecx, ebx
        ja     loc_1_0AD0
        test   byte ptr [eax + 0xc], 1
        je     loc_1_0AF4
        mov    dl, 6
        jmp    loc_1_0AE0
loc_1_0AD0:
        test   byte ptr [eax + 0xc], 1
        jne    loc_1_0AE0
        test   esi, esi
        je     loc_1_0ADE
        cmp    ebx, dword ptr [esi]
        jbe    loc_1_0AE0
loc_1_0ADE:
        mov    esi, eax
loc_1_0AE0:
        add    eax, 0x18
        cmp    eax, 0x20fc                   ; -> obj1:0x20FC
        jb     loc_1_0AC0
        test   esi, esi
        jne    loc_1_0B37
        mov    al, 6
loc_1_0AF0:
        pop    esi
        pop    ecx
        stc    
        ret    
loc_1_0AF4:
        or     byte ptr [eax + 0xc], 1
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    dword ptr [eax + 0x10], ebx
        mov    edx, dword ptr [eax + 4]
        mov    ebx, eax
        sub    ebx, 0x203b                   ; -> loc_1_203B
        cmp    ebx, 1
        jne    loc_1_0B15
loc_1_0B11:
        pop    esi
        pop    ecx
        clc    
        ret    
loc_1_0B15:
        xor    esi, esi
        cmp    dword ptr [0x20fc], esi       ; -> obj1:0x20FC
        jne    loc_1_0B11
        mov    eax, 0xea60
        mov    esi, 0xba5                    ; -> loc_1_0BA5
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x20fc], esi       ; -> obj1:0x20FC
        jmp    loc_1_0B11
loc_1_0B37:
        pushal 
        mov    eax, 4
        cmp    ecx, eax
        ja     loc_1_0B43
        mov    ecx, eax
loc_1_0B43:
        mov    edi, ecx
        mov    eax, ecx
        dec    eax
        bsr    cx, ax
        inc    cl
        mov    eax, 1
        shl    eax, cl
        dec    eax
        and    eax, 0x1f
        push   edx
        mov    edx, esp
        push   0xe
        push   edx
        push   dword ptr [0x2100]            ; -> obj1:0x2100
        push   0
        push   eax
        push   0
        push   1
        push   edi
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        test   eax, eax
        je     loc_1_0B9C
        pop    dword ptr [esi + 4]
        mov    dword ptr [esi], edi
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [esi + 8], eax
        test   edx, edx
        je     loc_1_0B94
        push   0
        push   edx
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
loc_1_0B94:
        popal  
        mov    eax, esi
        jmp    loc_1_0AF4
loc_1_0B9C:
        pop    ecx
        popal  
        mov    al, dl
        jmp    loc_1_0AF0
loc_1_0BA5:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        xor    ecx, ecx
        xor    esi, esi
        mov    ebx, 0x2054                   ; -> obj1:0x2054
loc_1_0BB4:
        cmp    dword ptr [ebx], ecx
        je     loc_1_0BE9
        test   byte ptr [ebx + 0xc], 1
        jne    loc_1_0BE8
        mov    edx, eax
        sub    edx, dword ptr [ebx + 0x14]
        cmp    edx, 0x1d4c0
        jb     loc_1_0BE8
        pushal 
        mov    edi, ebx
        push   0
        push   dword ptr [ebx + 8]
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
        mov    ecx, 6
        xor    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        popal  
        jmp    loc_1_0BE9
loc_1_0BE8:
        inc    esi
loc_1_0BE9:
        add    ebx, 0x18
        cmp    ebx, 0x20fc                   ; -> obj1:0x20FC
        jb     loc_1_0BB4
        test   esi, esi
        je     loc_1_0C08
        mov    eax, 0xea60
        mov    esi, 0xba5                    ; -> loc_1_0BA5
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
loc_1_0C08:
        mov    dword ptr [0x20fc], esi       ; -> obj1:0x20FC
        ret    
VDMAD_Release_Buffer:
        push   eax
        push   ebx
        movzx  ebx, bl
        dec    ebx
        test   ebx, 7
        jne    loc_1_0C3E
        add    ebx, 0x203c                   ; -> obj1:0x203C
        xor    eax, eax
        test   byte ptr [ebx + 0xc], 1
        je     loc_1_0C3E
        mov    dword ptr [ebx + 0xc], eax
        mov    dword ptr [ebx + 0x10], eax
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [ebx + 0x14], eax
        clc    
loc_1_0C3B:
        pop    ebx
        pop    eax
        ret    
loc_1_0C3E:
        stc    
        jmp    loc_1_0C3B
sub_1_0C41:
        bt     dword ptr [esi + 6], ecx
        jb     loc_1_0C4D
        bt     dword ptr [esi + 5], ecx
        jb     loc_1_0CBE
loc_1_0C4D:
        call   sub_1_02DA
        mov    edi, eax
        call   sub_1_067C
        test   byte ptr [edi + 0x30], 0x10
        jne    loc_1_0CA9
sub_1_0C5F:
        mov    edx, dword ptr [edi + 0x28]
        push   eax
        inc    edx
        inc    eax
        mov    ebx, 0xffffff
        cmp    byte ptr [0x211a], 2          ; -> loc_1_211A
        je     loc_1_0C76
        shr    ebx, 8
loc_1_0C76:
        and    eax, ebx
        and    edx, ebx
        pop    ebx
        cmp    eax, edx
        jae    loc_1_0CBE
        mov    dword ptr [edi + 0x28], ebx
        test   byte ptr [edi + 0x31], 0x40
        je     loc_1_0C8C
        shl    eax, 1
        shl    edx, 1
loc_1_0C8C:
        add    dword ptr [edi + 0x20], edx
        sub    dword ptr [edi + 0x20], eax
        cmp    byte ptr [edi + 0x15], 0
        je     loc_1_0CBE
        test   byte ptr [edi + 0x30], 4
        je     loc_1_0CBE
        push   ecx
        push   esi
        call   sub_1_0CBF
        pop    esi
        pop    ecx
        jmp    loc_1_0CBE
loc_1_0CA9:
        mov    dword ptr [edi + 0x28], eax
        sub    eax, dword ptr [edi + 0x2c]
        neg    eax
        bt     dword ptr [esi], ecx
        jae    loc_1_0CB8
        shl    eax, 1
loc_1_0CB8:
        add    eax, dword ptr [edi + 0x24]
        mov    dword ptr [edi + 0x20], eax
loc_1_0CBE:
        ret    
sub_1_0CBF:
        movzx  ebx, byte ptr [edi + 0x15]
        mov    esi, dword ptr [edi + 0xc]
        mov    edi, dword ptr [edi + 0x10]
        sub    edi, edx
        mov    ecx, edx
        sub    ecx, eax
        VxDCall  VDMAD.VDMAD_Copy_From_Buffer    ; cd2011000400
        jae    loc_1_0CD7
loc_1_0CD7:
        ret    
sub_1_0CD8:
        movzx  eax, bl
        dec    eax
        test   eax, 7
        jne    loc_1_0CFD
        add    eax, 0x203c                   ; -> obj1:0x203C
        push   edi
        add    edi, ecx
        jb     loc_1_0CF8
        dec    edi
        shr    edi, 0xc
        cmp    edi, dword ptr [eax]
        jae    loc_1_0CF8
        pop    edi
        clc    
        ret    
loc_1_0CF8:
        pop    edi
        mov    al, 0xb
        stc    
        ret    
loc_1_0CFD:
        mov    al, 0xa
        stc    
        ret    
VDMAD_Copy_To_Buffer:
        call   sub_1_0CD8
        jb     loc_1_0D2B
        pushal 
        add    edi, dword ptr [eax + 8]
        jmp    loc_1_0D1B
VDMAD_Copy_From_Buffer:
        call   sub_1_0CD8
        jb     loc_1_0D2B
        pushal 
        xchg   edi, esi
        add    esi, dword ptr [eax + 8]
loc_1_0D1B:
        cld    
        mov    eax, ecx
        shr    ecx, 2
loc_1_0D21:
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_0D2A:
        popal  
loc_1_0D2B:
        ret    
VDMAD_Get_EISA_Adr_Mode:
        push   eax
        cmp    eax, 7
        jbe    loc_1_0D34
        mov    eax, dword ptr [eax]
loc_1_0D34:
        mov    cl, byte ptr [eax + 0x1d78]   ; -> obj1:0x1D78
        and    cl, 0xc
        shr    cl, 2
        pop    eax
        ret    
VDMAD_Set_EISA_Adr_Mode:
        cmp    byte ptr [0x211a], 2          ; -> loc_1_211A
        jne    loc_1_0D77
        push   eax
        push   ecx
        cmp    eax, 7
        jbe    loc_1_0D54
        mov    eax, dword ptr [eax]
loc_1_0D54:
        shl    cl, 2
        and    cl, 0xc
        mov    byte ptr [eax + 0x1d78], cl   ; -> obj1:0x1D78
        imul   eax, eax, 0x34
        add    eax, 0x1e6c                   ; -> obj1:0x1E6C
        mov    byte ptr [eax + 0x31], 0
        cmp    cl, 4
        jne    loc_1_0D75
        mov    byte ptr [eax + 0x31], 0x40
loc_1_0D75:
        pop    ecx
        pop    eax
loc_1_0D77:
        ret    
loc_1_0D78:
        jmp    loc_1_210C
loc_1_0D7E:
        push   eax
        pushfd 
        cli    
        call   sub_1_0324
        and    al, byte ptr [0x2013]         ; -> loc_1_2013
        and    ah, byte ptr [0x201b]         ; -> loc_1_201B
        or     al, ah
        je     loc_1_0DB6
        cmp    dword ptr [0x2114], 0         ; -> loc_1_2114
        jne    loc_1_0DB6
        push   esi
        push   edx
        mov    esi, 0xdbe                    ; -> loc_1_0DBE
        xor    edx, edx
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    dword ptr [0x2114], esi       ; -> loc_1_2114
        pop    edx
        pop    esi
loc_1_0DB6:
        popfd  
        pop    eax
        jmp    loc_1_2108
loc_1_0DBE:
        pushfd 
        cli    
        xor    eax, eax
        mov    dword ptr [0x2114], eax       ; -> loc_1_2114
        xchg   byte ptr [0x2118], al         ; -> loc_1_2118
        xchg   byte ptr [0x2119], ah         ; -> obj1:0x2119
        popfd  
        push   eax
        mov    esi, 0x200c                   ; -> obj1:0x200C
        call   sub_1_0DE9
        pop    eax
        mov    al, ah
        mov    esi, 0x2014                   ; -> obj1:0x2014
        jmp    sub_1_0DE9
sub_1_0DE9:
        and    al, byte ptr [esi + 7]
        je     loc_1_0E2C
        movzx  eax, al
loc_1_0DF1:
        bsf    ecx, eax
        je     loc_1_0E2C
        btr    eax, ecx
        push   eax
        call   sub_1_02DA
        bts    dword ptr [esi + 5], ecx
        cmp    ebx, dword ptr [eax + 8]
        jne    loc_1_0E10
        call   sub_1_0FE9
        pop    eax
        jmp    loc_1_0DF1
loc_1_0E10:
        pushal 
        mov    edx, eax
        mov    eax, 0x400000
        mov    ebx, dword ptr [edx + 8]
        xor    ecx, ecx
        mov    esi, 0xfe7                    ; -> loc_1_0FE7
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        popal  
        pop    eax
        jmp    loc_1_0DF1
loc_1_0E2C:
        ret    
loc_1_0E2D:
        VxDCall  VDMAD.VDMAD_Default_Handler     ; cd2012800400
VDMAD_Default_Handler:
        mov    ecx, dword ptr [eax]
        VxDCall  VDMAD.VDMAD_Get_Virt_State      ; cd2004000400
        test   dl, 2
        jne    loc_1_0E49
        test   dl, 1
        jne    loc_1_0F0C
loc_1_0E49:
        cmp    byte ptr [eax + 0x14], 0
        jne    loc_1_0E55
        cmp    byte ptr [eax + 0x15], 0
        je     loc_1_0E7C
loc_1_0E55:
        cmp    esi, dword ptr [eax + 0xc]
        jne    loc_1_0E63
        cmp    ecx, dword ptr [eax + 0x10]
        je     loc_1_0EF9
loc_1_0E63:
        cmp    byte ptr [eax + 0x15], 0
        jne    loc_1_0E77
        test   byte ptr [eax + 0x30], 8
        je     loc_1_0E70
        stc    
loc_1_0E70:
        call   sub_1_0F64
        jmp    loc_1_0E7C
loc_1_0E77:
        call   sub_1_0FCC
loc_1_0E7C:
        mov    edi, dword ptr [eax]
        bt     dword ptr [0x2110], edi       ; -> loc_1_2110
        jb     loc_1_0EC9
        mov    edi, esi
        sub    edi, dword ptr [ebx + 4]
        cmp    edi, 0xa0000
        jae    loc_1_0EC9
        push   eax
        lea    edi, [ecx + edi]
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        shl    eax, 0xc
        cmp    edi, eax
        pop    eax
        jbe    loc_1_0EC9
        test   byte ptr [eax + 0x30], 0x10
        je     loc_1_0ED0
        call   sub_1_0F12
        jae    loc_1_0EF9
        call   sub_7_0000
        jb     loc_1_0EC1
        call   sub_1_0F12
        jae    loc_1_0EF9
loc_1_0EC1:
        mov    ecx, dword ptr [eax]
        VMMCall  VMM.Crash_Cur_VM                ; cd202f000100
loc_1_0EC9:
        call   sub_1_0F12
        jae    loc_1_0EF9
loc_1_0ED0:
        call   sub_1_0F9A
        test   dl, 8
        je     loc_1_0EEC
        pushal 
        xor    edi, edi
        VxDCall  VDMAD.VDMAD_Get_Region_Info     ; cd2002000400
        VxDCall  VDMAD.VDMAD_Copy_To_Buffer      ; cd2010000400
        popal  
        jmp    loc_1_0EF9
loc_1_0EEC:
        push   eax
        movzx  eax, byte ptr [eax + 0x15]
        or     dword ptr [eax + 0x2047], 2   ; -> loc_1_2047
        pop    eax
loc_1_0EF9:
        VxDCall  VDMAD.VDMAD_Set_Phys_State      ; cd2006000400
        test   dl, 1
        jne    loc_1_0F11
        VxDCall  VDMAD.VDMAD_UnMask_Channel      ; cd2008000400
        jmp    loc_1_0F11
loc_1_0F0C:
        call   sub_1_0FE9
loc_1_0F11:
        ret    
sub_1_0F12:
        pushal 
        mov    edx, dword ptr [eax]
        bt     dword ptr [0x2110], edx       ; -> loc_1_2110
        mov    edx, esi
        jb     loc_1_0F56
        xor    edx, edx
        test   byte ptr [0x211a], 3          ; -> loc_1_211A
        jne    loc_1_0F36
        mov    edx, dword ptr [eax]
        cmp    dl, 4
        mov    dl, 1
        jb     loc_1_0F36
        shl    edx, 1
loc_1_0F36:
        VxDCall  VDMAD.VDMAD_Lock_DMA_Region     ; cd2009000400
        jb     loc_1_0F62
        push   ecx
        add    ecx, edx
        shr    ecx, 0xc
        cmp    ecx, dword ptr [0x2100]       ; -> obj1:0x2100
        pop    ecx
        jb     loc_1_0F56
        VxDCall  VDMAD.VDMAD_Unlock_DMA_Region   ; cd200a000400
        stc    
        jmp    loc_1_0F62
loc_1_0F56:
        mov    ebx, 0xff00
        VxDCall  VDMAD.VDMAD_Set_Region_Info     ; cd2003000400
        clc    
loc_1_0F62:
        popal  
        ret    
sub_1_0F64:
        pushal 
        pushfd 
        mov    edx, dword ptr [eax]
        bt     dword ptr [0x2110], edx       ; -> loc_1_2110
        jb     loc_1_0F81
        VxDCall  VDMAD.VDMAD_Get_Region_Info     ; cd2002000400
        popfd  
        pushfd 
        jb     loc_1_0F92
        VxDCall  VDMAD.VDMAD_Unlock_DMA_Region   ; cd200a000400
loc_1_0F81:
        popfd  
        xor    ebx, ebx
        xor    esi, esi
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VDMAD.VDMAD_Set_Region_Info     ; cd2003000400
        popal  
        ret    
loc_1_0F92:
        VxDCall  VDMAD.VDMAD_Unlock_DMA_Region_No_Dirty ; cd2017000400
        jmp    loc_1_0F81
sub_1_0F9A:
        pushal 
loc_1_0F9B:
        VxDCall  VDMAD.VDMAD_Request_Buffer      ; cd200e000400
        jae    loc_1_0FC0
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        cmp    al, 5
        je     loc_7_0091
        push   eax
        mov    eax, 0x64
        VMMCall  VMM.Time_Slice_Sleep            ; cd2061010100
        pop    eax
        jmp    loc_1_0F9B
loc_1_0FC0:
        mov    eax, dword ptr [esp + 0x1c]
        VxDCall  VDMAD.VDMAD_Set_Region_Info     ; cd2003000400
        popal  
        ret    
sub_1_0FCC:
        pushal 
        movzx  ebx, byte ptr [eax + 0x15]
        VxDCall  VDMAD.VDMAD_Release_Buffer      ; cd200f000400
        xor    ebx, ebx
        xor    esi, esi
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VDMAD.VDMAD_Set_Region_Info     ; cd2003000400
        popal  
        ret    
loc_1_0FE7:
        mov    eax, edx
sub_1_0FE9:
        cmp    dword ptr [eax + 8], 0
        je     loc_1_1055
        push   edx
        mov    edx, 0xffffff
        cmp    byte ptr [0x211a], 2          ; -> loc_1_211A
        je     loc_1_1001
        shr    edx, 8
loc_1_1001:
        cmp    byte ptr [eax + 0x15], 0
        je     loc_1_1039
        pushal 
        movzx  ecx, byte ptr [eax + 0x15]
        test   byte ptr [ecx + 0x2047], 2    ; -> loc_1_2047
        je     loc_1_102E
        mov    ecx, dword ptr [eax]
        mov    esi, 0x200c                   ; -> obj1:0x200C
        btr    ecx, 2
        jae    loc_1_1025
        add    esi, 8
loc_1_1025:
        mov    edi, eax
        mov    eax, edx
        call   sub_1_0C5F
loc_1_102E:
        popal  
        mov    dword ptr [eax + 0x28], edx
        call   sub_1_0FCC
        jmp    loc_1_104E
loc_1_1039:
        mov    dword ptr [eax + 0x28], edx
        cmp    byte ptr [eax + 0x14], 0
        je     loc_1_104E
        test   byte ptr [eax + 0x30], 8
        je     loc_1_1049
        stc    
loc_1_1049:
        call   sub_1_0F64
loc_1_104E:
        VxDCall  VDMAD.VDMAD_Mask_Channel        ; cd2007000400
        pop    edx
loc_1_1055:
        ret    
        db  000h, 000h
loc_1_1058:
        mov    al, byte ptr [0x2126]         ; -> loc_1_2126
        mov    byte ptr [0x47b], al
        mov    eax, dword ptr [0x2128]       ; -> obj1:0x2128
        inc    eax
        je     loc_1_1070
        dec    eax
        mov    dword ptr [0x12c], eax
loc_1_1070:
        ret    
loc_1_1071:
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ah, 0x81
        jne    loc_1_1081
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        jmp    loc_1_10D4
loc_1_1081:
        test   byte ptr [0x2127], 8          ; -> obj1:0x2127
        jne    loc_1_1090
        VMMCall  VMM.Simulate_Iret               ; cd2047800100
loc_1_1090:
        movzx  ecx, word ptr [0x2124]        ; -> loc_1_2124
        mov    edx, dword ptr [0x2120]       ; -> loc_1_2120
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_1_10A3:
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ah, 0x81
        jne    loc_1_10B3
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        jmp    loc_1_10D4
loc_1_10B3:
        test   byte ptr [0x2127], 8          ; -> obj1:0x2127
        jne    loc_1_10C2
        VMMCall  VMM.Simulate_Iret               ; cd2047800100
loc_1_10C2:
        mov    ecx, dword ptr [0x2128]       ; -> obj1:0x2128
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_1_10D4:
        cmp    al, 0xc
        ja     loc_1_10EE
        sub    al, 2
        jb     loc_1_10EE
        movzx  eax, al
        shl    eax, 2
        and    word ptr [ebp + 0x2c], 0xfffe
        jmp    loc_1_10F8
loc_1_10EE:
        or     word ptr [ebp + 0x2c], 1
        mov    byte ptr [ebp + 0x1c], 0xf
        ret    
loc_1_10F8:
        xor    al, 0x11
        add    byte ptr [eax], al
        lea    edx, [ecx]
        add    byte ptr [eax], al
        pushal 
        adc    al, byte ptr [eax]
        add    byte ptr [edx + 0x25000012], dh ; -> loc_1_12B2
        adc    eax, dword ptr [eax]
        add    byte ptr [eax + 0x13], ch     ; -> loc_1_1368
        add    byte ptr [eax], al
        lodsb  al, byte ptr [esi]
        adc    eax, dword ptr [eax]
        add    dh, ch                        ; -> loc_1_13EE
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], ch            ; -> loc_1_1428
        adc    al, 0
        add    byte ptr [edx + 0x14], ah     ; -> loc_1_1462
        add    byte ptr [eax], al
        xchg   edx, eax
        adc    al, 0
        add    dh, al
        inc    ebp
        sbb    al, 0x10
loc_1_1128:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_1_112E:
        mov    byte ptr [ebp + 0x1c], 7
        jmp    loc_1_1128
loc_1_1134:
        test   word ptr [ebp + 0x14], 0xffff
        jne    0x1124
        mov    word ptr [ebp + 0x1c], 0x100
        mov    word ptr [ebp + 0x10], 3
        mov    word ptr [ebp + 0x18], 0x200
        mov    edx, dword ptr [0x203c]       ; -> obj1:0x203C
        mov    eax, 0x1fe00
        shl    edx, 0xc
        cmp    eax, edx
        ja     loc_1_1162
        mov    eax, edx
loc_1_1162:
        mov    word ptr [ebp], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 4], ax
        xor    eax, eax
        cmp    byte ptr [0x211a], 4          ; -> loc_1_211A
        jne    loc_1_1179
        inc    eax
loc_1_1179:
        cmp    dword ptr [0x2100], 0x100     ; -> obj1:0x2100
        ja     loc_1_1187
        or     al, 2
loc_1_1187:
        mov    word ptr [ebp + 0x14], ax
        clc    
        ret    
loc_1_118D:
        test   word ptr [ebp + 0x14], 0xffc1
        jne    0x1124
        call   sub_1_14C2
        jb     loc_1_112E
        mov    ecx, dword ptr [edi]
        jecxz  loc_1_11FF
        test   dword ptr [ebx], 0x20
        jne    loc_1_11DC
        movzx  eax, word ptr [edi + 8]
        shl    eax, 4
        add    eax, dword ptr [edi + 4]
        mov    edx, eax
        shr    edx, 0xc
        cmp    dx, 0xa0
        jae    loc_1_11DC
        push   eax
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        cmp    edx, eax
        pop    eax
        jbe    loc_1_11DC
        mov    edx, eax
        add    eax, ecx
        cmp    eax, 0xa0000
        jbe    loc_1_11DC
        mov    ecx, 0xa0000
        sub    ecx, edx
loc_1_11DC:
        mov    word ptr [edi + 0xa], 0
        mov    dword ptr [edi + 0xc], 0
        mov    dl, byte ptr [ebp + 0x14]
        and    dl, 0x30
        shr    dl, 4
        VxDCall  VDMAD.VDMAD_Lock_DMA_Region     ; cd2009000400
        jb     loc_1_122D
loc_1_11FA:
        mov    dword ptr [edi + 0xc], edx
        jmp    loc_1_125E
loc_1_11FF:
        push   eax
        mov    edx, esp
        mov    ecx, esi
        shr    ecx, 0xc
        push   0
        push   edx
        push   1
        push   ecx
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        pop    edx
        xor    ecx, ecx
        test   dl, 1
        je     loc_1_122D
        and    dx, 0xf000
        and    esi, 0xfff
        or     edx, esi
        jmp    loc_1_11FA
loc_1_122D:
        xchg   dword ptr [edi], ecx
        test   byte ptr [ebp + 0x14], 4
        jne    loc_1_1256
        VxDCall  VDMAD.VDMAD_Request_Buffer      ; cd200e000400
        jb     loc_1_1256
        mov    dword ptr [edi], ecx
        mov    word ptr [edi + 0xa], bx
        mov    dword ptr [edi + 0xc], edx
        test   byte ptr [ebp + 0x14], 2
        je     loc_1_125E
        xor    edi, edi
        VxDCall  VDMAD.VDMAD_Copy_To_Buffer      ; cd2010000400
        jae    loc_1_125E
loc_1_1256:
        or     word ptr [ebp + 0x2c], 1
        mov    byte ptr [ebp + 0x1c], al
loc_1_125E:
        clc    
        ret    
loc_1_1260:
        test   word ptr [ebp + 0x14], 0xfffd
        jne    0x1124
        call   sub_1_14C2
        jb     loc_1_112E
        mov    ecx, dword ptr [edi]
        movzx  ebx, word ptr [edi + 0xa]
        or     ebx, ebx
        je     loc_1_129F
        test   byte ptr [ebp + 0x14], 2
        je     loc_1_1291
        xor    edi, edi
        VxDCall  VDMAD.VDMAD_Copy_From_Buffer    ; cd2011000400
        jb     loc_1_1299
loc_1_1291:
        VxDCall  VDMAD.VDMAD_Release_Buffer      ; cd200f000400
        jae    loc_1_12B0
loc_1_1299:
        mov    byte ptr [ebp + 0x1c], 0xa
        jmp    loc_1_12AB
loc_1_129F:
        VxDCall  VDMAD.VDMAD_Unlock_DMA_Region   ; cd200a000400
        jae    loc_1_12B0
        mov    byte ptr [ebp + 0x1c], 8
loc_1_12AB:
        or     word ptr [ebp + 0x2c], 1
loc_1_12B0:
        clc    
        ret    
loc_1_12B2:
        test   word ptr [ebp + 0x14], 0xff3f
        jne    0x1124
        call   sub_1_14C2
        jb     loc_1_112E
        xchg   dword ptr [edi + 4], esi
        xor    ecx, ecx
        xchg   word ptr [edi + 8], cx
        mov    al, byte ptr [ebp + 0x14]
        and    al, 0xc0
        shr    al, 6
        push   esi
        VxDCall  VDMAD.VDMAD_Scatter_Lock        ; cd200b000400
        jbe    loc_1_12F0
        or     al, 4
        VxDCall  VDMAD.VDMAD_Scatter_Unlock      ; cd200c000400
        mov    word ptr [edi + 0xe], dx
        stc    
loc_1_12F0:
        pop    dword ptr [edi + 4]
        mov    word ptr [edi + 8], cx
        jb     loc_1_130C
        test   word ptr [ebp + 0x14], 0x40
        je     loc_1_1323
        and    si, 0xfff
        mov    word ptr [ebp + 0x10], si
        jmp    loc_1_1323
loc_1_130C:
        or     word ptr [ebp + 0x2c], 1
        movzx  edx, word ptr [edi + 0xe]
        mov    byte ptr [ebp + 0x1c], 3
        cmp    dx, word ptr [edi + 0xc]
        jbe    loc_1_1323
        mov    byte ptr [ebp + 0x1c], 9
loc_1_1323:
        clc    
        ret    
loc_1_1325:
        test   word ptr [ebp + 0x14], 0xff3f
        jne    0x1124
        call   sub_1_14C2
        jb     loc_1_112E
        xchg   dword ptr [edi + 4], esi
        xor    cx, cx
        xchg   word ptr [edi + 8], cx
        mov    al, byte ptr [ebp + 0x14]
        and    al, 0xc0
        shr    al, 6
        VxDCall  VDMAD.VDMAD_Scatter_Unlock      ; cd200c000400
        mov    word ptr [edi + 8], cx
        mov    dword ptr [edi + 4], esi
        jae    loc_1_1366
        or     word ptr [ebp + 0x2c], 1
        mov    byte ptr [ebp + 0x1c], 8
loc_1_1366:
        clc    
        ret    
loc_1_1368:
        test   word ptr [ebp + 0x14], 0xfffd
        jne    0x1124
        call   sub_1_14C2
        jb     loc_1_112E
        mov    ecx, dword ptr [edi]
        VxDCall  VDMAD.VDMAD_Request_Buffer      ; cd200e000400
        jb     loc_1_13A2
        mov    word ptr [edi + 0xa], bx
        mov    dword ptr [edi + 0xc], edx
        test   word ptr [ebp + 0x14], 2
        je     loc_1_13AA
        xor    edi, edi
        VxDCall  VDMAD.VDMAD_Copy_To_Buffer      ; cd2010000400
        jae    loc_1_13AA
loc_1_13A2:
        or     word ptr [ebp + 0x2c], 1
        mov    byte ptr [ebp + 0x1c], al
loc_1_13AA:
        clc    
        ret    
loc_1_13AC:
        test   word ptr [ebp + 0x14], 0xfffd
        jne    0x1124
        call   sub_1_14C2
        jb     loc_1_112E
        movzx  ebx, word ptr [edi + 0xa]
        test   word ptr [ebp + 0x14], 2
        je     loc_1_13DB
        mov    ecx, dword ptr [edi]
        xor    edi, edi
        VxDCall  VDMAD.VDMAD_Copy_From_Buffer    ; cd2011000400
        jb     loc_1_13E3
loc_1_13DB:
        VxDCall  VDMAD.VDMAD_Release_Buffer      ; cd200f000400
        jae    loc_1_13EC
loc_1_13E3:
        or     word ptr [ebp + 0x2c], 1
        mov    byte ptr [ebp + 0x1c], 0xa
loc_1_13EC:
        clc    
        ret    
loc_1_13EE:
        test   word ptr [ebp + 0x14], 0xffff
        jne    0x1124
        call   sub_1_14C2
        jb     loc_1_112E
        movzx  ebx, word ptr [edi + 0xa]
        mov    ecx, dword ptr [edi]
        mov    di, word ptr [ebp + 0x10]
        shl    edi, 0x10
        mov    di, word ptr [ebp + 0x18]
        VxDCall  VDMAD.VDMAD_Copy_To_Buffer      ; cd2010000400
        jae    loc_1_1426
        or     word ptr [ebp + 0x2c], 1
        mov    byte ptr [ebp + 0x1c], al
loc_1_1426:
        clc    
        ret    
loc_1_1428:
        test   word ptr [ebp + 0x14], 0xffff
        jne    0x1124
        call   sub_1_14C2
        jb     loc_1_112E
        movzx  ebx, word ptr [edi + 0xa]
        mov    ecx, dword ptr [edi]
        mov    di, word ptr [ebp + 0x10]
        shl    edi, 0x10
        mov    di, word ptr [ebp + 0x18]
        VxDCall  VDMAD.VDMAD_Copy_From_Buffer    ; cd2011000400
        jae    loc_1_1460
        or     word ptr [ebp + 0x2c], 1
        mov    byte ptr [ebp + 0x1c], al
loc_1_1460:
        clc    
        ret    
loc_1_1462:
        test   word ptr [ebp + 0x14], 0xffff
        jne    0x1124
        movzx  eax, word ptr [ebp + 0x10]
        call   sub_1_02CA
        jb     loc_1_1487
        VxDCall  VDMAD.VDMAD_Disable_Translation ; cd2013000400
        jae    loc_1_1490
        mov    byte ptr [ebp + 0x1c], 0xd
        jmp    loc_1_148B
loc_1_1487:
        mov    byte ptr [ebp + 0x1c], 0xc
loc_1_148B:
        or     word ptr [ebp + 0x2c], 1
loc_1_1490:
        clc    
        ret    
loc_1_1492:
        test   word ptr [ebp + 0x14], 0xffff
        jne    0x1124
        movzx  eax, word ptr [ebp + 0x10]
        call   sub_1_02CA
        jb     loc_1_14B7
        VxDCall  VDMAD.VDMAD_Enable_Translation  ; cd2014000400
        jae    loc_1_14C0
        mov    byte ptr [ebp + 0x1c], 0xe
        jmp    loc_1_14BB
loc_1_14B7:
        mov    byte ptr [ebp + 0x1c], 0xc
loc_1_14BB:
        or     word ptr [ebp + 0x2c], 1
loc_1_14C0:
        clc    
        ret    
sub_1_14C2:
        xchg   edi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        mov    esi, dword ptr [edi + 4]
        movzx  eax, word ptr [edi + 8]
        or     eax, eax
        je     loc_1_1506
        test   dword ptr [ebp + 0x2c], 0x20000
        jne    loc_1_14F0
        push   ecx
        push   esi
        add    esi, dword ptr [edi]
        dec    esi
        lsl    ecx, eax
        cmp    ecx, esi
        pop    esi
        pop    ecx
        jb     loc_1_1507
loc_1_14F0:
        push   dword ptr [ebp + 0x38]
        mov    word ptr [ebp + 0x38], ax
        mov    ax, 0x38ff
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        add    esi, eax
        pop    dword ptr [ebp + 0x38]
loc_1_1506:
        clc    
loc_1_1507:
        ret    
sub_1_1508:
        mov    esi, 0x200c                   ; -> obj1:0x200C
        cmp    dx, 0xc0
        jb     loc_1_1517
        add    esi, 8
loc_1_1517:
        cmc    
        ret    
sub_1_1519:
        mov    esi, 0x200c                   ; -> obj1:0x200C
        push   eax
        mov    edi, 0x212c                   ; -> loc_1_212C
        mov    ecx, 8
        mov    al, dl
        cld    
        repne scasb al, byte ptr es:[edi]
        pop    eax
        jne    loc_1_1545
        sub    edi, 0x212d                   ; -> obj1:0x212D
        mov    ecx, edi
        cmp    cl, 4
        jb     loc_1_1544
        sub    cl, 4
        add    esi, 8
loc_1_1544:
        ret    
loc_1_1545:
        ret    
sub_1_1546:
        mov    esi, 0x200c                   ; -> obj1:0x200C
        mov    cx, dx
        cmp    cx, 0xf
        jbe    loc_1_155F
        sub    cx, 0xc0
        shr    cx, 1
        add    esi, 8
loc_1_155F:
        shr    cl, 1
        cmp    cl, 4
        jb     loc_1_1568
        xor    ecx, ecx
loc_1_1568:
        ret    
sub_1_1569:
        mov    edi, dword ptr [0x1e68]       ; -> obj1:0x1E68
        lea    edi, [ebx + edi + 8]
        cmp    esi, 0x200c                   ; -> obj1:0x200C
        je     loc_1_157C
        inc    edi
loc_1_157C:
        ret    
loc_1_157D:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_158E
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_158E:
        je     loc_1_159B
        call   sub_1_1508
        xor    al, al
        xchg   byte ptr [esi + 4], al
        ret    
loc_1_159B:
        or     al, al
        je     loc_1_15A5
        VMMCall  VMM.Crash_Cur_VM                ; cd202f000100
loc_1_15A5:
        ret    
loc_1_15A6:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_15B7
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_15B7:
        jb     loc_1_1912
        call   sub_1_1508
        xor    al, al
        mov    byte ptr [esi + 4], al
        mov    byte ptr [esi + 6], al
        call   sub_1_1569
        mov    byte ptr [edi], al
        xor    ecx, ecx
        call   sub_1_02DA
        mov    cl, 4
loc_1_15DA:
        mov    byte ptr [eax + 0x30], 0x40
        and    byte ptr [eax + 0x31], 0x40
        add    eax, 0x34
        loop   loc_1_15DA
        movzx  eax, byte ptr [esi + 5]
        mov    byte ptr [esi + 5], 0xf
        call   sub_1_1914
        ret    
loc_1_15F5:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_1606
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_1606:
        jb     loc_1_1912
        call   sub_1_1508
        xchg   byte ptr [esi + 5], al
        movzx  eax, al
        call   sub_1_1914
        ret    
loc_1_161D:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_162E
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_162E:
        jb     loc_1_1912
        call   sub_1_1508
        movzx  eax, byte ptr [esi + 5]
        mov    byte ptr [esi + 5], 0
        call   sub_1_1914
        ret    
loc_1_1647:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_1658
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_1658:
        jb     loc_1_1912
        mov    ah, al
        and    ah, 0xc0
        test   al, 0x20
        je     loc_1_166D
        VMMCall  VMM.Crash_Cur_VM                ; cd202f000100
loc_1_166D:
        xor    ah, ah
        call   sub_1_1508
        movzx  ecx, al
        and    ecx, 3
        xchg   edi, eax
        call   sub_1_02DA
        xchg   edi, eax
        and    al, 0xfc
        mov    byte ptr [edi + 0x30], al
        mov    ah, byte ptr [edi + 0x31]
        bt     dword ptr [esi], ecx
        jae    loc_1_1693
        or     ah, 0x40
        jmp    loc_1_1696
loc_1_1693:
        and    ah, 0xbf
loc_1_1696:
        and    ah, 0xf2
        test   al, 4
        je     loc_1_16A2
        or     ah, 0xc
        jmp    loc_1_16A9
loc_1_16A2:
        test   al, 8
        je     loc_1_16A9
        or     ah, 4
loc_1_16A9:
        mov    dl, al
        and    dl, 0x10
        shr    dl, 3
        and    ah, 0xfd
        or     ah, dl
        mov    byte ptr [edi + 0x31], ah
        xor    edx, edx
        jmp    sub_1_1932
loc_1_16C0:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_16D1
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_16D1:
        jb     loc_1_1912
        call   sub_1_1508
        movzx  ecx, al
        and    cl, 3
        shr    al, 2
        and    al, 1
        mov    ah, 0xfe
        shl    al, cl
        rol    ah, cl
        and    ah, byte ptr [esi + 6]
        or     al, ah
        mov    byte ptr [esi + 6], al
        or     edx, 1
        jmp    sub_1_1932
loc_1_16FD:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_170E
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_170E:
        jb     loc_1_1912
        call   sub_1_1508
        movzx  ecx, al
        and    cl, 3
        shr    al, 2
        and    al, 1
        mov    ah, 0xfe
        shl    al, cl
        rol    ah, cl
        and    ah, byte ptr [esi + 5]
        or     al, ah
        mov    byte ptr [esi + 5], al
        or     edx, 1
        jmp    sub_1_1932
loc_1_173A:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_174B
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_174B:
        jb     loc_1_1912
        call   sub_1_1508
        call   sub_1_1569
        mov    byte ptr [edi], 0
        ret    
sub_1_175F:
        pushal 
        mov    bl, ah
        movzx  eax, al
        mov    edx, 0xffffff00
        bt     dword ptr [esi], ecx
        jae    loc_1_1788
        cmp    bl, 2
        je     loc_1_1783
        ja     loc_1_1788
        shl    eax, 1
        rol    edx, 1
        or     bl, bl
        jne    loc_1_1788
        and    dl, 0xfe
        jmp    loc_1_1788
loc_1_1783:
        and    al, 0xfe
        or     dl, 1
loc_1_1788:
        push   eax
        call   sub_1_02DA
        lea    esi, [eax + 0x20]
        pop    eax
        mov    cl, bl
        shl    cl, 3
        shl    eax, cl
        rol    edx, cl
        and    edx, dword ptr [esi]
        or     edx, eax
        cmp    bl, 3
        je     loc_1_17B3
        cmp    byte ptr [0x211a], 2          ; -> loc_1_211A
        jne    loc_1_17B3
        and    edx, 0xffffff
loc_1_17B3:
        mov    dword ptr [esi], edx
        mov    dword ptr [esi + 4], edx
        popal  
        ret    
loc_1_17BA:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_17CB
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_17CB:
        jb     loc_1_17EC
        call   sub_1_1546
        call   sub_1_1569
        mov    ah, byte ptr [edi]
        call   sub_1_175F
        xor    ah, 1
        mov    byte ptr [edi], ah
        jne    loc_1_1835
        xor    edx, edx
        jmp    sub_1_1932
loc_1_17EC:
        call   sub_1_1546
        call   sub_1_1569
        inc    edx
        cmp    edx, 0xf
        jbe    loc_1_17FD
        inc    edx
loc_1_17FD:
        cmp    byte ptr [edi], 0
        jne    loc_1_180B
        push   ebx
        push   edi
        call   sub_1_0C41
        pop    edi
        pop    ebx
loc_1_180B:
        call   sub_1_02DA
        test   word ptr [eax + 0x1a], 1
        je     loc_1_1822
        mov    edx, 3
        call   sub_1_1932
loc_1_1822:
        lea    edx, [eax + 0x20]
        movzx  eax, byte ptr [edi]
        xor    byte ptr [edi], 1
        mov    eax, dword ptr [edx + eax]
        bt     dword ptr [esi], ecx
        jae    loc_1_1835
        shr    eax, 1
loc_1_1835:
        ret    
loc_1_1836:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_1847
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_1847:
        jb     loc_1_1889
        call   sub_1_1546
        call   sub_1_1569
        push   eax
        call   sub_1_02DA
        lea    edx, [eax + 0x28]
        pop    eax
        push   ecx
        movzx  ecx, byte ptr [edi]
        xor    byte ptr [edi], 1
        mov    byte ptr [ecx + edx], al
        mov    byte ptr [ecx + edx + 4], al
        cmp    byte ptr [0x211a], 2          ; -> loc_1_211A
        jne    loc_1_187C
        mov    byte ptr [edx + 2], 0
        mov    byte ptr [edx + 6], 0
loc_1_187C:
        pop    ecx
        cmp    byte ptr [edi], 0
        jne    loc_1_18C4
        xor    edx, edx
        jmp    sub_1_1932
loc_1_1889:
        call   sub_1_1546
        call   sub_1_1569
        cmp    byte ptr [edi], 0
        jne    loc_1_18A1
        push   ebx
        push   edi
        call   sub_1_0C41
        pop    edi
        pop    ebx
loc_1_18A1:
        call   sub_1_02DA
        test   word ptr [eax + 0x1a], 1
        je     loc_1_18B8
        mov    edx, 1
        call   sub_1_1932
loc_1_18B8:
        lea    edx, [eax + 0x28]
        movzx  ecx, byte ptr [edi]
        xor    byte ptr [edi], 1
        mov    al, byte ptr [edx + ecx]
loc_1_18C4:
        ret    
loc_1_18C5:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_18D6
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_18D6:
        je     loc_1_1902
        call   sub_1_1519
        call   sub_1_02DA
        test   word ptr [eax + 0x1a], 1
        je     loc_1_18F6
        push   edx
        mov    edx, 2
        call   sub_1_1932
        pop    edx
loc_1_18F6:
        movzx  eax, word ptr [eax + 0x22]
        bt     dword ptr [esi], ecx
        jae    loc_1_1901
        and    al, 0xfe
loc_1_1901:
        ret    
loc_1_1902:
        call   sub_1_1519
        mov    ah, 2
        call   sub_1_175F
        xor    edx, edx
        jmp    sub_1_1932
loc_1_1912:
        in     al, dx
        ret    
sub_1_1914:
        movzx  edx, byte ptr [esi + 5]
        xor    eax, edx
        mov    edx, 1
        mov    ecx, 3
loc_1_1924:
        bt     ax, cx
        jae    loc_1_192F
        call   sub_1_1932
loc_1_192F:
        loop   loc_1_1924
        ret    
sub_1_1932:
        pushal 
        call   sub_1_02DA
        cmp    dword ptr [eax + 4], 0
        je     loc_1_1950
        or     edx, edx
        jne    loc_1_194B
        movzx  edx, byte ptr [esi + 5]
        bt     edx, ecx
        jb     loc_1_1950
loc_1_194B:
        xor    ecx, ecx
        call   dword ptr [eax + 4]
loc_1_1950:
        popal  
        ret    
loc_1_1952:
        pushfd 
        push   eax
        cli    
        call   sub_1_0324
        pop    eax
        popfd  
        ret    
loc_1_195D:
        pushfd 
        pushal 
        pushfd 
        cli    
        call   sub_1_0324
        xor    eax, eax
        xchg   byte ptr [0x2118], al         ; -> loc_1_2118
        xchg   byte ptr [0x2119], ah         ; -> obj1:0x2119
        popfd  
        push   eax
        mov    esi, 0x200c                   ; -> obj1:0x200C
        call   sub_1_0DE9
        pop    eax
        mov    al, ah
        mov    esi, 0x2014                   ; -> obj1:0x2014
        call   sub_1_0DE9
        popal  
        popfd  
        ret    
sub_1_1990:
        or     al, 0x70
        out    0x18, al
        jmp    loc_1_1996
loc_1_1996:
        in     al, 0x1a
        mov    byte ptr [esi + 0x31], al
        push   ecx
        mov    cl, al
        and    cl, 2
        shl    cl, 3
        and    ah, 0xef
        or     ah, cl
        pop    ecx
        test   al, 4
        je     loc_1_19BA
        test   al, 8
        jne    loc_1_19B7
        or     ah, 8
        jmp    loc_1_19BA
loc_1_19B7:
        or     ah, 4
loc_1_19BA:
        mov    byte ptr [esi + 0x30], ah
        ret    
loc_1_19BE:
        mov    ecx, 8
        xor    edi, edi
        mov    esi, 0x1e6c                   ; -> obj1:0x1E6C
loc_1_19CA:
        mov    eax, edi
        call   sub_1_1990
        mov    eax, edi
        or     al, 0x30
        out    0x18, al
        jmp    loc_1_19D9
loc_1_19D9:
        in     al, 0x1a
        ror    eax, 8
        in     al, 0x1a
        ror    eax, 8
        in     al, 0x1a
        ror    eax, 0x10
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0x24], eax
        mov    eax, edi
        or     al, 0x50
        out    0x18, al
        jmp    loc_1_19F6
loc_1_19F6:
        in     al, 0x1a
        shl    ax, 8
        in     al, 0x1a
        xchg   al, ah
        mov    dword ptr [esi + 0x28], eax
        mov    dword ptr [esi + 0x2c], eax
        inc    edi
        add    esi, 0x34
        loop   loc_1_19CA
        clc    
        ret    
loc_1_1A0E:
        pushal 
        mov    ch, dl
        mov    cl, dh
        and    cl, 2
        shl    cl, 3
        and    ch, 0xef
        or     ch, cl
        and    ch, 0xf3
        test   dh, 4
        je     loc_1_1A36
        test   dh, 8
        jne    loc_1_1A30
        or     ch, 8
        jmp    loc_1_1A33
loc_1_1A30:
        or     ch, 4
loc_1_1A33:
        and    ch, 0xff
loc_1_1A36:
        mov    dl, ch
        mov    edi, eax
        mov    ebx, edx
        mov    eax, dword ptr [edi]
        or     al, 0x20
        out    0x18, al
        jmp    loc_1_1A44
loc_1_1A44:
        movzx  eax, word ptr [edi + 0x18]
        out    0x1a, al
        jmp    loc_1_1A4C
loc_1_1A4C:
        mov    al, ah
        out    0x1a, al
        jmp    loc_1_1A52
loc_1_1A52:
        mov    ax, word ptr [edi + 0x16]
        out    0x1a, al
        jmp    loc_1_1A5A
loc_1_1A5A:
        mov    eax, dword ptr [edi]
        mov    edx, eax
        and    al, 3
        and    bl, 0xfc
        or     al, bl
        and    edx, 4
        shl    edx, 2
        movzx  edx, byte ptr [edx + 0x2020]  ; -> obj1:0x2020
        out    dx, al
        jmp    loc_1_1A75
loc_1_1A75:
        mov    eax, dword ptr [edi]
        or     al, 0x70
        out    0x18, al
        jmp    loc_1_1A7D
loc_1_1A7D:
        mov    al, bh
        out    0x1a, al
        mov    edx, dword ptr [edi + 0x10]
        dec    edx
        test   bh, 0x40
        je     loc_1_1A8C
        shr    edx, 1
loc_1_1A8C:
        mov    eax, dword ptr [edi]
        or     al, 0x40
        out    0x18, al
        jmp    loc_1_1A94
loc_1_1A94:
        mov    eax, edx
        out    0x1a, al
        jmp    loc_1_1A9A
loc_1_1A9A:
        mov    al, ah
        out    0x1a, al
        jmp    loc_1_1AA0
loc_1_1AA0:
        test   bh, 1
        je     loc_1_1ABD
        mov    eax, dword ptr [edi]
        movzx  ebx, word ptr [edi + 0x32]
        or     al, 0
        out    0x18, al
        jmp    loc_1_1AB1
loc_1_1AB1:
        mov    eax, ebx
        out    0x1a, al
        jmp    loc_1_1AB7
loc_1_1AB7:
        mov    al, ah
        out    0x1a, al
        jmp    loc_1_1ABD
loc_1_1ABD:
        popal  
        ret    
loc_1_1ABF:
        call   sub_1_2104
        cmp    ecx, 4
        jbe    loc_1_1AD0
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_1AD0:
        jb     loc_1_1912
        mov    esi, 0x200c                   ; -> obj1:0x200C
        movzx  edx, al
        and    eax, 7
        cmp    al, 4
        jb     loc_1_1AE8
        add    esi, 8
loc_1_1AE8:
        mov    ecx, edx
        and    ecx, 0xf0
        shr    ecx, 2
        jmp    loc_1_1AFA
loc_1_1AFA:
        inc    esp
        sbb    al, 0
        add    byte ptr [edx], bh            ; -> obj1:0x1B3A
        sbb    eax, dword ptr [eax]
        add    byte ptr [eax + 0x1c], bl     ; -> loc_1_1C58
        add    byte ptr [eax], al
        pop    esp
        sbb    al, 0
        add    byte ptr [edi + 0x1c], ch     ; -> loc_1_1C6F
        add    byte ptr [eax], al
        jae    0x1b2c
        add    byte ptr [eax], al
        xchg   byte ptr [eax + eax], bl
        add    byte ptr [esp + ebx + 0x1ca80000], dl ; -> loc_1_1C94
        add    byte ptr [eax], al
        mov    eax, 0xcb00001c               ; -> loc_1_1CCB
        sbb    al, 0
        add    byte ptr [edx], bh            ; -> obj1:0x1B3A
        sbb    eax, dword ptr [eax]
        add    byte ptr [edx], bh            ; -> obj1:0x1B3A
        sbb    eax, dword ptr [eax]
        add    dh, bl                        ; -> loc_1_1CDE
        sbb    al, 0
        add    byte ptr [edx], bh            ; -> obj1:0x1B3A
        sbb    eax, dword ptr [eax]
        add    byte ptr [edx], bh            ; -> obj1:0x1B3A
        sbb    eax, dword ptr [eax]
        add    bl, al
loc_1_1B3B:
        call   sub_1_2104
        mov    esi, ebx
        add    esi, dword ptr [0x1e68]       ; -> obj1:0x1E68
        cmp    ecx, 4
        jbe    loc_1_1B54
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_1B54:
        je     loc_1_1B80
        movzx  edx, byte ptr [esi + 4]
        and    edx, 0xf0
        cmp    edx, 0x80
        je     loc_1_1B74
        movzx  ebx, byte ptr [esi + 6]
        mov    al, byte ptr [esi + ebx]
        jmp    loc_1_1C1F
loc_1_1B74:
        mov    al, byte ptr [esi + 4]
        out    0x18, al
        in     al, 0x1a
        jmp    loc_1_1C2A
loc_1_1B80:
        cmp    byte ptr [esi + 5], 0xff
        jne    loc_1_1C2B
        movzx  edx, byte ptr [esi + 4]
        and    edx, 0xf0
        cmp    edx, 0x20
        je     loc_1_1C0C
        cmp    edx, 0x40
        je     loc_1_1BF7
        cmp    edx, 0
        je     loc_1_1BED
        cmp    edx, 0x70
        je     loc_1_1BBD
        cmp    edx, 0x80
        jne    loc_1_1C2B
        mov    ah, al
        mov    al, byte ptr [esi + 4]
        out    0x18, al
        mov    al, ah
        out    0x1a, al
        jmp    loc_1_1C2A
loc_1_1BBD:
        call   sub_1_1C2C
        mov    byte ptr [edi + 0x31], al
        mov    ah, byte ptr [edi + 0x30]
        mov    cl, al
        and    cl, 2
        shl    cl, 3
        and    ah, 0xef
        or     ah, cl
        and    ah, 0xf3
        test   al, 4
        je     loc_1_1BE8
        test   al, 8
        jne    loc_1_1BE5
        or     ah, 8
        jmp    loc_1_1BE8
loc_1_1BE5:
        or     ah, 4
loc_1_1BE8:
        mov    byte ptr [edi + 0x30], ah
        jmp    loc_1_1C2A
loc_1_1BED:
        call   sub_1_1C2C
        lea    edi, [edi + 0x32]
        jmp    loc_1_1BFF
loc_1_1BF7:
        call   sub_1_1C2C
        lea    edi, [edi + 0x28]
loc_1_1BFF:
        movzx  ebx, byte ptr [esi + 6]
        mov    byte ptr [edi + ebx], al
        mov    byte ptr [edi + ebx + 4], al
        jmp    loc_1_1C1F
loc_1_1C0C:
        call   sub_1_1C2C
        movzx  ebx, byte ptr [esi + 6]
        lea    edi, [edi + 0x20]
        mov    byte ptr [edi + ebx], al
        mov    byte ptr [edi + ebx + 4], al
loc_1_1C1F:
        inc    ebx
        cmp    bl, byte ptr [esi + 7]
        jb     loc_1_1C27
        xor    ebx, ebx
loc_1_1C27:
        mov    byte ptr [esi + 6], bl
loc_1_1C2A:
        ret    
loc_1_1C2B:
        ret    
sub_1_1C2C:
        push   eax
        mov    edi, ebx
        add    edi, dword ptr [0x1e68]       ; -> obj1:0x1E68
        movzx  eax, byte ptr [edi + 4]
        and    al, 7
        call   sub_1_02CA
        mov    edi, eax
        pop    eax
        ret    
loc_1_1C44:
        and    al, 3
        call   sub_1_02CA
        movzx  eax, word ptr [eax + 0x32]
        mov    cl, 2
        mov    ch, 0xff
        jmp    loc_1_1D4E
loc_1_1C58:
        mov    ch, 0xff
        jmp    loc_1_1C5E
loc_1_1C5C:
        mov    ch, 0
loc_1_1C5E:
        and    al, 3
        call   sub_1_02CA
        mov    eax, dword ptr [eax + 0x20]
        mov    cl, 3
        jmp    loc_1_1D4E
loc_1_1C6F:
        mov    ch, 0xff
        jmp    loc_1_1C75
loc_1_1C73:
        mov    ch, 0
loc_1_1C75:
        and    al, 3
        call   sub_1_02CA
        mov    eax, dword ptr [eax + 0x28]
        mov    cl, 2
        jmp    loc_1_1D4E
loc_1_1C86:
        xor    al, al
        xchg   byte ptr [esi + 4], al
        mov    cl, 1
        mov    ch, 0
        jmp    loc_1_1D4E
loc_1_1C94:
        and    al, 3
        call   sub_1_02CA
        movzx  eax, byte ptr [eax + 0x31]
        mov    cl, 1
        mov    ch, 0xff
        jmp    loc_1_1D4E
loc_1_1CA8:
        mov    esi, ebx
        add    esi, dword ptr [0x1e68]       ; -> obj1:0x1E68
        mov    byte ptr [esi + 4], dl
        mov    byte ptr [esi + 5], 0xff
        ret    
loc_1_1CB8:
        mov    ecx, eax
        and    cl, 3
        bts    dword ptr [esi + 5], ecx
        mov    edx, 1
        jmp    sub_1_1932
loc_1_1CCB:
        mov    ecx, eax
        and    cl, 3
        btr    dword ptr [esi + 5], ecx
        mov    edx, 1
        jmp    sub_1_1932
loc_1_1CDE:
        push   esi
        mov    esi, ebx
        add    esi, dword ptr [0x1e68]       ; -> obj1:0x1E68
        xor    eax, eax
        mov    byte ptr [esi + 4], al
        mov    byte ptr [esi + 5], al
        mov    byte ptr [esi + 6], al
        mov    byte ptr [esi + 7], al
        mov    dword ptr [esi], eax
        mov    word ptr [esi + 8], ax
        pop    esi
        mov    byte ptr [0x2010], al         ; -> obj1:0x2010
        mov    byte ptr [0x2018], al         ; -> obj1:0x2018
        mov    byte ptr [0x2012], al         ; -> obj1:0x2012
        mov    byte ptr [0x201a], al         ; -> obj1:0x201A
        mov    ecx, 8
        mov    eax, 0x1e6c                   ; -> obj1:0x1E6C
loc_1_1D1A:
        mov    byte ptr [eax + 0x30], 0x40
        mov    byte ptr [eax + 0x31], 0
        cmp    cl, 4
        ja     loc_1_1D2B
        mov    byte ptr [eax + 0x31], 0x40
loc_1_1D2B:
        add    eax, 0x34
        loop   loc_1_1D1A
        movzx  eax, byte ptr [esi + 5]
        mov    byte ptr [esi + 5], 0xf
        call   sub_1_1914
        add    esi, 8
        movzx  eax, byte ptr [esi + 5]
        mov    byte ptr [esi + 5], 0xf
        call   sub_1_1914
        ret    
loc_1_1D4E:
        mov    esi, ebx
        add    esi, dword ptr [0x1e68]       ; -> obj1:0x1E68
        mov    byte ptr [esi + 4], dl
        mov    byte ptr [esi + 5], ch
        mov    byte ptr [esi + 6], 0
        mov    byte ptr [esi + 7], cl
        mov    dword ptr [esi], eax
        ret    
        db  000h, 000h
sub_1_1D68:
        cmp    eax, 8
        jae    loc_1_1D74
        bts    dword ptr [0x211b], eax       ; -> obj1:0x211B
loc_1_1D74:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 004h, 004h, 004h
        dd  VDMAD_Get_Version        ; fixup
        dd  VDMAD_Virtualize_Channel ; fixup
        dd  VDMAD_Get_Region_Info    ; fixup
        dd  VDMAD_Set_Region_Info    ; fixup
        dd  VDMAD_Get_Virt_State     ; fixup
        dd  VDMAD_Set_Virt_State     ; fixup
        dd  VDMAD_Set_Phys_State     ; fixup
        dd  VDMAD_Mask_Channel       ; fixup
        dd  VDMAD_UnMask_Channel     ; fixup
        dd  VDMAD_Lock_DMA_Region    ; fixup
        dd  VDMAD_Unlock_DMA_Region  ; fixup
        dd  VDMAD_Scatter_Lock       ; fixup
        dd  VDMAD_Scatter_Unlock     ; fixup
        dd  VDMAD_Reserve_Buffer_Space ; fixup
        dd  VDMAD_Request_Buffer     ; fixup
        dd  VDMAD_Release_Buffer     ; fixup
        dd  VDMAD_Copy_To_Buffer     ; fixup
        dd  VDMAD_Copy_From_Buffer   ; fixup
        dd  VDMAD_Default_Handler    ; fixup
        dd  VDMAD_Disable_Translation ; fixup
        dd  VDMAD_Enable_Translation ; fixup
        dd  VDMAD_Get_EISA_Adr_Mode  ; fixup
        dd  VDMAD_Set_EISA_Adr_Mode  ; fixup
        dd  VDMAD_Unlock_DMA_Region_No_Dirty ; fixup
        dd  VDMAD_Phys_Mask_Channel  ; fixup
        dd  VDMAD_Phys_Unmask_Channel ; fixup
        dd  VDMAD_Unvirtualize_Channel ; fixup
        dd  VDMAD_Set_IO_Address     ; fixup
        dd  VDMAD_Get_Phys_Count     ; fixup
        dd  VDMAD_Get_Phys_Status    ; fixup
        dd  VDMAD_Get_Max_Phys_Page  ; fixup
        dd  VDMAD_Set_Channel_Callbacks ; fixup
        dd  VDMAD_Get_Virt_Count     ; fixup
        dd  VDMAD_Set_Virt_Count     ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00004h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VDMAD   '                  ; DDB_Name
        dd  090000000h                  ; DDB_Init_Order
        dd  VDMAD_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x1D80              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
        dd  loc_1_0D21               ; fixup
        dd  loc_1_0D2A               ; fixup
        dd  loc_1_0D2A               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_1_0E2D               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  loc_1_0E2D               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  loc_1_0E2D               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 000h, 000h, 000h
        dd  loc_1_0E2D               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1F3C:
        add    al, 0
        add    byte ptr [eax], al
loc_1_1F40:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, 0x2d000000               ; -> loc_1_0E2D
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [0xe], ch            ; -> loc_1_0E2D
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [0xe], ch            ; -> loc_1_0E2D
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2011:
        add    byte ptr [eax], al
loc_1_2013:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2019:
        add    byte ptr [eax], al
loc_1_201B:
        add    byte ptr [esi + ebx], ch      ; -> obj1:0x1E6C
        add    byte ptr [ebx], cl
loc_1_2021:
        or     cl, byte ptr [ecx]
loc_1_2023:
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi + ebx], bh      ; -> loc_1_1F3C
        add    byte ptr [eax], al
        salc   
        aam    0xd2
        fadd   dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_203B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2047:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_204B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        db  000h
sub_1_2104:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2108:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_210C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2110:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2114:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2118:
        add    byte ptr [eax], al
loc_1_211A:
        add    byte ptr [eax], al
sub_1_211C:
        fadd   dword ptr [edi]
        add    byte ptr [eax], al
loc_1_2120:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2124:
        add    byte ptr [eax], al
loc_1_2126:
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_212C:
        xchg   dword ptr [ebx - 0x74707d7f], eax
        mov    dword ptr [edx + 0x6040200], ecx
        rol    ah, 0xc8
        int3   
loc_1_213C:
        add    dword ptr [ebx], eax
        add    eax, 0xcac6c207
        into   
loc_1_2144:
        xchg   dword ptr [ebx + eax*4], eax
        add    al, 0x81
        add    al, 0x82
        add    al, 0
        add    byte ptr [ebx - 0x75fb76fc], cl
        add    al, 1
        add    al, 3
        add    al, 5
        add    al, 7
        add    al, 0
        add    dh, al
        add    al, 0xca
        add    al, 0xce
        db  004h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x8a  R-X
loc_2_0000:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jp     loc_2_0010
loc_2_0010:
        push   esp
        push   0x4d442065
        inc    ecx
        and    byte ptr [edx + 0x75], ah
        jb     loc_2_003F
        imul   esi, dword ptr [ebx + 0x20], 0x206f6f74
        jae    0x95
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        or     eax, 0x7465530a
        and    byte ptr [edx], ah
        inc    esp
        dec    ebp
        inc    ecx
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        push   ebx
        dec    ecx
        pop    edx
loc_2_003F:
        inc    ebp
        cmp    eax, 0x20227525
        imul   ebp, dword ptr [esi + 0x20], 0x54535953
        inc    ebp
        dec    ebp
        dec    ecx
        dec    esi
        dec    ecx
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ebx + 0x33], bl
        cmp    byte ptr [esi], dh
        inc    ebp
        outsb  dx, byte ptr [esi]
        push   0x6573205d
        arpl   word ptr [ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        or     eax, 0x610a0d0a
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[edx + 0x65], dh
        jae    0xea
        popal  
        jb     0xed
        and    byte ptr [ecx + 0x6f], bh
        jne    0xf0
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        jo     0xf9
        je     0xeb
        jb     0xb6
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x7cd  R-X
        db  068h
        dd  loc_6_0318               ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        dd  loc_6_0326               ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 068h
        dd  loc_6_032A               ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 0BFh
        dd  obj1:0x203C              ; fixup
        db  068h
        dd  loc_6_0374               ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 08Bh, 007h, 085h, 0C0h, 075h, 00Dh, 068h
        dd  loc_6_037C               ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 0EBh, 025h, 08Bh, 05Fh, 004h, 08Bh, 04Fh, 008h, 08Bh, 057h
        db  014h, 068h
        dd  loc_6_038C               ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 08Bh, 047h, 010h, 08Bh, 05Fh, 00Ch, 068h
        dd  obj6:0x03A9              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 083h, 0C7h, 018h, 081h, 0FFh
        dd  obj1:0x20FC              ; fixup
        db  072h, 0B2h, 068h
        dd  obj6:0x03B7              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 0A1h
        dd  obj1:0x1E68              ; fixup
        db  068h
        dd  obj6:0x03BB              ; fixup
        db  0CDh, 020h, 0F3h, 000h, 001h, 000h, 0A1h
        dd  loc_1_210C               ; fixup
        db  08Bh, 01Dh
        dd  loc_1_2108               ; fixup
        db  03Bh, 0C3h
loc_3_009C:
        je     loc_3_00D6
        push   0x3d5                         ; -> obj6:0x03D5
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        push   0x3ef                         ; -> obj6:0x03EF
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    al, byte ptr [0x2118]         ; -> loc_1_2118
        push   0x409                         ; -> obj6:0x0409
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    al, byte ptr [0x2119]         ; -> obj1:0x2119
        push   0x422                         ; -> loc_6_0422
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        jmp    loc_3_00E1
loc_3_00D6:
        push   0x43b                         ; -> obj6:0x043B
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_3_00E1:
        push   0x462                         ; -> loc_6_0462
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        push   0x466                         ; -> loc_6_0466
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        VMMCall  VMM.In_Debug_Chr                ; cd20c4000100
        je     loc_3_028E
        push   0x49b                         ; -> obj6:0x049B
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    al, byte ptr [0x2010]         ; -> obj1:0x2010
        mov    bl, byte ptr [0x2018]         ; -> obj1:0x2018
        push   0x4c5                         ; -> obj6:0x04C5
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    al, byte ptr [0x2011]         ; -> loc_1_2011
        mov    bl, byte ptr [0x2019]         ; -> loc_1_2019
        push   0x4f1                         ; -> obj6:0x04F1
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    al, byte ptr [0x2012]         ; -> obj1:0x2012
        mov    bl, byte ptr [0x201a]         ; -> obj1:0x201A
        push   0x51d                         ; -> obj6:0x051D
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        push   0x549                         ; -> obj6:0x0549
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    ecx, 4
        xor    eax, eax
loc_3_0162:
        push   ecx
        push   0x58a                         ; -> loc_6_058A
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        push   eax
        call   sub_1_02CA
        mov    ebx, dword ptr [eax + 0x20]
        mov    edi, dword ptr [eax + 0x28]
        mov    dl, byte ptr [eax + 0x30]
        mov    cl, byte ptr [eax + 0x31]
        push   0x599                         ; -> obj6:0x0599
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        pop    eax
        push   eax
        add    al, 4
        call   sub_1_02CA
        mov    ebx, dword ptr [eax + 0x20]
        mov    edi, dword ptr [eax + 0x28]
        mov    dl, byte ptr [eax + 0x30]
        mov    cl, byte ptr [eax + 0x31]
        pop    eax
        push   0x5ab                         ; -> obj6:0x05AB
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        inc    eax
        pop    ecx
        dec    ecx
        jecxz  loc_3_01B3
        jmp    loc_3_0162
loc_3_01B3:
        push   0x5c1                         ; -> obj6:0x05C1
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        push   0x5c5                         ; -> obj6:0x05C5
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        VMMCall  VMM.In_Debug_Chr                ; cd20c4000100
        je     loc_3_028E
        mov    esi, 0x1e6c                   ; -> obj1:0x1E6C
        mov    ecx, 8
loc_3_01DF:
        call   sub_3_028F
        add    esi, 0x34
        loop   loc_3_01DF
        push   0x5f7                         ; -> obj6:0x05F7
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        push   0x5fb                         ; -> obj6:0x05FB
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        VMMCall  VMM.In_Debug_Chr                ; cd20c4000100
        je     loc_3_028E
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        push   0x62b                         ; -> obj6:0x062B
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        push   0x64e                         ; -> loc_6_064E
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    esi, ebx
        add    esi, dword ptr [0x1e68]       ; -> obj1:0x1E68
        mov    al, byte ptr [esi + 8]
        mov    bl, byte ptr [esi + 9]
        push   0x652                         ; -> loc_6_0652
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        cmp    byte ptr [0x211a], 1          ; -> loc_1_211A
        jne    loc_3_028E
        mov    al, byte ptr [esi + 4]
        push   0x677                         ; -> obj6:0x0677
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    al, byte ptr [esi + 5]
        push   0x690                         ; -> loc_6_0690
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    al, byte ptr [esi + 6]
        push   0x6a9                         ; -> obj6:0x06A9
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    al, byte ptr [esi + 7]
        push   0x6c2                         ; -> loc_6_06C2
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [esi]
        push   0x6db                         ; -> obj6:0x06DB
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_3_028E:
        ret    
sub_3_028F:
        xor    ebx, ebx
        mov    eax, dword ptr [esi]
        push   0x6f5                         ; -> obj6:0x06F5
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_3_02B2
        push   0x71e                         ; -> loc_6_071E
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        jmp    loc_3_02BD
loc_3_02B2:
        push   0x725                         ; -> obj6:0x0725
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_3_02BD:
        movzx  eax, byte ptr [esi + 0x14]
        or     eax, eax
        je     loc_3_02D2
        push   0x72c                         ; -> loc_6_072C
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        jmp    loc_3_02E5
loc_3_02D2:
        movzx  eax, byte ptr [esi + 0x15]
        or     eax, eax
        je     loc_3_0317
        push   0x740                         ; -> loc_6_0740
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_3_02E5:
        mov    eax, dword ptr [esi + 0xc]
        mov    ebx, dword ptr [esi + 0x10]
VDMAD_Reserve_Buffer_Space:
        push   0x757                         ; -> obj6:0x0757
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    ax, word ptr [esi + 0x16]
        mov    bx, word ptr [esi + 0x18]
        push   0x783                         ; -> obj6:0x0783
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        mov    eax, dword ptr [esi + 8]
        push   0x7b1                         ; -> obj6:0x07B1
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_3_0317:
        ret    
        db  056h, 044h, 04Dh, 041h
loc_3_031C:
        inc    esp
        and    byte ptr [ebx + 0x74], dh
        popal  
        je     0x388
        or     eax, 0xd20000a
        or     al, byte ptr [eax]
        and    byte ptr [eax + 0x61], cl
        outsb  dx, byte ptr [esi]
        insb   byte ptr es:[edi], dx
        and    byte ptr gs:[eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0x20202020
        and    byte ptr [eax + 0x68], dl
        jns    loc_3_03B7
        imul   esp, dword ptr [ebx + 0x61], 0x2020206c
        and    byte ptr [ecx + ebp*2 + 0x6e], cl
        popal  
        jb     0x373
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx + ebp*2 + 0x6d], dl
        and    byte ptr gs:[eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [edi + 0x77], cl
        outsb  dx, byte ptr [esi]
        jb     0x387
        push   esi
        dec    ebp
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 0x74], dl
        popal  
        je     loc_3_03D6
        or     eax, 0x6523000a
        imul   esp, dword ptr fs:[eax], 0x28002020
        push   ebp
        outsb  dx, byte ptr [esi]
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ecx + 0x74], sp
        sub    dword ptr fs:[0x6523000a], ecx
        popal  
        js     0x3b1
        and    byte ptr [eax], ah
        and    esp, dword ptr [ebp + 0x62]
        js     0x3b8
        and    byte ptr [eax], ah
        and    esp, dword ptr [ebp + 0x63]
        js     0x3bf
        and    byte ptr [eax], ah
        and    esp, dword ptr [ebp + 0x64]
        js     0x3c6
        and    byte ptr [eax], ah
        add    byte ptr [ebx], ah
        popal  
        js     0x3ce
        and    byte ptr [eax], ah
        and    esp, dword ptr [ebp + 0x62]
        js     0x3c2
        or     al, byte ptr [eax]
loc_3_03B7:
        and    byte ptr [0x4243000a], cl
        and    byte ptr [edi + 0x66], ch
        jae    0x428
        je     loc_3_03E5
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        popal  
        js     0x3e0
        or     al, byte ptr [eax]
        dec    eax
loc_3_03D6:
        ja     0x3f8
        dec    ecx
        outsb  dx, byte ptr [esi]
        je     0x3fc
        imul   bp, word ptr [esp + esi*2 + 0x65], 0x2072
        and    byte ptr [eax], ah
loc_3_03E5:
        and    byte ptr [eax], ah
        and    byte ptr [edi], bh
        popal  
        js     0x3fa
        or     al, byte ptr [eax]
        dec    esi
        js     loc_3_0467
        and    byte ptr [eax + 0x77], cl
        and    byte ptr [ecx + 0x6e], cl
        je     0x41b
        imul   bp, word ptr [esp + esi*2 + 0x65], 0x2072
        aas    
        bound  edi, qword ptr gs:[eax + 0xd]
        or     al, byte ptr [eax]
        inc    esi
        imul   esi, dword ptr [edx + 0x73], 0x4d442074
        inc    ecx
        and    byte ptr [ebx + 0x74], dh
        popal  
        je     loc_3_048D
        jae    0x43a
        and    byte ptr [eax], ah
        and    esp, dword ptr [ecx + 0x6c]
        or     eax, 0x6553000a
        arpl   word ptr [edi + 0x6e], bp
        and    byte ptr fs:[ebp + ecx*2 + 0x41], al
        and    byte ptr [ebx + 0x74], dh
        popal  
        je     0x4a7
        jae    loc_3_0454
        and    byte ptr [ebx], ah
        popal  
        insb   byte ptr es:[edi], dx
        or     eax, 0x5056000a
        dec    ecx
        inc    ebx
        inc    esp
        and    byte ptr [eax + 0x57], cl
        and    byte ptr [ecx + 0x4e], cl
        push   esp
        and    byte ptr [esi + 0x6f], ch
        je     0x4b5
        imul   sp, word ptr [ebx + 0x61], 0x6974
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
loc_3_0454:
        and    byte ptr [esi + 0x6f], ch
        je     0x479
        push   0x656b6f6f
        or     eax, 0xd20000a
        or     al, byte ptr [eax]
        inc    ebp
loc_3_0467:
        push   ebx
        inc    ebx
        and    byte ptr [edi + ebp*2 + 0x20], dh
        jno    0x4e4
        imul   esi, dword ptr [esp + ebp + 0x20], 0x6120726f
        outsb  dx, byte ptr [esi]
        jns    0x49a
        arpl   word ptr [eax + 0x61], bp
        jb     0x49f
        je     0x4f0
        and    byte ptr [ebx + 0x65], dh
        and    byte ptr gs:[ebx + 0x6f], ah
        outsb  dx, byte ptr [esi]
        je     loc_3_04FD
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
loc_3_048D:
        insb   byte ptr es:[edi], dx
        jb     0x4b1
        jae    loc_3_0507
        popal  
        je     loc_3_04FB
        jae    loc_3_04D2
        or     eax, 0x6f43000a
        outsb  dx, byte ptr [esi]
        je     0x512
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        jb     0x4c6
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ecx], dh
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [edx], dh
        or     eax, 0x7473000a
        popal  
        je     loc_3_053F
        jae    0x4ec
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_3_04D2:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    esp, dword ptr [ecx + 0x6c]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    esp, dword ptr [edx + 0x6c]
        or     eax, 0x616d000a
        jae    0x560
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_3_04FB:
        and    byte ptr [eax], ah
loc_3_04FD:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        popal  
        insb   byte ptr es:[edi], dx
loc_3_0507:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    esp, dword ptr [edx + 0x6c]
        or     eax, 0x6572000a
        jno    0x596
        jae    0x598
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_3_052C:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    esp, dword ptr [ecx + 0x6c]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_3_053F:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    esp, dword ptr [edx + 0x6c]
        or     eax, 0x2020000a
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        popal  
        jb     0x57d
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        arpl   word ptr [esi + 0x74], bp
        and    byte ptr [eax], ah
        and    byte ptr [ebp + 0x6f], ch
        and    byte ptr fs:[ebp + 0x6d], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        popal  
        jb     loc_3_0597
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        arpl   word ptr [esi + 0x74], bp
        and    byte ptr [eax], ah
        and    byte ptr [ebp + 0x6f], ch
        and    byte ptr fs:[ebp + 0x6d], ah
        or     eax, 0x6863000a
        popal  
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebx], ah
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [eax], ah
loc_3_0597:
        and    byte ptr [eax], al
        and    esp, dword ptr [ebp + 0x62]
        js     loc_3_05BE
        and    esp, dword ptr [ebp + 0x64]
        imul   esp, dword ptr [eax], 0x206c6423
        and    esp, dword ptr [ebx + 0x6c]
        add    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        bound  edi, qword ptr gs:[eax + 0x20]
        and    esp, dword ptr [ebp + 0x64]
        imul   esp, dword ptr [eax], 0x206c6423
        and    esp, dword ptr [ebx + 0x6c]
loc_3_05BE:
        or     eax, 0xd20000a
        or     al, byte ptr [eax]
        inc    ebp
        push   ebx
        inc    ebx
        and    byte ptr [edi + ebp*2 + 0x20], dh
        jno    0x643
        imul   esi, dword ptr [esp + ebp + 0x20], 0x6120726f
        outsb  dx, byte ptr [esi]
        jns    loc_3_05F9
        arpl   word ptr [eax + 0x61], bp
        jb     loc_3_05FE
        je     0x64f
        and    byte ptr [ebx + 0x65], dh
        and    byte ptr gs:[ebx + 0x68], ah
        popal  
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebx + 0x74], dh
        popal  
        je     0x657
        jae    0x62e
        or     eax, 0xd20000a
loc_3_05F9:
        or     al, byte ptr [eax]
        inc    ebp
        push   ebx
        inc    ebx
loc_3_05FE:
        and    byte ptr [edi + ebp*2 + 0x20], dh
        jno    loc_3_0679
        imul   esi, dword ptr [esp + ebp + 0x20], 0x6120726f
        outsb  dx, byte ptr [esi]
        jns    loc_3_062F
        arpl   word ptr [eax + 0x61], bp
        jb     0x634
        je     loc_3_0685
        and    byte ptr [ebx + 0x65], dh
        and    byte ptr gs:[ebx + 0x75], al
        jb     0x675
        dec    ebp
loc_3_0620:
        and    byte ptr [ebx + 0x74], dh
        popal  
        je     0x68b
        jae    0x662
        or     eax, 0x6956000a
        jb     0x6a3
loc_3_062F:
        jne    loc_3_0692
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + ecx*2 + 0x41], al
        and    byte ptr [ebx + 0x74], dl
        popal  
        je     0x6a1
        and    byte ptr [esi + 0x6f], ah
        jb     loc_3_0661
        inc    ebx
        jne    loc_3_06B6
        push   esi
        dec    ebp
        and    byte ptr [ebx], ah
        inc    ebp
        inc    edx
        pop    eax
        or     eax, 0xd20000a
        or     al, byte ptr [eax]
        insb   byte ptr es:[edi], dx
        imul   esi, dword ptr [eax + 0x66], 0x3a706f6c
        and    byte ptr [eax], ah
        arpl   word ptr [edx + esi*2 + 0x6c], si
loc_3_0661:
        and    byte ptr [ecx], dh
        and    byte ptr [ebx], ah
        inc    ecx
        dec    esp
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 0x74], ah
        jb     0x6da
        and    byte ptr [edx], dh
        and    byte ptr [ebx], ah
        inc    edx
        dec    esp
        or     eax, 0x5350000a
loc_3_0679:
        xor    bh, byte ptr [edx]
        inc    esp
        dec    ebp
        inc    ecx
        pop    edi
        push   eax
        push   ebx
        xor    bl, byte ptr [edi + 0x63]
        insd   dword ptr es:[edi], dx
loc_3_0685:
        and    byte ptr fs:[eax], ah
        and    byte ptr [eax], ah
        and    esp, dword ptr [ecx + 0x6c]
        or     eax, 0x5350000a
loc_3_0692:
        xor    bh, byte ptr [edx]
        inc    esp
        dec    ebp
        inc    ecx
        pop    edi
        ja     0x70c
        imul   esi, dword ptr [ecx + ebp*2 + 0x6e], 0x20202067
        and    byte ptr [ebx], ah
        popal  
        insb   byte ptr es:[edi], dx
        or     eax, 0x5350000a
        xor    bh, byte ptr [edx]
        inc    esp
        dec    ebp
        inc    ecx
        pop    edi
        bound  edi, qword ptr [ecx + 0x74]
        push   eax
loc_3_06B6:
        je     0x72a
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    esp, dword ptr [ecx + 0x6c]
        or     eax, 0x5350000a
        xor    bh, byte ptr [edx]
        inc    esp
        dec    ebp
        inc    ecx
        pop    edi
        popal  
        je     0x72f
        inc    edx
        jns    0x745
        jae    0x6f4
        and    byte ptr [ebx], ah
        popal  
        insb   byte ptr es:[edi], dx
        or     eax, 0x5350000a
        xor    bh, byte ptr [edx]
        inc    esp
        dec    ebp
        inc    ecx
        pop    edi
        popal  
        je     0x748
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        popal  
        js     loc_3_0700
        or     al, byte ptr [eax]
        inc    esp
        dec    ebp
        inc    ecx
        and    byte ptr [ebx + 0x68], ah
        popal  
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        insb   byte ptr es:[edi], dx
loc_3_0700:
        and    byte ptr [ebx], ah
        and    esp, dword ptr [ecx + 0x6c]
        and    byte ptr [eax], ah
        insd   dword ptr es:[edi], dx
        popal  
        js     0x748
        and    esp, dword ptr [edx + 0x78]
        dec    ebx
        bound  esp, qword ptr [eax]
        and    byte ptr [ebx + 0x61], ah
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        bound  esp, qword ptr [ecx + 0x63]
        imul   esp, dword ptr [eax], 0x3d
        and    byte ptr [eax], al
        aas    
        inc    ebp
        inc    ecx
        pop    eax
        or     eax, 0x6f6e000a
        outsb  dx, byte ptr [esi]
        or     eax, 0x2020000a
        and    byte ptr [eax], ah
        jb     0x797
        imul   ebp, dword ptr [bx + 0x6e], 0x636f6c20
        imul   esp, dword ptr [ebp + 0x64], 0xd
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        bound  esi, qword ptr [ebp + 0x66]
        jb     loc_3_076B
        imul   esp, dword ptr [eax + 0x20], 0x61232020
        js     0x762
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        jb     0x7c2
        imul   ebp, dword ptr [bx + 0x6e], 0x72646120
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        popal  
loc_3_076B:
        js     loc_3_078D
        and    byte ptr [eax], ah
        jb     0x7d6
        imul   ebp, dword ptr [bx + 0x6e], 0x7a697320
        and    byte ptr gs:[eax], ah
        and    esp, dword ptr [ebp + 0x62]
        js     0x78e
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    esp
loc_3_0788:
        dec    ebp
        inc    ecx
        and    byte ptr [eax + 0x61], dh
loc_3_078D:
        and    byte ptr gs:[bx + si], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        popal  
        js     loc_3_07B8
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        inc    esp
        dec    ebp
        inc    ecx
        and    byte ptr [edx + 0x61], ah
        jae    0x80b
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        bound  edi, qword ptr [eax + 0xd]
        or     al, byte ptr [eax]
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        outsd  dx, dword ptr [esi]
        ja     0x826
loc_3_07B8:
        jb     0x7db
        push   esi
        dec    ebp
loc_3_07BD:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx], ah
        popal  
        js     0x7d4
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x10  R-X
VDMAD_Set_IO_Address:
        mov    edi, 0x52c                    ; -> loc_3_052C
        VMMCall  VMM.Install_Mult_IO_Handlers    ; cd2095000100
        xor    esi, esi
        db  0BFh
        dd  obj3:0x0729              ; fixup
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x30  RW-
        db  050h, 083h, 0E0h, 003h, 0E8h
        dd  sub_1_02CA               ; fixup
        db  066h, 089h, 050h, 032h, 058h, 0C3h, 000h
sub_5_0010:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_5_002C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x7cc  R-X
loc_6_0000:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    esi
        add    byte ptr [eax], al
        add    byte ptr [edx], al            ; -> loc_8_0202
        add    al, byte ptr [eax]
        add    byte ptr [edx - 0x40fffffe], dl ; -> loc_8_0292
        add    al, byte ptr [eax]
        add    bh, cl                        ; -> loc_8_02CF
        add    al, byte ptr [eax]
        add    dh, ch                        ; -> loc_8_02EE
        add    al, byte ptr [eax]
        add    byte ptr [edx + eax + 0x130000], al ; -> loc_8_0284
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_0318:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_0326:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_032A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_0374:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_037C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_038C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_0422:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_0462:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_0466:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_058A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_064E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_0652:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_0690:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_06C2:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_071E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_072C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_0740:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0xf0  R-X
sub_7_0000:
        sub    esp, 8
        pushal 
        sub    esi, dword ptr [ebx + 4]
        dec    ecx
        add    ecx, esi
        shr    esi, 0xc
        mov    dword ptr [esp + 0x20], esi
        shr    ecx, 0xc
        sub    ecx, esi
        inc    ecx
        mov    dword ptr [esp + 0x24], ecx
        mov    edx, ecx
        mov    eax, edx
        dec    eax
        bsr    cx, ax
        inc    cl
        mov    eax, 1
        shl    eax, cl
        dec    eax
        push   0xe
        push   0
        push   dword ptr [0x2100]            ; -> obj1:0x2100
        push   0
        push   eax
        push   0
        push   1
        push   edx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        test   eax, eax
        je     loc_7_008E
        mov    edi, eax
        mov    esi, dword ptr [esp + 0x20]
        shl    esi, 0xc
        mov    ecx, dword ptr [esp + 0x24]
        shl    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    esi, eax
        shr    eax, 0xc
        push   0
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x28]
        push   ebx
        push   eax
        VMMCall  VMM._LinMapIntoV86              ; cd2062000100
        add    esp, 0x14
        push   eax
        push   0
        push   esi
        VMMCall  VMM._PageFree                   ; cd2055000100
        pop    ecx
        pop    ecx
        pop    eax
        sub    eax, 1
loc_7_0089:
        popal  
        add    esp, 8
        ret    
loc_7_008E:
        stc    
        jmp    loc_7_0089
loc_7_0091:
        add    ecx, 0x3ff
        shr    ecx, 0xa
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_7_00D0
        push   0
        push   ecx
        push   dword ptr [0]                 ; -> obj5:0x0000
        VxDCall  CONFIGMG._CONFIGMG_Set_Private_Problem ; cd2050003300
        add    esp, 0xc
        mov    eax, 0x10009
        xor    edx, edx
        xor    ecx, ecx
        inc    ecx
        VMMCall  VMM.GetSetDetailedVMError       ; cd20e0000100
        VMMCall  VMM.End_Critical_Section        ; cd2020000100
        VMMCall  VMM.Crash_Cur_VM                ; cd202f000100
loc_7_00D0:
        push   ecx
        push   0xc0001
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        xchg   esi, eax
        pushal 
        mov    esi, esi
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 8  vsize=0x352  R-X
loc_8_0000:
        bts    word ptr [0xd], 1             ; -> obj5:0x000D
        jae    loc_8_0011
        mov    eax, 0x13
        ret    
loc_8_0011:
        mov    dword ptr [0], ebx            ; -> obj5:0x0000
        push   0
        push   0
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f003300
        add    esp, 0xc
        push   0
        push   0
        push   0
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e003300
        add    esp, 0x10
        mov    eax, 0
        ret    
loc_8_003B:
        push   ebx
        push   esi
        push   edi
        mov    eax, 1
        mov    esi, dword ptr [esp + 0x10]
        cmp    esi, 7
        ja     loc_8_005A
        mov    ecx, dword ptr [esp + 0x1c]
        call   sub_5_0010
loc_8_005A:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_8_005E:
        mov    ebx, dword ptr [ecx]
        call   sub_8_02F4
        mov    al, byte ptr [8]              ; -> obj5:0x0008
        mov    byte ptr [9], al              ; -> obj5:0x0009
        mov    al, byte ptr [0xb]            ; -> obj5:0x000B
        mov    byte ptr [0xc], al            ; -> obj5:0x000C
        call   sub_8_02F5
        mov    edx, dword ptr [ebx + 0xc]
        call   sub_8_0144
        clc    
        je     loc_8_00A1
        push   -1
        call   sub_8_00C0
        lea    esp, [esp + 4]
        jae    loc_8_00A1
        push   0
        call   sub_8_00C0
        lea    esp, [esp + 4]
loc_8_00A1:
        mov    dword ptr [0x2c], 0           ; -> loc_5_002C
        jae    loc_8_00B7
        mov    dword ptr [0x2c], 0x13        ; -> loc_5_002C
loc_8_00B7:
        mov    eax, dword ptr [0x2c]         ; -> loc_5_002C
        ret    
        db  02Eh, 08Bh, 0C0h
sub_8_00C0:
        enter  8, 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], eax
        mov    eax, 0xffffffff
        mov    dword ptr [ebp - 8], eax
loc_8_00D2:
        mov    eax, 0x80
loc_8_00D7:
        test   byte ptr [edi + 2], al
        je     loc_8_00EB
        call   sub_8_01A0
        jb     loc_8_00EB
        test   byte ptr [9], al              ; -> obj5:0x0009
        je     loc_8_0109
loc_8_00EB:
        shr    al, 1
        jne    loc_8_00D7
        test   dword ptr [ebp + 8], 0xffffffff
        jne    loc_8_0104
        xor    dword ptr [ebp - 4], 0xffffffff
        je     loc_8_00D2
        xor    dword ptr [ebp - 8], 0xffffffff
        je     loc_8_00D2
loc_8_0104:
        leave  
        stc    
        ret    
        db  0EBh, 0E2h
loc_8_0109:
        mov    ah, byte ptr [9]              ; -> obj5:0x0009
        or     byte ptr [9], al              ; -> obj5:0x0009
        pushal 
        call   sub_8_0144
        je     loc_8_0134
        push   dword ptr [ebp - 8]
        call   sub_8_00C0
        lea    esp, [esp + 4]
        jae    loc_8_0134
        popal  
        mov    byte ptr [9], ah              ; -> obj5:0x0009
        jmp    loc_8_00EB
loc_8_0134:
        popal  
        add    esp, 4
        bsf    eax, eax
        mov    byte ptr [edi + 1], al
        leave  
        clc    
        ret    
        db  02Eh, 08Bh, 0C0h
sub_8_0144:
        push   0
        push   0
        push   3
        push   edx
        push   4                             ; -> obj5:0x0004
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f003300
        add    esp, 0x14
        cmp    eax, 0
        jne    loc_8_0171
        mov    edi, dword ptr [4]            ; -> obj5:0x0004
        mov    edx, edi
        mov    al, byte ptr [edi + 2]
        call   sub_8_017C
        or     edi, edi
loc_8_0170:
        ret    
loc_8_0171:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     loc_8_0170
        mov    edx, dword ptr [ebx + 0xc]
        jmp    sub_8_0144
sub_8_017C:
        push   eax
        mov    ah, 0xaa
        and    ah, al
        xor    al, ah
        add    ah, ah
        adc    al, ah
        mov    ah, 0xcc
        and    ah, al
        xor    al, ah
        shr    ah, 2
        add    al, ah
        mov    ah, al
        shr    ah, 4
        and    al, 0xf
        add    al, ah
        movzx  ecx, al
        pop    eax
        ret    
sub_8_01A0:
        cmp    ecx, 1
        jbe    loc_8_01BD
        test   byte ptr [0x211b], al         ; -> obj1:0x211B
        je     loc_8_01B5
        cmp    dword ptr [ebp - 4], 0
        je     loc_8_01BB
loc_8_01B3:
        stc    
        ret    
loc_8_01B5:
        cmp    dword ptr [ebp - 4], 0
        je     loc_8_01B3
loc_8_01BB:
        clc    
        ret    
loc_8_01BD:
        test   byte ptr [0x211b], al         ; -> obj1:0x211B
        je     loc_8_01C7
        clc    
        ret    
loc_8_01C7:
        or     byte ptr [0x211b], al         ; -> obj1:0x211B
        or     byte ptr [0xd], 1             ; -> obj5:0x000D
        clc    
        ret    
sub_8_01D6:
        cmp    eax, 8
        jae    loc_8_01EB
        bts    dword ptr [8], eax            ; -> obj5:0x0008
        jb     loc_8_01EB
        bts    dword ptr [0xb], eax          ; -> obj5:0x000B
loc_8_01EB:
        ret    
sub_8_01EC:
        cmp    eax, 8
        jae    loc_8_0201
        btr    dword ptr [0xb], eax          ; -> obj5:0x000B
        jae    loc_8_0201
        btr    dword ptr [8], eax            ; -> obj5:0x0008
loc_8_0201:
        ret    
loc_8_0202:
        btr    word ptr [0xd], 2             ; -> obj5:0x000D
loc_8_020B:
        mov    ebx, dword ptr [ecx]
        mov    al, byte ptr [8]              ; -> obj5:0x0008
        mov    byte ptr [9], al              ; -> obj5:0x0009
        mov    al, byte ptr [0xb]            ; -> obj5:0x000B
        mov    byte ptr [0xc], al            ; -> obj5:0x000C
        call   sub_8_02F5
        cmp    eax, 0
        jne    loc_8_0263
        mov    edx, dword ptr [ebx + 0xc]
loc_8_022E:
        call   sub_8_0144
        je     loc_8_025E
        movzx  eax, byte ptr [edi + 1]
        cmp    eax, 7
        ja     loc_8_0269
        bts    dword ptr [0x211b], eax       ; -> obj1:0x211B
        bt     dword ptr [9], eax            ; -> obj5:0x0009
        jb     loc_8_0269
loc_8_024E:
        bts    dword ptr [9], eax            ; -> obj5:0x0009
        btr    dword ptr [0xc], eax          ; -> obj5:0x000C
        jmp    loc_8_022E
loc_8_025E:
        mov    eax, 0
loc_8_0263:
        mov    dword ptr [0x2c], eax         ; -> loc_5_002C
        ret    
loc_8_0269:
        bt     dword ptr [0xc], eax          ; -> obj5:0x000C
        jb     loc_8_024E
        bt     word ptr [0xd], 2             ; -> obj5:0x000D
        jb     loc_8_024E
        mov    eax, 0x13
        jmp    loc_8_0263
loc_8_0284:
        bts    word ptr [0xd], 2             ; -> obj5:0x000D
        jmp    loc_8_020B
loc_8_0292:
        cmp    dword ptr [0x2c], 0           ; -> loc_5_002C
        mov    eax, 0x13
        mov    dword ptr [0x2c], eax         ; -> loc_5_002C
        jne    loc_8_02BE
        mov    al, byte ptr [9]              ; -> obj5:0x0009
        mov    byte ptr [8], al              ; -> obj5:0x0008
        mov    al, byte ptr [0xc]            ; -> obj5:0x000C
        mov    byte ptr [0xb], al            ; -> obj5:0x000B
        mov    eax, 0
loc_8_02BE:
        ret    
loc_8_02BF:
        mov    dword ptr [0x2c], 0x13        ; -> loc_5_002C
        mov    eax, 0
        ret    
loc_8_02CF:
        mov    al, byte ptr [8]              ; -> obj5:0x0008
        not    al
        mov    byte ptr [0xa], al            ; -> obj5:0x000A
        mov    dword ptr [ecx], 0xa          ; -> obj5:0x000A
        mov    dword ptr [ecx + 4], 1
        mov    eax, 0
        ret    
loc_8_02EE:
        mov    eax, 0
        ret    
sub_8_02F4:
        ret    
sub_8_02F5:
        pushal 
loc_8_02F6:
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        je     loc_8_0345
        push   2
        push   eax
        push   4                             ; -> obj5:0x0004
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a003300
        add    esp, 0xc
        cmp    eax, 0
        jne    loc_8_0345
loc_8_0313:
        mov    eax, dword ptr [4]            ; -> obj5:0x0004
        push   0
        push   0
        push   3
        push   eax
        push   4                             ; -> obj5:0x0004
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f003300
        add    esp, 0x14
        cmp    eax, 0
        jne    loc_8_0345
        mov    edi, dword ptr [4]            ; -> obj5:0x0004
        movzx  eax, byte ptr [edi + 1]
        btr    dword ptr [9], eax            ; -> obj5:0x0009
        jmp    loc_8_0313
loc_8_0345:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_8_02F6
        popal  
        mov    eax, 0
        ret    
VxD__LTEXT ENDS
