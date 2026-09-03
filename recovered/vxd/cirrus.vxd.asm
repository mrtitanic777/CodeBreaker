; CIRRUS -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 2066 instructions recovered by recursive descent

Declare_Virtual_Device CIRRUS, 3, 1, CIRRUS_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x15c  R-X
CIRRUS_Control:
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
        db  000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'CIRRUS  '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  CIRRUS_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00F5:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00F9:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00FD:
        add    byte ptr [eax], al
loc_1_00FF:
        add    byte ptr [eax], al
loc_1_0101:
        add    byte ptr [eax], al
loc_1_0103:
        inc    dword ptr [eax]
loc_1_0105:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_014F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0153:
        add    byte ptr [eax], al
loc_1_0155:
        add    byte ptr [eax], al
loc_1_0157:
        add    byte ptr [eax], al
loc_1_0159:
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x498  R-X
loc_2_0000:
        push   ebx
        mov    dword ptr [0xd0], ebx         ; -> obj1:0x00D0
        mov    byte ptr [0xfd], 0            ; -> loc_1_00FD
        push   0
        push   0x10000
        push   0xa0000
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_2_02F6
        mov    dword ptr [0xe0], eax         ; -> obj1:0x00E0
        call   sub_2_0433
        or     ebx, ebx
        jne    loc_2_007E
        call   sub_2_03D1
        or     ebx, ebx
        je     loc_2_02F6
        mov    dx, 0x3d4
        mov    ax, 0x25
        out    dx, al
        in     ax, dx
        mov    byte ptr [0xf0], ah           ; -> obj1:0x00F0
        mov    dword ptr [0xf1], 0           ; -> obj1:0x00F1
        test   ebx, 0x20000
        je     loc_2_007E
        cmp    ah, 0xc6
        je     loc_2_0074
        cmp    ah, 0x68
        ja     loc_2_007E
loc_2_0074:
        mov    dword ptr [0xf1], 1           ; -> obj1:0x00F1
loc_2_007E:
        mov    dword ptr [0xec], ebx         ; -> obj1:0x00EC
        mov    ax, 0x1200
        mov    bl, 0x85
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        movzx  eax, al
        shl    eax, 0x10
        mov    dword ptr [0xdc], eax         ; -> obj1:0x00DC
        VxDCall  VDD.VDD_Get_Special_VM_IDs      ; cd2011000a00
        mov    dword ptr [0xd4], esi         ; -> obj1:0x00D4
        mov    dword ptr [0xd8], edi         ; -> obj1:0x00D8
        test   dword ptr [0xec], 3           ; -> obj1:0x00EC
        jne    loc_2_0254
        push   0
        push   0x57
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0xe4], eax         ; -> obj1:0x00E4
        or     eax, eax
        je     loc_2_02F6
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_02F6
        mov    dword ptr [edi], 0            ; -> loc_3_0000
        mov    dword ptr [edi + 4], 0x48     ; -> loc_4_0048
        mov    dword ptr [edi + 8], 0x101    ; -> loc_4_0101
        mov    dword ptr [edi + 0xc], 0x185  ; -> loc_4_0185
        mov    dword ptr [edi + 0x58], 0x21f ; -> loc_4_021F
        mov    dword ptr [edi + 0x5c], 0x291 ; -> loc_4_0291
        mov    dword ptr [edi + 0x10], 0x2c9 ; -> loc_4_02C9
        mov    dword ptr [edi + 0x14], 0x324 ; -> loc_4_0324
        mov    dword ptr [edi + 0x18], 0x345 ; -> loc_4_0345
        mov    dword ptr [edi + 0x1c], 0x34d ; -> loc_4_034D
        mov    dword ptr [edi + 0x20], 0x355 ; -> loc_4_0355
        mov    dword ptr [edi + 0x24], 0x4b1 ; -> loc_4_04B1
        mov    dword ptr [edi + 0x68], 0x438 ; -> loc_3_0438
        mov    dword ptr [edi + 0x90], 0xd22 ; -> loc_4_0D22
        mov    dword ptr [edi + 0x80], 0xd2a ; -> loc_4_0D2A
        mov    dword ptr [edi + 0x84], 0xd44 ; -> loc_4_0D44
        mov    dword ptr [edi + 0x94], 0xd6f ; -> loc_4_0D6F
        mov    dword ptr [edi + 0x88], 0xde0 ; -> loc_4_0DE0
        mov    dword ptr [edi + 0x9c], 0xd87 ; -> loc_4_0D87
        mov    dword ptr [edi + 0xa0], 0xdbe ; -> loc_4_0DBE
        mov    dword ptr [edi + 0xa8], 0x492 ; -> loc_2_0492
        mov    dword ptr [edi + 0x4c], 0x941 ; -> loc_4_0941
        mov    dword ptr [edi + 0x48], 0xa27 ; -> loc_4_0A27
        mov    dword ptr [edi + 0x70], 0xa8  ; -> loc_3_00A8
        mov    dword ptr [edi + 0x74], 0x39d ; -> loc_3_039D
        test   dword ptr [0xec], 0x1fe3f0    ; -> obj1:0x00EC
        je     loc_2_01C4
        mov    dword ptr [edi + 0x3c], 0x888 ; -> loc_4_0888
loc_2_01C4:
        test   dword ptr [0xec], 0x1f03c0    ; -> obj1:0x00EC
        je     loc_2_01DE
        mov    dword ptr [edi + 0x2c], 0     ; -> loc_4_0000
        mov    dword ptr [edi + 0x30], 0x20  ; -> loc_4_0020
loc_2_01DE:
        test   dword ptr [0xec], 0x1e000     ; -> obj1:0x00EC
        je     loc_2_0206
        mov    dword ptr [edi + 0x24], 0x60b ; -> loc_4_060B
        mov    dword ptr [edi + 0x44], 0xb54 ; -> loc_4_0B54
        mov    dword ptr [edi + 0x78], 0xb7d ; -> loc_4_0B7D
        mov    dword ptr [edi + 0x7c], 0xc24 ; -> loc_4_0C24
loc_2_0206:
        test   dword ptr [0xec], 0xc00       ; -> obj1:0x00EC
        je     loc_2_0220
        mov    dword ptr [edi + 0x44], 0xaea ; -> loc_4_0AEA
        mov    dword ptr [edi + 0x40], 0xb27 ; -> loc_4_0B27
loc_2_0220:
        test   dword ptr [0xf1], 1           ; -> obj1:0x00F1
        je     loc_2_0242
        mov    edx, 0x3c2
        mov    ecx, 0x922                    ; -> loc_4_0922
        VxDCall  VDD.VDD_Takeover_VGA_Port       ; cd2013000a00
        mov    dword ptr [0xf5], ecx         ; -> loc_1_00F5
