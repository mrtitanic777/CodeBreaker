; COMPAQ -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 1618 instructions recovered by recursive descent

Declare_Virtual_Device COMPAQ, 3, 1, COMPAQ_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2d8  R-X
COMPAQ_Control:
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
sub_1_007D:
        mov    edx, 0x63ca
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x83c6
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x13c8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x13c9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        ret    
sub_1_00AA:
        mov    edx, 0x63ca
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x83c6
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x13c8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x13c9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        ret    
loc_1_00D7:
        push   edi
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        jne    loc_1_0111
        mov    edi, ebx
        add    edi, dword ptr [0x244]        ; -> obj1:0x0244
        cmp    ecx, 4
        je     loc_1_0107
        cmp    ecx, 0xc
        je     loc_1_010A
        cmp    ecx, 8
        je     loc_1_0100
        in     al, dx
        jmp    loc_1_01A1
loc_1_0100:
        in     ax, dx
        jmp    loc_1_01A1
loc_1_0107:
        out    dx, al
        jmp    loc_1_0169
loc_1_010A:
        out    dx, ax
        jmp    loc_1_0195
loc_1_0111:
        mov    edi, ebx
        add    edi, dword ptr [0x244]        ; -> obj1:0x0244
        cmp    ecx, 4
        je     loc_1_0169
        cmp    ecx, 0xc
        je     loc_1_0195
        cmp    ecx, 8
        je     loc_1_015B
        cmp    edx, 0x13c8
        jne    loc_1_0135
        mov    al, byte ptr [edi + 2]
        jmp    loc_1_01A1
loc_1_0135:
        cmp    edx, 0x13c9
        jne    loc_1_0142
        mov    al, byte ptr [edi + 3]
        jmp    loc_1_01A1
loc_1_0142:
        cmp    edx, 0x83c6
        jne    loc_1_014F
        mov    al, byte ptr [edi + 6]
        jmp    loc_1_01A1
loc_1_014F:
        in     al, dx
        mov    ah, byte ptr [edi + 5]
        and    ax, 0x1fe0
        or     al, ah
        jmp    loc_1_01A1
loc_1_015B:
        cmp    edx, 0x13c8
        jne    loc_1_01A1
        mov    ax, word ptr [edi + 2]
        jmp    loc_1_01A1
loc_1_0169:
        cmp    edx, 0x13c8
        jne    loc_1_0176
        mov    byte ptr [edi + 2], al
        jmp    loc_1_01A1
loc_1_0176:
        cmp    edx, 0x13c9
        jne    loc_1_0183
        mov    byte ptr [edi + 3], al
        jmp    loc_1_01A1
loc_1_0183:
        cmp    edx, 0x83c6
        jne    loc_1_0190
        mov    byte ptr [edi + 6], al
        jmp    loc_1_01A1
loc_1_0190:
        mov    byte ptr [edi + 5], al
        jmp    loc_1_01A1
loc_1_0195:
        cmp    edx, 0x13c8
        jne    loc_1_01A1
        mov    word ptr [edi + 2], ax
loc_1_01A1:
        pop    esi
        pop    edi
        ret    
loc_1_01A4:
        push   edi
        push   esi
        test   byte ptr [0x248], 3           ; -> obj1:0x0248
        jne    loc_1_01C9
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        je     loc_1_01C9
        cmp    ecx, 4
        jne    loc_1_01C9
        push   eax
        push   edx
        mov    dh, 0x83
        in     al, dx
        test   al, 0x80
        pop    edx
        pop    eax
        jne    loc_1_01CF
loc_1_01C9:
        VxDCall  VDD.VDD_Do_Physical_IO          ; cd2015000a00
loc_1_01CF:
        pop    esi
        pop    edi
        ret    
        db  000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'COMPAQ  '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  COMPAQ_Control           ; DDB_Control_Proc
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  081h, 000h, 000h, 000h
loc_1_02D0:
        inc    ebp
        inc    esp
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x320  R-X
loc_2_0000:
        mov    dword ptr [0x224], ebx        ; -> obj1:0x0224
        push   0
        push   0x10000
        push   0xa0000
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_2_0159
        mov    dword ptr [0x228], eax        ; -> obj1:0x0228
        call   sub_2_015B
        jb     loc_2_015A
        push   0
        push   8
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        pop    ecx
        pop    ecx
        mov    dword ptr [0x244], eax        ; -> obj1:0x0244
        or     eax, eax
        je     loc_2_0159
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_0159
        mov    dword ptr [edi], 0            ; -> loc_3_0000
        mov    dword ptr [edi + 0x10], 0x27  ; -> loc_4_0027
        mov    dword ptr [edi + 0x14], 0x36  ; -> loc_4_0036
        mov    dword ptr [edi + 0x18], 0x43  ; -> loc_4_0043
        mov    dword ptr [edi + 0x1c], 0x50  ; -> loc_4_0050
        mov    dword ptr [edi + 0x20], 0x5d  ; -> loc_4_005D
        mov    dword ptr [edi + 0x24], 0x8e  ; -> loc_4_008E
        mov    dword ptr [edi + 0x34], 0x7d  ; -> sub_1_007D
        mov    dword ptr [edi + 0x38], 0xaa  ; -> sub_1_00AA
        mov    dword ptr [edi + 0x90], 0xb0  ; -> loc_4_00B0
        mov    dword ptr [edi + 0xa8], 0x317 ; -> loc_2_0317
        mov    dword ptr [edi + 0xc0], 0xb8  ; -> loc_4_00B8
        mov    dword ptr [edi + 0xa4], 0x5fe ; -> obj4:0x05FE
        mov    dword ptr [edi + 0x90], 0xb0  ; -> loc_4_00B0
        VxDCall  VDD.VDD_Get_Special_VM_IDs      ; cd2011000a00
        mov    dword ptr [0x238], esi        ; -> obj1:0x0238
        mov    dword ptr [0x23c], edi        ; -> obj1:0x023C
        mov    esi, 0xd7                     ; -> loc_1_00D7
        mov    edx, 0x63ca
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0159
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    esi, 0xd7                     ; -> loc_1_00D7
        mov    edx, 0x13c8
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0159
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    esi, 0xd7                     ; -> loc_1_00D7
        mov    edx, 0x13c9
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0159
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    esi, 0xd7                     ; -> loc_1_00D7
        mov    edx, 0x83c6
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0159
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    esi, 0x1a4                    ; -> loc_1_01A4
        mov    edx, 0x13c6
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_0159
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        clc    
        jmp    loc_2_015A
