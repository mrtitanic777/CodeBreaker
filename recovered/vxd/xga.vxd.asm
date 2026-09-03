; XGA -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 2357 instructions recovered by recursive descent

Declare_Virtual_Device XGA, 3, 1, XGA_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x168  R-X
XGA_Control:
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
        dd  loc_4_0000
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
loc_1_007D:
        push   edi
        mov    edi, ebx
        add    edi, dword ptr [0xf0]         ; -> obj1:0x00F0
        movzx  edi, word ptr [edi]
        and    edi, 0xffff3f7f
        cmp    edi, 0x100
        cmc    
        pop    edi
        ret    
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'XGA     '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  XGA_Control              ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x428  R-X
loc_2_0000:
        mov    dword ptr [0xe8], ebx         ; -> obj1:0x00E8
        push   0
        push   0x10000
        push   0xa0000
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_2_0203
        mov    dword ptr [0xec], eax         ; -> obj1:0x00EC
        call   sub_2_0205
        or     ax, dx
        je     loc_2_0203
        movzx  edi, di
        cmp    edi, 0x8fda
        je     loc_2_004F
        cmp    edi, 0x8fd9
        je     loc_2_004F
        mov    edi, 0x8fdb
loc_2_004F:
        mov    dword ptr [0x110], edi        ; -> obj1:0x0110
        push   0
        push   8
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0xf0], eax         ; -> obj1:0x00F0
        or     eax, eax
        je     loc_2_0203
        mov    eax, dword ptr [0x100]        ; -> obj1:0x0100
        mov    word ptr [0x413], ax          ; -> obj2:0x0413
        push   0
        push   0x15
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        mov    edi, eax
        mov    esi, eax
        and    eax, 0xf
        and    esi, 0xfffffff0
        shl    esi, 0xc
        or     eax, esi
        mov    dword ptr [0xf8], eax         ; -> obj1:0x00F8
        or     eax, eax
        je     loc_2_00AE
        mov    esi, 0x410                    ; -> loc_2_0410
        mov    ecx, 0x15
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_2_00AE:
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_0203
        mov    dword ptr [edi], 0            ; -> loc_3_0000
        mov    dword ptr [edi + 0xa8], 0x40a ; -> loc_2_040A
        mov    dword ptr [edi + 0x10], 0x5d  ; -> loc_4_005D
        mov    dword ptr [edi + 0x68], 0x117 ; -> loc_3_0117
        mov    dword ptr [edi + 0xc0], 0x7e  ; -> loc_4_007E
        mov    dword ptr [edi + 0xa4], 0xd25 ; -> loc_4_0D25
        mov    dword ptr [edi + 0x90], 0x14f5 ; -> loc_4_14F5
        mov    dword ptr [edi + 0xac], 0x7d  ; -> loc_1_007D
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        je     loc_2_0122
        mov    dword ptr [edi + 0x78], 0x14fd ; -> loc_4_14FD
        mov    dword ptr [edi + 0x7c], 0x15b0 ; -> loc_4_15B0
        jmp    loc_2_0200
loc_2_0122:
        mov    dword ptr [edi + 0x14], 0x76  ; -> loc_4_0076
        mov    esi, 0x1624                   ; -> loc_4_1624
        mov    edx, 0x2160
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0203
        mov    edx, 0x2160
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    esi, 0x164c                   ; -> loc_4_164C
        mov    edx, 0x216a
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0203
        mov    edx, 0x216a
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    esi, 0x164c                   ; -> loc_4_164C
        mov    edx, 0x216b
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0203
        mov    edx, 0x216b
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    esi, 0x164c                   ; -> loc_4_164C
        mov    edx, 0x216c
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0203
        mov    edx, 0x216c
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    esi, 0x164c                   ; -> loc_4_164C
        mov    edx, 0x216d
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0203
        mov    edx, 0x216d
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    esi, 0x164c                   ; -> loc_4_164C
        mov    edx, 0x216e
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0203
        mov    edx, 0x216e
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    esi, 0x164c                   ; -> loc_4_164C
        mov    edx, 0x216f
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0203
        mov    edx, 0x216f
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
loc_2_0200:
        clc    
        jmp    loc_2_0204
loc_2_0203:
        stc    
loc_2_0204:
        ret    
sub_2_0205:
        cld    
        mov    dword ptr [0xf4], 0           ; -> obj1:0x00F4
        mov    ebx, 1
        mov    ecx, 8
loc_2_021A:
        push   ecx
        push   ebx
        mov    al, bl
        dec    al
        or     al, 8
        out    0x96, al
        mov    edx, 0x100
        in     al, dx
        mov    bl, al
        inc    edx
        in     al, dx
        mov    bh, al
        mov    di, bx
        inc    edx
        in     al, dx
        mov    cl, al
        inc    edx
        in     al, dx
        mov    ch, al
        mov    si, cx
        inc    edx
        in     al, dx
        mov    ah, al
        inc    edx
        in     al, dx
        pop    ebx
        pop    ecx
        push   eax
        xor    al, al
        out    0x96, al
        pop    eax
        call   sub_2_03DF
        jb     loc_2_02F2
        inc    ebx
        dec    ecx
        jne    loc_2_021A
        mov    al, 0xdf
        out    0x94, al
        mov    edx, 0x100
        in     al, dx
        mov    bl, al
        inc    edx
        in     al, dx
        mov    bh, al
        mov    di, bx
        inc    edx
        in     al, dx
        mov    cl, al
        inc    edx
        in     al, dx
        mov    ch, al
        mov    si, cx
        inc    edx
        in     al, dx
        mov    ah, al
        inc    edx
        in     al, dx
        push   eax
        mov    al, 0xff
        out    0x94, al
        pop    eax
        xor    ebx, ebx
        call   sub_2_03DF
        jb     loc_2_02F2
        mov    dx, 0x2160
        mov    al, 0xa
        out    dx, al
        jmp    loc_2_0297
loc_2_0297:
        in     al, dx
        and    al, 0xf
        cmp    al, 0xa
        jne    loc_2_03D9
        mov    al, 1
        out    dx, al
        mov    dx, 0x2168
        mov    al, 5
        out    dx, al
        jmp    loc_2_02AE
loc_2_02AE:
        in     al, dx
        cmp    al, 5
        jne    loc_2_03D9
        mov    di, 0x8fd9
        mov    dword ptr [0xfc], 0x2160      ; -> obj1:0x00FC
        mov    dword ptr [0x100], 0x2168     ; -> obj1:0x0100
        mov    dword ptr [0x10c], 0xfd800000 ; -> obj1:0x010C
        mov    edx, 0x216a
        mov    al, 0x6d
        mov    ah, 3
        out    dx, ax
        mov    ebx, 0xb1f00
        mov    byte ptr [0x165], 0           ; -> obj1:0x0165
        jmp    loc_2_0354
loc_2_02F2:
        mov    byte ptr [0x165], bl          ; -> obj1:0x0165
        movzx  ecx, si
        mov    ch, cl
        and    cl, 0xe
        shr    cl, 1
        xor    ebx, ebx
        mov    bl, cl
        mov    al, cl
        shl    bl, 4
        or     ebx, 0x2100
        mov    dword ptr [0xfc], ebx         ; -> obj1:0x00FC
        add    bl, 8
        mov    dword ptr [0x100], ebx        ; -> obj1:0x0100
        mov    bl, ah
        and    bl, 0xfe
        shl    ebx, 0x18
        movzx  edx, al
        shl    edx, 0x16
        or     edx, ebx
        mov    dword ptr [0x10c], edx        ; -> obj1:0x010C
        movzx  ebx, cl
        shl    ebx, 7
        add    ebx, 0x1c00
        shr    ecx, 0xc
        mov    eax, 0x2000
        mul    ecx
        add    ebx, eax
        add    ebx, 0xc0000
loc_2_0354:
        push   0
        push   0x100
        push   ebx
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        mov    dword ptr [0x108], eax        ; -> obj1:0x0108
        cmp    byte ptr [0x165], 0           ; -> obj1:0x0165
        je     loc_2_03D1
        push   edi
        mov    al, 0xdf
        out    0x94, al
        mov    edx, 0x100
        in     al, dx
        mov    bl, al
        inc    edx
        in     al, dx
        mov    bh, al
        mov    di, bx
        call   sub_2_03DF
        jae    loc_2_0393
        call   sub_2_0400
loc_2_0393:
        mov    al, 0xff
        out    0x94, al
        mov    cl, 1
        mov    ch, 8
loc_2_039B:
        cmp    cl, byte ptr [0x165]          ; -> obj1:0x0165
        je     loc_2_03CA
        mov    al, cl
        dec    al
        or     al, 8
        out    0x96, al
        mov    edx, 0x100
        in     al, dx
        mov    bl, al
        inc    edx
        in     al, dx
        mov    bh, al
        mov    di, bx
        call   sub_2_03DF
        jae    loc_2_03C6
        call   sub_2_0400
loc_2_03C6:
        xor    al, al
        out    0x96, al
loc_2_03CA:
        inc    cl
        dec    ch
        jne    loc_2_039B
        pop    edi
loc_2_03D1:
        mov    ax, 1
        cwd    
        jmp    loc_2_03DE
loc_2_03D9:
        xor    ax, ax
        cwd    
loc_2_03DE:
        ret    
