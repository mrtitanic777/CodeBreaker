; CHIPS -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 611 instructions recovered by recursive descent

Declare_Virtual_Device CHIPS, 3, 1, CHIPS_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x140  R-X
CHIPS_Control:
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
        db  'CHIPS   '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  CHIPS_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h
loc_1_00ED:
        inc    dword ptr [eax]
loc_1_00EF:
        add    byte ptr [eax], al
loc_1_00F1:
        add    byte ptr [eax], al
loc_1_00F3:
        add    byte ptr [eax], al
loc_1_00F5:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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

VxD__LTEXT SEGMENT   ; object 2  vsize=0x1b0  R-X
loc_2_0000:
        mov    dword ptr [0xd0], ebx         ; -> obj1:0x00D0
        push   0
        push   0x10000
        push   0xa0000
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_2_019E
        mov    dword ptr [0xd4], eax         ; -> obj1:0x00D4
        mov    edx, 0x3d6
        in     al, dx
        cmp    al, 0xff
        je     loc_2_019E
        push   ebx
        mov    eax, 0x5f00
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        cmp    ax, 0x15f
        mov    ah, bl
        pop    ebx
        jne    loc_2_017A
        or     ah, ah
        je     loc_2_019E
        cmp    ah, 0xff
        je     loc_2_019E
loc_2_0063:
        mov    al, ah
        and    ax, 0xfc03
        mov    byte ptr [0xf2], al           ; -> obj1:0x00F2
        mov    byte ptr [0xf3], ah           ; -> loc_1_00F3
        mov    al, 0
        cmp    ah, 0xb0
        jb     loc_2_0087
        cmp    ah, 0xc0
        je     loc_2_0087
        cmp    ah, 0xd0
        je     loc_2_0087
        mov    al, 2
loc_2_0087:
        mov    byte ptr [0xf4], al           ; -> obj1:0x00F4
        mov    edx, 0x80000
        cmp    byte ptr [0xf3], 0x80         ; -> loc_1_00F3
        jb     loc_2_00BE
        push   edx
        mov    eax, 0x5f10
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edx
        cmp    ax, 0x15f
        jne    loc_2_00BE
        or     byte ptr [0xf4], 1            ; -> obj1:0x00F4
        ror    esi, 0x10
        mov    si, di
        mov    edx, esi
loc_2_00BE:
        mov    dword ptr [0xe0], edx         ; -> obj1:0x00E0
        push   0
        push   0x103
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0xe4], eax         ; -> obj1:0x00E4
        or     eax, eax
        je     loc_2_019E
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_019E
        mov    dword ptr [edi], 0            ; -> loc_3_0000
        mov    dword ptr [edi + 4], 0x1ff    ; -> loc_3_01FF
        mov    dword ptr [edi + 8], 0        ; -> loc_5_0000
        mov    dword ptr [edi + 0xc], 0x4d   ; -> loc_5_004D
        mov    dword ptr [edi + 0x58], 0xb2  ; -> loc_5_00B2
        mov    dword ptr [edi + 0x5c], 0xeb  ; -> loc_5_00EB
        mov    dword ptr [edi + 0x14], 0x111 ; -> loc_5_0111
        mov    dword ptr [edi + 0x3c], 0x231 ; -> loc_5_0231
        mov    dword ptr [edi + 0x2c], 0x15b ; -> loc_5_015B
        mov    dword ptr [edi + 0x30], 0x16f ; -> loc_5_016F
        mov    dword ptr [edi + 0x20], 0x131 ; -> loc_5_0131
        mov    dword ptr [edi + 0xa8], 0x1a0 ; -> loc_2_01A0
        mov    dword ptr [edi + 0x90], 0x270 ; -> loc_5_0270
        mov    dword ptr [edi + 0x70], 0x87  ; -> loc_3_0087
        mov    edx, 0x3d6
        mov    ecx, 0x195                    ; -> loc_5_0195
        VxDCall  VDD.VDD_Takeover_VGA_Port       ; cd2013000a00
        mov    edx, 0x3d7
        mov    ecx, 0x195                    ; -> loc_5_0195
        VxDCall  VDD.VDD_Takeover_VGA_Port       ; cd2013000a00
        clc    
        jmp    loc_2_019F
