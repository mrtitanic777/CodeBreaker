; avvxp500 -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 438 instructions recovered by recursive descent

Declare_Virtual_Device avvxp500, 4, 0, avvxp500_Control, 04321h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xf8  R-X
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  04321h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'avvxp500'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  avvxp500_Control         ; DDB_Control_Proc
        dd  avvxp500_PM_API          ; DDB_V86_API_Proc
        dd  avvxp500_PM_API          ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
avvxp500_Control:
        cmp    eax, 0xb
        je     loc_5_0134
loc_1_0085:
        cmp    eax, 0x1b
        je     loc_7_0000
loc_1_008E:
        cmp    eax, 0x1c
        je     loc_3_0000
loc_1_0097:
        cmp    eax, 0x22
        je     loc_4_0183
loc_1_00A0:
        clc    
        ret    
        db  08Bh, 0C0h
loc_1_00A4:
        push   eax
        push   edi
        mov    eax, dword ptr [0x1c]         ; -> obj1:0x006C
        or     eax, eax
        je     loc_1_00B1
        mov    ebx, eax
loc_1_00B1:
        pop    edi
        pop    eax
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        clc    
        ret    
        db  090h
loc_1_00BC:
        push   ecx
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        pop    ecx
        clc    
        ret    
loc_1_00CC:
        push   edi
        cmp    dword ptr [0x1c], ebx         ; -> obj1:0x006C
        jne    loc_1_00E7
        jecxz  loc_1_00DF
loc_1_00D7:
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_1_00F4
loc_1_00DF:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jmp    loc_1_00F4
loc_1_00E7:
        test   word ptr [0xc], 0x8000        ; -> obj1:0x005C
        jne    loc_1_00DF
        je     loc_1_00D7
loc_1_00F4:
        pop    edi
        clc    
        ret    
        db  0CCh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x5d  R-X
        db  001h, 000h, 000h, 000h
        dd  loc_2_0010               ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h, 04Dh, 000h
loc_2_0010:
        push   ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x88
        and    byte ptr [ecx + 0x63], ah
        arpl   word ptr [ebp + 0x73], sp
        jae    0x41
        arpl   word ptr [ecx + 0x70], sp
        je     0x9b
        jb     0x8d
        and    byte ptr [eax + 0x6f], dh
        jb     0xa1
        and    byte ptr [0x65687420], ch
        and    byte ptr [esi + 0x78], dl
        push   eax
        xor    eax, 0x69203030
        jae    0x5e
        imul   ebp, dword ptr [esi + 0x20], 0x20657375
        bound  edi, qword ptr [ecx + 0x20]
        popal  
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0xb5
        jb     0x70
        popal  
        jo     0xc3
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        db  02Eh, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x2  R-X
loc_3_0000:
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x197  R-X
sub_4_0000:
        cmp    dword ptr [0x1c], 0           ; -> obj1:0x006C
        jne    loc_4_0010
        mov    eax, 0
        jmp    loc_4_0015
loc_4_0010:
        mov    eax, 0xffffffff
loc_4_0015:
        ret    
sub_4_0016:
        push   ebp
        mov    ebp, esp
        pushal 
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0x14], eax         ; -> obj1:0x0064
        mov    ax, word ptr [ebp + 0xc]
        mov    word ptr [6], ax              ; -> obj1:0x0056
        mov    ax, word ptr [ebp + 0x10]
        mov    byte ptr [8], al              ; -> obj1:0x0058
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [0x10], eax         ; -> obj1:0x0060
        push   0
        push   0x8000
        push   dword ptr [ebp + 0x14]
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_4_00E0
        push   0
        push   0
        push   0x92
        push   0x7fff
        push   eax
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        mov    word ptr [0xe], ax            ; -> obj1:0x005E
        call   sub_5_01E3
loc_4_0089:
        mov    word ptr [0xa], ax            ; -> obj1:0x005A
        xor    eax, eax
        movsx  ecx, byte ptr [8]             ; -> obj1:0x0058
        cmp    ecx, -1
        je     loc_4_00D5
        push   edi
        mov    word ptr [0], cx              ; -> loc_6_0010
        mov    edi, 0                        ; -> loc_6_0010
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        pop    edi
        jb     loc_4_00E0
        mov    dword ptr [0x18], eax         ; -> obj1:0x0068
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 4
        jne    loc_4_00D5
        or     word ptr [0xc], 0x8000        ; -> obj1:0x005C