loc_2_0159:
        stc    
loc_2_015A:
        ret    
sub_2_015B:
        mov    eax, 0xc0000
        mov    ecx, 0x8000
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_2_0315
        cmp    word ptr [eax], 0xaa55
        je     loc_2_01AD
        mov    eax, 0xe0000
        mov    ecx, 0x8000
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_2_0315
        cmp    word ptr [eax], 0xaa55
        jne    loc_2_0315
loc_2_01AD:
        movzx  edx, byte ptr [eax + 2]
        shl    edx, 9
        sub    edx, 0x16
        cmp    dword ptr [edx + eax], 0x504d4f43
        jne    loc_2_0315
        cmp    word ptr [edx + eax + 4], 0x5141
        jne    loc_2_0315
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        xor    al, al
        out    dx, al
        inc    edx
        in     al, dx
        mov    cl, al
        dec    edx
        mov    al, 3
        out    dx, al
        inc    edx
        in     al, dx
        mov    ch, al
        ror    ecx, 0x10
        dec    edx
        mov    al, 0xf
        out    dx, al
        inc    edx
        in     al, dx
        mov    ch, al
        mov    al, 5
        out    dx, al
        mov    edx, 0x63ca
        mov    al, 0
        out    dx, al
        mov    edx, 0x3ce
        mov    al, 0x10
        out    dx, al
        inc    edx
        in     al, dx
        mov    cl, al
        dec    edx
        ror    ecx, 0x10
        and    al, 0xf
        cmp    cl, al
        jne    loc_2_022C
        xor    al, al
        mov    ah, cl
        not    ah
        out    dx, ax
        mov    al, 0x10
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf
        cmp    ah, al
        je     loc_2_0315
loc_2_022C:
        ror    ecx, 0x10
        mov    ah, cl
        or     ah, 8
        mov    al, 0x10
        mov    edx, 0x3ce
        out    dx, ax
        mov    esi, 0x33c8
        test   cl, 4
        je     loc_2_024B
        mov    si, 0x53c8
loc_2_024B:
        ror    ecx, 0x10
        mov    ah, ch
        and    ah, 7
        mov    edx, esi
        in     al, dx
        cmp    ah, al
        jne    loc_2_029A
        not    ah
        and    ah, 7
        mov    al, 3
        mov    edx, 0x3ce
        out    dx, ax
        mov    edx, esi
        in     al, dx
        cmp    ah, al
        jne    loc_2_0315
        xor    ah, ah
        mov    edx, 0x3ce
        mov    al, 0xc
        out    dx, al
        inc    dx
        in     al, dx
        test   al, 0x80
        jns    loc_2_02C1
        shr    al, 2
        and    al, 7
        cmp    al, 2
        jg     loc_2_0315
        mov    byte ptr [0x24a], 0x41        ; -> obj1:0x024A
        jmp    loc_2_02E0
loc_2_029A:
        mov    edx, 0x63c0
        in     al, dx
        xor    ah, ah
        ror    eax, 0x10
        mov    al, 0xab
        out    dx, al
        mov    ah, al
        in     al, dx
        cmp    ah, al
        jne    loc_2_02BB
        mov    al, 0x54
        out    dx, al
        mov    ah, al
        in     al, dx
        cmp    ah, al
        jne    loc_2_02BB
        jmp    loc_2_02C1
loc_2_02BB:
        ror    eax, 0x10
        out    dx, al
        jmp    loc_2_0315
loc_2_02C1:
        mov    byte ptr [0x24a], 0x51        ; -> obj1:0x024A
        shr    al, 3
        and    al, 7
        cmp    al, 7
        je     loc_2_02E0
        cmp    al, 6
        je     loc_2_02E0
        mov    byte ptr [0x24a], 0x41        ; -> obj1:0x024A
        cmp    al, 5
        jl     loc_2_0315
loc_2_02E0:
        mov    edx, 0x3da
        in     al, dx
        mov    edx, 0x3c0
        mov    al, 0
        out    dx, al
        mov    edx, 0x3ce
        mov    al, 3
        mov    ah, ch
        out    dx, ax
        xor    al, al
        mov    ah, cl
        out    dx, ax
        ror    ecx, 0x10
        mov    al, 0x10
        mov    ah, cl
        out    dx, ax
        mov    al, 0xf
        mov    ah, ch
        out    dx, ax
        rol    eax, 8
        out    dx, al
        clc    
        jmp    loc_2_0316
loc_2_0315:
        stc    
loc_2_0316:
        ret    
loc_2_0317:
        movzx  eax, byte ptr [0x24a]         ; -> obj1:0x024A
        ret    
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0xa1  R-X
loc_3_0000:
        mov    eax, dword ptr [ebp]
        cmp    ax, 0
        je     loc_3_0014
        cmp    ax, 1
        je     loc_3_0056
        jmp    loc_3_00A0
loc_3_0014:
        shr    eax, 0xa
        mov    dword ptr [0x234], eax        ; -> obj1:0x0234
        mov    eax, dword ptr [ebp + 0x18]
        shl    eax, 0xa
        mov    dword ptr [0x240], eax        ; -> obj1:0x0240
        movzx  eax, word ptr [ebp + 0x14]
        or     eax, eax
        je     loc_3_0035
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
loc_3_0035:
        mov    eax, dword ptr [ebp + 0x14]
        shr    eax, 0x10
        or     eax, eax
        je     loc_3_0045
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
loc_3_0045:
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [0x230], eax        ; -> obj1:0x0230
        jmp    loc_3_00A0
loc_3_0056:
        mov    ecx, dword ptr [ebp + 0x18]
        push   0
        push   0x2cc                         ; -> obj1:0x02CC
        push   0x24b                         ; -> obj1:0x024B
        push   3
        push   0x2d0                         ; -> loc_1_02D0
        push   0
        push   ecx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        mov    ecx, eax
        cmp    eax, 0
        jne    loc_3_009D
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    edi, eax
        mov    esi, 0x24b                    ; -> obj1:0x024B
        mov    ecx, 0x20
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, 0
loc_3_009D:
        mov    dword ptr [ebp + 0x18], ecx
