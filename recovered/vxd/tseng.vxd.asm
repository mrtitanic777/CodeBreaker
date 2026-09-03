; TSENG -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 514 instructions recovered by recursive descent

Declare_Virtual_Device TSENG, 3, 1, TSENG_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x16c  R-X
TSENG_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_000F
        jmp    dword ptr [eax*4 + 0x11]      ; -> obj1:0x0011
loc_1_000F:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_2_0000
        dd  loc_2_01EA
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
        test   byte ptr [0x161], 3           ; -> obj1:0x0161
        jne    loc_1_00D5
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        jne    loc_1_00B9
        cmp    ebx, dword ptr [0x144]        ; -> obj1:0x0144
        jne    loc_1_00D5
        and    edx, 0xffff
        sub    edx, 0x3cb
        shr    edx, 1
        add    edx, 0x165                    ; -> obj1:0x0165
        cmp    ecx, 0
        je     loc_1_00B5
        mov    byte ptr [edx], al
        jmp    loc_1_00F3
loc_1_00B5:
        mov    al, byte ptr [edx]
        jmp    loc_1_00F3
loc_1_00B9:
        cmp    ecx, 4
        je     loc_1_00DB
        and    edx, 0xffff
        sub    edx, 0x3cb
        add    edx, dword ptr [0x158]        ; -> obj1:0x0158
        mov    al, byte ptr [ebx + edx]
        jmp    loc_1_00F3
loc_1_00D5:
        cmp    ecx, 0
        je     loc_1_00F2
        out    dx, al
loc_1_00DB:
        and    edx, 0xffff
        sub    edx, 0x3cb
        add    edx, dword ptr [0x158]        ; -> obj1:0x0158
        mov    byte ptr [ebx + edx], al
        jmp    loc_1_00F3
loc_1_00F2:
        in     al, dx
loc_1_00F3:
        ret    
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'TSENG   '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  TSENG_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x218  R-X
loc_2_0000:
        mov    dword ptr [0x144], ebx        ; -> obj1:0x0144
        mov    byte ptr [0x161], 0           ; -> obj1:0x0161
        mov    dx, 0x3cb
        in     al, dx
        ror    eax, 8
        mov    al, 0x11
        out    dx, al
        in     al, dx
        and    al, 0x33
        cmp    al, 0x11
        jne    loc_2_0039
        mov    al, 0x22
        out    dx, al
        in     al, dx
        and    al, 0x33
        cmp    al, 0x22
        jne    loc_2_0039
        rol    eax, 8
        out    dx, al
        mov    byte ptr [0x160], 0x20        ; -> obj1:0x0160
        jmp    loc_2_00BF
loc_2_0039:
        rol    eax, 8
        out    dx, al
        mov    dx, 0x3cd
        in     al, dx
        ror    eax, 8
        cmp    al, 0xff
        je     loc_2_005B
        mov    dx, 0x3bf
        mov    al, 3
        out    dx, al
        mov    dx, 0x3d8
        mov    al, 0xa0
        out    dx, al
        mov    dx, 0x3cd
loc_2_005B:
        mov    al, 0x11
        out    dx, al
        in     al, dx
        cmp    al, 0x11
        jne    loc_2_00B6
        mov    al, 0x22
        out    dx, al
        in     al, dx
        cmp    al, 0x22
        jne    loc_2_00B6
        push   ebx
        mov    dx, 0x3d4
        in     al, dx
        mov    bl, al
        mov    al, 0x33
        out    dx, al
        inc    dx
        in     al, dx
        mov    bh, al
        mov    al, 5
        out    dx, al
        dec    dx
        mov    al, 0x33
        out    dx, al
        inc    dx
        in     al, dx
        mov    cl, al
        mov    al, 0xa
        out    dx, al
        dec    dx
        mov    al, 0x33
        out    dx, al
        inc    dx
        in     al, dx
        mov    ch, al
        mov    al, bh
        out    dx, al
        dec    dx
        mov    al, bl
        out    dx, al
        pop    ebx
        cmp    cx, 0xa05
        jne    loc_2_00B6
        mov    dx, 0x3cd
        rol    eax, 8
        out    dx, al
        mov    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        jmp    loc_2_00BF
loc_2_00B6:
        rol    eax, 8
        out    dx, al
        jmp    loc_2_01E8
