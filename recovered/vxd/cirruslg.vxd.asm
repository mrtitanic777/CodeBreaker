; LAGUNA -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 1112 instructions recovered by recursive descent

Declare_Virtual_Device LAGUNA, 3, 1, LAGUNA_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x330  R-X
LAGUNA_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_000F
        jmp    dword ptr [eax*4 + 0x11]      ; -> obj1:0x0011
loc_1_000F:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_2_0000
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_2_0000
        db  02Eh, 08Bh, 0C0h
loc_1_0080:
        push   esi
        cmp    ax, 0x13
        ja     loc_1_00E3
        push   edi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        pop    edi
        jne    loc_1_00E3
        push   eax
        mov    esi, dword ptr [0x1d0]        ; -> obj1:0x01D0
        and    word ptr [esi + 0xe6], 0xfffe
        mov    word ptr [esi + 0xc0], 0
        and    word ptr [esi + 0xf0], 0xefff
        mov    eax, dword ptr [esi + 0x3fc]
        and    eax, 0xefffffff
        mov    dword ptr [esi + 0x3f4], eax
        mov    dword ptr [esi + 0x3fc], eax
        mov    al, byte ptr [esi + 0xea]
        and    al, 0xc0
        or     al, 8
        and    al, 0x7f
        call   sub_4_04D9
        mov    byte ptr [esi + 0xea], al
        pop    eax
loc_1_00E3:
        pop    esi
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_00E8:
        cmp    cl, 6
        jb     loc_1_0122
        ja     loc_1_00F3
        mov    al, 0x12
        jmp    loc_1_0124
loc_1_00F3:
        cmp    cl, 0x1e
        ja     loc_1_0122
        push   ecx
        and    ecx, 0xff
        bt     dword ptr [0x23a], ecx        ; -> obj1:0x023A
        jae    loc_1_0121
        push   ebx
        mov    bh, ah
        dec    edx
        in     al, dx
        mov    ah, al
        mov    al, cl
        out    dx, al
        inc    edx
        in     al, dx
        xchg   al, ah
        dec    edx
        out    dx, al
        mov    al, ah
        inc    edx
        mov    ah, bh
        pop    ebx
        pop    ecx
        jmp    loc_1_0124
loc_1_0121:
        pop    ecx
loc_1_0122:
        clc    
        ret    
loc_1_0124:
        stc    
        ret    
loc_1_0126:
        cmp    cl, 6
        jb     loc_1_0154
        je     loc_1_0156
        cmp    cl, 0x1e
        ja     loc_1_0154
        push   ecx
        and    ecx, 0xff
        bt     dword ptr [0x23a], ecx        ; -> obj1:0x023A
        jae    loc_1_0153
        push   eax
        dec    edx
        mov    ah, al
        in     al, dx
        xchg   al, cl
        out    dx, ax
        mov    al, cl
        out    dx, al
        inc    edx
        pop    eax
        pop    ecx
        jmp    loc_1_0156
loc_1_0153:
        pop    ecx
loc_1_0154:
        clc    
        ret    
loc_1_0156:
        stc    
        ret    