loc_3_00A0:
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xdd5  R-X
        db  050h, 052h, 066h, 0BAh, 0CEh, 003h, 0ECh, 0C1h, 0C8h, 008h, 066h, 0B8h, 00Fh, 005h, 066h, 0EFh
        db  0B0h, 010h, 0EEh, 042h, 0ECh, 0A8h, 008h, 075h, 006h, 00Ch, 008h, 0EEh, 0EBh, 000h, 0ECh, 04Ah
        db  0C1h, 0C0h, 008h, 0EEh, 05Ah, 058h, 0C3h
loc_4_0027:
        pushal 
        call   sub_1_00AA
        or     byte ptr [0x248], 2           ; -> obj1:0x0248
        popal  
        ret    
loc_4_0036:
        call   sub_1_007D
        and    byte ptr [0x248], 0xfd        ; -> obj1:0x0248
        ret    
loc_4_0043:
        call   sub_1_00AA
        or     byte ptr [0x248], 1           ; -> obj1:0x0248
        ret    
loc_4_0050:
        call   sub_1_007D
        and    byte ptr [0x248], 0xfe        ; -> obj1:0x0248
        ret    
loc_4_005D:
        push   edx
        push   edi
        mov    edi, ebx
        add    edi, dword ptr [0x244]        ; -> obj1:0x0244
        call   0
        mov    edx, 0x63ca
        in     al, dx
        and    al, 0x1f
        mov    byte ptr [edi + 5], al
        mov    edx, 0x83c6
        in     al, dx
        mov    byte ptr [edi + 6], al
        mov    edx, 0x13c8
        in     ax, dx
        mov    word ptr [edi + 2], ax
        pop    edi
        pop    edx
        ret    
loc_4_008E:
        cmp    byte ptr [0x249], 0           ; -> obj1:0x0249
        je     loc_4_00AF
        call   0
        push   edx
        add    ecx, dword ptr [0x244]        ; -> obj1:0x0244
        mov    edx, 0x63ca
        mov    al, byte ptr [ecx + 5]
        and    al, 0x1f
        out    dx, al
        pop    edx
loc_4_00AF:
        ret    
loc_4_00B0:
        mov    ecx, dword ptr [0x240]        ; -> obj1:0x0240
        stc    
        ret    
loc_4_00B8:
        cmp    ebx, dword ptr [0x224]        ; -> obj1:0x0224
        je     loc_4_00CF
        push   edi
        push   esi
        mov    edi, ebx
        add    edi, dword ptr [0x244]        ; -> obj1:0x0244
        mov    word ptr [edi], ax
        pop    esi
        pop    edi
loc_4_00CF:
        ret    
loc_4_00D0:
        sbb    byte ptr [esi], al
        add    byte ptr [eax], al
        cld    
        push   es
        add    byte ptr [eax], al
        xor    cl, byte ptr [eax]
        add    byte ptr [eax], al
        add    cl, byte ptr [edx]
        add    byte ptr [eax], al
        sbb    byte ptr [edx], cl
        add    byte ptr [eax], al
        sub    ecx, dword ptr [edx]
        add    byte ptr [eax], al
        jp     0xf4
        add    byte ptr [eax], al
        cld    
        or     al, byte ptr [eax]
        add    byte ptr [ecx], ah            ; -> loc_4_0B21
        or     eax, dword ptr [eax]
        add    byte ptr [edx], bh            ; -> loc_4_0B3A
        or     eax, dword ptr [eax]
        add    al, bl                        ; -> loc_4_0BD8
        or     eax, dword ptr [eax]
        add    bh, bl                        ; -> loc_4_0BDF
        or     eax, dword ptr [eax]
        add    dh, ah                        ; -> loc_4_0BE6
        or     eax, dword ptr [eax]
        add    ch, ch                        ; -> loc_4_0BED
        or     eax, dword ptr [eax]
        add    ah, dh                        ; -> loc_4_0BF4
        or     eax, dword ptr [eax]
        add    bl, bh                        ; -> loc_4_0BFB
        or     eax, dword ptr [eax]
        add    byte ptr [edx], al            ; -> loc_4_0C02
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [ecx], eax
        add    dword ptr [ebx], eax
        add    dword ptr [ecx + eax], eax
        add    eax, 0x11010701
        add    edi, edi
        db  0FFh
loc_4_013C:
        pop    esp
        add    al, byte ptr [eax]
        add    byte ptr [eax - 0x1ffbfffe], al
        add    dword ptr [eax], ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx + eax], bl      ; -> obj4:0x025C
        add    byte ptr [eax - 0x1ffbfffe], al
        add    dword ptr [eax], ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dh, ah                        ; -> obj4:0x03E6
        add    eax, dword ptr [eax]
        add    byte ptr [eax], ah
        add    eax, dword ptr [eax]
        add    al, 0x58
        add    cl, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esp + eax], ch      ; -> obj4:0x046C
        add    byte ptr [eax], al
        add    al, 0
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax + eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    al, 0
        add    al, 0
        add    ecx, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 5], bh        ; -> obj4:0x0578
        add    byte ptr [eax], al
        add    byte ptr [0x4000800], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pushal 
        add    eax, dword ptr [eax]
        add    byte ptr [eax - 0x1ff7fffe], al
        add    dword ptr [eax], edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        inc    dword ptr [ebx]
        add    byte ptr [esi + ecx], al
        add    byte ptr [ebx], al
        add    eax, dword ptr [eax]
        out    dx, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [edi + 1], bl
        dec    edi
        add    dl, byte ptr [eax + 3]
        add    dword ptr [ebx + edx*2], 0xb069f05
        pop    es
        or     byte ptr ds:[eax], al
        or     dword ptr [eax + 0xa], eax
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0xf000e00
        add    byte ptr [eax], dl
        ljmp   0x8013:0xdf12ac11
        adc    al, 0
        adc    eax, 0x170316e5
        jecxz  0x2bb
        inc    dword ptr [edx]
        push   ecx
        or     byte ptr [eax], al
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    al, byte ptr [ebx]
        add    eax, dword ptr [esp + eax]
        add    eax, 0x7060605
        pop    es
        or     byte ptr [eax], cl
        or     dword ptr [ecx], ecx
        or     cl, byte ptr [edx]
        or     ecx, dword ptr [ebx]
        or     al, 0xc
        or     eax, 0xf0e0e0d
        movups xmm0, xmmword ptr [ecx + 0x11]
        add    byte ptr [edx], dl
        movlps qword ptr [eax], xmm0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 0x7050600
        invd   
        inc    dword ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        inc    dword ptr [ebx]
        add    byte ptr [esi + ecx], al
        add    byte ptr [ebx], al
        jecxz  loc_4_02F0