loc_2_0242:
        mov    edx, 0x3ce
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        jmp    loc_2_02ED
loc_2_0254:
        push   0
        push   5
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0xe4], eax         ; -> obj1:0x00E4
        or     eax, eax
        je     loc_2_02F6
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_02F6
        mov    dword ptr [edi], 0            ; -> loc_3_0000
        mov    dword ptr [edi + 4], 0x48     ; -> loc_4_0048
        mov    dword ptr [edi + 8], 0x140    ; -> loc_4_0140
        mov    dword ptr [edi + 0xc], 0x1d2  ; -> loc_4_01D2
        mov    dword ptr [edi + 0x58], 0x255 ; -> loc_4_0255
        mov    dword ptr [edi + 0x5c], 0x2ad ; -> loc_4_02AD
        mov    dword ptr [edi + 0x10], 0x2c9 ; -> loc_4_02C9
        mov    dword ptr [edi + 0x14], 0x324 ; -> loc_4_0324
        mov    dword ptr [edi + 0x18], 0x345 ; -> loc_4_0345
        mov    dword ptr [edi + 0x1c], 0x34d ; -> loc_4_034D
        mov    dword ptr [edi + 0x20], 0x459 ; -> loc_4_0459
        mov    dword ptr [edi + 0x24], 0x7b9 ; -> loc_4_07B9
        mov    dword ptr [edi + 0x50], 0xcbc ; -> loc_4_0CBC
        mov    dword ptr [edi + 0x54], 0xcf2 ; -> loc_4_0CF2
        mov    dword ptr [edi + 0x68], 0x438 ; -> loc_3_0438
        mov    dword ptr [edi + 0xa8], 0x492 ; -> loc_2_0492
        jmp    loc_2_02ED
loc_2_02ED:
        mov    ebx, dword ptr [0xd0]         ; -> obj1:0x00D0
        clc    
        jmp    loc_2_02F7
loc_2_02F6:
        stc    
loc_2_02F7:
        pop    ebx
        ret    