loc_1_0158:
        clc    
        ret    
        db  000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'LAGUNA  '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  LAGUNA_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_01ED:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x38000000], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x65], dl
        jb     0x356
        jae    0x35b
        push   edx
        popal  
        je     0x35c
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x4f], cl
        inc    esp
        inc    ebp
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x230  R-X
loc_2_0000:
        push   ebx
        mov    dword ptr [0x1ac], ebx        ; -> obj1:0x01AC
        mov    byte ptr [0x1dc], 0           ; -> obj1:0x01DC
        push   0
        push   0x10000
        push   0xa0000
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_2_01D0
        mov    dword ptr [0x1c0], eax        ; -> obj1:0x01C0
        call   sub_2_01D4
        or     ebx, ebx
        je     loc_2_01D0
        mov    dword ptr [0x1d4], ebx        ; -> obj1:0x01D4
        mov    ah, 0x12
        mov    bx, 0xa7
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        shl    edx, 0x10
        mov    dx, cx
        push   0
        push   0x4000
        push   edx
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_2_01D0
        mov    dword ptr [0x1d0], eax        ; -> obj1:0x01D0
        mov    ah, 0x12
        mov    bl, 0x85
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        movzx  eax, al
        shl    eax, 0x10
        mov    dword ptr [0x1bc], eax        ; -> obj1:0x01BC
        VxDCall  VDD.VDD_Get_Special_VM_IDs      ; cd2011000a00
        mov    dword ptr [0x1b4], esi        ; -> obj1:0x01B4
        mov    dword ptr [0x1b8], edi        ; -> obj1:0x01B8
        push   0
        push   0x2a
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0x1c4], eax        ; -> obj1:0x01C4
        or     eax, eax
        je     loc_2_01D0
        push   ebx
        add    ebx, dword ptr [0x1c4]        ; -> obj1:0x01C4
        mov    byte ptr [ebx], 0
        mov    byte ptr [ebx + 1], 0
        mov    byte ptr [ebx + 2], 0
        pop    ebx
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_01D0
        mov    dword ptr [edi], 0            ; -> loc_3_0000
        mov    dword ptr [edi + 4], 0        ; -> loc_4_0000
        mov    dword ptr [edi + 8], 0x30     ; -> loc_4_0030
        mov    dword ptr [edi + 0xc], 0x5c   ; -> loc_4_005C
        mov    dword ptr [edi + 0x58], 0x96  ; -> loc_4_0096
        mov    dword ptr [edi + 0x5c], 0xb9  ; -> loc_4_00B9
        mov    dword ptr [edi + 0x10], 0xd5  ; -> loc_4_00D5
        mov    dword ptr [edi + 0x14], 0x121 ; -> loc_4_0121
        mov    dword ptr [edi + 0x18], 0x1be ; -> loc_4_01BE
        mov    dword ptr [edi + 0x1c], 0x1cd ; -> loc_4_01CD
        mov    dword ptr [edi + 0x20], 0x1d5 ; -> loc_4_01D5
        mov    dword ptr [edi + 0x24], 0x214 ; -> loc_4_0214
        mov    dword ptr [edi + 0x68], 0x238 ; -> loc_3_0238
        mov    dword ptr [edi + 0x90], 0x287 ; -> loc_4_0287
        mov    dword ptr [edi + 0x80], 0x28e ; -> loc_4_028E
        mov    dword ptr [edi + 0x84], 0x2d3 ; -> loc_4_02D3
        mov    dword ptr [edi + 0x94], 0x2fe ; -> loc_4_02FE
        mov    dword ptr [edi + 0x88], 0x356 ; -> loc_4_0356
        mov    dword ptr [edi + 0x9c], 0x30b ; -> loc_4_030B
        mov    dword ptr [edi + 0xa0], 0x334 ; -> loc_4_0334
        mov    dword ptr [edi + 0xa8], 0x228 ; -> loc_2_0228
        mov    dword ptr [edi + 0x70], 0x8d  ; -> loc_3_008D
        mov    dword ptr [edi + 0x74], 0x1eb ; -> loc_3_01EB
        mov    dword ptr [edi + 0xac], 0x158 ; -> loc_1_0158
        mov    dword ptr [edi + 0xa4], 0x387 ; -> loc_4_0387
        mov    dword ptr [edi + 0xbc], 0x3cc ; -> loc_4_03CC
        mov    dword ptr [edi + 0x48], 0xe8  ; -> loc_1_00E8
        mov    dword ptr [edi + 0x4c], 0x126 ; -> loc_1_0126
        mov    dword ptr [edi + 0xc0], 0x80  ; -> loc_1_0080
        clc    
        jmp    loc_2_01D1
loc_2_01D0:
        stc    