sub_2_03DF:
        cmp    di, 0x8fd8
        je     loc_2_03FE
        cmp    di, 0x8fd9
        je     loc_2_03FE
        cmp    di, 0x8fda
        je     loc_2_03FE
        cmp    di, 0x8fdb
        je     loc_2_03FE
        clc    
        jmp    loc_2_03FF
loc_2_03FE:
        stc    
loc_2_03FF:
        ret    
sub_2_0400:
        mov    edx, 0x102
        in     al, dx
        and    al, 0xfe
        out    dx, al
        ret    
loc_2_040A:
        mov    eax, dword ptr [0x110]        ; -> obj1:0x0110
        ret    
loc_2_0410:
        mov    al, dl
        mov    edx, 0xff0a0000
        je     loc_2_0420
        xor    ah, ah
        in     al, dx
        mov    edx, eax
        jmp    loc_2_0421
loc_2_0420:
        out    dx, al
loc_2_0421:
        mov    eax, 0xcb004f
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x11d  R-X
loc_3_0000:
        cmp    byte ptr [ebp + 0x18], 0
        jne    loc_3_0024
        mov    eax, dword ptr [0x10c]        ; -> obj1:0x010C
        mov    dword ptr [ebp + 0x1c], eax
        mov    edx, dword ptr [0xfc]         ; -> obj1:0x00FC
        mov    word ptr [ebp + 0x14], dx
        mov    edi, dword ptr [0x110]        ; -> obj1:0x0110
        mov    word ptr [ebp], di
        jmp    loc_3_0094
loc_3_0024:
        call   sub_3_0095
        mov    dword ptr [ebp + 0x1c], esi
        movzx  eax, word ptr [0x162]         ; -> obj1:0x0162
        or     eax, eax
        jne    loc_3_0070
        mov    eax, dword ptr [0x108]        ; -> obj1:0x0108
        mov    ecx, 0x3ff
        mov    edx, 0x93
        mov    edi, 0x10
        push   0
        push   edi
        push   edx
        push   ecx
        push   eax
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        VMMCall  VMM._Allocate_LDT_Selector      ; cd2078000100
        add    esp, 0x14
        mov    word ptr [0x162], ax          ; -> obj1:0x0162
loc_3_0070:
        mov    word ptr [ebp + 0x14], ax
        mov    eax, 0x118                    ; -> obj1:0x0118
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        movzx  edi, word ptr [0x138]         ; -> obj1:0x0138
        mov    dword ptr [0x114], edi        ; -> obj1:0x0114
loc_3_0094:
        ret    
sub_3_0095:
        mov    edx, dword ptr [0xfc]         ; -> obj1:0x00FC
        add    dl, 4
        xor    al, al
        out    dx, al
        inc    dl
        mov    al, 0xff
        out    dx, al
        sub    dl, 5
        mov    al, 4
        out    dx, al
        inc    dl
        mov    al, 1
        out    dx, al
        add    dl, 7
        mov    al, 0xc
        out    dx, al
        mov    esi, 0x80000
        mov    edi, dword ptr [0xec]         ; -> obj1:0x00EC
        mov    byte ptr [edi], 0x5a
        cmp    byte ptr [edi], 0x5a
        jne    loc_3_00FE
        mov    byte ptr [edi], 0
        cmp    byte ptr [edi], 0
        jne    loc_3_00FE
        mov    esi, 0x100000
        cmp    dword ptr [0x110], 0x8fdb     ; -> obj1:0x0110
        je     loc_3_00FE
        mov    edx, dword ptr [0x100]        ; -> obj1:0x0100
        mov    al, 0x11
        out    dx, al
        mov    byte ptr [edi], 0x5a
        cmp    byte ptr [edi], 0x5a
        jne    loc_3_00FE
        mov    byte ptr [edi], 0
        cmp    byte ptr [edi], 0
        jne    loc_3_00FE
        add    esi, esi
loc_3_00FE:
        mov    dword ptr [0xf4], esi         ; -> obj1:0x00F4
        mov    edx, dword ptr [0x100]        ; -> obj1:0x0100
        xor    al, al
        out    dx, al
        add    dl, 2
        mov    dword ptr [0x104], edx        ; -> obj1:0x0104
        ret    
loc_3_0117:
        call   sub_4_0007
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x16a1  R-X
loc_4_0000:
        call   7
        clc    
        ret    
sub_4_0007:
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        je     loc_4_005C
        mov    edx, dword ptr [0xfc]         ; -> obj1:0x00FC
        inc    dl
        xor    al, al
        out    dx, al
        add    dl, 3
        out    dx, al
        inc    dl
        mov    al, 0xff
        out    dx, al
        add    dl, 5
        mov    ax, 0x1550
        out    dx, ax
        mov    ax, 0x1450
        out    dx, ax
        mov    ax, 0x51
        out    dx, ax
        mov    ax, 0x454
        out    dx, ax
        mov    ax, 0x70
        out    dx, ax
        mov    ax, 0x202a
        out    dx, ax
        sub    dl, 0xa
        mov    al, 1
        out    dx, al
        mov    edx, 0x3c3
        mov    al, 1
        out    dx, al
loc_4_005C:
        ret    
loc_4_005D:
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        jne    loc_4_0070
        mov    byte ptr [0x164], 0xff        ; -> obj1:0x0164
loc_4_0070:
        call   7
        ret    
loc_4_0076:
        mov    byte ptr [0x164], 0           ; -> obj1:0x0164
        ret    
loc_4_007E:
        cmp    ebx, dword ptr [0xe8]         ; -> obj1:0x00E8
        je     loc_4_00AC
        push   edi
        push   esi
        mov    edi, ebx
        add    edi, dword ptr [0xf0]         ; -> obj1:0x00F0
        mov    word ptr [edi], ax
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    edi, dword ptr [0xe8]         ; -> obj1:0x00E8
        je     loc_4_00AA
        push   eax
        push   edx
        call   7
        pop    edx
        pop    eax
loc_4_00AA:
        pop    esi
        pop    edi
loc_4_00AC:
        ret    
sub_4_00AD:
        inc    ebx
        or     eax, 0xe3d0000                ; -> loc_4_0E3D
        add    byte ptr [eax], al
        mov    edx, 0xaa00000f               ; -> loc_4_12AA
        adc    al, byte ptr [eax]
        add    al, al                        ; -> loc_4_12C0
        adc    al, byte ptr [eax]
        add    bl, dl                        ; -> loc_4_12D3
        adc    al, byte ptr [eax]
        add    bl, dh                        ; -> loc_4_12F3
        adc    al, byte ptr [eax]
        add    byte ptr [ecx - 0x49ffffed], dl ; -> loc_4_1391
        adc    eax, dword ptr [eax]
        add    bh, cl                        ; -> loc_4_13CF
        adc    eax, dword ptr [eax]
        add    dh, ch                        ; -> loc_4_14EE
        adc    al, 0
        add    byte ptr [eax], al
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx + eax], eax
        add    eax, 0xff011101
        inc    dword ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx + eax], eax
        add    eax, 0x7010601
        add    dword ptr [ecx], edx
        add    dword ptr [ecx + eax], edx
        pop    ss
        add    edi, edi
        db  0FFh
loc_4_00FD:
        jge    loc_4_0101
        add    byte ptr [eax], al
loc_4_0101:
        xor    dword ptr [eax], ecx