loc_2_017A:
        mov    dx, 0x3d6
        in     al, dx
        ror    eax, 8
        xor    al, al
        out    dx, al
        jmp    loc_2_0187
loc_2_0187:
        in     ax, dx
        rol    eax, 8
        out    dx, al
        ror    eax, 8
        test   al, al
        jne    loc_2_019E
        cmp    ah, 0xff
        je     loc_2_019E
        jmp    loc_2_0063
loc_2_019E:
        stc    
loc_2_019F:
        ret    
loc_2_01A0:
        movzx  eax, byte ptr [0xf2]          ; -> obj1:0x00F2
        or     al, byte ptr [0xf3]           ; -> loc_1_00F3
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x26d  R-X
loc_3_0000:
        movzx  eax, word ptr [ebp + 0x14]
        or     eax, eax
        je     loc_3_000E
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
loc_3_000E:
        xor    eax, eax
        mov    dword ptr [0xd8], eax         ; -> obj1:0x00D8
        mov    dword ptr [0xdc], eax         ; -> obj1:0x00DC
        test   byte ptr [0xf4], 1            ; -> obj1:0x00F4
        je     loc_3_006A
        mov    eax, dword ptr [ebp + 0x14]
        shr    eax, 0x10
        je     loc_3_0031
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
loc_3_0031:
        cmp    word ptr [ebp], 0
        je     loc_3_004D
        mov    ax, 0x3c00
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [0xd8], eax         ; -> obj1:0x00D8
        VxDCall  VDD.VDD_Set_Sleep_Flag_Addr     ; cd2016000a00
loc_3_004D:
        mov    eax, dword ptr [ebp + 0x14]
        shr    eax, 0x10
        je     loc_3_0065
        mov    ah, 0x16
        mov    al, 0xff
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        jne    loc_3_0065
        inc    eax
loc_3_0065:
        mov    dword ptr [0xdc], eax         ; -> obj1:0x00DC
loc_3_006A:
        push   eax
        push   edx
        mov    edx, 0x3d6
        in     al, dx
        ror    eax, 8
        mov    al, 0xb
        out    dx, al
        in     ax, dx
        mov    word ptr [0xe8], ax           ; -> obj1:0x00E8
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret    
loc_3_0087:
        mov    eax, 0xf5                     ; -> loc_1_00F5
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        push   eax
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        pop    eax
        test   word ptr [eax + 2], 0x110
        jne    loc_3_01FE
        movzx  edi, word ptr [eax + 0x20]
        or     edi, edi
        je     loc_3_01FE
        movzx  esi, word ptr [eax + 0x18]
        or     esi, esi
        je     loc_3_01FE
        movzx  edx, byte ptr [eax + 0x1f]
        xor    eax, eax
        cmp    esi, 0x280
        je     loc_3_0104
        mov    al, 2
        cmp    esi, 0x320
        je     loc_3_0104
        mov    al, 4
        cmp    esi, 0x400
        je     loc_3_0104
        mov    al, 7
        cmp    esi, 0x480
        je     loc_3_0104
        mov    al, 8
        cmp    esi, 0x500
        je     loc_3_0104
        add    al, 0xc
        cmp    esi, 0x640
        jne    loc_3_01FE
loc_3_0104:
        mov    ah, 0x20
        cmp    dl, 4
        je     loc_3_0124
        mov    ah, 0x30
        cmp    dl, 8
        je     loc_3_0124
        mov    ah, 0x40
        cmp    dl, 0x10
        je     loc_3_0124
        mov    ah, 0x50
        cmp    dl, 0x18
        jne    loc_3_01FE