loc_2_00BF:
        VxDCall  VDD.VDD_Get_Special_VM_IDs      ; cd2011000a00
        mov    dword ptr [0x14c], esi        ; -> obj1:0x014C
        mov    dword ptr [0x150], edi        ; -> obj1:0x0150
        push   0
        push   4
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0x158], eax        ; -> obj1:0x0158
        or     eax, eax
        je     loc_2_01E8
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_01E8
        mov    dword ptr [edi], 0            ; -> loc_3_0000
        mov    dword ptr [edi + 4], 0x85     ; -> loc_3_0085
        mov    dword ptr [edi + 8], 0        ; -> loc_4_0000
        mov    dword ptr [edi + 0xc], 0x54   ; -> loc_4_0054
        mov    dword ptr [edi + 0x58], 0x9a  ; -> loc_4_009A
        mov    dword ptr [edi + 0x5c], 0xd0  ; -> loc_4_00D0
        mov    dword ptr [edi + 0x10], 0xfa  ; -> loc_4_00FA
        mov    dword ptr [edi + 0x14], 0x116 ; -> loc_4_0116
        mov    dword ptr [edi + 0x18], 0x11e ; -> loc_4_011E
        mov    dword ptr [edi + 0x1c], 0x126 ; -> loc_4_0126
        mov    dword ptr [edi + 0x24], 0x142 ; -> loc_4_0142
        mov    dword ptr [edi + 0x84], 0x1a3 ; -> loc_4_01A3
        mov    dword ptr [edi + 0x80], 0x1c9 ; -> loc_4_01C9
        mov    dword ptr [edi + 0x88], 0x1ef ; -> loc_4_01EF
        mov    dword ptr [edi + 0xa8], 0x210 ; -> loc_2_0210
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        je     loc_2_018C
        mov    dword ptr [edi + 0x44], 0x244 ; -> loc_4_0244
        mov    dword ptr [edi + 0x34], 0x175 ; -> loc_4_0175
        mov    dword ptr [edi + 0x38], 0x18c ; -> loc_4_018C
loc_2_018C:
        mov    edx, 0x3cd
        mov    ecx, 0x7d                     ; -> loc_1_007D
        VxDCall  VDD.VDD_Takeover_VGA_Port       ; cd2013000a00
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        je     loc_2_01E5
        mov    edx, 0x3cb
        mov    ecx, 0x7d                     ; -> loc_1_007D
        VxDCall  VDD.VDD_Takeover_VGA_Port       ; cd2013000a00
        mov    esi, 0x261                    ; -> loc_4_0261
        mov    edx, 0x217a
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_01E8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    esi, 0x261                    ; -> loc_4_0261
        mov    edx, 0x217b
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_01E8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
loc_2_01E5:
        clc    
        jmp    loc_2_01E9
loc_2_01E8:
        stc    
loc_2_01E9:
        ret    
loc_2_01EA:
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        jne    loc_2_020E
        mov    edx, 0x3dd
        mov    ecx, 0x222                    ; -> loc_4_0222
        VxDCall  VDD.VDD_Takeover_VGA_Port       ; cd2013000a00
        mov    edx, 0x3dd
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
loc_2_020E:
        clc    
        ret    
loc_2_0210:
        movzx  eax, byte ptr [0x160]         ; -> obj1:0x0160
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x110  R-X
loc_3_0000:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [0x154], eax        ; -> obj1:0x0154
        mov    edx, 0x3cc
        in     al, dx
        mov    byte ptr [0x162], al          ; -> obj1:0x0162
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        jne    loc_3_002E
        movzx  eax, word ptr [ebp + 0x14]
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
        mov    word ptr [ebp + 0x14], 0
        jmp    loc_3_0084
loc_3_002E:
        mov    eax, dword ptr [0x15c]        ; -> obj1:0x015C
        or     eax, eax
        jne    loc_3_0080
        mov    edx, dword ptr [ebp + 0x14]
        push   0
        push   0x8000
        push   edx
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
        mov    dword ptr [0x15c], eax        ; -> obj1:0x015C
        push   eax
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
        pop    eax
loc_3_0080:
        mov    word ptr [ebp + 0x14], ax
loc_3_0084:
        ret    
loc_3_0085:
        mov    eax, ecx
        shr    eax, 0x10
        or     cx, cx
        je     loc_3_00A9
        mov    edx, dword ptr [0x154]        ; -> obj1:0x0154
        shr    edx, 0x10
        dec    dl
        cmp    al, dl
        jae    loc_3_010D
        inc    al
        xor    cx, cx
        add    ecx, 0x10000