loc_4_0103:
        add    byte ptr [edx], 0x80
        add    dl, byte ptr [eax + 1]
        add    bh, bl                        ; -> loc_4_02DF
        add    al, byte ptr [eax]
        add    byte ptr [ecx], dh
        or     byte ptr [eax - 0x1ffd7ffe], al
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
        add    byte ptr [edi + 4], ah        ; -> loc_4_0467
        add    byte ptr [eax], al
        aaa    
        or     byte ptr [eax], ah
        add    esp, dword ptr [eax]
        add    ebx, dword ptr [eax + 2]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    dword ptr [0x31000003], eax
        add    al, 0
        add    al, 0
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [0x31000004], al     ; -> loc_4_0405
        or     byte ptr [eax], al
        add    al, 0
        add    al, 0
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x37000006], cl ; -> loc_4_068D
        or     byte ptr [eax], al
        add    eax, 0x4000500
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ecx
        add    eax, dword ptr [eax]
        add    byte ptr [ecx], dh
        adc    byte ptr [eax - 0x1ffafffe], al
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 1], dl
        push   eax
        add    byte ptr [eax], dl
        arpl   word ptr [ecx], dx
        add    byte ptr [edx], dl
        dec    edi
        adc    eax, dword ptr [eax]
        adc    al, 0x4f
        adc    eax, 0x17631600
        add    byte ptr [eax], bl
        push   ebp
        sbb    dword ptr [eax], eax
        sbb    ah, byte ptr [ecx + 0x1b]
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        shl    byte ptr [ecx], 1
        and    cl, byte ptr [edi - 0x70dbfedd]
        and    eax, 0x27c02601
        add    dword ptr [eax], ebp
        pushfd 
        sub    dword ptr [ecx], eax
        sub    bl, byte ptr [esi - 0xd200d4]
        add    byte ptr ss:[eax], al
        inc    ecx
        add    byte ptr [edx], al
        inc    ebx
        push   eax
        inc    esp
        add    byte ptr [eax + eax + 0x51], dl
        add    esi, dword ptr [eax]
        push   eax
        inc    edi
        push   ebp
        add    byte ptr [edi + edi*8 + 0x32], ah
        add    byte ptr [0x39003800], dh
        add    byte ptr [edx], bh
        add    byte ptr [ebx], bh
        db  0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_02DF:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ebx + 0x11], ah
        add    byte ptr [edx], dl
        dec    edi
        adc    eax, dword ptr [eax]
        adc    al, 0x4f
        adc    eax, 0x17631600
        add    byte ptr [eax], bl
        push   ebp
        sbb    dword ptr [eax], eax
        sbb    ah, byte ptr [ecx + 0x1b]
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        or     al, 0x21
        add    ah, byte ptr [edx]
        fbld   tbyte ptr [ebx]
        add    dword ptr [edi + ebx*8], esp
        and    eax, 0x270c2601
        add    ch, byte ptr [eax]
        ljmp   0xff2c:0xec2a0129
        sub    eax, 0x400036ff
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx + 0x50], al
        inc    esp
        add    byte ptr [eax + eax + 0x51], dl
        add    esi, dword ptr [eax]
        push   eax
        db  0C7h, 055h, 000h, 064h, 0FFh, 032h, 000h, 035h, 000h, 038h, 000h, 039h, 000h, 03Ah, 000h, 03Bh
        db  0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_0341:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ebx + 0x11], ah
        add    byte ptr [edx], dl
        dec    edi
        adc    eax, dword ptr [eax]
        adc    al, 0x4f
        adc    eax, 0x17631600
        add    byte ptr [eax], bl
        push   ebp
        sbb    dword ptr [eax], eax
        sbb    ah, byte ptr [ecx + 0x1b]
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        or     al, 0x21
        add    ah, byte ptr [edx]
        fbld   tbyte ptr [ebx]
        add    dword ptr [edi + ebx*8], esp
        and    eax, 0x270c2601
        add    ch, byte ptr [eax]
        ljmp   0xff2c:0xec2a0129
        sub    eax, 0x400036ff
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx - 0x60], al
        inc    esp
        add    byte ptr [eax + eax + 0x51], dl
        add    al, 0x70
        add    byte ptr [eax - 0x39], dl
        push   ebp
        add    byte ptr [edi + edi*8 + 0x32], ah
        add    byte ptr [0x39003800], dh
        add    byte ptr [edx], bh
        add    byte ptr [ebx], bh
        db  0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_03A3:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ebp + 0x7f120011], bl
        adc    eax, dword ptr [eax]
        adc    al, 0x7f
        adc    eax, 0x179d1600
        add    byte ptr [eax], bl
        xchg   dword ptr [ecx], ebx
        add    byte ptr [edx], bl
        pushfd 
        sbb    eax, dword ptr [eax]
        sbb    al, 0x40
        push   ds
        add    al, 0x20
        xor    byte ptr [ecx], ah
        add    esp, dword ptr [edx]
        jmp    dword ptr [ebx]
        db  002h, 024h, 0FFh, 025h, 002h, 026h, 030h, 027h, 003h, 028h, 000h, 029h, 003h, 02Ah, 008h, 02Ch
        db  0FFh, 02Dh, 0FFh, 036h, 000h, 040h, 000h, 041h, 000h, 042h, 000h, 043h, 040h, 044h, 000h, 054h
        db  00Dh, 051h, 002h, 070h, 000h, 050h, 00Fh, 055h, 000h, 064h, 0FFh, 032h, 000h, 035h, 000h, 038h
        db  000h, 039h, 000h, 03Ah, 000h, 03Bh, 0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_0405:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ebp + 0x7f120011], bl
        adc    eax, dword ptr [eax]
        adc    al, 0x7f
        adc    eax, 0x179d1600
        add    byte ptr [eax], bl
        xchg   dword ptr [ecx], ebx
        add    byte ptr [edx], bl
        pushfd 
        sbb    eax, dword ptr [eax]
        sbb    al, 0x40
        push   ds
        add    al, 0x20
        xor    byte ptr [ecx], ah
        add    esp, dword ptr [edx]
        jmp    dword ptr [ebx]
        db  002h, 024h, 0FFh, 025h, 002h, 026h, 030h, 027h, 003h, 028h, 000h, 029h, 003h, 02Ah, 008h, 02Ch
        db  0FFh, 02Dh, 0FFh, 036h, 000h, 040h, 000h, 041h, 000h, 042h, 000h, 043h, 080h, 044h, 000h, 054h
        db  00Dh, 051h, 003h, 070h, 000h, 050h, 00Fh, 055h, 000h, 064h, 0FFh, 032h, 000h, 035h, 000h, 038h
        db  000h, 039h, 000h, 03Ah, 000h, 03Bh, 0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_0467:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ebx + 0x63120011], al
        adc    eax, dword ptr [eax]
        adc    al, 0x63
        adc    eax, 0x17831600
        add    byte ptr [eax], bl
        imul   ebx, dword ptr [ecx], 0
        sbb    bh, byte ptr [ebx + 0x1b]
        add    byte ptr [eax], bl
        push   ds
        add    ah, byte ptr [eax]
        jae    loc_4_04AB
        add    ah, byte ptr [edx]
        push   edi
        and    eax, dword ptr [edx]
        and    al, 0x57
        and    eax, 0x27732602
        add    ch, byte ptr [eax]
        pop    eax
        sub    dword ptr [edx], eax
        sub    bl, byte ptr [esp + ebp - 1]
        sub    eax, 0x400036ff
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx + 0x64], al
loc_4_04AB:
        inc    esp
        add    byte ptr [ecx + eax + 0x70], dl
        add    byte ptr [ecx + 3], dl
        pop    eax
        dec    edi
        push   esp
        adc    byte ptr [eax + 7], 0x55
        add    byte ptr [eax], ah
        popal  
        add    byte ptr [edx], ah
        arpl   word ptr [eax], ax
        push   dword ptr fs:[edx]
        add    byte ptr [0x39003800], dh
        add    byte ptr [edx], bh
        add    byte ptr [ebx], bh
        db  0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_04D5:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ecx + 0x63120011], al
        adc    eax, dword ptr [eax]
        adc    al, 0x63
        adc    eax, 0x17811600
        add    byte ptr [eax], bl
        jo     0x505
        add    byte ptr [edx], bl
        jg     0x50b
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        cdq    
        and    dword ptr [edx], eax
        and    dl, byte ptr [edi + 0x23]
        add    ah, byte ptr [edi + edx*2]
        and    eax, 0x27992602
        add    ch, byte ptr [eax]
        jl     loc_4_0531
        add    ch, byte ptr [edx]
        sub    byte ptr [edi + edi*8], 0x2d
        push   dword ptr [esi]
        add    byte ptr [eax], al
        inc    ecx
        add    byte ptr [edx], al
        inc    ebx
        inc    esp
        add    byte ptr [ecx + eax + 0x70], dl
        add    byte ptr [ecx + 3], dl
        pop    eax
        arpl   word ptr [ecx + eax*4 + 0x50], dx
        pop    es
        push   ebp
        add    byte ptr [eax], ah
        popal  
        add    byte ptr [edx], ah
        arpl   word ptr [eax], ax