loc_3_0124:
        or     al, ah
        mov    ah, 0
        push   ebx
        push   eax
        xchg   ebx, eax
        mov    eax, 0x5f04
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        movzx  eax, ax
        cmp    ax, 0x15f
        je     loc_3_0189
        pop    eax
        pop    ebx
        cmp    byte ptr [0xf3], 0x80         ; -> loc_1_00F3
        jb     loc_3_01FE
        cmp    byte ptr [0xf3], 0xa0         ; -> loc_1_00F3
        je     loc_3_01FE
        and    al, 0xf
        cmp    al, 4
        jne    loc_3_01FE
        cmp    edi, 0x3c
        jb     loc_3_01FE
        mov    edx, 0x3d6
        in     al, dx
        ror    eax, 8
        mov    al, 0xf
        out    dx, al
        in     ax, dx
        or     ah, 0x40
        out    dx, ax
        rol    eax, 8
        out    dx, al
        jmp    loc_3_01FE
loc_3_0189:
        or     bl, bl
        je     loc_3_01E5
        mov    al, 0x80
        test   al, bl
        je     loc_3_0198
        cmp    edi, 0x5a
        jae    loc_3_01E9
loc_3_0198:
        mov    al, 0x40
        test   al, bl
        je     loc_3_01A3
        cmp    edi, 0x54
        jae    loc_3_01E9
loc_3_01A3:
        mov    al, 0x20
        test   al, bl
        je     loc_3_01AE
        cmp    edi, 0x4b
        jae    loc_3_01E9
loc_3_01AE:
        mov    al, 0x10
        test   al, bl
        je     loc_3_01B9
        cmp    edi, 0x48
        jae    loc_3_01E9
loc_3_01B9:
        mov    al, 8
        test   al, bl
        je     loc_3_01C4
        cmp    edi, 0x46
        jae    loc_3_01E9
loc_3_01C4:
        mov    al, 4
        test   al, bl
        je     loc_3_01CF
        cmp    edi, 0x3c
        jae    loc_3_01E9
loc_3_01CF:
        mov    al, 2
        test   al, bl
        je     loc_3_01DA
        cmp    edi, 0x38
        jae    loc_3_01E9
loc_3_01DA:
        mov    al, 1
        test   al, bl
        je     loc_3_01E5
        cmp    edi, 0x19
        jae    loc_3_01E9
loc_3_01E5:
        pop    eax
        pop    ebx
        jmp    loc_3_01FE
loc_3_01E9:
        cmp    al, bh
        pop    ebx
        je     loc_3_01FD
        mov    bh, al
        mov    eax, 0x5f05
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_3_01FD:
        pop    ebx
loc_3_01FE:
        ret    
loc_3_01FF:
        mov    eax, ecx
        shr    eax, 0x10
        or     cx, cx
        je     loc_3_0223
        mov    edx, dword ptr [0xe0]         ; -> obj1:0x00E0
        shr    edx, 0x10
        dec    dl
        cmp    al, dl
        jae    loc_3_026A
        inc    al
        xor    cx, cx
        add    ecx, 0x10000
loc_3_0223:
        mov    edx, dword ptr [0xe0]         ; -> obj1:0x00E0
        sub    edx, ecx
        cmp    edx, 0x10000
        mov    edx, 0x10000
        ja     loc_3_023D
        mov    edx, 0x8000
loc_3_023D:
        push   edx
        push   ebx
        mov    byte ptr [0xed], 0xff         ; -> loc_1_00ED
        mov    byte ptr [0xf1], 0            ; -> loc_1_00F1
        mov    bl, al
        mov    bh, al
        and    bl, 0xf
        shl    bl, 4
        mov    byte ptr [0xeb], bl           ; -> obj1:0x00EB
        and    bh, 0x30
        mov    byte ptr [0xec], bh           ; -> obj1:0x00EC
        pop    ebx
        pop    edx
        jmp    loc_3_026C
