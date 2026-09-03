; VIDEO7 -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 550 instructions recovered by recursive descent

Declare_Virtual_Device VIDEO7, 3, 1, VIDEO7_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x17c  R-X
VIDEO7_Control:
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
loc_1_007D:
        mov    ecx, dword ptr [0x16a]        ; -> obj1:0x016A
        stc    
        ret    
        db  000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VIDEO7  '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  VIDEO7_Control           ; DDB_Control_Proc
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h
loc_1_0172:
        inc    dword ptr [eax]
loc_1_0174:
        add    byte ptr [eax], al
loc_1_0176:
        add    byte ptr [eax], al
loc_1_0178:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x100  R-X
loc_2_0000:
        push   ebx
        mov    dword ptr [0xd8], ebx         ; -> obj1:0x00D8
        xor    ebx, ebx
        mov    eax, 0x6f00
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        cmp    bx, 0x5637
        mov    ebx, dword ptr [0xd8]         ; -> obj1:0x00D8
        jne    loc_2_00F5
        push   0
        push   0x82
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0xdc], eax         ; -> obj1:0x00DC
        or     eax, eax
        je     loc_2_00F5
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_00F5
        mov    dword ptr [edi], 0            ; -> loc_3_0000
        mov    dword ptr [edi + 4], 0x13     ; -> loc_3_0013
        mov    dword ptr [edi + 8], 0        ; -> loc_4_0000
        mov    dword ptr [edi + 0xc], 0x75   ; -> loc_4_0075
        mov    dword ptr [edi + 0x58], 0x11b ; -> loc_4_011B
        mov    dword ptr [edi + 0x5c], 0x178 ; -> loc_4_0178
        mov    dword ptr [edi + 0x20], 0x1b5 ; -> loc_4_01B5
        mov    dword ptr [edi + 0x24], 0x237 ; -> loc_4_0237
        mov    dword ptr [edi + 0x4c], 0x30d ; -> loc_4_030D
        mov    dword ptr [edi + 0x54], 0x420 ; -> loc_4_0420
        mov    dword ptr [edi + 0x48], 0x3da ; -> loc_4_03DA
        mov    dword ptr [edi + 0x68], 0xa2  ; -> loc_3_00A2
        mov    dword ptr [edi + 0x10], 0x2fd ; -> loc_4_02FD
        mov    dword ptr [edi + 0x14], 0x305 ; -> loc_4_0305
        mov    dword ptr [edi + 0x18], 0x2fd ; -> loc_4_02FD
        mov    dword ptr [edi + 0x1c], 0x305 ; -> loc_4_0305
        mov    dword ptr [edi + 0xa8], 0xf8  ; -> loc_2_00F8
        mov    dword ptr [edi + 0xb4], 0xaa  ; -> loc_3_00AA
        mov    dword ptr [edi + 0x90], 0x7d  ; -> loc_1_007D
        VxDCall  VDD.VDD_Get_Special_VM_IDs      ; cd2011000a00
        mov    dword ptr [0xe4], edi         ; -> obj1:0x00E4
        mov    dword ptr [0xe0], esi         ; -> obj1:0x00E0
        clc    
        jmp    loc_2_00F6
loc_2_00F5:
        stc    
loc_2_00F6:
        pop    ebx
        ret    
loc_2_00F8:
        mov    eax, 0x5637
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0xd1  R-X
loc_3_0000:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [0x16a], eax        ; -> obj1:0x016A
        movzx  eax, word ptr [ebp + 0x14]
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
        ret    
loc_3_0013:
        mov    byte ptr [0x176], 0xff        ; -> loc_1_0176
        mov    ecx, 0xc0000
        cmp    ecx, dword ptr [0x16a]        ; -> obj1:0x016A
        jae    loc_3_009F
        mov    eax, ecx
        shr    eax, 0x10
        or     cx, cx
        je     loc_3_004B
        mov    edx, dword ptr [0x16a]        ; -> obj1:0x016A
        shr    edx, 0x10
        dec    dl
        cmp    al, dl
        jae    loc_3_009F
        inc    al
        xor    cx, cx
        add    ecx, 0x10000
loc_3_004B:
        mov    edx, dword ptr [0x16a]        ; -> obj1:0x016A
        sub    edx, ecx
        cmp    edx, 0x10000
        mov    edx, 0x10000
        ja     loc_3_0065
        mov    edx, 0x8000