loc_4_0531:
        push   dword ptr fs:[edx]
        add    byte ptr [0x39003800], dh
        add    byte ptr [edx], bh
        add    byte ptr [ebx], bh
        db  0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_0543:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [edi + 0x7f120011], ah
        adc    eax, dword ptr [eax]
        adc    al, 0x7f
        adc    eax, 0x17a71600
        add    byte ptr [eax], bl
        mov    byte ptr [ecx], bl
        add    byte ptr [edx], bl
        cdq    
        sbb    eax, dword ptr [eax]
        sbb    al, 0
        push   ds
        add    byte ptr [eax], ah
        and    eax, 0xff220321
        and    eax, dword ptr [edx]
        and    al, 0xff
        and    eax, 0x27252602
        add    ebp, dword ptr [eax]
        add    ch, byte ptr [ecx]
        add    ebp, dword ptr [edx]
        or     byte ptr [edi + edi*8], ch
        sub    eax, 0x400036ff
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx - 0x80], al
        inc    esp
        add    byte ptr [ecx + eax + 0x70], dl
        add    byte ptr [ecx + 3], dl
        pop    eax
        adc    byte ptr [ecx + eax*4 + 0x50], 0xc7
        push   ebp
        add    byte ptr [eax], ah
        popal  
        add    byte ptr [edx], ah
        arpl   word ptr [eax], ax
        push   dword ptr fs:[edx]
        add    byte ptr [0x39003800], dh
        add    byte ptr [edx], bh
        add    byte ptr [ebx], bh
        db  0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_05B1:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ecx + 0x7f120011], ch
        adc    eax, dword ptr [eax]
        adc    al, 0x7f
        adc    eax, 0x17a91600
        add    byte ptr [eax], bl
        xchg   byte ptr [ecx], bl
        add    byte ptr [edx], bl
        test   eax, 0x1c001b
        push   ds
        add    byte ptr [eax], ah
        and    dword ptr es:[ebx], eax
        and    bh, bh
        and    eax, dword ptr [edx]
        and    al, 0xff
        and    eax, 0x27262602
        add    ebp, dword ptr [eax]
        jmp    ptr [ecx]
        db  002h, 02Ah, 007h, 02Ch, 0FFh, 02Dh, 0FFh, 036h, 000h, 040h, 000h, 041h, 000h, 042h, 000h, 043h
        db  080h, 044h, 000h, 054h, 001h, 070h, 000h, 051h, 003h, 058h, 08Eh, 054h, 081h, 050h, 007h, 055h
        db  000h, 060h, 000h, 061h, 000h, 062h, 000h, 063h, 000h, 064h, 0FFh, 032h, 000h, 035h, 000h, 038h
        db  000h, 039h, 000h, 03Ah, 000h, 03Bh, 0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_061F:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [edi + 0x7f120011], ch
        adc    eax, dword ptr [eax]
        adc    al, 0x7f
        adc    eax, 0x17af1600
        add    byte ptr [eax], bl
        xchg   byte ptr [ecx], bl
        add    byte ptr [edx], bl
        scasb  al, byte ptr es:[edi]
        sbb    eax, dword ptr [eax]
        sbb    al, 0
        push   ds
        add    byte ptr [eax], ah
        and    eax, 0xff220321
        and    eax, dword ptr [edx]
        and    al, 0xff
        and    eax, 0x27252602
        add    ebp, dword ptr [eax]
        jmp    ptr [ecx]
        db  002h, 02Ah, 007h, 02Ch, 0FFh, 02Dh, 0FFh, 036h, 000h, 040h, 000h, 041h, 000h, 042h, 000h, 043h
        db  080h, 044h, 000h, 054h, 001h, 070h, 000h, 051h, 003h, 058h, 095h, 054h, 081h, 050h, 007h, 055h
        db  000h, 060h, 000h, 061h, 000h, 062h, 000h, 063h, 000h, 064h, 0FFh, 032h, 000h, 035h, 000h, 038h
        db  000h, 039h, 000h, 03Ah, 000h, 03Bh, 0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_068D:
        push   eax
        add    dword ptr [eax], edx
        adc    cl, dl
        adc    dword ptr [eax], eax
        adc    bl, byte ptr [edi - 0x60ebffed]
        adc    eax, 0x17d11600
        add    byte ptr [eax], bl
        cmpsd  dword ptr [esi], dword ptr es:[edi]
        sbb    dword ptr [eax], eax
        sbb    cl, bh
        sbb    eax, dword ptr [eax]
        sbb    al, 0
        push   ds
        add    byte ptr [eax], ah
        inc    esp
        and    dword ptr [edx], eax
        jmp    dword ptr [ebx]
        db  003h, 024h, 0FFh, 025h, 003h, 026h, 044h, 027h, 004h, 028h, 002h, 029h, 004h, 02Ah, 00Ah, 02Ch
        db  0FFh, 02Dh, 0FFh, 036h, 000h, 040h, 000h, 041h, 000h, 042h, 000h, 043h, 0A0h, 044h, 000h, 054h
        db  001h, 070h, 000h, 051h, 003h, 058h, 091h, 054h, 081h, 050h, 04Fh, 055h, 000h, 060h, 000h, 061h
        db  000h, 062h, 000h, 063h, 000h, 064h, 0FFh, 032h, 000h, 035h, 000h, 038h, 000h, 039h, 000h, 03Ah
        db  000h, 03Bh, 0FFh, 03Ch, 0FFh, 03Dh, 0FFh
loc_4_06FB:
        jge    loc_4_06FF
        add    byte ptr [eax], al
loc_4_06FF:
        xor    dword ptr [eax], ecx
loc_4_0701:
        add    byte ptr [edx], 0x80
        add    dl, byte ptr [eax + 1]
        add    byte ptr [ebx + 8], bh        ; -> obj4:0x087B
        add    byte ptr [eax], al
        sub    eax, 0x28008
        add    al, 0xe0
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
        add    byte ptr [edi], ch            ; -> loc_4_092F
        or     dword ptr [eax], eax
        add    byte ptr [0x32008], ch
        add    al, 0x58
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    bl, ah                        ; -> obj4:0x09E3
        or     dword ptr [eax], eax
        add    byte ptr [0x40004], ch
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [0x2d00000a], bh     ; -> loc_4_0A3D
        or     byte ptr [eax], al
        add    al, 0
        add    al, 0
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    cl, dh                        ; -> obj4:0x0AF1
        or     al, byte ptr [eax]
        add    byte ptr [0x80050008], ch
        add    al, byte ptr [eax]
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0xb], cl      ; -> obj4:0x0B4B
        add    byte ptr [eax], al
        sub    eax, 0x50008
        or     al, al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ch, dl                        ; -> loc_4_08D5
        or     byte ptr [eax], al
        add    byte ptr [0x28010], ch
        or     al, ah
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
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x2d000009], cl ; -> loc_4_0989
        adc    byte ptr [eax], ah
        add    eax, dword ptr [eax + 6]
        pop    eax
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi + 0x2d00000a], dl ; -> loc_4_0A97
        adc    byte ptr [eax], al
        add    al, 0
        or     byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax + 1], dl
        push   eax
        add    byte ptr [eax], dl
        pop    edi
        adc    dword ptr [eax], eax
        adc    cl, byte ptr [edi + 0x13]
        add    byte ptr [edi + ecx*2], dl
        adc    eax, 0x175f1600
        add    byte ptr [eax], bl
        push   ebp
        sbb    dword ptr [eax], eax
        sbb    ah, byte ptr [ecx + 0x1b]
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        or     al, 0x21
        add    ah, byte ptr [edx]
        fbld   tbyte ptr [ebx]
        add    dword ptr [edi + ebx*8], esp
        and    eax, 0x270c2601
        add    ch, byte ptr [eax]
        ljmp   0xff2c:0xec2a0129
        sub    eax, 0x400036ff
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx - 0x80], al
        inc    esp
        add    byte ptr [eax + eax + 0x51], dl
        add    esi, dword ptr [eax]
        push   eax
        db  0C7h, 055h, 000h, 07Fh, 030h, 06Dh, 003h, 06Eh, 0DFh, 06Fh, 041h, 077h, 004h
loc_4_08D5:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [edi + 0x11], bl
        add    byte ptr [edx], dl
        dec    edi
        adc    eax, dword ptr [eax]
        adc    al, 0x4f
        adc    eax, 0x175f1600
        add    byte ptr [eax], bl
        push   ebp
        sbb    dword ptr [eax], eax
        sbb    ah, byte ptr [ecx + 0x1b]
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        or     al, 0x21
        add    ah, byte ptr [edx]
        fbld   tbyte ptr [ebx]
        add    dword ptr [edi + ebx*8], esp
        and    eax, 0x270c2601
        add    ch, byte ptr [eax]
        ljmp   0xff2c:0xec2a0129
        sub    eax, 0x400036ff
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx], al
        inc    esp
        add    dword ptr [ebp + ecx + 0x51], edx
        add    al, 0x70
        add    byte ptr [eax - 0x39], dl
        push   ebp
        add    byte ptr [edi + 0x30], bh
        insd   dword ptr es:[edi], dx
        add    ebp, dword ptr [esi + 0x3f]
        outsd  dx, dword ptr [esi]
        push   ebp
        ja     0x951
loc_4_092F:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [eax + 0x63120011], al
        adc    eax, dword ptr [eax]
        adc    al, 0x63
        adc    eax, 0x17801600
        add    byte ptr [eax], bl
        outsb  dx, byte ptr [esi]
        sbb    dword ptr [eax], eax
        sbb    bh, byte ptr [eax + 0x1b]
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        jae    loc_4_0973
        add    ah, byte ptr [edx]
        push   edi
        and    eax, dword ptr [edx]
        and    al, 0x57
        and    eax, 0x27732602
        add    ch, byte ptr [eax]
        pop    eax
        sub    dword ptr [edx], eax
        sub    bl, byte ptr [esp + ebp]
        jmp    ptr [0x40003607]
        db  000h, 041h, 000h, 042h, 000h, 043h, 080h
loc_4_0973:
        inc    esp
        add    byte ptr [ebp + ecx + 0x51], dl
        add    esi, dword ptr [eax]
        push   eax
        pop    es
        push   ebp
        add    byte ptr [edi + 0x12], bh
        insd   dword ptr es:[edi], dx
        add    ebp, dword ptr [esi - 0x61]
        outsd  dx, dword ptr [esi]
        push   ecx
        ja     loc_4_098D
loc_4_0989:
        push   eax
        add    dword ptr [eax], edx
