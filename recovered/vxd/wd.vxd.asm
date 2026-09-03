; WD -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 356 instructions recovered by recursive descent

Declare_Virtual_Device WD, 3, 1, WD_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x160  R-X
WD_Control:
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
        dd  loc_3_0000
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
        mov    edi, ebx
        add    edi, dword ptr [0x150]        ; -> obj1:0x0150
        test   byte ptr [edi + 1], 2
        je     loc_1_00A5
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4806
        out    dx, ax
        mov    al, 0x27
        mov    ah, byte ptr [edi + 3]
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_1_00A5:
        test   byte ptr [edi + 1], 1
        je     loc_1_00FB
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    byte ptr [edi + 2], 0xa6
        mov    al, 0x32
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        inc    al
        out    dx, al
        jmp    loc_1_00C5
loc_1_00C5:
        in     al, dx
        dec    al
        out    dx, al
        dec    dl
        cmp    al, ah
        je     loc_1_00D9
        mov    byte ptr [edi + 2], 0
        mov    ax, 0xa034
        out    dx, ax
loc_1_00D9:
        mov    al, 0x32
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, byte ptr [edi]
        and    ax, 0xcf3
        or     al, ah
        out    dx, al
        dec    dl
        cmp    byte ptr [edi + 2], 0
        jne    loc_1_00F7
        mov    al, 0x34
        mov    ah, byte ptr [edi + 2]
        out    dx, ax
loc_1_00F7:
        rol    eax, 8
        out    dx, al
loc_1_00FB:
        ret    
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'WD      '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  WD_Control               ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h
loc_1_0159:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x1a0  R-X
loc_2_0000:
        push   ebx
        mov    dword ptr [0x14c], ebx        ; -> obj1:0x014C
        mov    cl, 5
        push   ecx
        call   sub_2_0103
        pop    ecx
        cmp    byte ptr [0x15a], 0           ; -> obj1:0x015A
        jne    loc_2_0022
        dec    cl
        jne    9
        jmp    loc_2_0100
loc_2_0022:
        push   0
        push   4
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0x150], eax        ; -> obj1:0x0150
        or     eax, eax
        je     loc_2_0100
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_0100
        mov    dword ptr [edi], 0            ; -> loc_4_0000
        mov    dword ptr [edi + 0x10], 0x17  ; -> loc_5_0017
        mov    dword ptr [edi + 0x18], 0x21  ; -> loc_5_0021
        mov    dword ptr [edi + 0x40], 0x60  ; -> loc_5_0060
        mov    dword ptr [edi + 0x48], 0xbc  ; -> loc_5_00BC
        mov    dword ptr [edi + 0x34], 0xe9  ; -> loc_5_00E9
        mov    dword ptr [edi + 0x38], 0x12c ; -> loc_5_012C
        mov    dword ptr [edi + 0xa8], 0x19a ; -> loc_2_019A
        mov    dword ptr [edi + 0xb0], 0x169 ; -> loc_5_0169
        mov    dword ptr [edi + 0x24], 0     ; -> loc_5_0000
        mov    dword ptr [edi + 0x14], 0x7d  ; -> loc_1_007D
        mov    dword ptr [edi + 0x44], 0x7d  ; -> loc_5_007D
        mov    dword ptr [edi + 0x4c], 0xa6  ; -> loc_5_00A6
        mov    edx, 0x23c0
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x23c2
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x23c4
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    ax, 0x7f
        mov    bx, 0x22ff
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        cmp    bl, 1
        ja     loc_2_00FD
        mov    byte ptr [0x159], bl          ; -> loc_1_0159
        mov    ax, 0x7f
        mov    bh, 0x23
        mov    bl, 1
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_2_00FD:
        clc    
        jmp    loc_2_0101
loc_2_0100:
        stc    
loc_2_0101:
        pop    ebx
        ret    
sub_2_0103:
        mov    byte ptr [0x15a], 0           ; -> obj1:0x015A
        mov    edx, 0x3ce
        in     al, dx
        ror    eax, 8
        mov    al, 0xc
        out    dx, al
        inc    edx
        in     al, dx
        mov    cl, al
        and    al, 0xbf
        out    dx, al
        dec    edx
        mov    al, 0xf
        out    dx, al
        inc    edx
        in     al, dx
        mov    ch, al
        xor    al, al
        out    dx, al
        dec    edx
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        ror    ecx, 0x10
        mov    ch, al
        inc    al
        out    dx, al
        mov    ah, al
        in     al, dx
        cmp    al, ah
        je     loc_2_017E
        dec    edx
        mov    al, 0xf
        out    dx, al
        inc    edx
        mov    al, 5
        out    dx, al
        dec    edx
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        inc    al
        out    dx, al
        in     al, dx
        dec    al
        cmp    al, ch
        jne    loc_2_017E
        mov    byte ptr [0x15a], 0xff        ; -> obj1:0x015A
        mov    dl, 0xcc
        in     al, dx
        mov    dl, 0xd4
        test   al, 1
        jne    loc_2_0167
        mov    dl, 0xb4