loc_4_02F0:
        add    byte ptr [eax], al
        add    bl, al
        add    dword ptr [edi - 0x7afc5efe], ebx
        add    al, 0xa6
        add    eax, 0x70b061f
        or     byte ptr ds:[eax], al
        or     dword ptr [eax + 0xa], eax
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0xf000e00
        add    byte ptr [eax], dl
        ljmp   0x8013:0xdf12ac11
        adc    al, 0x40
        adc    eax, 0x170416e7
        jecxz  0x33b
        inc    dword ptr [edx]
        add    byte ptr [eax], al
        add    dword ptr [ecx], eax
        add    al, byte ptr [edx]
        add    eax, dword ptr [ebx]
        add    al, 4
        add    eax, 0x7060605
        pop    es
        or     byte ptr [eax], cl
        or     dword ptr [ecx], ecx
        or     cl, byte ptr [edx]
        or     ecx, dword ptr [ebx]
        or     al, 0xc
        or     eax, 0xf0e0e0d
        movups xmm0, xmmword ptr [ecx + 0x11]
        add    byte ptr [edx], dl
        movlps qword ptr [eax], xmm0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 0x7050600
        invd   
        inc    dword ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        inc    dword ptr [ebx]
        add    byte ptr [esi + ecx], al
        add    byte ptr [ebx], al
        add    eax, 0xef00
        add    byte ptr [eax], ah
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        pop    edi
        add    dword ptr [edi + 2], ecx
        push   eax
        add    eax, dword ptr [ecx - 0x60faacfc]
        push   es
        or     eax, dword ptr [edi]
        or     byte ptr ds:[eax], al
        or     dword ptr [eax + 0xa], eax
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0xf000e00
        add    byte ptr [eax], dl
        ljmp   0x13:0xdf12ac11
        adc    al, 0
        adc    eax, 0x170316e5
        jecxz  0x3bf
        inc    dword ptr [edx + 0x10]
        push   ecx
        or     byte ptr [eax], al
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    al, byte ptr [ebx]
        add    eax, dword ptr [esp + eax]
        add    eax, 0x7060605
        pop    es
        or     byte ptr [eax], cl
        or     dword ptr [ecx], ecx
        or     cl, byte ptr [edx]
        or     ecx, dword ptr [ebx]
        or     al, 0xc
        or     eax, 0xf0e0e0d
        movups xmm0, xmmword ptr [ecx + 0x11]
        add    byte ptr [edx], dl
        movlps qword ptr [eax], xmm0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 0x7050600
        invd   
        inc    dword ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        inc    dword ptr [ebx]
        add    byte ptr [esi + ecx], al
        add    byte ptr [ebx], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [edi + 1], bh
        arpl   word ptr [edx], ax
        add    eax, dword ptr fs:[eax + 0x19056804]
        push   es
        jae    0x412
        lock or byte ptr [eax], al
        or     dword ptr [eax + 0xa], esp
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0xf000e00
        add    byte ptr [eax], dl
        pop    eax
        adc    dword ptr [edx + edx + 0x14801357], edi
        add    byte ptr [0x17721658], dl
        jecxz  0x445
        inc    dword ptr [edx]
        push   ecx
        and    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    al, byte ptr [ebx]
        add    eax, dword ptr [esp + eax]
        add    eax, 0x7060605
        pop    es
        or     byte ptr [eax], cl
        or     dword ptr [ecx], ecx
        or     cl, byte ptr [edx]
        or     ecx, dword ptr [ebx]
        or     al, 0xc
        or     eax, 0xf0e0e0d
        movups xmm0, xmmword ptr [ecx + 0x11]
        add    byte ptr [edx], dl
        movlps qword ptr [eax], xmm0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 0x7050600
        invd   
        inc    dword ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        inc    dword ptr [ebx]
        add    byte ptr [esi + ecx], al
        add    byte ptr [ebx], al
        add    dword ptr [eax], eax
        jecxz  loc_4_047C
loc_4_047C:
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        pushal 
        add    byte ptr [eax], al
        wait   
        add    dword ptr [edi + 2], edi
        jg     loc_4_048C
        sahf   
        add    al, 0x87
loc_4_048C:
        add    eax, 0x7310617
        cmc    
        or     byte ptr [eax], al
        or     dword ptr [eax + 0xa], esp
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0xf000e00
        add    byte ptr [eax], dl
        or     eax, 0xff12b411
        adc    eax, dword ptr [eax + 0x14]
        add    byte ptr [0x173116ff], dl
        jecxz  0x4cb
        inc    dword ptr [edx]
        push   ecx
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    al, byte ptr [ebx]
        add    eax, dword ptr [esp + eax]
        add    eax, 0x7060605
        pop    es
        or     byte ptr [eax], cl
        or     dword ptr [ecx], ecx
        or     cl, byte ptr [edx]
        or     ecx, dword ptr [ebx]
        or     al, 0xc
        or     eax, 0xf0e0e0d
        movups xmm0, xmmword ptr [ecx + 0x11]
        add    byte ptr [edx], dl
        movlps qword ptr [eax], xmm0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 0x7050600
        invd   
        inc    dword ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        inc    dword ptr [ebx]
        add    byte ptr [esi + ecx], al
        add    byte ptr [ebx], al
        add    eax, dword ptr [eax]
        jecxz  loc_4_0502
loc_4_0502:
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        wait   
        add    dword ptr [edi + 2], edi
        jg     loc_4_0512
        sahf   
        add    al, 0x87