loc_2_02F9:
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        adc    eax, 0x10000000
        add    byte ptr [eax], al
        add    byte ptr [esi], dl
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    dword ptr [eax], eax
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [edx], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [ecx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [edx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [ebx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax + eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], 0
        add    byte ptr [ebx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [esi], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [ecx], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x430000], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_2_03B9:
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
sub_2_03D1:
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    al, 6
        out    dx, al
        inc    edx
        in     al, dx
        ror    eax, 8
        mov    al, 0x12
        out    dx, al
        in     al, dx
        cmp    al, 0x12
        jne    loc_2_041E
        not    al
        out    dx, al
        in     al, dx
        cmp    al, 0xf
        jne    loc_2_041E
        mov    al, 0x12
        out    dx, al
        mov    ax, 0x1200
        mov    bl, 0x80
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        lea    ebx, [0x2f9]                  ; -> loc_2_02F9
loc_2_040C:
        mov    ecx, dword ptr [ebx]
        and    ecx, ecx
        je     loc_2_041E
        add    ebx, 8
        cmp    al, cl
        jne    loc_2_040C
        mov    ebx, dword ptr [ebx - 4]
        jmp    loc_2_0420
loc_2_041E:
        xor    ebx, ebx
loc_2_0420:
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
sub_2_0433:
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xa
        out    dx, al
        inc    edx
        in     al, dx
        ror    eax, 8
        mov    al, 0xce
        out    dx, al
        in     al, dx
        cmp    al, 0
        jne    loc_2_047D
        mov    al, 0xec
        out    dx, al
        in     al, dx
        cmp    al, 1
        jne    loc_2_047D
        mov    ax, 0x1200
        mov    bl, 0x80
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        lea    ebx, [0x3b9]                  ; -> loc_2_03B9
loc_2_046B:
        mov    ecx, dword ptr [ebx]
        and    ecx, ecx
        je     loc_2_047D
        add    ebx, 8
        cmp    al, cl
        jne    loc_2_046B
        mov    ebx, dword ptr [ebx - 4]
        jmp    loc_2_047F
loc_2_047D:
        xor    ebx, ebx
loc_2_047F:
        mov    edx, 0x3ce
        mov    al, 0xa
        out    dx, al
        inc    edx
        rol    eax, 8
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        ret    
loc_2_0492:
        mov    eax, dword ptr [0xec]         ; -> obj1:0x00EC
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x440  R-X
loc_3_0000:
        mov    edx, 0x3cc
        in     al, dx
        mov    byte ptr [0x100], al          ; -> obj1:0x0100
        mov    eax, dword ptr [ebp + 0x14]
        cmp    eax, 0xb8000
        jne    loc_3_0066
        mov    ecx, dword ptr [0xe8]         ; -> obj1:0x00E8
        or     ecx, ecx
        jne    loc_3_005D
        push   0
        push   0x8000
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        push   0
        push   0x80
        push   0x92
        push   8
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
        mov    dword ptr [0xe8], eax         ; -> obj1:0x00E8
loc_3_005D:
        mov    eax, dword ptr [0xe8]         ; -> obj1:0x00E8
        mov    word ptr [ebp + 0x14], ax
loc_3_0066:
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
        cmp    word ptr [ebp], 0
        je     loc_3_0083
        mov    ax, 0x3c00
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        VxDCall  VDD.VDD_Set_Sleep_Flag_Addr     ; cd2016000a00
loc_3_0083:
        test   dword ptr [0xec], 0x1f03c0    ; -> obj1:0x00EC
        je     loc_3_00A7
        push   edx
        mov    edx, 0x3c4
        in     al, dx
        push   eax
        mov    al, 7
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf0
        mov    byte ptr [0xff], al           ; -> loc_1_00FF
        dec    edx
        pop    eax
        out    dx, al
        pop    edx
loc_3_00A7:
        ret    
loc_3_00A8:
        mov    ax, 0x1200
        mov    bl, 0x9a
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    word ptr [0x153], ax          ; -> loc_1_0153
        mov    word ptr [0x155], bx          ; -> loc_1_0155
        mov    word ptr [0x157], cx          ; -> loc_1_0157
        test   dword ptr [0xec], 0x160000    ; -> obj1:0x00EC
        je     loc_3_00EA
        mov    ax, 0x1200
        mov    bl, 0xae
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    word ptr [0x159], ax          ; -> loc_1_0159
loc_3_00EA:
        mov    dword ptr [0x14f], 0          ; -> loc_1_014F
        mov    eax, 0x105                    ; -> loc_1_0105
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        push   eax
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        pop    edi
        test   word ptr [edi + 2], 0x110
        jne    loc_3_039C
        cmp    word ptr [edi + 0x20], 0
        je     loc_3_039C
        push   edi
        mov    ax, 0x1200
        mov    bl, 0x9a
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edi
        push   eax
        push   ebx
        push   ecx
        test   dword ptr [0xec], 0x160000    ; -> obj1:0x00EC
        jne    loc_3_01AB
        test   word ptr [edi + 2], 0x200
        jne    loc_3_0188
        movzx  ecx, word ptr [edi + 0x24]
        movzx  edx, word ptr [edi + 0x26]
        mov    esi, edx
        or     esi, ecx
        je     loc_3_0188
        mov    al, 7
        cmp    edx, 0x40
        jae    loc_3_017A
        mov    al, 6
        cmp    edx, 0x38
        jae    loc_3_017A
        mov    al, 5
        cmp    edx, 0x30
        jae    loc_3_017A
        mov    al, 4
        cmp    edx, 0x25
        jae    loc_3_017A
        mov    al, 3
        cmp    edx, 0x23
        jae    loc_3_017A
        xor    al, al
loc_3_017A:
        push   edi
        mov    ah, 0x12
        mov    bl, 0xa2
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edi
loc_3_0188:
        movzx  ecx, word ptr [edi + 0x20]
        cmp    word ptr [edi + 0x18], 0x280
        jne    loc_3_01AB
        mov    al, 1
        cmp    ecx, 0x48
        jae    loc_3_019D
        xor    al, al
loc_3_019D:
        push   edi
        mov    ah, 0x12
        mov    bl, 0xa3
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edi
loc_3_01AB:
        pop    ecx
        pop    ebx
        pop    eax
        mov    al, 3
        and    ah, 0x40
        shr    ah, 2
        or     al, ah
        mov    bh, ch
        mov    ch, cl
        shl    ch, 3
        and    ch, 0x70
        shr    dh, 6
        and    dh, 3
        or     ch, dh
        movzx  esi, word ptr [edi + 0x20]
        movzx  edx, word ptr [edi + 0x1a]
        or     edx, edx
        je     loc_3_039C
        cmp    edx, 0x1e0
        je     loc_3_031E
        cmp    edx, 0x258
        je     loc_3_02D5
        cmp    edx, 0x300
        je     loc_3_0276
        cmp    edx, 0x360
        je     loc_3_025F
        cmp    edx, 0x400
        je     loc_3_020F
        jmp    loc_3_039C
loc_3_020F:
        test   dword ptr [0xec], 0x160000    ; -> obj1:0x00EC
        je     loc_3_022A
        and    ch, 0xf
        or     ch, 0x40
        cmp    esi, 0x55
        jae    loc_3_038E
loc_3_022A:
        and    ch, 0xf
        or     ch, 0x30
        cmp    esi, 0x4b
        jae    loc_3_038E
        and    ch, 0xf
        or     ch, 0x20
        cmp    esi, 0x46
        jae    loc_3_038E
        and    ch, 0xf
        or     ch, 0x10
        cmp    esi, 0x3c
        jae    loc_3_038E
        and    ch, 0xf
        jmp    loc_3_038E
loc_3_025F:
        and    ch, 0xf0
        or     ch, 1
        cmp    esi, 0x4b
        jae    loc_3_038E
        and    ch, 0xf0
        jmp    loc_3_038E
loc_3_0276:
        test   dword ptr [0xec], 0x160000    ; -> obj1:0x00EC
        je     loc_3_0291
        and    bh, 0xf
        or     bh, 0x50
        cmp    esi, 0x55
        jae    loc_3_038E
loc_3_0291:
        and    bh, 0xf
        or     bh, 0x40
        cmp    esi, 0x4b
        jae    loc_3_038E
        and    bh, 0xf
        or     bh, 0x30
        cmp    esi, 0x48
        jae    loc_3_038E
        and    bh, 0xf
        or     bh, 0x20
        cmp    esi, 0x46
        jae    loc_3_038E
        and    bh, 0xf
        or     bh, 0x10
        cmp    esi, 0x3c
        jae    loc_3_038E
        and    bh, 0xf
        jmp    loc_3_038E
loc_3_02D5:
        test   dword ptr [0xec], 0x160000    ; -> obj1:0x00EC
        je     loc_3_02F0
        and    bh, 0xf0
        or     bh, 4
        cmp    esi, 0x55
        jae    loc_3_038E
loc_3_02F0:
        and    bh, 0xf0
        or     bh, 3
        cmp    esi, 0x4b
        jae    loc_3_038E
        and    bh, 0xf0
        or     bh, 2
        cmp    esi, 0x48
        jae    loc_3_038E
        and    bh, 0xf0
        or     bh, 1
        cmp    esi, 0x3c
        jae    loc_3_038E
        and    bh, 0xf0
        jmp    loc_3_038E
loc_3_031E:
        test   dword ptr [0xec], 0x160000    ; -> obj1:0x00EC
        jne    loc_3_0357
        test   dword ptr [0xec], 0x1e000     ; -> obj1:0x00EC
        je     loc_3_034C
        and    al, 0xcf
        or     al, 0x20
        cmp    esi, 0x4b
        jae    loc_3_038E
        and    al, 0xcf
        or     al, 0x10
        cmp    esi, 0x48
        jae    loc_3_038E
        and    al, 0xcf
        jmp    loc_3_038E
loc_3_034C:
        or     al, 0x10
        cmp    esi, 0x4b
        jae    loc_3_038E
        and    al, 0xef
        jmp    loc_3_038E
loc_3_0357:
        or     al, 0x10
        cmp    esi, 0x48
        jae    loc_3_0360
        and    al, 0xef
loc_3_0360:
        push   edi
        push   esi
        mov    ah, 0x12
        mov    bl, 0xa4
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    esi
        mov    al, 2
        cmp    esi, 0x55
        jae    loc_3_0376
loc_3_0376:
        mov    al, 1
        cmp    esi, 0x4b
        jae    loc_3_037F
        mov    al, 0
loc_3_037F:
        mov    ah, 0x12
        mov    bl, 0xae
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edi
        jmp    loc_3_039C
loc_3_038E:
        push   edi
        mov    ah, 0x12
        mov    bl, 0xa4
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edi
loc_3_039C:
        ret    
loc_3_039D:
        mov    ax, word ptr [0x157]          ; -> loc_1_0157
        mov    cx, ax
        shl    cx, 0xb
        and    cx, 0x70
        mov    bx, ax
        and    bx, 0xf000
        and    ax, 0xf00
        or     bx, ax
        mov    al, 3
        mov    dx, word ptr [0x153]          ; -> loc_1_0153
        and    dx, 0x4000
        shr    dx, 0xa
        or     ax, dx
        test   dword ptr [0xec], 0x1e000     ; -> obj1:0x00EC
        je     loc_3_03FF
        mov    dx, word ptr [0x155]          ; -> loc_1_0155
        and    dx, 0x3000
        shr    dx, 8
        or     ax, dx
        mov    ah, 0x12
        mov    bl, 0xa4
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jmp    loc_3_0437
loc_3_03FF:
        test   dword ptr [0xec], 0x160000    ; -> obj1:0x00EC
        je     loc_3_042B
        mov    ah, 0x12
        mov    bl, 0xa4
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    ax, word ptr [0x159]          ; -> loc_1_0159
        mov    ah, 0x12
        mov    bl, 0xaf
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jmp    loc_3_0437
loc_3_042B:
        mov    ah, 0x12
        mov    bl, 0xa4
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_3_0437:
        ret    
loc_3_0438:
        mov    byte ptr [0xfe], 0            ; -> obj1:0x00FE
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xdff  R-X
loc_4_0000:
        cmp    byte ptr [0xff], 0            ; -> loc_1_00FF
        je     loc_4_001F
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    al, 7
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_001F:
        ret    
loc_4_0020:
        cmp    byte ptr [0xff], 0            ; -> loc_1_00FF
        je     loc_4_0047
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    al, 7
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf
        mov    ah, byte ptr [0xff]           ; -> loc_1_00FF
        or     al, ah
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_0047:
        ret    
loc_4_0048:
        test   dword ptr [0xec], 0x20000     ; -> obj1:0x00EC
        je     loc_4_0061
        cmp    byte ptr [0xf0], 0x6a         ; -> obj1:0x00F0
        jl     loc_4_00FC
loc_4_0061:
        mov    byte ptr [0xfe], 0xff         ; -> obj1:0x00FE
        mov    byte ptr [0x103], 0xff        ; -> loc_1_0103
        mov    eax, ecx
        shr    eax, 0x10
        or     cx, cx
        je     loc_4_0093
        mov    edx, dword ptr [0xdc]         ; -> obj1:0x00DC
        shr    edx, 0x10
        dec    dl
        cmp    al, dl
        jae    loc_4_00FC
        inc    al
        xor    cx, cx
        add    ecx, 0x10000
loc_4_0093:
        test   dword ptr [0xec], 0x20000     ; -> obj1:0x00EC
        jne    loc_4_00FC
        mov    edx, dword ptr [0xdc]         ; -> obj1:0x00DC
        sub    edx, ecx
        cmp    edx, 0x10000
        mov    edx, 0x10000
        ja     loc_4_00B9
        mov    edx, 0x8000
loc_4_00B9:
        test   dword ptr [0xec], 3           ; -> obj1:0x00EC
        jne    loc_4_00EF
        push   ecx
        mov    ecx, 4
        cmp    dword ptr [0xdc], 0x100000    ; -> obj1:0x00DC
        jle    loc_4_00DC
        mov    ecx, 0x2002
loc_4_00DC:
        shl    al, cl
        mov    byte ptr [0x102], ch          ; -> obj1:0x0102
        shr    al, 2
        mov    byte ptr [0x101], al          ; -> loc_1_0101
        pop    ecx
        jmp    loc_4_00FE
loc_4_00EF:
        shl    al, 4
        shr    al, 2
        mov    byte ptr [0x101], al          ; -> loc_1_0101
        jmp    loc_4_00FE
loc_4_00FC:
        mov    edx, ecx
loc_4_00FE:
        xor    ah, ah
        ret    
loc_4_0101:
        cmp    byte ptr [0x103], 0xff        ; -> loc_1_0103
        jne    loc_4_013F
        push   eax
        push   edx
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x1206
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x103], al          ; -> loc_1_0103
        mov    al, byte ptr [0x101]          ; -> loc_1_0101
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_013F:
        ret    
loc_4_0140:
        cmp    byte ptr [0x103], 0xff        ; -> loc_1_0103
        jne    loc_4_0184
        push   eax
        push   edx
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    ax, 0xec0a
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    ax, 0x30d
        out    dx, ax
        mov    al, 0xe
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x103], al          ; -> loc_1_0103
        mov    al, byte ptr [0x101]          ; -> loc_1_0101
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_0184:
        ret    