loc_4_098D:
        adc    ah, bh
        adc    dword ptr [eax], eax
        adc    al, bh
        adc    eax, dword ptr [eax]
        adc    al, 0xc7
        adc    eax, 0x17fc1600
        add    byte ptr [eax], bl
        rcr    dword ptr [ecx], 1
        add    byte ptr [edx], bl
        fstp   qword ptr [ebx]
        add    byte ptr [eax + eax], bl
        push   ds
        add    byte ptr [eax], ah
        and    byte ptr [ecx], 2
        and    dl, byte ptr [edi + 0x23]
        add    ah, byte ptr [edi + edx*2]
        and    eax, 0x27802602
        add    ch, byte ptr [eax]
        pop    esi
        sub    dword ptr [edx], eax
        sub    cl, byte ptr [0xff2dff2c]
        add    byte ptr ss:[eax], al
        inc    ecx
        add    byte ptr [edx], al
        inc    ebx
        enter  0x44, 0x54
        or     eax, 0x700351
        push   eax
        pop    es
        push   ebp
        add    byte ptr [edi + 0x12], bh
        insd   dword ptr es:[edi], dx
        add    ebp, dword ptr [esi - 0x61]
        outsd  dx, dword ptr [esi]
        adc    eax, 0x1500077
        push   eax
        add    byte ptr [eax], dl
        movsb  byte ptr es:[edi], byte ptr [esi]
        adc    dword ptr [eax], eax
        adc    bh, byte ptr [edi + 0x13]
        add    byte ptr [edi + edi*2], dl
        adc    eax, 0x17a41600
        add    byte ptr [eax], bl
        mov    bl, byte ptr [ecx]
        add    byte ptr [edx], bl
        xchg   ebx, eax
        sbb    eax, dword ptr [eax]
        sbb    al, 0x40
        push   ds
        add    al, 0x20
        and    eax, 0xff220321
        and    eax, dword ptr [edx]
        and    al, 0xff
        and    eax, 0x27252602
        add    ebp, dword ptr [eax]
        add    byte ptr [ecx], ch
        add    ebp, dword ptr [edx]
        or     byte ptr [edi + edi*8], ch
        sub    eax, 0x400036ff
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx + 0x40], al
        inc    esp
        add    byte ptr [ebp + ecx + 0x51], dl
        add    dh, byte ptr [eax]
        push   eax
        andnps xmm0, xmmword ptr [eax]
        jg     loc_4_0A67
        insd   dword ptr es:[edi], dx
        add    ebp, dword ptr [esi + 0x1f]
        outsd  dx, dword ptr [esi]
        push   ebp
        ja     0xa71
loc_4_0A3D:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [ecx + edx + 0x137f1200], ah
        add    byte ptr [edi + edi*2], dl
        adc    eax, 0x17a41600
        add    byte ptr [eax], bl
        mov    bl, byte ptr [ecx]
        add    byte ptr [edx], bl
        xchg   ebx, eax
        sbb    eax, dword ptr [eax]
        sbb    al, 0x40
        push   ds
        add    al, 0x20
        and    eax, 0xff220321
        and    eax, dword ptr [edx]
        and    al, 0xff
loc_4_0A67:
        and    eax, 0x27252602
        add    ebp, dword ptr [eax]
        add    byte ptr [ecx], ch
        add    ebp, dword ptr [edx]
        or     byte ptr [edi + edi*8], ch
        sub    eax, 0x400036ff
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx - 0x80], al
        inc    esp
        add    byte ptr [ebp + ecx + 0x51], dl
        add    esi, dword ptr [eax]
        push   eax
        pop    ss
        push   ebp
        add    byte ptr [edi + 0x32], bh
        insd   dword ptr es:[edi], dx
        add    ebp, dword ptr [esi + 0x5f]
        outsd  dx, dword ptr [esi]
        ja     0xacb
loc_4_0A97:
        push   eax
        add    dword ptr [eax], edx
        adc    byte ptr [edi + 0x7f120011], ah
        adc    eax, dword ptr [eax]
        adc    al, 0x7f
        adc    eax, 0x17a71600
        add    byte ptr [eax], bl
        sbb    byte ptr [ecx], 0
        sbb    dl, byte ptr [ebx + 0x401c001b]
        push   ds
        add    al, 0x20
        and    eax, 0xff220321
        and    eax, dword ptr [edx]
        and    al, 0xff
        and    eax, 0x27252602
        add    ebp, dword ptr [eax]
        add    ch, byte ptr [ecx]
        add    ebp, dword ptr [edx]
        or     byte ptr [edi + edi*8], ch
        sub    eax, 0x40003607
        add    byte ptr [ecx], al
        inc    edx
        add    byte ptr [ebx], al
        inc    esp
        add    dword ptr [ebp + ecx + 0x51], edx
        add    al, 0x70
        add    byte ptr [eax + 0xb], dl
        push   ebp
        add    byte ptr [edi], bh
        insd   dword ptr es:[edi], dx
        add    ebp, dword ptr [esi + 0x5c]
        outsd  dx, dword ptr [esi]
        je     0xb67
        push   eax
        add    dword ptr [eax], edx
        adc    ah, cl
        adc    dword ptr [eax], eax
        adc    bl, byte ptr [edi - 0x60ebffed]
        adc    eax, 0x17cc1600
        add    byte ptr [eax], bl
        mov    al, 0x19
        add    byte ptr [edx], bl
        mov    esi, 0x401c001b
        push   ds
        add    byte ptr [eax], ah
        loopne 0xb35
        add    esp, dword ptr [edx]
        mov    edi, 0xbf240323
        and    eax, 0x27e02603
        add    ebp, dword ptr [eax]
        shr    dword ptr [ecx], 3
        sub    al, byte ptr [0x72dff2c]
        add    byte ptr ss:[eax], al
        inc    ecx
        add    byte ptr [edx], al
        inc    ebx
        add    byte ptr [ecx + eax + 0x54], al
        or     eax, 0x700351
        push   eax
        add    byte ptr [ebp], dl
        jg     0xb79
        insd   dword ptr es:[edi], dx
        add    ebp, dword ptr [esi + 1]
        outsd  dx, dword ptr [esi]
        ja     loc_4_0BC1
        adc    al, 0x50
        add    dword ptr [eax], edx
        adc    ah, cl
        adc    dword ptr [eax], eax
        adc    bl, byte ptr [edi - 0x60ebffed]
        adc    eax, 0x17cc1600
        add    byte ptr [eax], bl
        mov    al, 0x19
        add    byte ptr [edx], bl
        mov    esi, 0x401c001b
        push   ds
        add    byte ptr [eax], ah
        loopne 0xb8f
        add    esp, dword ptr [edx]
        mov    edi, 0xbf240323
        and    eax, 0x27e02603
        add    ebp, dword ptr [eax]
        shr    dword ptr [ecx], 3
        sub    al, byte ptr [0x72dff2c]
        add    byte ptr ss:[eax], al
        inc    ecx
        add    byte ptr [edx], al
        inc    ebx
        add    byte ptr [ecx + eax + 0x54], al
        or     eax, 0x700351
        push   eax
        add    byte ptr [ebp], dl
        jg     loc_4_0BD3
        insd   dword ptr es:[edi], dx
        add    ebp, dword ptr [esi + 1]
        outsd  dx, dword ptr [esi]
        ja     loc_4_0C1B
        and    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        sub    byte ptr [eax], al
        add    byte ptr [eax], dh
        add    byte ptr [eax], al
        cmp    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0BC1:
        add    byte ptr [eax], cl
        add    byte ptr [eax], dl
        add    byte ptr [eax], bl
        add    byte ptr [eax], ah
        add    byte ptr [eax], ch
        add    byte ptr [eax], dh
loc_4_0BD3:
        add    byte ptr [eax], bh
        add    byte ptr [eax + 0x880000], al
        add    byte ptr [eax + 0x980000], dl
        add    byte ptr [eax + 0xa80000], ah
        add    byte ptr [eax + 0xb80000], dh
        add    al, al
        add    byte ptr [eax], al
        enter  0, -0x30
        add    byte ptr [eax], al
        fadd   dword ptr [eax]
        add    al, ah
        add    byte ptr [eax], al
        call   0xf00c03
        add    al, bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        xor    byte ptr [eax], al
loc_4_0C1B:
        add    byte ptr [eax], bh
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        dec    eax
        add    byte ptr [eax], al
        push   eax
        add    byte ptr [eax], al
        pop    eax
        add    byte ptr [eax], al
        pushal 
        add    byte ptr [eax], al
        push   0x700000
        add    byte ptr [eax], bh
        add    byte ptr [eax + 0x880000], al
        add    byte ptr [eax + 0x980000], dl
        add    byte ptr [eax + 0xa80000], ah
        add    byte ptr [eax + 0xb80000], dh
        add    al, al
        add    byte ptr [eax], al
        enter  0, -0x30
        add    byte ptr [eax], al
        fadd   dword ptr [eax]
        add    al, ah
        add    byte ptr [eax], al
        call   0xf00c63
        add    al, bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        xor    byte ptr [eax], al
        add    byte ptr [eax], bh
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        dec    eax
        add    byte ptr [eax], al
        push   eax
        add    byte ptr [eax], al
        pop    eax
        add    byte ptr [eax], al
        pushal 
        add    byte ptr [eax], al
        push   0x700000
        add    byte ptr [eax], bh
        add    byte ptr [eax + 0x880000], al
        add    byte ptr [eax + 0x980000], dl
        add    byte ptr [eax + 0xa80000], ah
        add    byte ptr [eax + 0xb80000], dh
        add    al, al
        add    byte ptr [eax], al
        enter  0, -0x30
        add    byte ptr [eax], al
        fadd   dword ptr [eax]
        add    al, ah
        add    byte ptr [eax], al
        call   0xf00cc3
        add    al, bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        adc    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        xor    byte ptr [eax], al
        add    byte ptr [eax], bh
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        dec    eax
        add    byte ptr [eax], al
        push   eax
        add    byte ptr [eax], al
        pop    eax
        add    byte ptr [eax], al
        pushal 
        add    byte ptr [eax], al
        push   0x700000
        add    byte ptr [eax], bh
        add    byte ptr [eax + 0x880000], al
        add    byte ptr [eax + 0x980000], dl
        add    byte ptr [eax + 0xa80000], ah
        add    byte ptr [eax + 0xb80000], dh
        add    al, al
        add    byte ptr [eax], al
        enter  0, -0x30
        add    byte ptr [eax], al
        fadd   dword ptr [eax]
        add    al, ah
        add    byte ptr [eax], al
        call   0xf00d23
        add    al, bh