loc_2_01D1:
        pop    ebx
        ret    
        db  090h
sub_2_01D4:
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    al, 6
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        ror    eax, 8
        cmp    al, 0x12
        jne    loc_2_0212
        mov    al, 0xfa
        out    dx, al
        in     al, dx
        cmp    al, 0x12
        jne    loc_2_0212
        mov    ah, 0x12
        mov    bl, 0x80
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        cmp    ax, 0x60
        je     loc_2_020A
        jmp    loc_2_0212
loc_2_020A:
        shl    ebx, 0x10
        mov    bx, ax
        jmp    loc_2_0214
loc_2_0212:
        xor    ebx, ebx
loc_2_0214:
        mov    edx, 0x3c4
        mov    al, 6
        out    dx, al
        inc    edx
        rol    eax, 8
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        ret    
        db  090h
loc_2_0228:
        mov    eax, dword ptr [0x1d4]        ; -> obj1:0x01D4
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x2fc  R-X
loc_3_0000:
        mov    edx, 0x3cc
        in     al, dx
        mov    byte ptr [0x1e4], al          ; -> obj1:0x01E4
        cmp    word ptr [ebp], 0
        je     loc_3_008C
        mov    ax, 0x3c00
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [0x1c8], eax        ; -> obj1:0x01C8
        VxDCall  VDD.VDD_Set_Sleep_Flag_Addr     ; cd2016000a00
        mov    eax, dword ptr [ebp + 0x14]
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
        mov    eax, dword ptr [ebp + 0x14]
        push   0
        push   dword ptr [0x1ac]             ; -> obj1:0x01AC
        push   eax
        VMMCall  VMM._GetDescriptor              ; cd207b000100
        add    esp, 0xc
        mov    al, dh
        rol    eax, 0x10
        and    eax, 0xffffff
        and    edx, 0xff000000
        add    eax, edx
        add    eax, 0x800000
        push   0
        push   0x50000
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_2_01D0
        mov    dword ptr [0x1cc], eax        ; -> obj1:0x01CC
        mov    eax, dword ptr [ebp + 4]
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [0x1e9], eax        ; -> obj1:0x01E9
loc_3_008C:
        ret    
loc_3_008D:
        mov    ax, 0x1200
        mov    bl, 0x9a
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    word ptr [0x327], ax          ; -> obj1:0x0327
        mov    word ptr [0x329], cx          ; -> obj1:0x0329
        mov    word ptr [0x32b], dx          ; -> obj1:0x032B
        mov    dword ptr [0x1d8], 0          ; -> obj1:0x01D8
        mov    edx, 0x3c4
        mov    al, 0x15
        out    dx, al
        in     ax, dx
        or     ah, 0x80
        out    dx, ax
        mov    eax, 0x1f0                    ; -> obj1:0x01F0
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        push   eax
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        pop    edi
        test   word ptr [edi + 2], 0x110
        jne    loc_3_01EA
        push   edi
        mov    ah, 0x12
        mov    bl, 0x9a
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edi
        mov    al, cl
        shr    al, 4
        and    al, 0xf
        mov    ah, dl
        shl    ah, 4
        or     al, ah
        mov    bh, ch
        shr    dl, 4
        and    dl, 0xf
        mov    ch, dl
        shl    cl, 3
        or     ch, cl
        movzx  esi, word ptr [edi + 0x20]
        movzx  edx, word ptr [edi + 0x1a]
        or     edx, edx
        je     loc_3_01EA
        cmp    edx, 0x1e0
        je     loc_3_01C3
        cmp    edx, 0x258
        je     loc_3_019E
        cmp    edx, 0x300
        je     loc_3_017B
        cmp    edx, 0x400
        je     loc_3_0156
        and    ch, 0xf0
        mov    ah, 1
        cmp    esi, 0x3c
        jae    loc_3_0177
        xor    ah, ah
        jmp    loc_3_0177