loc_4_0185:
        cmp    byte ptr [0x103], 0xff        ; -> loc_1_0103
        je     loc_4_01D1
        push   eax
        push   edx
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x1206
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        cmp    al, byte ptr [0x101]          ; -> loc_1_0101
        jne    loc_4_01BF
        mov    al, byte ptr [0x103]          ; -> loc_1_0103
        out    dx, al
loc_4_01BF:
        mov    byte ptr [0x103], 0xff        ; -> loc_1_0103
        mov    edx, 0x3ce
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_01D1:
        ret    
loc_4_01D2:
        cmp    byte ptr [0x103], 0xff        ; -> loc_1_0103
        je     loc_4_021E
        push   eax
        push   edx
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    ax, 0xec0a
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xe
        out    dx, al
        inc    edx
        in     al, dx
        cmp    al, byte ptr [0x101]          ; -> loc_1_0101
        jne    loc_4_020C
        mov    al, byte ptr [0x103]          ; -> loc_1_0103
        out    dx, al
loc_4_020C:
        mov    byte ptr [0x103], 0xff        ; -> loc_1_0103
        mov    edx, 0x3ce
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_021E:
        ret    
loc_4_021F:
        push   eax
        push   edx
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x1206
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x104], al          ; -> obj1:0x0104
        mov    al, byte ptr [0x101]          ; -> loc_1_0101
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret    
loc_4_0255:
        push   eax
        push   edx
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    ax, 0xec0a
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    ax, 0x30d
        out    dx, ax
        mov    al, 0xe
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x104], al          ; -> obj1:0x0104
        mov    al, byte ptr [0x101]          ; -> loc_1_0101
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret    
loc_4_0291:
        push   eax
        push   edx
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 9
        mov    ah, byte ptr [0x104]          ; -> obj1:0x0104
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret    
loc_4_02AD:
        push   eax
        push   edx
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xe
        mov    ah, byte ptr [0x104]          ; -> obj1:0x0104
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret    
loc_4_02C9:
        or     byte ptr [0xfd], 2            ; -> loc_1_00FD
        test   dword ptr [0xec], 0x1fe3f0    ; -> obj1:0x00EC
        je     loc_4_0323
        mov    edx, 0x3ce
        mov    ax, 0x431
        out    dx, ax
        mov    ah, 0
        out    dx, ax
        test   dword ptr [0xec], 0x100080    ; -> obj1:0x00EC
        je     loc_4_0323
        mov    edx, 0x3d4
        mov    eax, 0x3e
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xfe
        out    dx, al
        test   dword ptr [0xec], 0x18000     ; -> obj1:0x00EC
        je     loc_4_0323
        mov    edx, 0x3d4
        mov    eax, 0x3c
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xef
        out    dx, al