loc_4_0512:
        add    eax, 0x7310617
        cmc    
        or     byte ptr [eax], al
        or     dword ptr [eax + 0xa], esp
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0xf000e00
        add    byte ptr [eax], dl
        or     eax, 0xff12b411
        adc    eax, dword ptr [eax - 0xeaffec]
        push   ss
        xor    dword ptr [edi], edx
        jecxz  0x551
        inc    dword ptr [edx]
        push   ecx
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    dword ptr [edx], eax
        add    al, byte ptr [ebx]
        add    eax, dword ptr [esp + eax]
        add    eax, 0x7060605
        pop    es
        or     byte ptr [eax], cl
        or     dword ptr [ecx], ecx
        or     cl, byte ptr [edx]
        or     ecx, dword ptr [ebx]
        or     al, 0xc
        or     eax, 0xf0e0e0d
        movups xmm0, xmmword ptr [ecx + 0x11]
        add    byte ptr [edx], dl
        movlps qword ptr [eax], xmm0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 0x7050600
        invd   
        inc    dword ptr [eax]
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        inc    dword ptr [ebx]
        add    byte ptr [esi + ecx], al
        add    byte ptr [ebx], al
        add    eax, dword ptr [eax]
        and    eax, dword ptr [eax]
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        in     eax, dx
        add    dword ptr [edi + 0x10039f02], ebx
        add    al, 0xae
        add    eax, 0x72a0690
        pop    edx
        or     byte ptr [eax], al
        or     dword ptr [eax + 0xa], esp
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0xf000e00
        add    byte ptr [eax], dl
        add    edx, dword ptr [ecx]
        mov    bh, 0x12
        call   dword ptr [ebx]
        add    byte ptr [eax + eax], dl
        adc    eax, 0x172a16ff
        jecxz  0x5d7
        inc    dword ptr [edx + 1]
        push   ecx
        enter  0, 1
        add    dword ptr [edx], eax
        add    al, byte ptr [ebx]
        add    eax, dword ptr [esp + eax]
        add    eax, 0x7060605
        pop    es
        or     byte ptr [eax], cl
        or     dword ptr [ecx], ecx
        or     cl, byte ptr [edx]
        or     ecx, dword ptr [ebx]
        or     al, 0xc
        or     eax, 0xf0e0e0d
        movups xmm0, xmmword ptr [ecx + 0x11]
        add    byte ptr [edx], dl
        movlps qword ptr [eax], xmm0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 0x7050600
        invd   
        jmp    dword ptr [eax + 0x3c]
        db  010h, 077h, 00Ch, 00Fh, 0B6h, 0C0h, 0FFh, 014h, 085h
        dd  loc_4_00D0               ; fixup
        db  0EBh, 006h, 066h, 0C7h, 045h, 01Ch, 000h, 001h, 0F9h, 061h, 0C3h
loc_4_0618:
        mov    ah, 0x38
        mov    al, 0
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        je     loc_4_06F5
        cld    
        mov    cl, 1
        mov    edx, dword ptr [eax]
        cmp    dl, 0x56
        je     loc_4_063A
        cmp    dl, 0x76
        jne    loc_4_0658
loc_4_063A:
        cmp    dh, 0x42
        je     loc_4_0644
        cmp    dh, 0x62
        jne    loc_4_0658
loc_4_0644:
        shr    edx, 0x10
        cmp    dl, 0x45
        je     loc_4_0651
        cmp    dl, 0x65
        jne    loc_4_0658
loc_4_0651:
        cmp    dh, 0x32
        jne    loc_4_0658
        mov    cl, 2
loc_4_0658:
        mov    dword ptr [eax], 0x41534556
        mov    word ptr [eax + 4], 0x200
        mov    si, word ptr [ebp + 0x38]
        shl    esi, 0x10
        lea    edx, [eax + 0x14]
        mov    si, 0x14
        cmp    cl, 1
        je     loc_4_0681
        lea    edx, [eax + 0x100]
        mov    si, 0x100
loc_4_0681:
        mov    dword ptr [eax + 6], esi
        mov    dword ptr [edx], 0x515043
        mov    byte ptr [edx + 3], 0
        mov    dword ptr [eax + 0xa], 2
        push   ecx
        mov    si, word ptr [ebp + 0x38]
        shl    esi, 0x10
        lea    edi, [eax + 0x18]
        mov    si, 0x18
        cmp    cl, 1
        je     loc_4_06B0
        lea    edi, [eax + 0x22]
        mov    si, 0x22
loc_4_06B0:
        mov    dword ptr [eax + 0xe], esi
        mov    esi, 0x12c                    ; -> obj4:0x012C
        mov    ecx, 0x10
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        mov    edx, dword ptr [0x240]        ; -> obj1:0x0240
        shr    edx, 0x10
        mov    word ptr [eax + 0x12], dx
        cmp    cl, 1
        je     loc_4_06ED
        mov    word ptr [eax + 0x14], 0x200
        mov    dword ptr [eax + 0x16], 0
        mov    dword ptr [eax + 0x1a], 0
        mov    dword ptr [eax + 0x1e], 0
loc_4_06ED:
        mov    word ptr [ebp + 0x1c], 0x4f
        jmp    loc_4_06FB
loc_4_06F5:
        mov    word ptr [ebp + 0x1c], 0x14f
loc_4_06FB:
        ret    
loc_4_06FC:
        mov    ah, 0x38
        mov    al, 0
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        je     loc_4_082B
        cld    
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 0x40
        rep stosd dword ptr es:[edi], eax
        mov    ax, word ptr [ebp + 0x18]
        and    ax, 0x3f7f
        mov    ecx, 7
        mov    edi, 0x12c                    ; -> obj4:0x012C
        repne scasw ax, word ptr es:[edi]
        movzx  ecx, ax
        pop    eax
        jne    loc_4_082B
        or     word ptr [eax], 0x3b
        cmp    cl, 1
        jbe    loc_4_0752
        cmp    byte ptr [0x24a], 0x41        ; -> obj1:0x024A
        jne    loc_4_0752
loc_4_074D:
        and    byte ptr [eax], 0xfe
        jmp    loc_4_0774
loc_4_0752:
        cmp    cl, 4
        jbe    loc_4_0774
        cmp    dword ptr [0x240], 0x100000   ; -> obj1:0x0240
        jb     loc_4_074D
        cmp    cl, 7
        jne    loc_4_0774
        cmp    dword ptr [0x240], 0x200000   ; -> obj1:0x0240
        jb     loc_4_074D