loc_3_00A9:
        mov    edx, dword ptr [0x154]        ; -> obj1:0x0154
        sub    edx, ecx
        cmp    edx, 0x10000
        mov    edx, 0x10000
        ja     loc_3_00C3
        mov    edx, 0x8000
loc_3_00C3:
        push   edx
        push   ebx
        mov    ah, al
        shr    al, 2
        and    ah, 3
        shl    ah, 2
        mov    byte ptr [0x169], ah          ; -> obj1:0x0169
        mov    bl, al
        mov    bh, al
        and    bl, 0xf
        mov    al, bl
        shl    bl, 4
        or     al, bl
        mov    byte ptr [0x164], al          ; -> obj1:0x0164
        mov    byte ptr [0x163], 0           ; -> obj1:0x0163
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        je     loc_3_0109
        and    bh, 0x30
        mov    bl, bh
        shr    bl, 4
        or     bl, bh
        mov    byte ptr [0x163], bl          ; -> obj1:0x0163
loc_3_0109:
        pop    ebx
        pop    edx
        jmp    loc_3_010F
loc_3_010D:
        mov    edx, ecx
loc_3_010F:
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x2a1  R-X
loc_4_0000:
        cmp    dword ptr [0x154], 0          ; -> obj1:0x0154
        je     loc_4_0053
        push   eax
        push   edx
        mov    edx, 0x3cd
        in     al, dx
        mov    ah, al
        xor    al, al
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        je     loc_4_0021
        mov    dl, 0xcb
        in     al, dx
loc_4_0021:
        cmp    ax, word ptr [0x163]          ; -> obj1:0x0163
        je     loc_4_0049
        mov    word ptr [0x165], ax          ; -> obj1:0x0165
        mov    dl, 0xcd
        mov    al, byte ptr [0x164]          ; -> obj1:0x0164
        out    dx, al
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        je     loc_4_0049
        mov    dl, 0xcb
        mov    al, byte ptr [0x163]          ; -> obj1:0x0163
        out    dx, al
loc_4_0049:
        pop    edx
        pop    eax
        and    al, 0xf
        add    al, byte ptr [0x169]          ; -> obj1:0x0169
loc_4_0053:
        ret    
loc_4_0054:
        push   eax
        push   edx
        cmp    dword ptr [0x154], 0          ; -> obj1:0x0154
        je     loc_4_0097
        mov    edx, 0x3cd
        in     al, dx
        mov    ah, al
        xor    al, al
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        je     loc_4_0075
        mov    dl, 0xcb
        in     al, dx
loc_4_0075:
        cmp    ax, word ptr [0x163]          ; -> obj1:0x0163
        jne    loc_4_0097
        mov    dl, 0xcd
        mov    al, byte ptr [0x166]          ; -> obj1:0x0166
        out    dx, al
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        jbe    loc_4_0097
        mov    dl, 0xcb
        mov    al, byte ptr [0x165]          ; -> obj1:0x0165
        out    dx, al
loc_4_0097:
        pop    edx
        pop    eax
        ret    
loc_4_009A:
        push   eax
        push   edx
        cmp    dword ptr [0x154], 0          ; -> obj1:0x0154
        je     loc_4_00CD
        mov    edx, 0x3cd
        in     al, dx
        mov    byte ptr [0x167], al          ; -> obj1:0x0167
        mov    al, byte ptr [0x164]          ; -> obj1:0x0164
        out    dx, al
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        jbe    loc_4_00CD
        mov    dl, 0xcb
        in     al, dx
        mov    byte ptr [0x168], al          ; -> obj1:0x0168
        mov    al, byte ptr [0x163]          ; -> obj1:0x0163
        out    dx, al
loc_4_00CD:
        pop    edx
        pop    eax
        ret    
loc_4_00D0:
        push   eax
        push   edx
        cmp    dword ptr [0x154], 0          ; -> obj1:0x0154
        je     loc_4_00F7
        mov    edx, 0x3cd
        mov    al, byte ptr [0x167]          ; -> obj1:0x0167
        out    dx, al
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        jbe    loc_4_00F7
        mov    dl, 0xcb
        mov    al, byte ptr [0x168]          ; -> obj1:0x0168
        out    dx, al
loc_4_00F7:
        pop    edx
        pop    eax
        ret    
loc_4_00FA:
        or     byte ptr [0x161], 2           ; -> obj1:0x0161
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        jne    loc_4_0115
        mov    edx, 0x3dd
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
loc_4_0115:
        ret    