loc_4_0323:
        ret    
loc_4_0324:
        mov    byte ptr [0xfd], 0            ; -> loc_1_00FD
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    ax, 9
        out    dx, ax
        mov    ax, 0xa
        out    dx, ax
        rol    eax, 8
        out    dx, al
        ret    
loc_4_0345:
        or     byte ptr [0xfd], 1            ; -> loc_1_00FD
        ret    
loc_4_034D:
        mov    byte ptr [0xfd], 0            ; -> loc_1_00FD
        ret    
loc_4_0355:
        push   edx
        push   edi
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x1206
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edi, ebx
        add    edi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 5], al
        xor    al, al
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi], al
        mov    al, 1
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 1], al
        mov    al, 5
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 2], al
        rol    eax, 8
        out    dx, al
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    al, 2
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 6], al
        mov    al, 0x12
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 0xa], al
        mov    byte ptr [edi + 0x10], 0
        rol    eax, 8
        out    dx, al
        test   dword ptr [0xec], 0x1e000     ; -> obj1:0x00EC
        je     loc_4_044C
        mov    byte ptr [edi + 0xf], 0
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 8], al
        mov    al, 0xa
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 9], al
        mov    al, 0x14
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 0xb], al
        mov    al, 0x15
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 0xc], al
        mov    al, 0x28
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 0xd], al
        mov    al, 0x29
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 0xe], al
        mov    al, 0x12
        out    dx, al
        inc    edx
        in     al, dx
        ror    eax, 8
        or     al, 2
        out    dx, al
        mov    edx, 0x3c7
        add    edi, 0x17
        xor    ah, ah
        mov    ecx, 0x10
        cld    
loc_4_042A:
        mov    al, ah
        out    dx, al
        stosb  byte ptr es:[edi], al
        add    edx, 2
        in     al, dx
        stosb  byte ptr es:[edi], al
        in     al, dx
        stosb  byte ptr es:[edi], al
        in     al, dx
        stosb  byte ptr es:[edi], al
        sub    edx, 2
        inc    ah
        loop   loc_4_042A
        mov    edx, 0x3c5
        rol    eax, 8
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_044C:
        mov    dword ptr [0x14f], 1          ; -> loc_1_014F
        pop    edi
        pop    edx
        ret    
loc_4_0459:
        push   edx
        push   edi
        mov    edi, ebx
        add    edi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    ax, 0xec0a
        out    dx, ax
        mov    al, 0xa1
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 2], al
        mov    al, 0xd
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 1], al
        mov    al, 0xa
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi], al
        test   dword ptr [0xec], 2           ; -> obj1:0x00EC
        je     loc_4_04AA
        mov    al, 0xae
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 3], al
        mov    al, 0xb0
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 4], al
loc_4_04AA:
        rol    eax, 8
        out    dx, al
        pop    edi
        pop    edx
        ret    
loc_4_04B1:
        test   dword ptr [0xf1], 1           ; -> obj1:0x00F1
        je     loc_4_04D1
        cmp    dword ptr [0xf9], 0           ; -> loc_1_00F9
        je     loc_4_04D1
        mov    dword ptr [0xf9], 0           ; -> loc_1_00F9
        sti    
loc_4_04D1:
        push   edx
        cmp    byte ptr [0xfe], 0            ; -> obj1:0x00FE
        je     loc_4_0609
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x1206
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    al, 0x1b
        out    dx, al
        inc    edx
        in     al, dx
        or     al, 2
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        cmp    ecx, dword ptr [0xd0]         ; -> obj1:0x00D0
        jne    loc_4_053F
        mov    al, byte ptr [0x100]          ; -> obj1:0x0100
        or     al, al
        je     loc_4_053F
        test   dword ptr [0xf1], 1           ; -> obj1:0x00F1
        je     loc_4_052C
        pushfd 
        cli    
        call   sub_4_08EF
loc_4_052C:
        mov    edx, 0x3c2
        out    dx, al
        test   dword ptr [0xf1], 1           ; -> obj1:0x00F1
        je     loc_4_053F
        popfd  
loc_4_053F:
        cmp    esi, dword ptr [0xd8]         ; -> obj1:0x00D8
        je     loc_4_0609
        cmp    esi, dword ptr [0xd4]         ; -> obj1:0x00D4
        je     loc_4_05BB
        test   byte ptr [0xfd], 3            ; -> loc_1_00FD
        jne    loc_4_0609
        cmp    esi, dword ptr [0xd0]         ; -> obj1:0x00D0
        jne    loc_4_05D4
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb
        mov    ah, byte ptr [esi + 5]
        out    dx, ax
        xor    al, al
        mov    ah, byte ptr [esi]
        out    dx, ax
        mov    al, 1
        mov    ah, byte ptr [esi + 1]
        out    dx, ax
        cmp    ecx, dword ptr [0xd0]         ; -> obj1:0x00D0
        jne    loc_4_059A
        mov    al, 5
        mov    ah, byte ptr [esi + 2]
        out    dx, ax
loc_4_059A:
        rol    eax, 8
        out    dx, al
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    al, 2
        mov    ah, byte ptr [esi + 6]
        out    dx, ax
        mov    al, 0x12
        mov    ah, byte ptr [esi + 0xa]
        out    dx, ax
        rol    eax, 8
        out    dx, al
        jmp    loc_4_0609
loc_4_05BB:
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb
        mov    ah, byte ptr [0x102]          ; -> obj1:0x0102
        out    dx, ax
        rol    eax, 8
        out    dx, al
        jmp    loc_4_0609
loc_4_05D4:
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb
        mov    ah, byte ptr [esi + 5]
        cmp    ecx, dword ptr [0xd0]         ; -> obj1:0x00D0
        jne    loc_4_05F6
        mov    ah, byte ptr [0x102]          ; -> obj1:0x0102
loc_4_05F6:
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        rol    eax, 8
        out    dx, al
loc_4_0609:
        pop    edx
        ret    