loc_3_0065:
        push   edx
        push   ebx
        mov    byte ptr [0x172], 0xff        ; -> loc_1_0172
        mov    byte ptr [0x16e], al          ; -> obj1:0x016E
        mov    bl, al
        and    bl, 1
        mov    byte ptr [0x16f], bl          ; -> obj1:0x016F
        mov    ah, al
        and    ah, 2
        shl    ah, 4
        mov    byte ptr [0x171], ah          ; -> obj1:0x0171
        and    al, 0xc
        mov    bh, al
        shr    al, 2
        or     bh, al
        mov    byte ptr [0x170], bh          ; -> obj1:0x0170
        pop    ebx
        pop    edx
        jmp    loc_3_00A1
loc_3_009F:
        mov    edx, ecx
loc_3_00A1:
        ret    
loc_3_00A2:
        mov    byte ptr [0x176], 0           ; -> loc_1_0176
        ret    
loc_3_00AA:
        cld    
        push   esi
        push   edi
        push   ecx
        mov    esi, dword ptr [0xd8]         ; -> obj1:0x00D8
        add    esi, dword ptr [0xdc]         ; -> obj1:0x00DC
        mov    edi, 0xe8                     ; -> obj1:0x00E8
        mov    ecx, 0x82
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    byte ptr [0x179], 0xff        ; -> obj1:0x0179
        pop    ecx
        pop    edi
        pop    esi
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x436  R-X
loc_4_0000:
        cmp    byte ptr [0x172], 0xff        ; -> loc_1_0172
        jne    loc_4_0074
        cmp    byte ptr [0x176], 0           ; -> loc_1_0176
        je     loc_4_0074
        push   eax
        push   ecx
        push   edx
        mov    edx, 0x3cc
        in     al, dx
        mov    cl, al
        shr    cl, 4
        and    cl, 2
        and    al, 0xdf
        or     al, byte ptr [0x171]          ; -> obj1:0x0171
        mov    dl, 0xc2
        out    dx, al
        mov    dl, 0xc4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xea06
        out    dx, ax
        mov    al, 0xf9
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 1
        or     cl, al
        mov    al, byte ptr [0x16f]          ; -> obj1:0x016F
        out    dx, al
        dec    dl
        mov    al, 0xf6
        out    dx, al
        inc    dl
        in     al, dx
        mov    ch, al
        and    ch, 3
        shl    ch, 2
        or     cl, ch
        mov    byte ptr [0x172], cl          ; -> loc_1_0172
        and    al, 0xf0
        or     al, byte ptr [0x170]          ; -> obj1:0x0170
        out    dx, al
        dec    dl
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    ecx
        pop    eax
loc_4_0074:
        ret    
loc_4_0075:
        cmp    byte ptr [0x172], 0xff        ; -> loc_1_0172
        je     loc_4_011A
        push   eax
        push   ecx
        push   edx
        mov    edx, 0x3cc
        in     al, dx
        mov    cl, al
        shr    cl, 4
        and    cl, 2
        mov    dl, 0xc4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xea06
        out    dx, ax
        mov    al, 0xf9
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 1
        or     cl, al
        dec    dl
        mov    al, 0xf6
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 3
        shl    al, 2
        or     cl, al
        dec    dl
        rol    eax, 8
        out    dx, al
        cmp    cl, byte ptr [0x16e]          ; -> obj1:0x016E
        jne    loc_4_0110
        mov    dl, 0xcc
        in     al, dx
        mov    ah, byte ptr [0x172]          ; -> loc_1_0172
        and    ah, 2
        shl    ah, 4
        and    ax, 0x20df
        or     al, ah
        out    dx, al
        mov    dl, 0xc4
        in     al, dx
        ror    eax, 8
        mov    al, 0xf9
        mov    ah, byte ptr [0x172]          ; -> loc_1_0172
        and    ah, 1
        out    dx, ax
        mov    al, byte ptr [0x172]          ; -> loc_1_0172
        and    al, 0xc
        mov    ah, al
        shr    al, 2
        or     ah, al
        mov    al, 0xf6
        out    dx, al
        inc    dl
        in     al, dx
        and    ax, 0xff0
        or     al, ah
        out    dx, al
        dec    dl
        rol    eax, 8
        out    dx, al
loc_4_0110:
        mov    byte ptr [0x172], 0xff        ; -> loc_1_0172
        pop    edx
        pop    ecx
        pop    eax
loc_4_011A:
        ret    