loc_3_0156:
        and    ch, 0xf
        mov    ah, 0x40
        cmp    esi, 0x55
        jae    loc_3_0177
        mov    ah, 0x30
        cmp    esi, 0x4b
        jae    loc_3_0177
        mov    ah, 0x20
        cmp    esi, 0x47
        jae    loc_3_0177
        mov    ah, 0x10
        cmp    esi, 0x3c
        jae    loc_3_0177
        xor    ah, ah
loc_3_0177:
        or     ch, ah
        jmp    loc_3_01DE
loc_3_017B:
        and    bh, 0xf
        mov    ah, 0x50
        cmp    esi, 0x55
        jae    loc_3_01BF
        mov    ah, 0x40
        cmp    esi, 0x4b
        jae    loc_3_01BF
        mov    ah, 0x20
        cmp    esi, 0x46
        jae    loc_3_01BF
        mov    ah, 0x10
        cmp    esi, 0x3c
        jae    loc_3_01BF
        xor    ah, ah
        jmp    loc_3_01BF
loc_3_019E:
        and    bh, 0xf0
        mov    ah, 4
        cmp    esi, 0x55
        jae    loc_3_01BF
        dec    ah
        cmp    esi, 0x4b
        jae    loc_3_01BF
        dec    ah
        cmp    esi, 0x48
        jae    loc_3_01BF
        dec    ah
        cmp    esi, 0x3c
        jae    loc_3_01BF
        xor    ah, ah
loc_3_01BF:
        or     bh, ah
        jmp    loc_3_01DE
loc_3_01C3:
        and    al, 0xf
        mov    ah, 0x30
        cmp    esi, 0x55
        jae    loc_3_01DC
        mov    ah, 0x20
        cmp    esi, 0x4b
        jae    loc_3_01DC
        mov    ah, 0x10
        cmp    esi, 0x48
        jae    loc_3_01DC
        xor    ah, ah
loc_3_01DC:
        or     al, ah
loc_3_01DE:
        mov    ah, 0x12
        mov    bl, 0xa4
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_3_01EA:
        ret    
loc_3_01EB:
        mov    ax, word ptr [0x329]          ; -> obj1:0x0329
        mov    cx, ax
        shl    cx, 0xb
        and    cx, 0x70
        mov    bx, ax
        and    bx, 0xf000
        and    ax, 0xf00
        or     bx, ax
        mov    dx, word ptr [0x32b]          ; -> obj1:0x032B
        shl    dx, 4
        and    dx, 0xf00
        or     cx, dx
        mov    ax, word ptr [0x327]          ; -> obj1:0x0327
        shl    al, 4
        and    al, 0xf0
        or     al, 4
        mov    ah, 0x12
        mov    bl, 0xa4
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        ret    
loc_3_0238:
        push   eax
        push   edx
        mov    ax, 3
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    edx, dword ptr [0x1d0]        ; -> obj1:0x01D0
        and    word ptr [edx + 0xe6], 0xfffe
        mov    eax, dword ptr [edx + 0x3fc]
        and    eax, 0xefffffff
        mov    dword ptr [edx + 0x3f4], eax
        mov    dword ptr [edx + 0x3fc], eax
        and    word ptr [edx + 0xf0], 0xefff
        mov    word ptr [edx + 0xc0], 0
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    byte ptr [ebx + 0x1d], 0x12
        mov    byte ptr [ebx + 0x10], 0xb5
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    ax, word ptr [ebx + 0x1c]
        and    ah, 0x3f
        mov    al, byte ptr [edx + 0xea]
        and    al, 0xc0
        and    al, 0x7f
        or     ah, al
        call   sub_4_04D9
        mov    byte ptr [edx + 0xea], ah
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebx
        mov    edx, 0x3c4
        mov    al, 0x15
        out    dx, al
        in     ax, dx
        and    ah, 0x7f
        out    dx, ax
        pop    edx
        pop    eax
        mov    byte ptr [0x1e0], 0           ; -> obj1:0x01E0
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x507  R-X
loc_4_0000:
        mov    byte ptr [0x1e0], 0xff        ; -> obj1:0x01E0
        mov    byte ptr [0x1ed], 0xff        ; -> loc_1_01ED
        cmp    dword ptr [0x1e9], 0          ; -> obj1:0x01E9
        je     loc_4_002B
        mov    ecx, 4
        mov    byte ptr [0x1e8], 0           ; -> obj1:0x01E8
        mov    edx, dword ptr [0x1e9]        ; -> obj1:0x01E9
        jmp    loc_4_002D