loc_4_060B:
        push   edx
        cmp    byte ptr [0xfe], 0            ; -> obj1:0x00FE
        je     loc_4_0609
        cmp    dword ptr [0x14f], 1          ; -> loc_1_014F
        jne    loc_4_0609
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x1206
        out    dx, ax
        rol    eax, 8
        out    dx, al
        cmp    ecx, dword ptr [0xd0]         ; -> obj1:0x00D0
        jne    loc_4_064F
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    al, 0x1b
        out    dx, al
        inc    edx
        in     al, dx
        or     al, 2
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_064F:
        cmp    ecx, dword ptr [0xd0]         ; -> obj1:0x00D0
        jne    loc_4_0666
        mov    al, byte ptr [0x100]          ; -> obj1:0x0100
        or     al, al
        je     loc_4_0666
        mov    edx, 0x3c2
        out    dx, al
loc_4_0666:
        cmp    esi, dword ptr [0xd8]         ; -> obj1:0x00D8
        je     loc_4_07B7
        cmp    esi, dword ptr [0xd4]         ; -> obj1:0x00D4
        je     loc_4_070F
        test   byte ptr [0xfd], 3            ; -> loc_1_00FD
        jne    loc_4_07B7
        cmp    esi, dword ptr [0xd0]         ; -> obj1:0x00D0
        jne    loc_4_072B
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb
        mov    ah, byte ptr [esi + 5]
        out    dx, ax
        xor    al, al
        mov    ah, byte ptr [esi]
        out    dx, ax
        mov    al, 1
        mov    ah, byte ptr [esi + 1]
        out    dx, ax
        mov    al, 5
        mov    ah, byte ptr [esi + 2]
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    al, 2
        mov    ah, byte ptr [esi + 6]
        out    dx, ax
        mov    al, 0x12
        mov    ah, byte ptr [esi + 0xa]
        out    dx, ax
        mov    al, 9
        mov    ah, byte ptr [esi + 8]
        out    dx, ax
        mov    al, 0xa
        out    dx, al
        inc    edx
        in     al, dx
        or     al, 0x20
        out    dx, al
        dec    edx
        mov    al, 0x14
        mov    ah, byte ptr [esi + 0xb]
        mov    al, 0x15
        mov    ah, byte ptr [esi + 0xc]
        out    dx, ax
        mov    al, 0x28
        mov    ah, byte ptr [esi + 0xd]
        out    dx, ax
        mov    al, 0x29
        mov    ah, byte ptr [esi + 0xe]
        out    dx, ax
        rol    eax, 8
        out    dx, al
        jmp    loc_4_07B7
loc_4_070F:
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb
        mov    ah, byte ptr [0x102]          ; -> obj1:0x0102
        out    dx, ax
        rol    eax, 8
        out    dx, al
        jmp    loc_4_07B7
loc_4_072B:
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb
        mov    ah, byte ptr [esi + 5]
        cmp    ecx, dword ptr [0xd0]         ; -> obj1:0x00D0
        jne    loc_4_074D
        mov    ah, byte ptr [0x102]          ; -> obj1:0x0102
loc_4_074D:
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    al, 0xa
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xdf
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    al, 0x12
        out    dx, al
        inc    edx
        in     al, dx
        ror    eax, 8
        or     al, 2
        out    dx, al
        mov    edx, 0x3c8
        add    esi, 0x17
        mov    ecx, 0x10
loc_4_078A:
        mov    al, byte ptr [esi]
        and    al, 0xf
        cmp    al, 0xb
        jb     loc_4_07A4
        cmp    al, 0xf
        je     loc_4_07A4
        out    dx, al
        inc    edx
        mov    al, byte ptr [esi + 1]
        out    dx, al
        mov    al, byte ptr [esi + 2]
        out    dx, al
        mov    al, byte ptr [esi + 3]
        dec    edx
loc_4_07A4:
        add    esi, 4
        loop   loc_4_078A
        mov    edx, 0x3c5
        rol    eax, 8
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_07B7:
        pop    edx
        ret    
loc_4_07B9:
        push   edx
        cmp    byte ptr [0xfe], 0            ; -> obj1:0x00FE
        je     loc_4_0886
        cmp    ecx, dword ptr [0xd0]         ; -> obj1:0x00D0
        jne    loc_4_07DE
        mov    al, byte ptr [0x100]          ; -> obj1:0x0100
        or     al, al
        je     loc_4_07DE
        mov    edx, 0x3c2
        out    dx, al
loc_4_07DE:
        cmp    esi, dword ptr [0xd8]         ; -> obj1:0x00D8
        je     loc_4_0886
        cmp    esi, dword ptr [0xd4]         ; -> obj1:0x00D4
        je     loc_4_0800
        test   byte ptr [0xfd], 3            ; -> loc_1_00FD
        je     loc_4_0830
        jmp    loc_4_0886
loc_4_0800:
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    ax, 0xec0a
        out    dx, ax
        mov    al, 0xa1
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xcf
        out    dx, al
        dec    edx
        test   dword ptr [0xec], 2           ; -> obj1:0x00EC
        je     loc_4_082A
        mov    ax, 0xb0
        out    dx, ax
loc_4_082A:
        rol    eax, 8
        out    dx, al
        jmp    loc_4_0886
loc_4_0830:
        mov    ecx, esi
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    ax, 0xec0a
        out    dx, ax
        mov    al, 0xd
        mov    ah, byte ptr [esi + 1]
        cmp    ecx, dword ptr [0xd0]         ; -> obj1:0x00D0
        je     loc_4_0857
        or     ah, 3
loc_4_0857:
        out    dx, ax
        mov    al, 0xa1
        mov    ah, byte ptr [esi + 2]
        out    dx, ax
        test   dword ptr [0xec], 2           ; -> obj1:0x00EC
        je     loc_4_0882
        mov    al, 0xb0
        mov    ah, byte ptr [esi + 4]
        out    dx, ax
        cmp    ecx, dword ptr [0xd0]         ; -> obj1:0x00D0
        jne    loc_4_0882
        mov    al, 0xae
        mov    ah, byte ptr [esi + 3]
        out    dx, ax
loc_4_0882:
        rol    eax, 8
        out    dx, al
loc_4_0886:
        pop    edx
        ret    
loc_4_0888:
        push   edx
        cmp    eax, dword ptr [0xd0]         ; -> obj1:0x00D0
        jne    loc_4_08C5
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x1206
        out    dx, ax
        rol    eax, 8
        out    dx, al
        mov    edx, 0x3ce
        mov    ecx, 0x1000
        mov    al, 0x31
        out    dx, al
loc_4_08B1:
        in     ax, dx
        test   ah, 1
        je     loc_4_08C5
        call   sub_4_08E3
        loop   loc_4_08B1
        mov    al, 4
        out    dx, al
        mov    al, 0
        out    dx, al
loc_4_08C5:
        pop    edx
        test   dword ptr [0xf1], 1           ; -> obj1:0x00F1
        je     loc_4_08E2
        cli    
        mov    dword ptr [0xf9], 1           ; -> loc_1_00F9
        call   sub_4_08EF