loc_3_026A:
        mov    edx, ecx
loc_3_026C:
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x7  R-X
        db  0E8h
        dd  loc_5_0131               ; fixup
        db  0F8h, 0C3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x278  R-X
loc_5_0000:
        cmp    byte ptr [0xed], 0xff         ; -> loc_1_00ED
        jne    loc_5_0044
        push   eax
        push   edx
        mov    edx, 0x3d6
        in     al, dx
        ror    eax, 8
        mov    al, 0x10
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0xed], al           ; -> loc_1_00ED
        mov    al, byte ptr [0xeb]           ; -> obj1:0x00EB
        out    dx, al
        dec    edx
        mov    al, 0xc
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        and    ax, 0xcf30
        mov    byte ptr [0xee], al           ; -> obj1:0x00EE
        mov    al, byte ptr [0xec]           ; -> obj1:0x00EC
        or     al, ah
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_5_0044:
        and    al, 0xf
        add    al, byte ptr [0xf1]           ; -> loc_1_00F1
        ret    
loc_5_004D:
        cmp    byte ptr [0xed], 0xff         ; -> loc_1_00ED
        je     loc_5_00B1
        push   eax
        push   edx
        mov    edx, 0x3d6
        in     al, dx
        ror    eax, 8
        mov    al, 0xc
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        and    ah, 0x30
        dec    edx
        mov    al, 0x10
        out    dx, al
        inc    edx
        in     al, dx
        mov    dl, byte ptr [0xeb]           ; -> obj1:0x00EB
        mov    dh, byte ptr [0xec]           ; -> obj1:0x00EC
        cmp    ax, dx
        jne    loc_5_009F
        mov    edx, 0x3d7
        mov    al, byte ptr [0xed]           ; -> loc_1_00ED
        out    dx, al
        dec    edx
        mov    al, 0xc
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xcf
        mov    ah, byte ptr [0xee]           ; -> obj1:0x00EE
        or     al, ah
        out    dx, al
        dec    edx
loc_5_009F:
        mov    byte ptr [0xed], 0xff         ; -> loc_1_00ED
        mov    edx, 0x3d6
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_5_00B1:
        ret    
loc_5_00B2:
        push   eax
        push   edx
        mov    edx, 0x3d6
        in     al, dx
        ror    eax, 8
        mov    al, 0x10
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0xef], al           ; -> loc_1_00EF
        mov    al, byte ptr [0xeb]           ; -> obj1:0x00EB
        out    dx, al
        dec    edx
        mov    al, 0xc
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0xf0], al           ; -> obj1:0x00F0
        mov    ah, byte ptr [0xec]           ; -> obj1:0x00EC
        and    al, 0xcf
        or     al, ah
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret    
loc_5_00EB:
        push   eax
        push   edx
        mov    edx, 0x3d6
        in     al, dx
        ror    eax, 8
        mov    al, 0x10
        mov    ah, byte ptr [0xef]           ; -> loc_1_00EF
        out    dx, ax
        mov    al, 0xc
        mov    ah, byte ptr [0xf0]           ; -> obj1:0x00F0
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        ret    
loc_5_0111:
        mov    edx, 0x3d6
        in     al, dx
        ror    eax, 8
        mov    ax, 0x10b
        out    dx, ax
        mov    ax, 0x10
        out    dx, ax
        mov    ax, 0x11
        out    dx, ax
        rol    eax, 8
        out    dx, al
        ret    
loc_5_0131:
        push   edx
        push   edi
        mov    edx, 0x3d6
        in     al, dx
        ror    eax, 8
        mov    edi, ebx
        add    edi, dword ptr [0xe4]         ; -> obj1:0x00E4
        mov    ecx, 0xff
loc_5_0149:
        mov    al, cl
        out    dx, al
        in     ax, dx
        mov    byte ptr [ecx + edi], ah
        dec    ecx
        jns    loc_5_0149
        rol    eax, 8
        out    dx, al
        pop    edi
        pop    edx
        ret    
