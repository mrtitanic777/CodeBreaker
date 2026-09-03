; COMBUFF -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 539 instructions recovered by recursive descent

Declare_Virtual_Device COMBUFF, 1, 0, COMBUFF_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 02FFFFFFFh   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x264  R-X
COMBUFF_Control:
        cmp    eax, 1
        je     loc_2_0000
        cmp    eax, 0
        je     loc_2_0000
        clc    
        ret    
loc_1_0014:
        mov    edx, dword ptr [esi + 0x18]
        add    dx, 2
        in     al, dx
        test   al, 1
        je     loc_1_002E
        stc    
        ret    
loc_1_0022:
        mov    edx, dword ptr [esi + 0x18]
        add    dx, 2
        in     al, dx
        test   al, 1
        jne    loc_1_0062
loc_1_002E:
        and    al, 7
        movzx  eax, al
        jmp    loc_1_0254
loc_1_003A:
        add    dx, 4
        in     al, dx
        and    al, 0xf
        mov    byte ptr [esi + 0x7a], al
        or     byte ptr [esi + 0x7b], 8
        jmp    loc_1_0022
loc_1_004A:
        or     byte ptr [esi + 0x7b], 2
        jmp    loc_1_0022
loc_1_0050:
        add    dx, 3
loc_1_0054:
        in     al, dx
        and    al, 0x1f
        je     loc_1_0022
        push   edx
        call   sub_1_00BB
        pop    edx
        jmp    loc_1_0054
loc_1_0062:
        mov    eax, dword ptr [esi + 0x20]
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        call   sub_1_0134
        call   sub_1_007E
        clc    
        ret    
        db  090h
loc_1_0078:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003800300
sub_1_007E:
        push   eax
        push   ebx
        push   ecx
        mov    ebx, dword ptr [esi + 0x1c]
        or     ebx, ebx
        je     loc_1_00B7
        mov    cl, byte ptr [esi + 0x7b]
        and    cl, byte ptr [esi + 0x74]
        je     loc_1_00B7
        test   byte ptr [esi + 0x75], 0x80
        jne    loc_1_00B7
        test   byte ptr [esi + 0x76], 8
        je     loc_1_00B7
        mov    eax, dword ptr [esi + 0x20]
        or     eax, eax
        je     loc_1_00B7
        VxDCall  VPICD.VPICD_Get_Status          ; cd2006000300
        test   ecx, 1
        jne    loc_1_00B7
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_1_00B7:
        pop    ecx
        pop    ebx
        pop    eax
        ret    
sub_1_00BB:
        mov    ah, al
        test   ah, 1
        je     loc_1_00C6
        add    edx, -5
        in     al, dx
loc_1_00C6:
        mov    bl, byte ptr [esi + 0x78]
        test   bl, 1
        jne    loc_1_00F0
        cmp    byte ptr [esi + 0x7c], 0xff
        je     loc_1_00F0
        or     bl, ah
        mov    byte ptr [esi + 0x78], bl
        test   ah, 1
        je     loc_1_00EA
        mov    byte ptr [esi + 0x79], al
        or     byte ptr [esi + 0x7b], 1
        test   bl, 0x1e
        jne    loc_1_00EB
loc_1_00EA:
        ret    
loc_1_00EB:
        or     byte ptr [esi + 0x7b], 4
        ret    
loc_1_00F0:
        mov    bx, word ptr [esi + 0x85]
        cmp    word ptr [esi + 0x83], bx
        je     loc_1_0124
        movzx  ebx, word ptr [esi + 0x7f]
        mov    word ptr [esi + ebx*2 + 0x87], ax
        inc    word ptr [esi + 0x83]
        inc    ebx
        cmp    bx, word ptr [esi + 0x85]
        jb     loc_1_011F
        xor    ebx, ebx
loc_1_011F:
        mov    word ptr [esi + 0x7f], bx
        ret    
loc_1_0124:
        movzx  ebx, word ptr [esi + 0x7f]
        or     ah, 2
        mov    word ptr [esi + ebx*2 + 0x87], ax
        ret    