loc_4_08E2:
        ret    
sub_4_08E3:
        push   ecx
        mov    ecx, 0x3e8
loc_4_08E9:
        push   ecx
        pop    ecx
        loop   loc_4_08E9
        pop    ecx
        ret    
sub_4_08EF:
        push   edx
        push   ecx
        push   eax
        mov    edx, 0x3cc
        in     al, dx
        mov    edx, 0x3da
        test   al, 1
        jne    loc_4_0906
        mov    edx, 0x3ba
loc_4_0906:
        mov    ecx, 0xffffffff
loc_4_090B:
        in     al, dx
        test   al, 1
        jne    loc_4_0912
        loop   loc_4_090B
loc_4_0912:
        mov    ecx, 0xffffffff
loc_4_0917:
        in     al, dx
        test   al, 1
        je     loc_4_091E
        loop   loc_4_0917
loc_4_091E:
        pop    eax
        pop    ecx
        pop    edx
        ret    
loc_4_0922:
        pushfd 
        test   cl, 4
        je     loc_4_093A
        cli    
        call   sub_4_08EF
        and    byte ptr [esp + 1], 0xfd
        popfd  
        jmp    loc_1_00F5
loc_4_093A:
        popfd  
        jmp    loc_1_00F5
loc_4_0941:
        cmp    cl, 0x12
        jne    loc_4_095A
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    byte ptr [esi + 0xa], al
        jmp    loc_4_09FA
loc_4_095A:
        test   dword ptr [0xec], 0x1e000     ; -> obj1:0x00EC
        je     loc_4_09FA
        cmp    cl, 9
        jne    loc_4_0984
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    byte ptr [esi + 8], al
        stc    
        jmp    loc_4_0A26
loc_4_0984:
        cmp    cl, 0xa
        jne    loc_4_099E
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    byte ptr [esi + 9], al
        stc    
        jmp    loc_4_0A26
loc_4_099E:
        cmp    cl, 0x14
        jne    loc_4_09B5
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    byte ptr [esi + 0xb], al
        stc    
        jmp    loc_4_0A26
loc_4_09B5:
        cmp    cl, 0x15
        jne    loc_4_09CC
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    byte ptr [esi + 0xc], al
        stc    
        jmp    loc_4_0A26
loc_4_09CC:
        cmp    cl, 0x28
        jne    loc_4_09E3
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    byte ptr [esi + 0xd], al
        stc    
        jmp    loc_4_0A26
loc_4_09E3:
        cmp    cl, 0x29
        jne    loc_4_09FA
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    byte ptr [esi + 0xe], al
        stc    
        jmp    loc_4_0A26
loc_4_09FA:
        test   dword ptr [0xf1], 1           ; -> obj1:0x00F1
        je     loc_4_0A25
        cmp    cl, 1
        je     loc_4_0A1F
        cmp    cl, 0xb
        jb     loc_4_0A25
        cmp    cl, 0xe
        jbe    loc_4_0A1F
        cmp    cl, 0x1b
        jb     loc_4_0A25
        cmp    cl, 0x1f
        ja     loc_4_0A25
loc_4_0A1F:
        cli    
        call   sub_4_08EF
loc_4_0A25:
        clc    
loc_4_0A26:
        ret    
loc_4_0A27:
        cmp    cl, 0x12
        jne    loc_4_0A41
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    al, byte ptr [esi + 0xa]
        stc    
        jmp    loc_4_0AE9
loc_4_0A41:
        test   dword ptr [0xec], 0x1e000     ; -> obj1:0x00EC
        je     loc_4_0AE8
        cmp    cl, 6
        jne    loc_4_0A5E
        mov    al, 0xf
        stc    
        jmp    loc_4_0AE9
loc_4_0A5E:
        cmp    cl, 9
        jne    loc_4_0A75
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    al, byte ptr [esi + 8]
        stc    
        jmp    loc_4_0AE9
loc_4_0A75:
        cmp    cl, 0xa
        jne    loc_4_0A8C
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    al, byte ptr [esi + 9]
        stc    
        jmp    loc_4_0AE9
loc_4_0A8C:
        cmp    cl, 0x14
        jne    loc_4_0AA3
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    al, byte ptr [esi + 0xb]
        stc    
        jmp    loc_4_0AE9
loc_4_0AA3:
        cmp    cl, 0x15
        jne    loc_4_0ABA
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    al, byte ptr [esi + 0xc]
        stc    
        jmp    loc_4_0AE9
loc_4_0ABA:
        cmp    cl, 0x28
        jne    loc_4_0AD1
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    al, byte ptr [esi + 0xd]
        stc    
        jmp    loc_4_0AE9
loc_4_0AD1:
        cmp    cl, 0x29
        jne    loc_4_0AE8
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    al, byte ptr [esi + 0xe]
        stc    
        jmp    loc_4_0AE9
loc_4_0AE8:
        clc    
loc_4_0AE9:
        ret    
loc_4_0AEA:
        cmp    cl, 0x19
        jb     loc_4_0B17
        cmp    cl, 0x1d
        jne    loc_4_0B01
        push   ebx
        add    ebx, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    byte ptr [ebx + 0x10], al
        pop    ebx
        jmp    loc_4_0B15
loc_4_0B01:
        cmp    cl, 0x1b
        jne    loc_4_0B08
        or     al, 2
loc_4_0B08:
        cmp    cl, 0x1e
        je     loc_4_0B15
        dec    edx
        mov    ah, al
        mov    al, cl
        out    dx, ax
        inc    edx
loc_4_0B15:
        stc    
        ret    
loc_4_0B17:
        push   ebx
        add    ebx, dword ptr [0xe4]         ; -> obj1:0x00E4
        test   byte ptr [ebx + 0x10], 0x80
        pop    ebx
        jne    loc_4_0B15
        clc    
        ret    
loc_4_0B27:
        cmp    cl, 0x19
        jb     loc_4_0B52
        cmp    cl, 0x1d
        jne    loc_4_0B3E
        push   ebx
        add    ebx, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    al, byte ptr [ebx + 0x10]
        pop    ebx
        jmp    loc_4_0B50
loc_4_0B3E:
        dec    edx
        in     al, dx
        ror    eax, 8
        mov    al, cl
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        inc    edx
        mov    al, ah
loc_4_0B50:
        stc    
        ret    
loc_4_0B52:
        clc    
        ret    
loc_4_0B54:
        cmp    cl, 7
        ja     loc_4_0B69
        push   ebx
        add    ebx, dword ptr [0xe4]         ; -> obj1:0x00E4
        test   byte ptr [ebx + 0xf], 0x80
        pop    ebx
        je     loc_4_0B79
        jmp    loc_4_0B7B