loc_4_0116:
        mov    byte ptr [0x161], 0           ; -> obj1:0x0161
        ret    
loc_4_011E:
        or     byte ptr [0x161], 1           ; -> obj1:0x0161
        ret    
loc_4_0126:
        mov    byte ptr [0x161], 0           ; -> obj1:0x0161
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        jne    loc_4_0141
        mov    edx, 0x3dd
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
loc_4_0141:
        ret    
loc_4_0142:
        push   edx
        cmp    ecx, dword ptr [0x144]        ; -> obj1:0x0144
        jne    loc_4_015A
        mov    al, byte ptr [0x162]          ; -> obj1:0x0162
        or     al, al
        je     loc_4_015A
        mov    edx, 0x3c2
        out    dx, al
loc_4_015A:
        cmp    esi, dword ptr [0x150]        ; -> obj1:0x0150
        je     loc_4_0173
        cmp    esi, dword ptr [0x14c]        ; -> obj1:0x014C
        je     loc_4_0173
        test   byte ptr [0x161], 3           ; -> obj1:0x0161
        jne    loc_4_0173
loc_4_0173:
        pop    edx
        ret    
loc_4_0175:
        mov    edx, 0x217a
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x217b
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        ret    
loc_4_018C:
        mov    edx, 0x217a
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x217b
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        ret    
loc_4_01A3:
        push   eax
        mov    edx, 0x3cd
        in     al, dx
        mov    ah, al
        xor    al, al
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        je     loc_4_01BA
        mov    dl, 0xcb
        in     al, dx
loc_4_01BA:
        and    ax, 0xf030
        shr    ah, 4
        or     al, ah
        movzx  edx, al
        pop    eax
        stc    
        ret    
loc_4_01C9:
        push   eax
        mov    edx, 0x3cd
        in     al, dx
        mov    ah, al
        xor    al, al
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        je     loc_4_01E0
        mov    dl, 0xcb
        in     al, dx
loc_4_01E0:
        and    ax, 0xf03
        shl    al, 4
        or     al, ah
        movzx  edx, al
        pop    eax
        stc    
        ret    
loc_4_01EF:
        push   eax
        push   edx
        mov    dh, al
        mov    eax, edx
        and    ax, 0xf0f
        shl    ah, 4
        or     al, ah
        and    dx, 0xf0f0
        shr    dl, 4
        or     dl, dh
        mov    ah, dl
        mov    edx, 0x3cd
        out    dx, al
        cmp    byte ptr [0x160], 0x10        ; -> obj1:0x0160
        je     loc_4_021E
        mov    dl, 0xcb
        mov    al, ah
        out    dx, al
loc_4_021E:
        pop    edx
        pop    eax
        stc    
        ret    
loc_4_0222:
        push   edi
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    edi, dword ptr [0x144]        ; -> obj1:0x0144
        jne    loc_4_0236
        cmp    ebx, edi
        jne    loc_4_0241
loc_4_0236:
        cmp    ecx, 4
        je     loc_4_023E
        in     al, dx
        jmp    loc_4_0241
loc_4_023E:
        out    dx, al
        jmp    loc_4_0241
loc_4_0241:
        pop    esi
        pop    edi
        ret    
loc_4_0244:
        cmp    ecx, 0x31
        jb     loc_4_025F
        test   byte ptr [0x161], 3           ; -> obj1:0x0161
        jne    loc_4_025F
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        je     loc_4_025F
        stc    
        jmp    loc_4_0260
loc_4_025F:
        clc    
loc_4_0260:
        ret    
loc_4_0261:
        push   edi
        push   esi
        test   byte ptr [0x161], 3           ; -> obj1:0x0161
        jne    loc_4_027E
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    edi, dword ptr [0x144]        ; -> obj1:0x0144
        jne    loc_4_027E
        cmp    ebx, edi
        jne    loc_4_0292
loc_4_027E:
        cmp    ecx, 4
        je     loc_4_028F
        cmp    ecx, 0xc
        je     loc_4_028B
loc_4_0288:
        in     al, dx
        jmp    loc_4_029E
loc_4_028B:
        out    dx, ax
        jmp    loc_4_029E
loc_4_028F:
        out    dx, al
        jmp    loc_4_029E
loc_4_0292:
        cmp    ecx, 4
        je     loc_4_029E
        cmp    ecx, 0xc
        je     loc_4_029E
        jmp    loc_4_0288
loc_4_029E:
        pop    esi
        pop    edi
        ret    
VxD__LTEXT ENDS