sub_1_0134:
        cmp    word ptr [esi + 0x83], 0
        je     loc_1_01A3
        cmp    byte ptr [esi + 0x7c], 0xff
        je     loc_1_01A3
loc_1_0144:
        test   byte ptr [esi + 0x78], 1
        jne    loc_1_01A3
        dec    word ptr [esi + 0x83]
        movzx  ebx, word ptr [esi + 0x81]
        mov    dx, word ptr [esi + ebx*2 + 0x87]
        inc    ebx
        cmp    bx, word ptr [esi + 0x85]
        jb     loc_1_016C
        xor    ebx, ebx
loc_1_016C:
        mov    word ptr [esi + 0x81], bx
        or     dh, byte ptr [esi + 0x78]
        mov    byte ptr [esi + 0x78], dh
        test   dh, 1
        je     loc_1_0190
        mov    byte ptr [esi + 0x79], dl
        or     byte ptr [esi + 0x7b], 1
        test   dh, 0x1e
        je     loc_1_01A3
        or     byte ptr [esi + 0x7b], 4
        jmp    loc_1_01A3
loc_1_0190:
        cmp    word ptr [esi + 0x83], 0
        jne    loc_1_0144
        test   dh, 0x1e
        je     loc_1_01A3
        or     byte ptr [esi + 0x7b], 4
loc_1_01A3:
        ret    
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'COMBUFF '                  ; DDB_Name
        dd  02FFFFFFFh                  ; DDB_Init_Order
        dd  COMBUFF_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        dd  loc_3_0000               ; fixup
        dd  loc_1_0014               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_1_0078               ; fixup
        dd  loc_3_0205               ; fixup
        dd  sub_1_007E               ; fixup
        dd  loc_3_00CC               ; fixup
        dd  loc_3_00E8               ; fixup
        dd  loc_3_0124               ; fixup
        dd  loc_3_0130               ; fixup
        dd  loc_3_0145               ; fixup
        dd  loc_3_016D               ; fixup
        dd  loc_3_0186               ; fixup
        dd  loc_3_018C               ; fixup
        dd  loc_3_0193               ; fixup
        dd  loc_3_0199               ; fixup
        dd  loc_3_01B0               ; fixup
        dd  loc_3_01D2               ; fixup
        dd  loc_3_01EE               ; fixup
        dd  loc_3_01FD               ; fixup
loc_1_0244:
        add    dword ptr [edx + eax], eax
        add    al, 6
        push   es
        push   es
        push   es
        add    byte ptr [edx + eax], al
        add    al, 6
        push   es
        push   es
        push   es
loc_1_0254:
        cmp    al, byte ptr [eax]
        add    byte ptr [eax], al
        dec    edx
        add    byte ptr [eax], al
        add    byte ptr [eax], dl            ; -> loc_1_0050
        add    byte ptr [eax], al
        push   eax
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x15c  R-X
loc_2_0000:
        mov    ecx, eax
        VxDCall  VCD.VCD_Get_Version             ; cd2000000e00
        cmp    eax, 0x300
        jb     loc_2_0100
        cmp    eax, 0x400
        jae    loc_2_0024
        cmp    ecx, 0
        je     loc_2_002D
        jmp    loc_2_00FF
loc_2_0024:
        cmp    ecx, 1
        jne    loc_2_00FF
loc_2_002D:
        xor    ecx, ecx
loc_2_002F:
        inc    ecx
        push   ecx
        lea    eax, [ecx + 0x30]
        mov    byte ptr [0x137], al          ; -> loc_2_0137
        mov    byte ptr [0x142], al          ; -> loc_2_0142
        mov    byte ptr [0x154], al          ; -> loc_2_0154
        mov    eax, 0x100
        xor    esi, esi
        mov    edi, 0x134                    ; -> loc_2_0134
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        test   eax, eax
        je     loc_2_00D8
        cmp    eax, 0x2710
        jb     loc_2_0065
        mov    eax, 0x2710