loc_2_0167:
        in     al, dx
        ror    eax, 8
        mov    al, 0x37
        out    dx, al
        inc    edx
        in     al, dx
        or     al, al
        je     loc_2_0179
        mov    byte ptr [0x15a], al          ; -> obj1:0x015A
loc_2_0179:
        dec    edx
        rol    eax, 8
        out    dx, al
loc_2_017E:
        mov    dl, 0xce
        mov    al, 9
        mov    ah, ch
        out    dx, ax
        ror    ecx, 0x10
        mov    al, 0xf
        mov    ah, ch
        out    dx, ax
        mov    al, 0xc
        mov    ah, cl
        out    dx, ax
        rol    eax, 8
        out    dx, al
        ret    
loc_2_019A:
        mov    eax, 0x4457
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x7  R-X
loc_3_0000:
        call   sub_5_003F
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x1a  R-X
loc_4_0000:
        mov    byte ptr [0x158], 0xff        ; -> obj1:0x0158
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [0x154], eax        ; -> obj1:0x0154
        movzx  eax, word ptr [ebp + 0x14]
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x184  R-X
loc_5_0000:
        mov    edx, 0x3c4
        in     al, dx
        mov    ah, al
        mov    al, 1
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0xdf
        out    dx, al
        dec    dl
        mov    al, ah
        out    dx, al
        ret    
loc_5_0017:
        cmp    byte ptr [0x158], 0           ; -> obj1:0x0158
        je     loc_5_0020
loc_5_0020:
        ret    
loc_5_0021:
        push   ebx
        cmp    byte ptr [0x159], 0           ; -> loc_1_0159
        jne    loc_5_003D
        mov    eax, 0x7f
        mov    ebx, 0x2301
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_5_003D:
        pop    ebx
        ret    
sub_5_003F:
        push   ebx
        cmp    byte ptr [0x159], 1           ; -> loc_1_0159
        ja     loc_5_005E
        mov    eax, 0x7f
        mov    bh, 0x23
        mov    bl, byte ptr [0x159]          ; -> loc_1_0159
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
loc_5_005E:
        pop    ebx
        ret    
loc_5_0060:
        pushfd 
        cmp    cl, 0x2a
        jne    loc_5_007B
        popfd  
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
        mov    al, ah
        stc    
        jmp    loc_5_007C
loc_5_007B:
        popfd  
loc_5_007C:
        ret    
loc_5_007D:
        mov    edi, ebx
        add    edi, dword ptr [0x150]        ; -> obj1:0x0150
        cmp    cl, 0x32
        jne    loc_5_009B
        mov    ah, byte ptr [edi + 2]
        and    ah, 0xa0
        cmp    ah, 0xa0
        jne    loc_5_00A4
        or     byte ptr [edi + 1], 1
        jmp    loc_5_00A0
loc_5_009B:
        cmp    cl, 0x34
        jne    loc_5_00A4
loc_5_00A0:
        mov    byte ptr [ecx + edi - 0x32], al
loc_5_00A4:
        clc    
        ret    
loc_5_00A6:
        mov    edi, ebx
        add    edi, dword ptr [0x150]        ; -> obj1:0x0150
        cmp    cl, 0x27
        jne    loc_5_00BA
        or     byte ptr [edi + 1], 2
        mov    byte ptr [edi + 3], al
loc_5_00BA:
        clc    
        ret    
loc_5_00BC:
        pushfd 
        cmp    cl, 7
        je     loc_5_00CC
        cmp    cl, 8
        je     loc_5_00CC
        cmp    cl, 9
        jne    loc_5_00E7
loc_5_00CC:
        popfd  
        dec    edx
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4806
        out    dx, ax
        mov    al, cl
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        mov    al, ah
        stc    
        jmp    loc_5_00E8
loc_5_00E7:
        popfd  
loc_5_00E8:
        ret    
loc_5_00E9:
        mov    edx, 0x23c0
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x23c1
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x23c2
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x23c3
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x23c4
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x23c5
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        ret    
loc_5_012C:
        mov    edx, 0x23c0
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x23c1
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x23c2
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x23c3
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x23c4
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x23c5
        ret    
loc_5_0169:
        push   esi
        push   edi
        cmp    ebx, dword ptr [0x14c]        ; -> obj1:0x014C
        je     loc_5_0180
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    edi, ebx
        je     loc_5_0180
        stc    
        jmp    loc_5_0181
loc_5_0180:
        clc    
loc_5_0181:
        pop    edi
        pop    esi
        ret    
VxD__LTEXT ENDS