loc_4_0B69:
        cmp    cl, 0x11
        jne    loc_4_0B79
        push   ebx
        add    ebx, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    byte ptr [ebx + 0xf], al
        pop    ebx
loc_4_0B79:
        clc    
        ret    
loc_4_0B7B:
        stc    
        ret    
loc_4_0B7D:
        cmp    ebx, dword ptr [0xd0]         ; -> obj1:0x00D0
        clc    
        je     loc_4_0C23
        cmp    edx, 0x3c6
        jne    loc_4_0B98
        clc    
        jmp    loc_4_0C23
loc_4_0B98:
        cmp    edx, 0x3c7
        jne    loc_4_0BBF
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    byte ptr [esi + 0x11], al
        mov    dword ptr [esi + 0x12], 1
        mov    byte ptr [esi + 0x16], 3
        pop    esi
        clc    
        jmp    loc_4_0C23
loc_4_0BBF:
        cmp    edx, 0x3c8
        jne    loc_4_0BE6
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    byte ptr [esi + 0x11], al
        mov    dword ptr [esi + 0x12], 1
        mov    byte ptr [esi + 0x16], 0
        pop    esi
        clc    
        jmp    loc_4_0C23
loc_4_0BE6:
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        test   byte ptr [esi + 0xa], 2
        jne    loc_4_0BFD
        pop    esi
        clc    
        jmp    loc_4_0C23
loc_4_0BFD:
        push   ebx
        movzx  ebx, byte ptr [esi + 0x11]
        and    ebx, 0xf
        shl    ebx, 2
        add    ebx, dword ptr [esi + 0x12]
        mov    byte ptr [ebx + esi + 0x17], al
        mov    ebx, dword ptr [esi + 0x12]
        cmp    ebx, 3
        jb     loc_4_0C1C
        xor    ebx, ebx
        inc    byte ptr [esi + 0x11]
loc_4_0C1C:
        inc    ebx
        mov    dword ptr [esi + 0x12], ebx
        pop    ebx
        pop    esi
        stc    
loc_4_0C23:
        ret    
loc_4_0C24:
        cmp    ebx, dword ptr [0xd0]         ; -> obj1:0x00D0
        clc    
        je     loc_4_0CBB
        cmp    edx, 0x3c6
        jne    loc_4_0C3C
        clc    
        jmp    loc_4_0CBB
loc_4_0C3C:
        cmp    edx, 0x3c7
        jne    loc_4_0C58
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    al, byte ptr [esi + 0x16]
        pop    esi
        stc    
        jmp    loc_4_0CBB
loc_4_0C58:
        cmp    edx, 0x3c8
        jne    loc_4_0C7E
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        test   byte ptr [esi + 0xa], 2
        jne    loc_4_0C77
        pop    esi
        clc    
        jmp    loc_4_0CBB
loc_4_0C77:
        mov    al, byte ptr [esi + 0x11]
        pop    esi
        stc    
        jmp    loc_4_0CBB
loc_4_0C7E:
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        test   byte ptr [esi + 0xa], 2
        jne    loc_4_0C95
        pop    esi
        clc    
        jmp    loc_4_0CBB
loc_4_0C95:
        push   ebx
        movzx  ebx, byte ptr [esi + 0x11]
        and    ebx, 0xf
        shl    ebx, 2
        add    ebx, dword ptr [esi + 0x12]
        mov    al, byte ptr [ebx + esi + 0x17]
        mov    ebx, dword ptr [esi + 0x12]
        cmp    ebx, 3
        jb     loc_4_0CB4
        xor    ebx, ebx
        inc    byte ptr [esi + 0x11]
loc_4_0CB4:
        inc    ebx
        mov    dword ptr [esi + 0x12], ebx
        pop    ebx
        pop    esi
        stc    
loc_4_0CBB:
        ret    
loc_4_0CBC:
        cmp    cl, 0xa
        je     loc_4_0CDE
        cmp    cl, 0x30
        ja     loc_4_0CC9
        clc    
        jmp    loc_4_0CF1
loc_4_0CC9:
        dec    edx
        in     al, dx
        ror    eax, 8
        mov    al, cl
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        inc    edx
        mov    al, ah
        stc    
        jmp    loc_4_0CF1
loc_4_0CDE:
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        add    esi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    al, byte ptr [esi]
        and    al, 1
        pop    esi
        stc    
loc_4_0CF1:
        ret    
loc_4_0CF2:
        cmp    cl, 0xb0
        je     loc_4_0D15
        cmp    cl, 0xa
        jne    loc_4_0D20
        mov    edi, ebx
        add    edi, dword ptr [0xe4]         ; -> obj1:0x00E4
        cmp    al, 0xec
        jne    loc_4_0D0B
        mov    byte ptr [edi], 1
loc_4_0D0B:
        cmp    al, 0xce
        jne    loc_4_0D12
        mov    byte ptr [edi], 0
loc_4_0D12:
        stc    
        jmp    loc_4_0D21
loc_4_0D15:
        mov    edi, ebx
        add    edi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    byte ptr [edi + 4], al
loc_4_0D20:
        clc    
loc_4_0D21:
        ret    
loc_4_0D22:
        mov    ecx, dword ptr [0xdc]         ; -> obj1:0x00DC
        stc    
        ret    
loc_4_0D2A:
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc    
        ret    
loc_4_0D44:
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
        jne    loc_4_0D60
        mov    al, 9
loc_4_0D60:
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc    
        ret    
loc_4_0D6F:
        mov    edx, 0x1000
        cmp    dword ptr [0xdc], 0x100000    ; -> obj1:0x00DC
        jbe    loc_4_0D83
        shl    edx, 2
loc_4_0D83:
        xor    eax, eax
        stc    
        ret    
loc_4_0D87:
        push   edx
        push   eax
        push   ebx
        add    ebx, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [ebx + 5], al
        xor    al, al
        cmp    dword ptr [0xdc], 0x100000    ; -> obj1:0x00DC
        jbe    loc_4_0DB2
        mov    al, 0x20
loc_4_0DB2:
        out    dx, al
        rol    eax, 8
        dec    dl
        out    dx, al
        pop    ebx
        pop    eax
        pop    edx
        stc    
        ret    
loc_4_0DBE:
        push   edx
        push   eax
        push   ebx
        add    ebx, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xb
        mov    ah, byte ptr [ebx + 5]
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    ebx
        pop    eax
        pop    edx
        stc    
        ret    
loc_4_0DE0:
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
        rol    eax, 8
        out    dx, al
        pop    eax
        pop    edx
        pop    ecx
        stc    
        ret    
VxD__LTEXT ENDS