loc_4_011B:
        cmp    byte ptr [0x176], 0           ; -> loc_1_0176
        je     loc_4_0177
        push   eax
        push   edx
        mov    edx, 0x3cc
        in     al, dx
        mov    byte ptr [0x175], al          ; -> obj1:0x0175
        and    al, 0xdf
        or     al, byte ptr [0x171]          ; -> obj1:0x0171
        mov    dl, 0xc2
        out    dx, al
        mov    dl, 0xc4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xea06
        out    dx, ax
        mov    al, 0xf9
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [0x173], al          ; -> obj1:0x0173
        mov    al, byte ptr [0x16f]          ; -> obj1:0x016F
        out    dx, al
        dec    dl
        mov    al, 0xf6
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [0x174], al          ; -> loc_1_0174
        and    al, 0xf0
        or     al, byte ptr [0x170]          ; -> obj1:0x0170
        out    dx, al
        dec    dl
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_0177:
        ret    
loc_4_0178:
        cmp    byte ptr [0x176], 0           ; -> loc_1_0176
        je     loc_4_01B4
        push   eax
        push   edx
        mov    edx, 0x3c2
        mov    al, byte ptr [0x175]          ; -> obj1:0x0175
        out    dx, al
        mov    dl, 0xc4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xea06
        out    dx, ax
        mov    al, 0xf9
        mov    ah, byte ptr [0x173]          ; -> obj1:0x0173
        out    dx, ax
        mov    al, 0xf6
        mov    ah, byte ptr [0x174]          ; -> loc_1_0174
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_01B4:
        ret    
loc_4_01B5:
        push   edx
        push   edi
        cmp    ebx, dword ptr [0xe4]         ; -> obj1:0x00E4
        je     loc_4_0234
        cmp    ebx, dword ptr [0xe0]         ; -> obj1:0x00E0
        je     loc_4_0234
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        cmp    ebx, dword ptr [0xd8]         ; -> obj1:0x00D8
        je     loc_4_020D
        push   ebx
        add    ebx, dword ptr [0xdc]         ; -> obj1:0x00DC
        test   byte ptr [ebx + 0x80], 1
        pop    ebx
        jne    loc_4_020D
        mov    al, 4
        out    dx, al
        inc    dl
        in     al, dx
        cmp    al, 6
        jne    loc_4_01F6
        mov    al, 2
        out    dx, al
loc_4_01F6:
        mov    dl, 0xce
        in     al, dx
        mov    ah, al
        mov    al, 8
        out    dx, al
        inc    dl
        in     al, dx
        or     al, al
        jne    loc_4_0208
        dec    al
        out    dx, al
loc_4_0208:
        dec    dl
        mov    al, ah
        out    dx, al
loc_4_020D:
        mov    dl, 0xc4
        mov    ax, 0xea06
        out    dx, ax
        mov    edi, ebx
        add    edi, dword ptr [0xdc]         ; -> obj1:0x00DC
        mov    ecx, 0x80
loc_4_0222:
        mov    al, cl
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [ecx + edi - 0x80], al
        dec    edx
        inc    cl
        jne    loc_4_0222
        rol    eax, 8
        out    dx, al
loc_4_0234:
        pop    edi
        pop    edx
        ret    
loc_4_0237:
        push   edx
        cmp    ebx, dword ptr [0xd8]         ; -> obj1:0x00D8
        je     loc_4_027F
        cmp    ebx, dword ptr [0xe4]         ; -> obj1:0x00E4
        je     loc_4_027F
        cmp    ebx, dword ptr [0xe0]         ; -> obj1:0x00E0
        je     loc_4_02FB
        push   ebx
        add    ebx, dword ptr [0xdc]         ; -> obj1:0x00DC
        test   byte ptr [ebx + 0x80], 1
        pop    ebx
        jne    loc_4_027F
        mov    edx, 0x3ce
        in     al, dx
        mov    ah, al
        mov    al, 8
        out    dx, al
        inc    dl
        in     al, dx
        or     al, al
        jne    loc_4_027A
        dec    al
        out    dx, al
loc_4_027A:
        dec    dl
        mov    al, ah
        out    dx, al
loc_4_027F:
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xea06
        out    dx, ax
        cmp    ecx, dword ptr [0xe4]         ; -> obj1:0x00E4
        jne    loc_4_02A1
        mov    ecx, 0xe8                     ; -> obj1:0x00E8
        sub    ecx, dword ptr [0xdc]         ; -> obj1:0x00DC