loc_2_0065:
        push   eax
        push   ecx
        xchg   ecx, eax
        mov    ebx, 1
        add    ecx, ecx
        add    ecx, 0x13
        xor    edx, edx
        mov    esi, 0x1f4                    ; -> obj1:0x01F4
        VxDCall  VCD.VCD_Virtualize_Port         ; cd2003000e00
        mov    ebx, eax
        pop    ecx
        pop    eax
        jb     loc_2_00D8
        mov    word ptr [ebx + 0x85], ax
        xor    edx, edx
        xor    esi, esi
        mov    edi, 0x13f                    ; -> loc_2_013F
        VMMCall  VMM.Get_Profile_String          ; cd20b3000100
        mov    byte ptr [ebx + 0x7d], 0
        test   edx, edx
        je     loc_2_00B5
        mov    esi, 0x14c                    ; -> obj2:0x014C
        mov    edi, edx
        call   sub_2_0101
        jb     loc_2_00B5
        mov    byte ptr [ebx + 0x7d], 1
loc_2_00B5:
        mov    eax, 2
        xor    esi, esi
        mov    edi, 0x151                    ; -> obj2:0x0151
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        cmp    eax, 1
        jb     loc_2_00D4
        ja     loc_2_00D8
        mov    byte ptr [ebx + 0x7e], 0xff
        jmp    loc_2_00D8
loc_2_00D4:
        mov    byte ptr [ebx + 0x7e], 1
loc_2_00D8:
        pop    ecx
        cmp    ecx, 4
        jb     loc_2_002F
        VxDCall  VCD.VCD_Get_Version             ; cd2000000e00
        cmp    eax, 0x190
        jb     loc_2_00FF
        mov    eax, 0xe000b
        mov    esi, 0x98                     ; -> loc_3_0098
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
loc_2_00FF:
        clc    
loc_2_0100:
        ret    
sub_2_0101:
        pushal 
        xor    ecx, ecx
        cld    
loc_2_0105:
        lodsb  al, byte ptr [esi]
        test   al, al
        je     loc_2_0123
        call   sub_2_0126
        mov    bl, al
        mov    al, byte ptr [edi]
        inc    edi
        call   sub_2_0126
        cmp    al, bl
        jne    loc_2_0120
        inc    ecx
        jmp    loc_2_0105
loc_2_0120:
        stc    
        jmp    loc_2_0124
loc_2_0123:
        clc    
loc_2_0124:
        popal  
        ret    
sub_2_0126:
        cmp    al, 0x61
        jb     loc_2_0130
        cmp    al, 0x7a
        ja     loc_2_0130
        sub    al, 0x20
loc_2_0130:
        ret    
        db  000h, 000h, 000h
loc_2_0134:
        inc    ebx
        dec    edi
        dec    ebp
loc_2_0137:
        add    byte ptr [edx + 0x55], al
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        db  000h
loc_2_013F:
        inc    ebx
        dec    edi
        dec    ebp
loc_2_0142:
        add    byte ptr [eax + 0x52], dl
        dec    edi
        push   esp
        dec    edi
        inc    ebx
        dec    edi
        dec    esp
        add    byte ptr [eax + 0x4f], bl
        inc    esi
        inc    esi
        add    byte ptr [ebx + 0x4f], al
        dec    ebp
loc_2_0154:
        add    byte ptr [esi + 0x49], al
        inc    esi
        dec    edi
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x27b  R-X
loc_3_0000:
        test   eax, eax
        jne    loc_3_008E
        push   edx
        mov    edx, dword ptr [esi + 0x18]
        test   edx, edx
        je     loc_3_003A
        cmp    byte ptr [esi + 0x7e], al
        jne    loc_3_003A
        mov    byte ptr [esi + 0x7e], 0xff
        add    dl, 2
        in     al, dx
        mov    ah, al
        mov    al, 1
        out    dx, al
        jmp    loc_3_0024
loc_3_0024:
        in     al, dx
        and    al, 0xc0
        cmp    al, 0xc0
        jne    loc_3_0036
        and    ah, al
        cmp    ah, al
        je     loc_3_003A
        xor    al, al
        out    dx, al
        jmp    loc_3_003A