loc_4_002B:
        mov    edx, ecx
loc_4_002D:
        xor    ah, ah
        ret    
loc_4_0030:
        cmp    byte ptr [0x1ed], 0xff        ; -> loc_1_01ED
        jne    loc_4_005B
        push   eax
        push   edx
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x1ed], al          ; -> loc_1_01ED
        mov    al, byte ptr [0x1e8]          ; -> obj1:0x01E8
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_005B:
        ret    
loc_4_005C:
        cmp    byte ptr [0x1ed], 0xff        ; -> loc_1_01ED
        je     loc_4_0095
        push   eax
        push   edx
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        cmp    al, byte ptr [0x1e8]          ; -> obj1:0x01E8
        jne    loc_4_0083
        mov    al, byte ptr [0x1ed]          ; -> loc_1_01ED
        out    dx, al
loc_4_0083:
        mov    byte ptr [0x1ed], 0xff        ; -> loc_1_01ED
        mov    edx, 0x3ce
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_0095:
        ret    
loc_4_0096:
        push   eax
        push   edx
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x1ee], al          ; -> obj1:0x01EE
        mov    al, byte ptr [0x1e8]          ; -> obj1:0x01E8
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret    
loc_4_00B9:
        push   eax
        push   edx
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 9
        mov    ah, byte ptr [0x1ee]          ; -> obj1:0x01EE
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret    
loc_4_00D5:
        or     byte ptr [0x1dc], 2           ; -> obj1:0x01DC
        mov    byte ptr [0x1e8], 0x40        ; -> obj1:0x01E8
        push   esi
        push   edi
        push   ecx
        mov    esi, dword ptr [0x1d0]        ; -> obj1:0x01D0
        and    word ptr [esi + 0xe6], 0xfffe
        mov    word ptr [esi + 0xc0], 0
        and    word ptr [esi + 0xf0], 0xefff
        mov    eax, dword ptr [esi + 0x3fc]
        and    eax, 0xefffffff
        mov    dword ptr [esi + 0x3f4], eax
        mov    dword ptr [esi + 0x3fc], eax
        pop    ecx
        pop    edi
        pop    esi
        ret    
loc_4_0121:
        push   edx
        push   eax
        push   edi
        mov    byte ptr [0x1dc], 0           ; -> obj1:0x01DC
        mov    edi, ebx
        add    edi, dword ptr [0x1c4]        ; -> obj1:0x01C4
        mov    al, byte ptr [edi]
        or     al, al
        je     loc_4_014E
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    al, 0x15
        out    dx, al
        inc    edx
        mov    al, byte ptr [edi]
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_014E:
        mov    edx, dword ptr [0x1d0]        ; -> obj1:0x01D0
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    byte ptr [ebx + 0x1d], 0x12
        mov    byte ptr [ebx + 0x10], 0xb5
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    ax, word ptr [ebx + 0x1c]
        and    ah, 0x3f
        mov    al, byte ptr [edx + 0xea]
        and    al, 0xc0
        and    al, 0x7f
        or     ah, al
        call   sub_4_04D9
        mov    byte ptr [edx + 0xea], ah
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebx
        pop    edi
        pop    eax
        pop    edx
        ret    
loc_4_01BE:
        or     byte ptr [0x1dc], 1           ; -> obj1:0x01DC
        mov    byte ptr [0x1e8], 0           ; -> obj1:0x01E8
        ret    