loc_4_0774:
        mov    byte ptr [eax + 2], 7
        mov    byte ptr [eax + 3], 0
        mov    word ptr [eax + 4], 0x40
        mov    word ptr [eax + 6], 0x40
        mov    word ptr [eax + 8], 0xa000
        mov    word ptr [eax + 0xa], 0
        mov    dword ptr [eax + 0xc], 0
        movzx  esi, cl
        shl    esi, 4
        mov    edi, 0x13c                    ; -> loc_4_013C
        add    edi, esi
        mov    dx, word ptr [edi + 4]
        mov    word ptr [eax + 0x12], dx
        mov    dx, word ptr [edi + 6]
        mov    word ptr [eax + 0x10], dx
        mov    dx, word ptr [edi + 8]
        mov    word ptr [eax + 0x14], dx
        mov    dl, byte ptr [edi + 0xa]
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
        jne    loc_4_0803
        mov    byte ptr [eax + 0x1f], 5
        mov    byte ptr [eax + 0x20], 0xb
        mov    byte ptr [eax + 0x21], 6
        mov    byte ptr [eax + 0x22], 5
        mov    byte ptr [eax + 0x23], 5
        mov    byte ptr [eax + 0x24], 0
loc_4_0803:
        push   eax
        movzx  edx, word ptr [eax + 0x12]
        movzx  eax, word ptr [eax + 0x14]
        mul    edx
        mov    edx, eax
        pop    eax
        mov    dword ptr [eax + 0x2c], edx
        mov    ecx, dword ptr [0x240]        ; -> obj1:0x0240
        sub    ecx, edx
        shr    ecx, 0xa
        mov    word ptr [eax + 0x30], cx
        mov    word ptr [ebp + 0x1c], 0x4f
        jmp    loc_4_0831
loc_4_082B:
        mov    word ptr [ebp + 0x1c], 0x14f
loc_4_0831:
        ret    
loc_4_0832:
        cld    
        movzx  eax, word ptr [ebp + 0x10]
        test   ah, 4
        jne    loc_4_09FB
        and    ax, 0x3f7f
        cmp    ax, 0x13
        jbe    loc_4_09E7
        mov    ecx, 7
        mov    edi, 0x12c                    ; -> obj4:0x012C
        repne scasw ax, word ptr es:[edi]
        jne    loc_4_09FB
        cmp    al, 1
        jbe    loc_4_0872
        cmp    byte ptr [0x24a], 0x41        ; -> obj1:0x024A
        je     loc_4_09FB
loc_4_0872:
        cmp    al, 4
        jbe    loc_4_089A
        cmp    dword ptr [0x240], 0x100000   ; -> obj1:0x0240
        jb     loc_4_09FB
        cmp    al, 7
        jne    loc_4_089A
        cmp    dword ptr [0x240], 0x200000   ; -> obj1:0x0240
        jb     loc_4_09FB
loc_4_089A:
        cmp    byte ptr [0x24a], 0x41        ; -> obj1:0x024A
        jne    loc_4_08AA
        mov    esi, 0x2e2                    ; -> obj4:0x02E2
        jmp    loc_4_08B8
loc_4_08AA:
        sub    eax, 0x100
        shl    eax, 4
        mov    esi, dword ptr [eax + 0x13c]  ; -> loc_4_013C
loc_4_08B8:
        mov    edx, 0x3c4
        mov    ecx, 6
        rep outsw dx, word ptr [esi]
        mov    dl, 0xce
        mov    ax, 0x50f
        out    dx, ax
        mov    ax, 0x4140
        out    dx, ax
        mov    dl, 0xc4
        mov    ax, 0xff02
        out    dx, ax
        mov    dl, 0xce
        mov    ax, 0x2810
        out    dx, ax
        mov    dl, 0xd4
        mov    al, 0x11
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0x7f
        out    dx, al
        cmp    byte ptr [0x24a], 0x41        ; -> obj1:0x024A
        je     loc_4_08FF
        mov    edx, 0x63ca
        lodsw  ax, word ptr [esi]
        out    dx, al
loc_4_08FF:
        mov    edx, 0x3c2
        lodsw  ax, word ptr [esi]
        out    dx, al
        mov    edx, 0x83c6
        lodsw  ax, word ptr [esi]
        out    dx, al
        cmp    byte ptr [0x24a], 0x41        ; -> obj1:0x024A
        je     loc_4_0928
        mov    edx, 0x13c9
        lodsw  ax, word ptr [esi]
        out    dx, al
        mov    edx, 0x13c8
        lodsw  ax, word ptr [esi]
        out    dx, al
loc_4_0928:
        mov    edx, 0x3d4
        mov    ecx, 0x19
        rep outsw dx, word ptr [esi]
        mov    dl, 0xce
        outsw  dx, word ptr [esi]
        cmp    byte ptr [0x24a], 0x41        ; -> obj1:0x024A
        je     loc_4_0944
        outsw  dx, word ptr [esi]
loc_4_0944:
        mov    dl, 0xda
        in     al, dx
        mov    dl, 0xc0
        mov    cl, 0x14
loc_4_094B:
        lodsw  ax, word ptr [esi]
        out    dx, al
        mov    al, ah
        out    dx, al
        dec    cl
        jne    loc_4_094B
        mov    al, 0x20
        out    dx, al
        mov    dl, 0xce
        mov    ecx, 9
        rep outsw dx, word ptr [esi]
        cmp    byte ptr [ebp + 0x10], 7
        jne    loc_4_0982
        mov    edx, 0x83c6
        in     al, dx
        or     al, 0x80
        out    dx, al
        mov    edx, 0x3c8
        mov    al, 1
        out    dx, al
        mov    edx, 0x13c6
        in     al, dx
        or     al, 8
        out    dx, al
loc_4_0982:
        mov    ax, word ptr [ebp + 0x10]
        mov    edi, ebx
        add    edi, dword ptr [0x244]        ; -> obj1:0x0244
        mov    word ptr [edi], ax
        test   byte ptr [edi + 1], 0x80
        jne    loc_4_09F3
        mov    esi, dword ptr [0x240]        ; -> obj1:0x0240
        mov    edx, 0x3ce
        mov    eax, 0x8460045
        out    dx, ax
        ror    eax, 0x10
        out    dx, ax
        ror    eax, 0x10