loc_5_015B:
        mov    edx, 0x3d6
        in     al, dx
        ror    eax, 8
        mov    ax, 0x10b
        out    dx, ax
        rol    eax, 8
        out    dx, al
        ret    
loc_5_016F:
        mov    edx, 0x3d6
        in     al, dx
        ror    eax, 8
        mov    ax, word ptr [0xe8]           ; -> obj1:0x00E8
        out    dx, ax
        mov    ax, 0x10
        out    dx, ax
        mov    al, 0xc
        out    dx, al
        in     ax, dx
        and    ah, 0xcf
        out    dx, ax
        rol    eax, 8
        out    dx, al
        ret    
loc_5_0195:
        push   edi
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    edi, dword ptr [0xd0]         ; -> obj1:0x00D0
        jne    loc_5_01A9
        cmp    ebx, edi
        jne    loc_5_01C6
loc_5_01A9:
        cmp    ecx, 4
        je     loc_5_01C3
        cmp    ecx, 0xc
        je     loc_5_01BF
        cmp    ecx, 8
        je     loc_5_01BB
        in     al, dx
        jmp    loc_5_022E
loc_5_01BB:
        in     ax, dx
        jmp    loc_5_022E
loc_5_01BF:
        out    dx, ax
        jmp    loc_5_022E
loc_5_01C3:
        out    dx, al
        jmp    loc_5_022E
loc_5_01C6:
        mov    edi, ebx
        add    edi, dword ptr [0xe4]         ; -> obj1:0x00E4
        cmp    ecx, 4
        je     loc_5_0214
        cmp    ecx, 0xc
        je     loc_5_0204
        cmp    ecx, 8
        je     loc_5_01F8
        cmp    dx, 0x3d6
        jne    loc_5_01EC
        mov    al, byte ptr [edi + 0x100]
        jmp    loc_5_022E
loc_5_01EC:
        movzx  eax, byte ptr [edi + 0x100]
        mov    al, byte ptr [eax + edi]
        jmp    loc_5_022E
loc_5_01F8:
        movzx  eax, byte ptr [edi + 0x100]
        mov    ah, byte ptr [eax + edi]
        jmp    loc_5_022E
loc_5_0204:
        mov    byte ptr [edi + 0x100], al
        mov    cl, ah
        movzx  eax, al
        mov    byte ptr [eax + edi], cl
        jmp    loc_5_022E
loc_5_0214:
        cmp    edx, 0x3d6
        jne    loc_5_0224
        mov    byte ptr [edi + 0x100], al
        jmp    loc_5_022E
loc_5_0224:
        movzx  ecx, byte ptr [edi + 0x100]
        mov    byte ptr [ecx + edi], al
loc_5_022E:
        pop    esi
        pop    edi
        ret    
loc_5_0231:
        test   byte ptr [0xf4], 1            ; -> obj1:0x00F4
        je     loc_5_026F
        push   edx
loc_5_023B:
        mov    edx, dword ptr [0xdc]         ; -> obj1:0x00DC
        or     edx, edx
        je     loc_5_024D
        mov    eax, dword ptr [edx + 0x93d0]
        jmp    loc_5_0253
loc_5_024D:
        mov    edx, 0x93d0
        in     eax, dx
loc_5_0253:
        cmp    eax, -1
        je     loc_5_026E
        test   ax, 0x4000
        je     loc_5_0267
        test   eax, 0x100000
        je     loc_5_0267
        jmp    loc_5_026E
loc_5_0267:
        test   eax, 0x100000
        jne    loc_5_023B
loc_5_026E:
        pop    edx
loc_5_026F:
        ret    
loc_5_0270:
        mov    ecx, dword ptr [0xe0]         ; -> obj1:0x00E0
        stc    
        ret    
VxD__LTEXT ENDS