loc_4_01CD:
        mov    byte ptr [0x1dc], 0           ; -> obj1:0x01DC
        ret    
loc_4_01D5:
        push   edx
        push   edi
        mov    edi, ebx
        add    edi, dword ptr [0x1c4]        ; -> obj1:0x01C4
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 3], al
        dec    edx
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 1], al
        dec    edx
        mov    al, 0xa
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 2], al
        dec    edx
        rol    eax, 8
        out    dx, al
        mov    dword ptr [0x1d8], 1          ; -> obj1:0x01D8
        pop    edi
        pop    edx
        ret    
loc_4_0214:
        push   edx
        cmp    byte ptr [0x1e0], 0           ; -> obj1:0x01E0
        je     loc_4_0285
        cmp    dword ptr [0x1d8], 1          ; -> obj1:0x01D8
        jne    loc_4_0285
        cmp    ecx, dword ptr [0x1ac]        ; -> obj1:0x01AC
        jne    loc_4_023E
        mov    al, byte ptr [0x1e4]          ; -> obj1:0x01E4
        or     al, al
        je     loc_4_023E
        mov    edx, 0x3c2
        out    dx, al
loc_4_023E:
        cmp    esi, dword ptr [0x1b8]        ; -> obj1:0x01B8
        je     loc_4_0285
        cmp    esi, dword ptr [0x1b4]        ; -> obj1:0x01B4
        je     loc_4_0285
        test   byte ptr [0x1dc], 3           ; -> obj1:0x01DC
        jne    loc_4_0285
        add    esi, dword ptr [0x1c4]        ; -> obj1:0x01C4
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb
        out    dx, al
        inc    edx
        mov    al, byte ptr [esi + 3]
        out    dx, al
        dec    edx
        mov    al, 9
        out    dx, al
        inc    edx
        mov    al, byte ptr [esi + 1]
        out    dx, al
        dec    edx
        mov    al, 0xa
        out    dx, al
        inc    edx
        mov    al, byte ptr [esi + 2]
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_0285:
        pop    edx
        ret    
loc_4_0287:
        mov    ecx, 0x100000
        stc    
        ret    
loc_4_028E:
        push   ebx
        add    ebx, dword ptr [0x1c4]        ; -> obj1:0x01C4
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    al, 0x15
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [ebx], al
        dec    edx
        rol    eax, 8
        out    dx, al
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xa
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [ebx + 2], al
        dec    dl
        mov    al, 9
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [ebx + 1], al
        pop    ebx
        dec    dl
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc    
        ret    
loc_4_02D3:
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        dec    dl
        mov    al, 0xa
        test   ah, 1
        jne    loc_4_02EF
        mov    al, 9
loc_4_02EF:
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc    
        ret    
loc_4_02FE:
        push   ecx
        push   ebx
        xor    eax, eax
        mov    edx, 0x1000
        pop    ebx
        pop    ecx
        stc    
        ret    
loc_4_030B:
        push   edx
        push   eax
        push   ebx
        add    ebx, dword ptr [0x1c4]        ; -> obj1:0x01C4
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [ebx + 3], al
        xor    al, al
        out    dx, al
        rol    eax, 8
        dec    dl
        out    dx, al
        pop    ebx
        pop    eax
        pop    edx
        stc    
        ret    
loc_4_0334:
        push   edx
        push   eax
        push   ebx
        add    ebx, dword ptr [0x1c4]        ; -> obj1:0x01C4
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb
        mov    ah, byte ptr [ebx + 3]
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    ebx
        pop    eax
        pop    edx
        stc    
        ret    
loc_4_0356:
        push   ecx
        push   edx
        push   eax
        mov    ecx, eax
        mov    ah, dl
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    ah, al
        mov    al, 9
        out    dx, ax
        mov    al, 0xb
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        test   al, 1
        je     loc_4_037E
        mov    al, 0xa
        mov    ah, cl
        out    dx, ax