loc_4_0D25:
        pushal 
        cmp    al, 0x13
        je     loc_4_0D41
        cmp    al, 0xa
        ja     loc_4_0D3A
        movzx  eax, al
        call   sub_4_00AD
        jmp    loc_4_0D40
loc_4_0D3A:
        mov    word ptr [ebp + 0x1c], 0x100
loc_4_0D40:
        stc    
loc_4_0D41:
        popal  
        ret    
loc_4_0D43:
        mov    ah, 0x38
        mov    al, 0
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        je     loc_4_0E36
        cld    
        mov    cl, 1
        mov    edx, dword ptr [eax]
        cmp    dl, 0x56
        je     loc_4_0D65
        cmp    dl, 0x76
        jne    loc_4_0D83
loc_4_0D65:
        cmp    dh, 0x42
        je     loc_4_0D6F
        cmp    dh, 0x62
        jne    loc_4_0D83
loc_4_0D6F:
        shr    edx, 0x10
        cmp    dl, 0x45
        je     loc_4_0D7C
        cmp    dl, 0x65
        jne    loc_4_0D83
loc_4_0D7C:
        cmp    dh, 0x32
        jne    loc_4_0D83
        mov    cl, 2
loc_4_0D83:
        mov    dword ptr [eax], 0x41534556
        mov    word ptr [eax + 4], 0x200
        mov    si, word ptr [ebp + 0x38]
        shl    esi, 0x10
        lea    edx, [eax + 0x14]
        mov    si, 0x14
        cmp    cl, 1
        je     loc_4_0DAC
        lea    edx, [eax + 0x100]
        mov    si, 0x100
loc_4_0DAC:
        mov    dword ptr [eax + 6], esi
        mov    dword ptr [edx], 0x414758
        mov    byte ptr [edx + 3], 0
        mov    dword ptr [eax + 0xa], 2
        push   ecx
        mov    si, word ptr [ebp + 0x38]
        shl    esi, 0x10
        lea    edi, [eax + 0x18]
        mov    si, 0x18
        cmp    cl, 1
        je     loc_4_0DDB
        lea    edi, [eax + 0x22]
        mov    si, 0x22
loc_4_0DDB:
        mov    dword ptr [eax + 0xe], esi
        mov    esi, 0xd9                     ; -> obj4:0x00D9
        mov    ecx, 0xe
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        jne    loc_4_0DFE
        mov    esi, 0xe7                     ; -> obj4:0x00E7
        mov    ecx, 0x16
loc_4_0DFE:
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        mov    edx, dword ptr [0xf4]         ; -> obj1:0x00F4
        shr    edx, 0x10
        mov    word ptr [eax + 0x12], dx
        cmp    cl, 1
        je     loc_4_0E2E
        mov    word ptr [eax + 0x14], 0x200
        mov    dword ptr [eax + 0x16], 0
        mov    dword ptr [eax + 0x1a], 0
        mov    dword ptr [eax + 0x1e], 0
loc_4_0E2E:
        mov    word ptr [ebp + 0x1c], 0x4f
        jmp    loc_4_0E3C
loc_4_0E36:
        mov    word ptr [ebp + 0x1c], 0x14f
loc_4_0E3C:
        ret    
loc_4_0E3D:
        mov    ah, 0x38
        mov    al, 0
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        je     loc_4_0FB3
        cld    
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 0x40
        rep stosd dword ptr es:[edi], eax
        mov    ax, word ptr [ebp + 0x18]
        and    ax, 0x3f7f
        mov    ecx, 6
        mov    edi, 0xd9                     ; -> obj4:0x00D9
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        jne    loc_4_0E85
        mov    ecx, 0xa
        mov    edi, 0xe7                     ; -> obj4:0x00E7
loc_4_0E85:
        repne scasw ax, word ptr es:[edi]
        movzx  ecx, ax
        pop    eax
        jne    loc_4_0FB3
        or     word ptr [eax], 0x3b
        cmp    cl, 3
        jne    loc_4_0EAC
        cmp    dword ptr [0x110], 0x8fdb     ; -> obj1:0x0110
        jne    loc_4_0EAC
loc_4_0EA7:
        and    byte ptr [eax], 0xfe
        jmp    loc_4_0EE9
loc_4_0EAC:
        cmp    cl, 4
        jbe    loc_4_0EE9
        cmp    dword ptr [0xf4], 0x100000    ; -> obj1:0x00F4
        jb     loc_4_0EA7
        cmp    cl, 7
        je     loc_4_0ECC
        cmp    cl, 0x14
        je     loc_4_0ECC
        cmp    cl, 0x17
        jne    loc_4_0EE9
loc_4_0ECC:
        cmp    dword ptr [0xf4], 0x200000    ; -> obj1:0x00F4
        jb     loc_4_0EA7
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        je     loc_4_0EE9
        cmp    cl, 0x11
        ja     loc_4_0EA7
loc_4_0EE9:
        mov    byte ptr [eax + 2], 7
        mov    byte ptr [eax + 3], 0
        mov    word ptr [eax + 4], 0x40
        mov    word ptr [eax + 6], 0x40
        mov    word ptr [eax + 8], 0xa000
        mov    word ptr [eax + 0xa], 0
        mov    esi, dword ptr [0xf8]         ; -> obj1:0x00F8
        mov    dword ptr [eax + 0xc], esi
        movzx  esi, cl
        shl    esi, 4
        mov    edi, 0xfd                     ; -> loc_4_00FD
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        jne    loc_4_0F2E
        mov    edi, 0x6fb                    ; -> loc_4_06FB
loc_4_0F2E:
        add    edi, esi
        mov    dx, word ptr [edi + 6]
        mov    word ptr [eax + 0x12], dx
        mov    dx, word ptr [edi + 8]
        mov    word ptr [eax + 0x10], dx
        mov    dx, word ptr [edi + 0xa]
        mov    word ptr [eax + 0x14], dx
        mov    dl, byte ptr [edi + 5]
        mov    byte ptr [eax + 0x19], dl
        mov    byte ptr [eax + 0x16], 0
        mov    byte ptr [eax + 0x17], 0
        mov    byte ptr [eax + 0x18], 1
        mov    byte ptr [eax + 0x1b], 4
        mov    byte ptr [eax + 0x1a], 1
        mov    byte ptr [eax + 0x1c], 0
        mov    byte ptr [eax + 0x1d], 0
        mov    byte ptr [eax + 0x1e], 1
        cmp    cl, 0x11
        jne    loc_4_0F8B
        mov    byte ptr [eax + 0x1f], 5
        mov    byte ptr [eax + 0x20], 0xb
        mov    byte ptr [eax + 0x21], 6
        mov    byte ptr [eax + 0x22], 5
        mov    byte ptr [eax + 0x23], 5
        mov    byte ptr [eax + 0x24], 0
loc_4_0F8B:
        push   eax
        movzx  edx, word ptr [eax + 0x10]
        movzx  eax, word ptr [eax + 0x14]
        mul    edx
        mov    edx, eax
        pop    eax
        mov    dword ptr [eax + 0x2c], edx
        mov    ecx, dword ptr [0xf4]         ; -> obj1:0x00F4
        sub    ecx, edx
        shr    ecx, 0xa
        mov    word ptr [eax + 0x30], cx
        mov    word ptr [ebp + 0x1c], 0x4f
        jmp    loc_4_0FB9
loc_4_0FB3:
        mov    word ptr [ebp + 0x1c], 0x14f
loc_4_0FB9:
        ret    
loc_4_0FBA:
        cld    
        movzx  eax, word ptr [ebp + 0x10]
        test   ah, 4
        jne    loc_4_12A3
        and    ax, 0x3f7f
        cmp    ax, 0x13
        jbe    loc_4_128A
        mov    ecx, 6
        mov    edi, 0xd9                     ; -> obj4:0x00D9
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        jne    loc_4_0FF6
        mov    ecx, 0xa
        mov    edi, 0xe7                     ; -> obj4:0x00E7
loc_4_0FF6:
        repne scasw ax, word ptr es:[edi]
        jne    loc_4_12A3
        cmp    al, 3
        jne    loc_4_1013
        cmp    dword ptr [0x110], 0x8fdb     ; -> obj1:0x0110
        je     loc_4_12A3
loc_4_1013:
        cmp    al, 4
        jbe    loc_4_1057
        cmp    dword ptr [0xf4], 0x100000    ; -> obj1:0x00F4
        jb     loc_4_12A3
        cmp    al, 7
        je     loc_4_1033
        cmp    al, 0x14
        je     loc_4_1033
        cmp    al, 0x17
        jne    loc_4_1057
loc_4_1033:
        cmp    dword ptr [0xf4], 0x200000    ; -> obj1:0x00F4
        jb     loc_4_12A3
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        je     loc_4_1057
        cmp    al, 0x11
        ja     loc_4_12A3
loc_4_1057:
        sub    eax, 0x100
        shl    eax, 4
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        jne    loc_4_108A
        mov    cl, byte ptr [ebp + 0x10]
        cmp    cl, 4
        je     loc_4_108A
        cmp    cl, 5
        je     loc_4_108A
        mov    esi, dword ptr [eax + 0x6fb]  ; -> loc_4_06FB
        movzx  ecx, byte ptr [eax + 0x6ff]   ; -> loc_4_06FF
        jmp    loc_4_111D