loc_4_09B1:
        push   eax
        mov    edi, dword ptr [0x228]        ; -> obj1:0x0228
        mov    ecx, 0x4000
        xor    eax, eax
        rep stosd dword ptr es:[edi], eax
        pop    eax
        add    eax, 0x10001000
        out    dx, ax
        ror    eax, 0x10
        out    dx, ax
        ror    eax, 0x10
        sub    esi, 0x10000
        jg     loc_4_09B1
        mov    eax, 0x84600045
        out    dx, ax
        ror    eax, 0x10
        out    dx, ax
        jmp    loc_4_09F3
loc_4_09E7:
        movzx  eax, byte ptr [ebp + 0x10]
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_4_09F3:
        mov    word ptr [ebp + 0x1c], 0x4f
        jmp    loc_4_0A01
loc_4_09FB:
        mov    word ptr [ebp + 0x1c], 0x14f
loc_4_0A01:
        ret    
loc_4_0A02:
        mov    edi, ebx
        add    edi, dword ptr [0x244]        ; -> obj1:0x0244
        mov    ax, word ptr [edi]
        mov    word ptr [ebp + 0x10], ax
        mov    word ptr [ebp + 0x1c], 0x4f
        ret    
loc_4_0A18:
        cmp    byte ptr [ebp + 0x14], 0
        jne    loc_4_0A24
        mov    word ptr [ebp + 0x10], 0
loc_4_0A24:
        mov    word ptr [ebp + 0x1c], 0x4f
        ret    
loc_4_0A2B:
        mov    edx, 0x3ce
        cmp    byte ptr [ebp + 0x11], 0
        je     loc_4_0A50
        xor    ah, ah
        mov    al, 0x45
        out    dx, al
        inc    dl
        in     al, dx
        mov    cl, 4
        cmp    byte ptr [ebp], 7
        jne    loc_4_0A48
        mov    cl, 2
loc_4_0A48:
        shr    al, cl
        mov    word ptr [ebp + 0x14], ax
        jmp    loc_4_0A73
loc_4_0A50:
        mov    ah, byte ptr [ebp + 0x14]
        mov    cl, 4
        cmp    byte ptr [ebp], 7
        jne    loc_4_0A5D
        mov    cl, 2
loc_4_0A5D:
        shl    ah, cl
        mov    al, 0x45
        out    dx, ax
        inc    al
        mov    cl, 8
        cmp    byte ptr [ebp], 7
        jne    loc_4_0A6F
        mov    cl, 2
loc_4_0A6F:
        add    ah, cl
        out    dx, ax
loc_4_0A73:
        mov    word ptr [ebp + 0x1c], 0x4f
        ret    
loc_4_0A7A:
        movzx  ecx, byte ptr [ebp + 0x10]
        cmp    cl, 4
        jae    loc_4_0AE5
        mov    edi, ebx
        add    edi, dword ptr [0x244]        ; -> obj1:0x0244
        movzx  edi, word ptr [edi]
        cmp    edi, 0x111
        ja     loc_4_0AE5
        sub    edi, 0x100
        jl     loc_4_0AE5
        shl    edi, 4
        movzx  esi, word ptr [edi + 0x140]   ; -> obj4:0x0140
        movzx  edi, word ptr [edi + 0x142]   ; -> obj4:0x0142
        or     esi, esi
        je     loc_4_0AE5
        mov    eax, dword ptr [0x240]        ; -> obj1:0x0240
        xor    edx, edx
        div    edi
        movzx  edx, word ptr [ebp + 0x18]
        mov    word ptr [ebp + 0x10], di
        mov    word ptr [ebp + 0x18], si
        mov    word ptr [ebp + 0x14], ax
        jmp    dword ptr [ecx*4 + 0xaec]     ; -> obj4:0x0AEC
loc_4_0AD3:
        cmp    edx, esi
        jne    loc_4_0AE5
        jmp    loc_4_0ADD
loc_4_0AD9:
        cmp    edx, edi
        jne    loc_4_0AE5
loc_4_0ADD:
        mov    word ptr [ebp + 0x1c], 0x4f
        jmp    loc_4_0AEB
loc_4_0AE5:
        mov    word ptr [ebp + 0x1c], 0x14f
loc_4_0AEB:
        ret    
        ; ---- dispatch table (4 entries) ----
        dd  loc_4_0AD3
        dd  loc_4_0ADD
        dd  loc_4_0AD9
        dd  loc_4_0ADD
loc_4_0AFC:
        cmp    byte ptr [ebp + 0x10], 1
        jne    loc_4_0B1A
        mov    byte ptr [ebp + 0x11], 0
        mov    word ptr [ebp + 0x18], 0
        mov    word ptr [ebp + 0x14], 0
        mov    word ptr [ebp + 0x1c], 0x4f
        jmp    loc_4_0B20
loc_4_0B1A:
        mov    word ptr [ebp + 0x1c], 0x14f
loc_4_0B20:
        ret    
loc_4_0B21:
        cmp    byte ptr [ebp + 0x10], 1
        jne    loc_4_0B33
        mov    byte ptr [ebp + 0x11], 6
        mov    word ptr [ebp + 0x1c], 0x4f
        jmp    loc_4_0B39
loc_4_0B33:
        mov    word ptr [ebp + 0x1c], 0x14f
loc_4_0B39:
        ret    
loc_4_0B3A:
        mov    cl, byte ptr [ebp + 0x10]
        and    cl, 0x7f
        or     cl, cl
        je     loc_4_0B89
        cmp    cl, 1
        jne    loc_4_0BD1
        mov    ah, 0x38
        mov    al, 0
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        je     loc_4_0BC9
        mov    edi, eax
        movzx  ecx, word ptr [ebp + 0x14]
        movzx  esi, word ptr [ebp + 0x18]
        mov    edx, 0x3c7
loc_4_0B6B:
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
        jne    loc_4_0B6B
        jmp    loc_4_0BC1
loc_4_0B89:
        mov    ah, 0x38
        mov    al, 0
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        je     loc_4_0BC9
        mov    esi, eax
        movzx  ecx, word ptr [ebp + 0x14]
        movzx  edi, word ptr [ebp + 0x18]
        mov    edx, 0x3c8
loc_4_0BA7:
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
        jne    loc_4_0BA7