loc_4_037E:
        rol    eax, 8
        out    dx, al
        pop    eax
        pop    edx
        pop    ecx
        stc    
        ret    
loc_4_0387:
        push   edx
        push   eax
        mov    al, byte ptr [ebp + 0x1c]
        cmp    al, 8
        jne    loc_4_039E
        mov    byte ptr [ebp + 0x11], 6
        mov    word ptr [ebp + 0x1c], 0x4f
        pop    eax
        pop    edx
        stc    
        ret    
loc_4_039E:
        cmp    al, 2
        jbe    loc_4_03A6
        cmp    al, 6
        jne    loc_4_03C8
loc_4_03A6:
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    al, 0x14
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        and    ax, 0x7f8
        out    dx, al
        mov    byte ptr [0x1dd], ah          ; -> obj1:0x01DD
        rol    eax, 8
        dec    dl
        out    dx, al
loc_4_03C8:
        pop    eax
        pop    edx
        clc    
        ret    
loc_4_03CC:
        push   eax
        push   edx
        cmp    dl, 2
        jbe    loc_4_03DC
        cmp    dl, 6
        jne    loc_4_04D6
loc_4_03DC:
        mov    dx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    al, 0x14
        out    dx, al
        inc    dl
        in     al, dx
        or     al, byte ptr [0x1dd]          ; -> obj1:0x01DD
        out    dx, al
        rol    eax, 8
        dec    dl
        out    dx, al
        mov    dl, byte ptr [esp]
        cmp    dl, 0
        jne    loc_4_0415
        movzx  eax, word ptr [ebp + 0x38]
        movzx  edx, word ptr [ebp]
        shl    eax, 4
        add    eax, edx
        and    byte ptr [eax + 0xa], 0xfe
        jmp    loc_4_04D6
loc_4_0415:
        cmp    dl, 2
        jne    loc_4_04D6
        cmp    word ptr [ebp + 0x1c], 0x4f
        jne    loc_4_04D6
        push   edi
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        pop    esi
        cmp    ebx, edi
        pop    edi
        jne    loc_4_04D6
        mov    edx, dword ptr [0x1d0]        ; -> obj1:0x01D0
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    byte ptr [ebp + 0x1d], 0x12
        mov    byte ptr [ebp + 0x10], 0xb5
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    ax, word ptr [ebp + 0x1c]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        or     ah, 0x80
        cmp    word ptr [esp + 2], 0x13
        jbe    loc_4_0490
        and    ah, 0x7f
loc_4_0490:
        call   sub_4_04D9
        mov    byte ptr [edx + 0xea], ah
        mov    byte ptr [edx + 0xc1], al
        shr    al, 3
        and    al, 0x10
        and    byte ptr [edx + 0xf1], 0x10
        or     byte ptr [edx + 0xf1], al
        mov    eax, dword ptr [edx + 0x3fc]
        and    eax, 0xefffffff
        test   byte ptr [esp + 3], 0x40
        je     loc_4_04CA
        or     eax, 0x10000000
loc_4_04CA:
        mov    dword ptr [edx + 0x3f4], eax
        mov    dword ptr [edx + 0x3fc], eax
loc_4_04D6:
        pop    edx
        pop    eax
        ret    
sub_4_04D9:
        push   ax
        push   cx
        push   dx
        xor    cx, cx
        mov    dx, 0x3cc
        in     al, dx
        mov    dx, 0x3da
        test   al, 1
        jne    loc_4_04F3
        mov    dx, 0x3ba
loc_4_04F3:
        in     al, dx
        test   al, 8
        loopne loc_4_04F3
        xor    cx, cx
loc_4_04FB:
        in     al, dx
        test   al, 8
        loope  loc_4_04FB
        pop    dx
        pop    cx
        pop    ax
        ret    
VxD__LTEXT ENDS