loc_4_00D5:
        mov    dword ptr [esp + 0x1c], 0xffffffff
loc_4_00DD:
        popal  
        pop    ebp
        ret    
loc_4_00E0:
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_4_00DD
sub_4_00EA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    eax, dword ptr [0x18]         ; -> obj1:0x0068
        or     eax, eax
        je     loc_4_0100
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
loc_4_0100:
        popal  
        add    esp, 4
        pop    ebp
        ret    
sub_4_0106:
        push   ebp
        mov    ebp, esp
        pushal 
        mov    dword ptr [esp + 0x1c], 0x13
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_0016
        add    esp, 0x10
        or     eax, eax
        je     loc_4_0132
        mov    dword ptr [esp + 0x1c], 0
loc_4_0132:
        popal  
        pop    ebp
        ret    
        db  0C7h, 044h, 024h, 01Ch, 024h, 000h, 000h, 000h, 0EBh, 0F3h
sub_4_013F:
        push   edi
        push   ebx
        call   0
        or     eax, eax
        je     loc_4_0159
        mov    ebx, dword ptr [0x1c]         ; -> obj1:0x006C
        or     ebx, ebx
        je     loc_4_0159
        call   sub_5_014D
loc_4_0159:
        call   sub_4_00EA
        mov    eax, 0xffffffff
        pop    ebx
        pop    edi
        ret    
loc_4_0166:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_0000
loc_4_017D:
        add    esp, 0x14
        leave  
        clc    
        ret    
loc_4_0183:
        mov    eax, ebx
        mov    ebx, 0                        ; -> loc_4_0166
        VxDCall  VFAT.Service_0000h              ; cd2000004a04
        mov    eax, 0
        stc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x2c2  R-X
sub_5_0000:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xd8
        test   eax, eax
        je     loc_5_012D
        cmp    eax, 1
        je     loc_5_003F
        cmp    eax, 2
        je     loc_5_00CC
        cmp    eax, 3
        je     loc_5_00D9
        cmp    eax, 4
        je     loc_5_0122
        mov    eax, 1
        jmp    loc_5_012F
loc_5_003F:
        cmp    dword ptr [ebp + 0xc], 1
        jbe    loc_5_004F
        mov    eax, 1
        jmp    loc_5_012F
loc_5_004F:
        push   0
        lea    ecx, [ebp - 0xd8]
        push   dword ptr [ebp + 0x10]
        push   ecx
        call   sub_5_02BC
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_012F
        cmp    word ptr [ebp - 0x7c], 1
        jne    loc_5_00C5
        cmp    word ptr [ebp - 0x52], 2
        jne    loc_5_00C5
        cmp    word ptr [ebp - 0x2a], 1
        mov    ax, word ptr [ebp - 0x7a]
        je     loc_5_008E
        mov    eax, 0x13
        jmp    loc_5_012F
loc_5_008E:
        movzx  cx, byte ptr [ebp - 0x28]
        cmp    word ptr [ebp - 0xd8], 1
        jne    loc_5_00BE
        cmp    dword ptr [ebp - 0xb2], 0x8000
        jne    loc_5_00BE
        push   dword ptr [ebp - 0xd6]
        push   ecx
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_4_0106
loc_5_00B9:
        add    esp, 0x10
        jmp    loc_5_012F
loc_5_00BE:
        mov    eax, 0x13
        jmp    loc_5_012F
loc_5_00C5:
        mov    eax, 0x13
        jmp    loc_5_012F
loc_5_00CC:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_5_0122
        mov    eax, 1
        jmp    loc_5_012F
loc_5_00D9:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_00EC
        cmp    eax, 1
        je     loc_5_0107
        mov    eax, 1
        jmp    loc_5_012F
loc_5_00EC:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0000
loc_5_00F4:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_5_012F
loc_5_0107:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0000
loc_5_010F:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_5_012F
loc_5_0122:
        push   dword ptr [ebp + 0x10]
        call   sub_4_013F