loc_3_0036:
        mov    byte ptr [esi + 0x7e], 1
loc_3_003A:
        pop    edx
        xchg   edx, ebx
        or     ebx, ebx
        je     loc_3_005C
        mov    eax, dword ptr [esi + 0x20]
        or     eax, eax
        je     loc_3_005C
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 0x80
        je     loc_3_005C
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
loc_3_005C:
        mov    ebx, edx
        test   ebx, ebx
        je     loc_3_008E
        call   sub_3_0256
        mov    eax, dword ptr [esi + 0x20]
        or     eax, eax
        je     loc_3_007C
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 0x10
        je     loc_3_0081
loc_3_007C:
        call   sub_3_0216
loc_3_0081:
        lea    edi, [esi + 0x74]
        mov    esi, dword ptr [esi]
        lea    esi, [ebx + esi + 2]
        cld    
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_3_008E:
        clc    
        ret    
        db  0EBh, 006h, 0FFh, 025h
        dd  obj4:0x0000              ; fixup
loc_3_0098:
        push   eax
        or     eax, ecx
        pop    eax
        jne    loc_3_00C3
        mov    ebx, 1
        mov    edi, 0x1f4                    ; -> obj1:0x01F4
        add    eax, 0x413
        call   dword ptr [0]                 ; -> obj4:0x0000
        jb     loc_3_00C2
        mov    word ptr [esi + 0x85], 0x200
        mov    byte ptr [esi + 0x7d], 0
loc_3_00C2:
        ret    
loc_3_00C3:
        jmp    dword ptr [0]                 ; -> obj4:0x0000
        db  02Eh, 08Bh, 0C0h
loc_3_00CC:
        cli    
        test   byte ptr [esi + 0x75], 0x80
        jne    loc_3_00E5
        mov    al, byte ptr [esi + 0x79]
        and    byte ptr [esi + 0x78], 0xfe
        and    byte ptr [esi + 0x7b], 0xfe
        call   sub_1_0134
        sti    
        ret    
loc_3_00E5:
        in     al, dx
        sti    
        ret    
loc_3_00E8:
        cli    
        test   byte ptr [esi + 0x75], 0x80
        jne    loc_3_00FD
        and    byte ptr [esi + 0x7b], 0xfd
        cmp    al, 0x13
        je     loc_3_0115
        cmp    byte ptr [esi + 0x7c], 0xff
        je     loc_3_0100
loc_3_00FD:
        out    dx, al
        sti    
        ret    
loc_3_0100:
        mov    byte ptr [esi + 0x7c], 0
        push   eax
        push   edx
        call   sub_1_0134
        call   sub_1_007E
        pop    edx
        pop    eax
        out    dx, al
        sti    
        ret    
loc_3_0115:
        cmp    byte ptr [esi + 0x7d], 1
        jne    loc_3_011F
        mov    byte ptr [esi + 0x7c], 0xff
loc_3_011F:
        out    dx, al
        sti    
        ret    
        db  08Bh, 0C0h
loc_3_0124:
        cli    
        test   byte ptr [esi + 0x75], 0x80
        jne    loc_3_00E5
        mov    al, byte ptr [esi + 0x74]
        sti    
        ret    
loc_3_0130:
        cli    
        test   byte ptr [esi + 0x75], 0x80
        jne    loc_3_00FD
        xchg   byte ptr [esi + 0x74], al
        out    dx, al
        jmp    loc_3_013D
loc_3_013D:
        mov    al, byte ptr [esi + 0x74]
        or     al, 1
        out    dx, al
        sti    
        ret    
loc_3_0145:
        cli    
        movzx  eax, byte ptr [esi + 0x7b]
        and    al, byte ptr [esi + 0x74]
        and    al, 0xf
        mov    al, byte ptr [eax + 0x244]    ; -> loc_1_0244
        test   byte ptr [esi + 0x77], 1
        je     loc_3_015D
        mov    ah, 0xc0