loc_4_0BC1:
        mov    word ptr [ebp + 0x1c], 0x4f
        jmp    loc_4_0BD7
loc_4_0BC9:
        mov    word ptr [ebp + 0x1c], 0x14f
        jmp    loc_4_0BD7
loc_4_0BD1:
        mov    word ptr [ebp + 0x1c], 0x24f
loc_4_0BD7:
        ret    
loc_4_0BD8:
        mov    word ptr [ebp + 0x1c], 0x14f
        ret    
loc_4_0BDF:
        mov    word ptr [ebp + 0x1c], 0x14f
        ret    
loc_4_0BE6:
        mov    word ptr [ebp + 0x1c], 0x14f
        ret    
loc_4_0BED:
        mov    word ptr [ebp + 0x1c], 0x14f
        ret    
loc_4_0BF4:
        mov    word ptr [ebp + 0x1c], 0x14f
        ret    
loc_4_0BFB:
        mov    word ptr [ebp + 0x1c], 0x14f
        ret    
loc_4_0C02:
        mov    al, byte ptr [ebp + 0x10]
        cmp    al, 0
        je     loc_4_0C1A
        cmp    al, 1
        je     loc_4_0C28
        cmp    al, 2
        je     loc_4_0CEF
        jmp    loc_4_0D04
loc_4_0C1A:
        mov    eax, 0x710
        mov    word ptr [ebp + 0x10], ax
        jmp    loc_4_0D0C
loc_4_0C28:
        movzx  ecx, word ptr [ebp + 0x18]
        cmp    cx, 8
        jg     loc_4_0D04
        call   0
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0x40
        out    dx, al
        inc    edx
        in     al, dx
        mov    edx, 0x3ce
        rol    eax, 8
        out    dx, al
        ror    eax, 8
        test   al, 1
        jne    loc_4_0D04
        mov    al, byte ptr [ebp + 0x11]
        cmp    al, 0
        je     loc_4_0C75
        cmp    al, 1
        je     loc_4_0C96
        cmp    al, 2
        je     loc_4_0CB2
        cmp    al, 4
        je     loc_4_0CCE
        jmp    loc_4_0D04
loc_4_0C75:
        mov    byte ptr [ecx + 0x114], al    ; -> obj4:0x0114
        call   sub_4_0D13
        call   sub_4_0D31
        call   sub_4_0D5A
        call   sub_4_0D22
        call   sub_4_0DB7
        jmp    loc_4_0D0C
loc_4_0C96:
        mov    byte ptr [ecx + 0x114], al    ; -> obj4:0x0114
        call   sub_4_0D13
        call   sub_4_0D43
        call   sub_4_0D22
        call   sub_4_0DC6
        jmp    loc_4_0D0C
loc_4_0CB2:
        mov    byte ptr [ecx + 0x114], al    ; -> obj4:0x0114
        call   sub_4_0D13
        call   sub_4_0D78
        call   sub_4_0D22
        call   sub_4_0DC6
        jmp    loc_4_0D0C
loc_4_0CCE:
        mov    byte ptr [ecx + 0x114], al    ; -> obj4:0x0114
        call   sub_4_0D13
        call   sub_4_0D43
        call   sub_4_0D78
        call   sub_4_0D22
        call   sub_4_0DC6
        jmp    loc_4_0D0C
loc_4_0CEF:
        movzx  ecx, word ptr [ebp + 0x18]
        cmp    cx, 8
        jg     loc_4_0D04
        mov    al, byte ptr [ecx + 0x114]    ; -> obj4:0x0114
        mov    byte ptr [ebp + 0x11], al
        jmp    loc_4_0D0C
loc_4_0D04:
        mov    word ptr [ebp + 0x1c], 0x14f
        jmp    loc_4_0D12
loc_4_0D0C:
        mov    word ptr [ebp + 0x1c], 0x4f
loc_4_0D12:
        ret    
sub_4_0D13:
        mov    edx, 0x3d4
        mov    al, 0x11
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0x7f
        out    dx, al
        ret    
sub_4_0D22:
        mov    edx, 0x3d4
        mov    al, 0x11
        out    dx, al
        inc    dx
        in     al, dx
        or     al, 0x80
        out    dx, al
        ret    
sub_4_0D31:
        mov    edx, 0x3d4
        mov    al, 4
        out    dx, al
        inc    dx
        mov    al, byte ptr [ecx + 0x11c]    ; -> obj4:0x011C
        out    dx, al
        ret    
sub_4_0D43:
        mov    edx, 0x3d4
        mov    al, 4
        out    dx, al
        inc    dx
        in     al, dx
        mov    byte ptr [ecx + 0x11c], al    ; -> obj4:0x011C
        mov    ax, 0xff
        out    dx, al
        ret    
sub_4_0D5A:
        mov    edx, 0x3d4
        mov    al, 0x10
        out    dx, al
        inc    dx
        mov    al, byte ptr [ecx + 0x124]    ; -> obj4:0x0124
        out    dx, al
        dec    edx
        mov    al, 7
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0x7b
        or     al, 4
        out    dx, al
        ret    
sub_4_0D78:
        mov    edx, 0x3d4
        mov    al, 0x10
        out    dx, al
        inc    dx
        in     al, dx
        mov    byte ptr [ecx + 0x124], al    ; -> obj4:0x0124
        mov    ax, 0xff
        out    dx, al
        dec    edx
        mov    al, 7
        out    dx, al
        inc    dx
        in     al, dx
        or     al, 0x84
        out    dx, al
        ret    
        db  0BAh, 0D4h, 003h, 000h, 000h, 0B0h, 017h, 0EEh, 066h, 042h, 0ECh, 00Ch, 080h, 0EEh, 0C3h, 0BAh
        db  0D4h, 003h, 000h, 000h, 0B0h, 017h, 0EEh, 066h, 042h, 0ECh, 024h, 07Fh, 0EEh, 0C3h
sub_4_0DB7:
        mov    edx, 0x3da
        in     al, dx
        mov    edx, 0x3c0
        mov    al, 0x20
        out    dx, al
        ret    
sub_4_0DC6:
        mov    edx, 0x3da
        in     al, dx
        mov    edx, 0x3c0
        mov    al, 0
        out    dx, al
        ret    
VxD__LTEXT ENDS