loc_4_108A:
        mov    esi, dword ptr [eax + 0xfd]   ; -> loc_4_00FD
        movzx  ecx, byte ptr [eax + 0x101]   ; -> loc_4_0101
        cmp    ebx, dword ptr [0xe8]         ; -> obj1:0x00E8
        jne    loc_4_111D
        cmp    dword ptr [0x110], 0x8fdb     ; -> obj1:0x0110
        je     loc_4_111D
        cmp    dword ptr [0x114], 0x3c       ; -> obj1:0x0114
        jb     loc_4_111D
        movzx  eax, byte ptr [ebp + 0x10]
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        jne    loc_4_10DF
        cmp    dword ptr [0x114], 0x48       ; -> obj1:0x0114
        jb     loc_4_111D
        shl    eax, 4
        mov    esi, dword ptr [eax + 0x6fb]  ; -> loc_4_06FB
        movzx  ecx, byte ptr [eax + 0x6ff]   ; -> loc_4_06FF
        jmp    loc_4_111D
loc_4_10DF:
        cmp    al, 3
        jne    loc_4_10F3
        cmp    dword ptr [0x114], 0x48       ; -> obj1:0x0114
        jb     loc_4_111D
        mov    esi, 0x4d5                    ; -> loc_4_04D5
        jmp    loc_4_111D
loc_4_10F3:
        cmp    al, 5
        jne    loc_4_111D
        mov    esi, 0x543                    ; -> loc_4_0543
        mov    ecx, 0x37
        cmp    dword ptr [0x114], 0x48       ; -> obj1:0x0114
        jb     loc_4_111D
        mov    esi, 0x5b1                    ; -> loc_4_05B1
        cmp    dword ptr [0x114], 0x4b       ; -> obj1:0x0114
        jb     loc_4_111D
        mov    esi, 0x61f                    ; -> loc_4_061F
loc_4_111D:
        mov    eax, 0x12
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    eax, dword ptr [ebx + 4]
        add    eax, 0xa0000
        mov    dl, 0x10
loc_4_1134:
        mov    dword ptr [eax], 0xf0f0f0f
        add    eax, 0x1000
        dec    dl
        jne    loc_4_1134
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        je     loc_4_1157
        mov    edx, 0x3c3
        xor    al, al
        out    dx, al
loc_4_1157:
        mov    edx, dword ptr [0xfc]         ; -> obj1:0x00FC
        add    dl, 4
        xor    al, al
        out    dx, al
        inc    dl
        mov    al, 0xff
        out    dx, al
        sub    dl, 5
        mov    al, 4
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        jne    loc_4_117B
        mov    al, 5
loc_4_117B:
        out    dx, al
        inc    dl
        mov    al, 1
        out    dx, al
        add    dl, 5
        xor    al, al
        out    dx, al
        add    dl, 2
        out    dx, al
        inc    dl
        mov    al, 3
        cmp    byte ptr [ebp + 0x10], 0x11
        je     loc_4_119F
        mov    al, 0xa
        cmp    byte ptr [ebp + 0x10], 4
        je     loc_4_119F
        mov    al, 3
loc_4_119F:
        out    dx, al
        inc    dl
        rep outsw dx, word ptr [esi]
        cmp    byte ptr [ebp + 0x10], 0x11
        jb     loc_4_1211
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        jne    loc_4_11D5
        mov    edx, 0x3c6
        mov    al, 0xff
        out    dx, al
        mov    edx, 0x216a
        mov    al, 0x51
        out    dx, al
        mov    edx, 0x3c6
        in     al, dx
        in     al, dx
        in     al, dx
        in     al, dx
        mov    al, 0xe0
        out    dx, al
        jmp    loc_4_1211
loc_4_11D5:
        mov    esi, 0xba5                    ; -> obj4:0x0BA5
        mov    cl, 0x80
        mov    al, 0x55
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        test   al, 0x80
        mov    ch, 0
        jne    loc_4_11EC
        mov    ch, 0x80
loc_4_11EC:
        mov    ax, 0x66
        out    dx, ax
loc_4_11F2:
        mov    al, 0x60
        mov    ah, ch
        out    dx, ax
        mov    al, 0x65
        mov    ah, byte ptr [esi]
        out    dx, ax
        mov    ah, byte ptr [esi + 1]
        out    dx, ax
        mov    ah, byte ptr [esi + 2]
        out    dx, ax
        add    esi, 3
        inc    ch
        dec    cl
        jne    loc_4_11F2
loc_4_1211:
        mov    ax, word ptr [ebp + 0x10]
        mov    edi, ebx
        add    edi, dword ptr [0xf0]         ; -> obj1:0x00F0
        mov    word ptr [edi], ax
        cmp    ebx, dword ptr [0xe8]         ; -> obj1:0x00E8
        je     loc_4_1255
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        je     loc_4_1255
        mov    edx, 0x3c7
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        mov    edx, 0x3c8
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        mov    edx, 0x3c9
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
loc_4_1255:
        test   byte ptr [edi + 1], 0x80
        jne    loc_4_129B
        mov    esi, dword ptr [0xf4]         ; -> obj1:0x00F4
        mov    edx, dword ptr [0x100]        ; -> obj1:0x0100
        xor    eax, eax
        out    dx, al
loc_4_126A:
        push   eax
        mov    edi, dword ptr [0xec]         ; -> obj1:0x00EC
        mov    ecx, 0x4000
        xor    eax, eax
        rep stosd dword ptr es:[edi], eax
        pop    eax
        inc    eax
        out    dx, al
        sub    esi, 0x10000
        jg     loc_4_126A
        xor    eax, eax
        out    dx, al
        jmp    loc_4_129B
loc_4_128A:
        call   7
        movzx  eax, byte ptr [ebp + 0x10]
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_4_129B:
        mov    word ptr [ebp + 0x1c], 0x4f
        jmp    loc_4_12A9
loc_4_12A3:
        mov    word ptr [ebp + 0x1c], 0x14f
loc_4_12A9:
        ret    
loc_4_12AA:
        mov    edi, ebx
        add    edi, dword ptr [0xf0]         ; -> obj1:0x00F0
        mov    ax, word ptr [edi]
        mov    word ptr [ebp + 0x10], ax
        mov    word ptr [ebp + 0x1c], 0x4f
        ret    
loc_4_12C0:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_4_12CC
        mov    word ptr [ebp + 0x10], 0
loc_4_12CC:
        mov    word ptr [ebp + 0x1c], 0x4f
        ret    
loc_4_12D3:
        mov    edx, dword ptr [0x100]        ; -> obj1:0x0100
        cmp    byte ptr [ebp + 0x11], 0
        je     loc_4_12E8
        xor    ah, ah
        in     al, dx
        mov    word ptr [ebp + 0x14], ax
        jmp    loc_4_12EC
loc_4_12E8:
        mov    al, byte ptr [ebp + 0x14]
        out    dx, al
loc_4_12EC:
        mov    word ptr [ebp + 0x1c], 0x4f
        ret    
loc_4_12F3:
        movzx  ecx, byte ptr [ebp + 0x10]
        cmp    cl, 4
        jae    loc_4_137A
        mov    edi, ebx
        add    edi, dword ptr [0xf0]         ; -> obj1:0x00F0
        movzx  edi, word ptr [edi]
        cmp    edi, 0x117
        ja     loc_4_137A
        sub    edi, 0x100
        jl     loc_4_137A
        shl    edi, 4
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        jne    loc_4_1336
        movzx  esi, word ptr [edi + 0x701]   ; -> loc_4_0701
        movzx  edi, word ptr [edi + 0x703]   ; -> obj4:0x0703
        jmp    loc_4_1344
loc_4_1336:
        movzx  esi, word ptr [edi + 0x103]   ; -> loc_4_0103
        movzx  edi, word ptr [edi + 0x105]   ; -> obj4:0x0105
loc_4_1344:
        or     esi, esi
        je     loc_4_137A
        mov    eax, dword ptr [0xf4]         ; -> obj1:0x00F4
        xor    edx, edx
        div    edi
        movzx  edx, word ptr [ebp + 0x18]
        mov    word ptr [ebp + 0x10], di
        mov    word ptr [ebp + 0x18], si
        mov    word ptr [ebp + 0x14], ax
        jmp    loc_4_1381
loc_4_1368:
        cmp    edx, esi
        jne    loc_4_137A
        jmp    loc_4_1372
loc_4_136E:
        cmp    edx, edi
        jne    loc_4_137A
loc_4_1372:
        mov    word ptr [ebp + 0x1c], 0x4f
        jmp    loc_4_1380
loc_4_137A:
        mov    word ptr [ebp + 0x1c], 0x14f
loc_4_1380:
        ret    
loc_4_1381:
        push   0x72000013                    ; -> loc_4_1372
        adc    eax, dword ptr [eax]
        add    byte ptr [esi + 0x13], ch     ; -> loc_4_136E
        add    byte ptr [eax], al
        jb     0x13a2
        add    byte ptr [eax], al
loc_4_1391:
        cmp    byte ptr [ebp + 0x10], 1
        jne    loc_4_13AF
        mov    byte ptr [ebp + 0x11], 0
        mov    word ptr [ebp + 0x18], 0
        mov    word ptr [ebp + 0x14], 0
        mov    word ptr [ebp + 0x1c], 0x4f
        jmp    loc_4_13B5