loc_3_015D:
        cmp    al, 2
        je     loc_3_0165
        or     al, ah
        sti    
        ret    
loc_3_0165:
        or     al, ah
        and    byte ptr [esi + 0x7b], 0xfd
        sti    
        ret    
loc_3_016D:
        cli    
        cmp    byte ptr [esi + 0x7e], 0xff
        jne    loc_3_0184
        out    dx, al
        test   al, 1
        je     loc_3_0180
        and    al, 0xc1
        mov    byte ptr [esi + 0x77], al
        sti    
        ret    
loc_3_0180:
        mov    byte ptr [esi + 0x77], 0
loc_3_0184:
        sti    
        ret    
loc_3_0186:
        cli    
        mov    al, byte ptr [esi + 0x75]
        sti    
        ret    
loc_3_018C:
        cli    
        mov    byte ptr [esi + 0x75], al
        sti    
        out    dx, al
        ret    
loc_3_0193:
        cli    
        mov    al, byte ptr [esi + 0x76]
        sti    
        ret    
loc_3_0199:
        cli    
        xchg   byte ptr [esi + 0x76], al
        out    dx, al
        jmp    loc_3_01A0
loc_3_01A0:
        mov    al, byte ptr [esi + 0x76]
        or     al, 8
        out    dx, al
        test   al, 1
        je     sub_3_0256
        sti    
        ret    
loc_3_01B0:
        cli    
        in     al, dx
        test   al, 0x1f
        je     loc_3_01C1
        push   eax
        and    al, 0x1f
        call   sub_1_00BB
        pop    eax
        and    al, 0x60
loc_3_01C1:
        mov    bl, byte ptr [esi + 0x78]
        or     al, bl
        and    bl, 1
        mov    byte ptr [esi + 0x78], bl
        and    byte ptr [esi + 0x7b], 0xfb
        sti    
        ret    
loc_3_01D2:
        cli    
        and    al, 0x1f
        mov    byte ptr [esi + 0x78], al
        and    byte ptr [esi + 0x7b], 0xfa
        test   al, 1
        je     loc_3_01E4
        or     byte ptr [esi + 0x7b], 1
loc_3_01E4:
        test   al, 0x1e
        je     loc_3_01EC
        or     byte ptr [esi + 0x7b], 4
loc_3_01EC:
        sti    
        ret    
loc_3_01EE:
        cli    
        in     al, dx
        or     al, byte ptr [esi + 0x7a]
        mov    byte ptr [esi + 0x7a], 0
        and    byte ptr [esi + 0x7b], 0xf7
        sti    
        ret    
loc_3_01FD:
        cli    
        out    dx, al
        mov    byte ptr [esi + 0x7a], 0
        sti    
        ret    
loc_3_0205:
        jecxz  loc_3_022A
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 2
        je     sub_3_0216
        ret    
sub_3_0216:
        pushal 
        mov    edx, dword ptr [esi + 0x18]
        mov    ecx, 7
loc_3_021F:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    edx
        loop   loc_3_021F
        popal  
        ret    
loc_3_022A:
        pushal 
        mov    edx, dword ptr [esi + 0x18]
        inc    edx
        in     al, dx
        mov    byte ptr [esi + 0x74], al
        add    dl, 2
        jmp    loc_3_0238
loc_3_0238:
        in     al, dx
        mov    byte ptr [esi + 0x75], al
        inc    edx
        jmp    loc_3_023F
loc_3_023F:
        in     al, dx
        mov    byte ptr [esi + 0x76], al
        sub    dl, 4
        mov    ecx, 7
loc_3_024B:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    edx
        loop   loc_3_024B
        popal  
        ret    
sub_3_0256:
        mov    word ptr [esi + 0x83], 0
        mov    word ptr [esi + 0x7f], 0
        mov    word ptr [esi + 0x81], 0
        and    byte ptr [esi + 0x78], 0xfe
        and    byte ptr [esi + 0x7b], 0xfe
        mov    byte ptr [esi + 0x7c], 0
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x4  RW-
VxD__LDATA ENDS