loc_5_012A:
        add    esp, 4
loc_5_012D:
        xor    eax, eax
loc_5_012F:
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh
loc_5_0134:
        cmp    dword ptr [0x1c], ebx         ; -> obj1:0x006C
        jne    loc_5_014B
        mov    dword ptr [0x20], 0xffffffff  ; -> obj1:0x0070
        call   sub_5_014D
loc_5_014B:
        clc    
        ret    
sub_5_014D:
        push   eax
        push   edx
        cmp    dword ptr [0x1c], ebx         ; -> obj1:0x006C
        jne    loc_5_0183
        mov    dword ptr [0x1c], 0           ; -> obj1:0x006C
        mov    eax, dword ptr [0x18]         ; -> obj1:0x0068
        or     eax, eax
        je     loc_5_017C
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
loc_5_017C:
        and    dword ptr [0], 0xfffffffe     ; -> obj1:0x0050
loc_5_0183:
        pop    edx
        pop    eax
        clc    
        ret    
sub_5_0187:
        push   eax
        push   ecx
        push   edx
        cmp    dword ptr [0x1c], 0           ; -> obj1:0x006C
        je     loc_5_019E
        cmp    dword ptr [0x1c], ebx         ; -> obj1:0x006C
        je     loc_5_01DE
        stc    
        jb     loc_5_01DF
loc_5_019E:
        mov    dword ptr [0x1c], ebx         ; -> obj1:0x006C
        cmp    dword ptr [0x20], ebx         ; -> obj1:0x0070
        je     loc_5_01B3
        movzx  edx, word ptr [6]             ; -> obj1:0x0056
loc_5_01B3:
        mov    dword ptr [0x20], ebx         ; -> obj1:0x0070
        mov    eax, dword ptr [0x18]         ; -> obj1:0x0068
        or     eax, eax
        je     loc_5_01DE
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 0x10
        je     loc_5_01D8
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_5_01DE
loc_5_01D8:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_5_01DE:
        clc    
loc_5_01DF:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_5_01E3:
        mov    eax, 0x400
        clc    
        ret    
        db  0CCh, 0CCh
avvxp500_PM_API:
        movzx  eax, word ptr [ebp + 0x14]
        movzx  eax, ax
        cmp    eax, 4
        jae    loc_5_0209
        call   sub_6_0000
        jb     loc_5_0209
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_5_0209:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_5_020F:
        call   sub_5_01E3
        mov    word ptr [ebp + 0x1c], ax
        clc    
        ret    
loc_5_021A:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_023E
        mov    ecx, dword ptr [0x20]         ; -> obj1:0x0070
        mov    word ptr [ebp + 0x1c], 0
        call   sub_5_0187
        jb     loc_5_0236
        ret    
loc_5_0236:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_0244
loc_5_023E:
        mov    word ptr [ebp + 0x1c], 1
loc_5_0244:
        stc    
        ret    
loc_5_0246:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_026D
        mov    ecx, dword ptr [0x1c]         ; -> obj1:0x006C
        cmp    ecx, ebx
        je     loc_5_0260
        mov    word ptr [ebp + 0x1c], 3
        jmp    loc_5_0273
loc_5_0260:
        call   sub_5_014D
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_026D:
        mov    word ptr [ebp + 0x1c], 1
loc_5_0273:
        stc    
        ret    
loc_5_0275:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        lea    esi, [0]                      ; -> obj1:0x0050
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_02B0
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_02B0
        cmp    ecx, 0x28
        ja     loc_5_02B0
        mov    dword ptr [esi], ecx
        shr    ecx, 1
        rep movsw word ptr es:[edi], word ptr [esi]
        adc    cl, cl
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_02B0:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
        db  0CCh, 0CCh, 0CCh
sub_5_02BC:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x30  RW-
sub_6_0000:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_0010:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        hlt    
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0x1a  R-X
loc_7_0000:
        push   0
        push   4
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        or     eax, eax
        jne    loc_7_0013
        stc    
        ret    
loc_7_0013:
        mov    dword ptr [0], eax            ; -> obj1:0x0078
        clc    
        ret    
VxD__LTEXT ENDS