loc_4_13AF:
        mov    word ptr [ebp + 0x1c], 0x14f
loc_4_13B5:
        ret    
loc_4_13B6:
        cmp    byte ptr [ebp + 0x10], 1
        jne    loc_4_13C8
        mov    byte ptr [ebp + 0x11], 6
        mov    word ptr [ebp + 0x1c], 0x4f
        jmp    loc_4_13CE
loc_4_13C8:
        mov    word ptr [ebp + 0x1c], 0x14f
loc_4_13CE:
        ret    
loc_4_13CF:
        mov    cl, byte ptr [ebp + 0x10]
        and    cl, 0x7f
        or     cl, cl
        je     loc_4_1466
        cmp    cl, 1
        jne    loc_4_14E7
        mov    ah, 0x38
        mov    al, 0
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        je     loc_4_14DF
        mov    edi, eax
        movzx  ecx, word ptr [ebp + 0x14]
        movzx  esi, word ptr [ebp + 0x18]
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        je     loc_4_1443
        mov    edx, dword ptr [0x104]        ; -> obj1:0x0104
        mov    eax, 0x66
        out    dx, ax
loc_4_141C:
        mov    al, 0x60
        mov    ah, cl
        out    dx, ax
        mov    al, 0x65
        out    dx, al
        mov    byte ptr [edi], cl
        inc    dl
        in     al, dx
        mov    byte ptr [edi + 1], al
        in     al, dx
        mov    byte ptr [edi + 2], al
        in     al, dx
        mov    byte ptr [edi + 3], al
        dec    dl
        add    edi, 4
        inc    ecx
        dec    esi
        jne    loc_4_141C
        jmp    loc_4_14D7
loc_4_1443:
        mov    edx, 0x3c7
loc_4_1448:
        mov    al, cl
        out    dx, al
        mov    dl, 0xc9
        mov    byte ptr [edi], cl
        in     al, dx
        mov    byte ptr [edi + 1], al
        in     al, dx
        mov    byte ptr [edi + 2], al
        in     al, dx
        mov    byte ptr [edi + 3], al
        mov    dl, 0xc7
        add    edi, 4
        inc    ecx
        dec    esi
        jne    loc_4_1448
        jmp    loc_4_14D7
loc_4_1466:
        mov    ah, 0x38
        mov    al, 0
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        je     loc_4_14DF
        mov    esi, eax
        movzx  ecx, word ptr [ebp + 0x14]
        movzx  edi, word ptr [ebp + 0x18]
        cmp    dword ptr [0x110], 0x8fd9     ; -> obj1:0x0110
        je     loc_4_14B8
        mov    edx, dword ptr [0x104]        ; -> obj1:0x0104
        mov    eax, 0x66
        out    dx, ax
loc_4_1498:
        mov    al, 0x60
        mov    ah, cl
        out    dx, ax
        mov    al, 0x65
        mov    ah, byte ptr [esi + 1]
        out    dx, ax
        mov    ah, byte ptr [esi + 2]
        out    dx, ax
        mov    ah, byte ptr [esi + 3]
        out    dx, ax
        add    esi, 4
        inc    ecx
        dec    edi
        jne    loc_4_1498
        jmp    loc_4_14D7
loc_4_14B8:
        mov    edx, 0x3c8
loc_4_14BD:
        mov    al, cl
        out    dx, al
        inc    dl
        mov    al, byte ptr [esi + 1]
        out    dx, al
        mov    al, byte ptr [esi + 2]
        out    dx, al
        mov    al, byte ptr [esi + 3]
        out    dx, al
        dec    dl
        add    esi, 4
        inc    ecx
        dec    edi
        jne    loc_4_14BD
loc_4_14D7:
        mov    word ptr [ebp + 0x1c], 0x4f
        jmp    loc_4_14ED
loc_4_14DF:
        mov    word ptr [ebp + 0x1c], 0x14f
        jmp    loc_4_14ED
loc_4_14E7:
        mov    word ptr [ebp + 0x1c], 0x24f
loc_4_14ED:
        ret    
loc_4_14EE:
        mov    word ptr [ebp + 0x1c], 0x14f
        ret    
loc_4_14F5:
        mov    ecx, dword ptr [0xf4]         ; -> obj1:0x00F4
        stc    
        ret    
loc_4_14FD:
        push   esi
        push   edi
        cmp    ebx, dword ptr [0xe8]         ; -> obj1:0x00E8
        je     loc_4_15A9
        cmp    dl, 0xc6
        je     loc_4_15A9
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        jne    loc_4_15A9
        mov    edi, ebx
        add    edi, dword ptr [0xf0]         ; -> obj1:0x00F0
        movzx  esi, word ptr [edi]
        and    esi, 0xffff3f7f
        cmp    esi, 0x100
        jb     loc_4_15A9
        cmp    dl, 0xc7
        jne    loc_4_155D
        mov    edx, dword ptr [0x104]        ; -> obj1:0x0104
        mov    cl, al
        mov    ax, 0x66
        out    dx, ax
        mov    ah, cl
        mov    al, 0x62
        out    dx, ax
        mov    byte ptr [edi + 3], ah
        mov    byte ptr [edi + 5], 0
        jmp    loc_4_15AC
loc_4_155D:
        cmp    dl, 0xc8
        jne    loc_4_157F
        mov    edx, dword ptr [0x104]        ; -> obj1:0x0104
        mov    cl, al
        mov    ax, 0x66
        out    dx, ax
        mov    ah, cl
        mov    al, 0x60
        out    dx, ax
        mov    byte ptr [edi + 2], ah
        mov    byte ptr [edi + 4], 0
        jmp    loc_4_15AC
loc_4_157F:
        mov    edx, dword ptr [0x104]        ; -> obj1:0x0104
        mov    cl, al
        cmp    byte ptr [edi + 4], 3
        jb     loc_4_159B
        mov    byte ptr [edi + 4], 0
        inc    byte ptr [edi + 2]
        mov    al, 0x60
        mov    ah, byte ptr [edi + 2]
        out    dx, ax
loc_4_159B:
        mov    ah, cl
        shl    ah, 2
        mov    al, 0x65
        out    dx, ax
        inc    byte ptr [edi + 4]
        jmp    loc_4_15AC
loc_4_15A9:
        clc    
        jmp    loc_4_15AD
loc_4_15AC:
        stc    
loc_4_15AD:
        pop    edi
        pop    esi
        ret    
loc_4_15B0:
        push   esi
        push   edi
        cmp    ebx, dword ptr [0xe8]         ; -> obj1:0x00E8
        je     loc_4_161D
        cmp    dl, 0xc6
        je     loc_4_161D
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        jne    loc_4_161D
        mov    edi, ebx
        add    edi, dword ptr [0xf0]         ; -> obj1:0x00F0
        movzx  esi, word ptr [edi]
        and    esi, 0xffff3f7f
        cmp    esi, 0x100
        jb     loc_4_161D
        cmp    dl, 0xc7
        jne    loc_4_15EB
        xor    al, al
        jmp    loc_4_1620
loc_4_15EB:
        cmp    dl, 0xc8
        jne    loc_4_15F5
        mov    al, byte ptr [edi + 2]
        jmp    loc_4_1620
loc_4_15F5:
        mov    edx, dword ptr [0x104]        ; -> obj1:0x0104
        cmp    byte ptr [edi + 5], 3
        jb     loc_4_160F
        mov    byte ptr [edi + 5], 0
        inc    byte ptr [edi + 3]
        mov    al, 0x62
        mov    ah, byte ptr [edi + 3]
        out    dx, ax
loc_4_160F:
        mov    al, 0x65
        out    dx, al
        inc    dl
        in     al, dx
        shr    al, 2
        inc    byte ptr [edi + 5]
        jmp    loc_4_1620
loc_4_161D:
        clc    
        jmp    loc_4_1621
loc_4_1620:
        stc    
loc_4_1621:
        pop    edi
        pop    esi
        ret    
loc_4_1624:
        cmp    byte ptr [0x164], 0           ; -> obj1:0x0164
        jne    loc_4_1646
        test   cl, 4
        je     loc_4_1646
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        je     loc_4_1646
        cmp    edi, dword ptr [0xe8]         ; -> obj1:0x00E8
        jne    loc_4_1646
        or     al, 5
loc_4_1646:
        VxDCall  VDD.VDD_Do_Physical_IO          ; cd2015800a00
loc_4_164C:
        test   cl, 4
        je     loc_4_1663
        cmp    dl, 0x6a
        jne    loc_4_1687
        mov    edi, ebx
        add    edi, dword ptr [0xf0]         ; -> obj1:0x00F0
        mov    byte ptr [edi + 6], al
        jmp    loc_4_1687
loc_4_1663:
        mov    edi, ebx
        add    edi, dword ptr [0xf0]         ; -> obj1:0x00F0
        cmp    byte ptr [edi + 6], 0x6f
        jne    loc_4_169A
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        je     loc_4_169A
        mov    dl, 0x6a
        mov    al, 0x6f
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0xfb
        jmp    loc_4_16A0
loc_4_1687:
        cmp    byte ptr [0x164], 0           ; -> obj1:0x0164
        jne    loc_4_169A
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        jne    loc_4_16A0
loc_4_169A:
        VxDCall  VDD.VDD_Do_Physical_IO          ; cd2015800a00
loc_4_16A0:
        ret    
VxD__LTEXT ENDS