loc_4_02A1:
        add    ecx, dword ptr [0xdc]         ; -> obj1:0x00DC
        xor    al, al
        or     al, byte ptr [ecx + 0x24]
        or     al, byte ptr [ecx + 0x78]
        or     al, byte ptr [ecx + 0x7d]
        je     loc_4_02F7
        mov    al, 0xa4
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        cmp    al, byte ptr [ecx + 0x24]
        jne    loc_4_02D5
        mov    al, 0xf8
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        cmp    al, byte ptr [ecx + 0x78]
        jne    loc_4_02D5
        mov    al, 0xfd
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        cmp    al, byte ptr [ecx + 0x7d]
        je     loc_4_02F7
loc_4_02D5:
        mov    eax, 0x100
        out    dx, ax
        mov    al, 0xa4
        mov    ah, byte ptr [ecx + 0x24]
        out    dx, ax
        mov    al, 0xf8
        mov    ah, byte ptr [ecx + 0x78]
        out    dx, ax
        mov    al, 0xfd
        mov    ah, byte ptr [ecx + 0x7d]
        out    dx, ax
        mov    ax, 0x300
        out    dx, ax
loc_4_02F7:
        rol    eax, 8
        out    dx, al
loc_4_02FB:
        pop    edx
        ret    
loc_4_02FD:
        mov    byte ptr [0x177], 0xff        ; -> obj1:0x0177
        ret    
loc_4_0305:
        mov    byte ptr [0x177], 0           ; -> obj1:0x0177
        ret    
loc_4_030D:
        cmp    byte ptr [0x178], 0           ; -> loc_1_0178
        jne    loc_4_03D1
        mov    byte ptr [0x178], 0xff        ; -> loc_1_0178
        cmp    cl, 4
        jne    loc_4_0378
        cmp    ebx, dword ptr [0xd8]         ; -> obj1:0x00D8
        je     loc_4_0378
        cmp    al, 6
        jne    loc_4_0378
        push   ebx
        add    ebx, dword ptr [0xdc]         ; -> obj1:0x00DC
        mov    ah, byte ptr [ebx + 0x80]
        mov    ch, byte ptr [ebx + 0x81]
        pop    ebx
        cmp    ch, 3
        mov    ch, 0
        jne    loc_4_0378
        test   ah, 1
        jne    loc_4_0378
        push   eax
        push   ebx
        push   ecx
        push   edx
        mov    eax, 0x204
        mov    ecx, 0xc
        dec    dl
        VMMCall  VMM.Simulate_VM_IO              ; cd2030010100
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        je     loc_4_03BD
        jmp    loc_4_03CE
loc_4_0378:
        cmp    byte ptr [0x177], 0           ; -> obj1:0x0177
        jne    loc_4_03BD
        mov    edi, ebx
        add    edi, dword ptr [0xdc]         ; -> obj1:0x00DC
        cmp    cl, 2
        jne    loc_4_0396
        mov    byte ptr [edi + 0x81], al
        jmp    loc_4_03D1
loc_4_0396:
        cmp    cl, 0x80
        jb     loc_4_03D1
        and    ecx, 0xff
        sub    cl, 0x80
        mov    byte ptr [ecx + edi], al
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        jne    loc_4_03CE
        cmp    byte ptr [0x179], 0           ; -> obj1:0x0179
        je     loc_4_03CE
        add    cl, 0x80
loc_4_03BD:
        dec    dl
        mov    ch, al
        in     al, dx
        ror    eax, 8
        mov    ax, cx
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_4_03CE:
        stc    
        jmp    loc_4_03D2
loc_4_03D1:
        clc    
loc_4_03D2:
        mov    byte ptr [0x178], 0           ; -> loc_1_0178
        ret    
loc_4_03DA:
        pushf  
        cmp    cl, 0x80
        jb     loc_4_041D
        popf   
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        je     loc_4_0408
        cmp    byte ptr [0x177], 0           ; -> obj1:0x0177
        jne    loc_4_0408
        sub    ecx, 0x80
        add    ecx, dword ptr [0xdc]         ; -> obj1:0x00DC
        mov    al, byte ptr [ebx + ecx]
        stc    
        jmp    loc_4_041F
loc_4_0408:
        dec    dl
        in     al, dx
        mov    ah, al
        mov    al, cl
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        xchg   ah, al
        out    dx, al
        mov    al, ah
        stc    
        jmp    loc_4_041F
loc_4_041D:
        popf   
loc_4_041F:
        ret    
loc_4_0420:
        pushfd 
        cmp    cl, 6
        jne    loc_4_0434
        push   ebx
        add    ebx, dword ptr [0xdc]         ; -> obj1:0x00DC
        mov    byte ptr [ebx + 0x80], al
        pop    ebx
loc_4_0434:
        popfd  
        ret    
VxD__LTEXT ENDS
