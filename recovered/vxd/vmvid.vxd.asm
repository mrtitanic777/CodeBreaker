; VdspD -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 2788 instructions recovered by recursive descent

Declare_Virtual_Device VdspD, 3, 0, VdspD_Control, 03202h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h
;   service table: obj1:0x80, 2 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x6c0  R-X
        db  083h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  085h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  084h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  086h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
loc_1_0040:
        sal    byte ptr [ebx], 0x42
        mov    esp, cs
        mov    ah, 0xce
        adc    dword ptr [edx + 0x53454494], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0050:
        sal    dword ptr [ebx], 0x42
        mov    esp, cs
        mov    ah, 0xce
        adc    dword ptr [edx + 0x53454494], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0060:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        dd  VdspD_Service_00         ; fixup
        dd  VdspD_Service_01         ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  03202h                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VdspD   '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  VdspD_Control            ; DDB_Control_Proc
        dd  VdspD_PM_API             ; DDB_V86_API_Proc
        dd  VdspD_PM_API             ; DDB_PM_API_Proc
        dd  obj1:0x0080              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00E0:
        add    al, 0
        mov    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    dword ptr [ebx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    al, byte ptr [ebx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    eax, dword ptr [ebx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00FA:
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0108:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_010C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0110:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0114:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0118:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_011C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0120:
        add    byte ptr [eax], al
loc_1_0122:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0126:
        add    byte ptr [eax], al
loc_1_0128:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_012C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0130:
        and    byte ptr [edx], al
loc_1_0132:
        pop    es
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], dh
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
        add    byte ptr [eax], al            ; -> loc_1_0559
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0565
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_05A6
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_05E0
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_065B
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_069F
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_062D
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0559
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_3_0126
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_3_00C4
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_3_00E5
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_3_0012
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_3_0097
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0559
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_3_03F8
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_3_0399
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_3_03C4
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x0172
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_01D6:
        push   es
        add    byte ptr [edi], al
        add    byte ptr [edx], cl
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     eax, 0xf000e00
        add    ah, cl
        int3   
VdspD_Control:
        cmp    eax, 0xb
        je     loc_1_0377
        cmp    eax, 0x1b
        je     loc_7_0050
loc_1_01FA:
        cmp    eax, 0x1c
        je     loc_3_0000
loc_1_0203:
        cmp    eax, 0x22
        je     loc_4_03D3
loc_1_020C:
        cmp    eax, 0x22
        je     loc_4_03D3
loc_1_0215:
        clc    
        ret    
loc_1_0217:
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x013E
        or     eax, eax
        je     loc_1_023C
        cmp    word ptr [0], 0               ; -> obj1:0x0160
        je     loc_1_023A
        pop    eax
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004800300
        jmp    loc_1_0243
loc_1_023A:
        mov    ebx, eax
loc_1_023C:
        pop    eax
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_1_0243:
        clc    
        ret    
loc_1_0245:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004800300
loc_1_0251:
        cmp    dword ptr [0], ebx            ; -> obj1:0x013E
        jne    loc_1_026B
        jecxz  loc_1_0263
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_1_0271
loc_1_0263:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jmp    loc_1_0271
loc_1_026B:
        VxDCall  VPICD.VPICD_Set_Auto_Masking    ; cd200a000300
loc_1_0271:
        clc    
        ret    
sub_1_0273:
        push   ecx
        push   edx
        add    dx, 0xe
        mov    ecx, 0x3e8
loc_1_027E:
        in     al, dx
        test   al, 0x80
        jne    loc_1_0288
        loop   loc_1_027E
        stc    
        jmp    loc_1_028E
loc_1_0288:
        sub    dx, 4
        in     al, dx
        clc    
loc_1_028E:
        pop    edx
        pop    ecx
        ret    
sub_1_0291:
        push   edx
        push   ecx
        push   eax
        add    dx, 0xc
        mov    ecx, 0x100000
loc_1_029D:
        in     al, dx
        test   al, 0x80
        je     loc_1_02A8
        loop   loc_1_029D
        pop    eax
        stc    
        jmp    loc_1_02AB
loc_1_02A8:
        pop    eax
        out    dx, al
        clc    
loc_1_02AB:
        pop    ecx
        pop    edx
        ret    
sub_1_02AE:
        push   eax
        push   ecx
        push   edx
        add    dx, 6
        mov    al, 1
        out    dx, al
        mov    ecx, 0xc8
loc_1_02BD:
        loop   loc_1_02BD
        xor    al, al
        out    dx, al
        mov    ecx, 0x19
        pop    edx
        push   edx
loc_1_02C9:
        call   sub_1_0273
        jb     loc_1_02D4
        cmp    al, 0xaa
        je     loc_1_02D9
loc_1_02D4:
        loop   loc_1_02C9
        stc    
        jmp    loc_1_02E0
loc_1_02D9:
        mov    al, 0xd1
        call   sub_1_0291
loc_1_02E0:
        pop    edx
        pop    ecx
        pop    eax
        ret    
loc_1_02E4:
        mov    al, 0xe1
        call   sub_1_0291
        jb     loc_1_02FD
        call   sub_1_0273
        jb     loc_1_02FD
        mov    ah, al
        call   sub_1_0273
        jae    loc_1_0302
loc_1_02FD:
        xor    ax, ax
        stc    
        ret    
loc_1_0302:
        clc    
        ret    
loc_1_0304:
        xor    ah, ah
        mov    al, 0xfa
        call   sub_1_0291
        jb     loc_1_0316
        call   sub_1_0273
        jae    loc_1_031B
loc_1_0316:
        xor    ax, ax
        stc    
        ret    
loc_1_031B:
        clc    
        ret    
sub_1_031D:
        push   esi
        push   eax
        push   ecx
        mov    ecx, 8
        mov    esi, 0                        ; -> loc_1_01D6
loc_1_032A:
        mov    edx, 0
        mov    dx, word ptr [esi]
        add    dx, word ptr [0]              ; -> loc_1_0130
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        add    esi, 2
        loop   loc_1_032A
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_1_0349:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    ecx, 8
        mov    esi, 0                        ; -> loc_1_01D6
loc_1_0357:
        mov    edx, 0
        mov    dx, word ptr [esi]
        add    dx, word ptr [0]              ; -> loc_1_0130
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        add    esi, 2
        loop   loc_1_0357
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
loc_1_0377:
        push   eax
        cmp    dword ptr [0], ebx            ; -> obj1:0x013E
        jne    loc_1_03AE
        mov    dword ptr [0], 0xffffffff     ; -> obj1:0x0142
        mov    eax, 2
        call   sub_1_03E4
        or     edx, edx
        je     loc_1_03E1
        movzx  edx, word ptr [0]             ; -> loc_1_0130
        call   sub_1_02AE
        mov    dword ptr [0], 0              ; -> obj1:0x0142
loc_1_03AE:
        cmp    dword ptr [0], ebx            ; -> loc_1_0108
        jne    loc_1_03C5
        call   sub_3_0244
loc_1_03BB:
        mov    dword ptr [0], 0              ; -> loc_1_0108
loc_1_03C5:
        cmp    dword ptr [0], ebx            ; -> loc_1_0110
        jne    loc_1_03E1
        mov    eax, 1
        call   sub_3_0534
loc_1_03D7:
        mov    dword ptr [0], 0              ; -> loc_1_0110
loc_1_03E1:
        pop    eax
        clc    
        ret    
sub_1_03E4:
        push   eax
        push   edx
        test   eax, 2
        je     loc_1_0431
        cmp    dword ptr [0], ebx            ; -> obj1:0x013E
        jne    loc_1_0431
        mov    eax, dword ptr [0]            ; -> obj1:0x013A
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        call   sub_1_031D
        mov    dword ptr [0], 0              ; -> obj1:0x013E
        mov    edx, ebx
        add    edx, dword ptr [0]            ; -> loc_1_0118
        and    dword ptr [edx], 0xfffffffe
        mov    eax, dword ptr [0]            ; -> obj1:0x013A
        VxDCall  VPICD.VPICD_Set_Auto_Masking    ; cd200a000300
loc_1_0431:
        pop    edx
        pop    eax
        clc    
        ret    
sub_1_0435:
        push   eax
        push   ecx
        push   edx
        test   eax, 2
        je     loc_1_0494
        cmp    dword ptr [0], 0              ; -> obj1:0x013E
        je     loc_1_0453
        cmp    dword ptr [0], ebx            ; -> obj1:0x013E
        je     loc_1_0493
        stc    
        jb     loc_1_0494
loc_1_0453:
        mov    dword ptr [0], ebx            ; -> obj1:0x013E
        call   sub_1_0349
        test   eax, 4
        je     loc_1_046D
        cmp    dword ptr [0], ebx            ; -> obj1:0x0142
        je     loc_1_0482
loc_1_046D:
        cmp    dword ptr [0], 0              ; -> obj1:0x0142
        je     loc_1_0482
        movzx  edx, word ptr [0]             ; -> loc_1_0130
        call   sub_1_02AE
loc_1_0482:
        mov    dword ptr [0], ebx            ; -> obj1:0x0142
        mov    eax, dword ptr [0]            ; -> obj1:0x013A
        VxDCall  VPICD.VPICD_Set_Auto_Masking    ; cd200a000300
loc_1_0493:
        clc    
loc_1_0494:
        pop    edx
        pop    ecx
        pop    eax
        ret    
VdspD_Service_00:
        mov    eax, 0x400
        clc    
        ret    
VdspD_Service_01:
        cmp    word ptr [0], ax              ; -> loc_1_0130
        je     loc_1_04AA
        stc    
        ret    
loc_1_04AA:
        mov    ebx, dword ptr [0]            ; -> obj1:0x013E
        clc    
        ret    
sub_1_04B2:
        mov    esi, ebx
        add    esi, dword ptr [0]            ; -> loc_1_0118
        test   dword ptr [esi], 1
        jne    loc_1_04EC
        or     dword ptr [esi], 1
        test   word ptr [0], 1               ; -> obj1:0x0138
        jne    loc_1_04EC
        mov    eax, 0x30
        movzx  ecx, word ptr [0xc]           ; -> obj2:0x000C
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        xor    esi, esi
        xor    edi, edi
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
loc_1_04EC:
        ret    
loc_1_04ED:
        push   eax
        mov    eax, dword ptr [0]            ; -> obj1:0x013E
        cmp    eax, ebx
        je     loc_1_050D
        or     eax, eax
        jne    loc_1_0520
        push   ebx
        push   ecx
        push   edx
        mov    eax, 6
        call   sub_1_0435
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_1_0520
loc_1_050D:
        pop    eax
        cmp    ecx, 4
        jbe    loc_1_0519
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_1_0519:
        je     loc_1_051E
        in     al, dx
        jmp    loc_1_051F
loc_1_051E:
        out    dx, al
loc_1_051F:
        ret    
loc_1_0520:
        pop    eax
        call   sub_1_04B2
        xor    eax, eax
        dec    eax
        ret    
VdspD_PM_API:
        movzx  eax, word ptr [ebp + 0x14]
        cmp    ah, 7
        jae    loc_1_0554
        movzx  ecx, ah
        shl    ecx, 3
        lea    ecx, [ecx]                    ; -> obj1:0x01B6
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    loc_1_0554
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     loc_1_0554
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_1_0554:
        or     dword ptr [ebp + 0x2c], 1
        ret    
loc_1_0559:
        VxDCall  Dev_3202.Service_0000h          ; cd2000000232
        mov    word ptr [ebp + 0x1c], ax
        clc    
        ret    
loc_1_0565:
        movzx  eax, word ptr [ebp + 0x10]
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_1_057A
        cmp    word ptr [0], dx              ; -> loc_1_0130
        je     loc_1_0582
loc_1_057A:
        mov    word ptr [ebp + 0x1c], 1
loc_1_0580:
        stc    
        ret    
loc_1_0582:
        mov    word ptr [ebp + 0x1c], 0
        cmp    dword ptr [0], ebx            ; -> obj1:0x0142
        je     loc_1_0596
        mov    word ptr [ebp + 0x1c], 0x8000
loc_1_0596:
        call   sub_1_0435
        jb     loc_1_059E
        ret    
loc_1_059E:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_1_0580
loc_1_05A6:
        movzx  eax, word ptr [ebp + 0x10]
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_1_05BB
        cmp    word ptr [0], dx              ; -> loc_1_0130
        je     loc_1_05C3
loc_1_05BB:
        mov    word ptr [ebp + 0x1c], 1
loc_1_05C1:
        stc    
        ret    
loc_1_05C3:
        cmp    dword ptr [0], ebx            ; -> obj1:0x013E
        je     loc_1_05D3
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_1_05C1
loc_1_05D3:
        call   sub_1_03E4
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_1_05E0:
        movzx  eax, word ptr [0]             ; -> obj1:0x0146
        mov    word ptr [ebp + 0x1c], ax
        movzx  eax, word ptr [0]             ; -> obj1:0x0148
        mov    word ptr [ebp + 0x10], ax
        movzx  eax, word ptr [0]             ; -> loc_1_0130
        mov    word ptr [ebp + 0x14], ax
        movzx  eax, word ptr [0]             ; -> loc_1_0132
        mov    ch, al
        movzx  eax, word ptr [0]             ; -> obj1:0x0134
        mov    cl, al
        mov    ax, word ptr [0]              ; -> obj1:0x0136
        shl    ax, 1
        shl    ax, 1
        shl    ax, 1
        shl    ax, 1
        or     cl, al
        mov    word ptr [ebp + 0x18], cx
        clc    
        ret    
loc_1_062D:
        movzx  eax, word ptr [0]             ; -> obj1:0x0146
        mov    word ptr [ebp + 0x1c], ax
        movzx  eax, word ptr [0]             ; -> obj1:0x0148
        mov    word ptr [ebp + 0x10], ax
        movzx  eax, word ptr [0]             ; -> obj1:0x015A
        mov    word ptr [ebp + 0x14], ax
        movzx  eax, word ptr [0]             ; -> obj1:0x0158
        mov    word ptr [ebp + 0x18], ax
        clc    
        ret    
loc_1_065B:
        cmp    dword ptr [0], 0              ; -> obj1:0x0150
        je     loc_1_068F
        mov    cx, word ptr [0]              ; -> obj1:0x0154
        mov    word ptr [ebp + 0x18], cx
        mov    eax, dword ptr [0]            ; -> obj1:0x014C
        mov    word ptr [ebp + 0x1c], ax
        ror    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        ror    eax, 0x10
        mov    bx, word ptr [0]              ; -> obj1:0x0156
        mov    word ptr [ebp + 0x10], bx
        jmp    loc_1_069D
loc_1_068F:
        mov    word ptr [ebp + 0x1c], 0
        mov    word ptr [ebp + 0x14], 0
        stc    
        ret    
loc_1_069D:
        clc    
        ret    
loc_1_069F:
        mov    dx, word ptr [0]              ; -> loc_1_0130
        mov    ax, 0xfe
        call   sub_1_0291
        call   sub_1_0273
        mov    ah, al
        call   sub_1_0273
        mov    word ptr [ebp + 0x1c], ax
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x92  R-X
        db  002h, 000h, 000h, 000h
        dd  loc_2_0014               ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        pop    ecx
        add    byte ptr [esi], bh
        int3   
        int3   
loc_2_0014:
        push   ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x8c
        and    byte ptr [eax + 0x6c], dh
        popal  
        jns    0x43
        jae    0x94
        jne    0x95
        and    byte ptr fs:[0x65687420], ch
        and    byte ptr [ebp + 0x65], cl
        imul   esp, dword ptr fs:[ecx + 0x20], 0x69736956
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [ecx + 0x75], ah
        imul   ebp, dword ptr fs:[edi + 0x20], 0x64726168
        ja     0xa9
        jb     0xaf
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [ebp + 0x73], dh
        and    byte ptr gs:[edx + 0x79], ah
        and    byte ptr [ecx + 0x6e], ah
        outsd  dx, dword ptr [esi]
        je     0xc5
        jb     0x80
        popal  
        jo     0xd3
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        add    byte ptr cs:[ebp + 0x65], cl
        imul   esp, dword ptr fs:[ecx + 0x56], 0x6f697369
        outsb  dx, byte ptr [esi]
        and    byte ptr [edx + 0x61], cl
        jp     0xf7
        and    byte ptr [ecx + ebp*2 + 0x72], al
        arpl   word ptr gs:[eax + 0x53], si
        outsd  dx, dword ptr [esi]
        jne    0xf7
        and    byte ptr fs:[edx + esi*2 + 0x69], al
        jbe    0xf5
        jb     0x92
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x5a0  R-X
loc_3_0000:
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_4_00FC
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        clc    
        ret    
loc_3_0012:
        xchg   esi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        push   dword ptr [ebp + 0x3c]
        push   dword ptr [ebp + 0x14]
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x3c], ax
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x3c], ax
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [esi + 8], eax
        pop    dword ptr [ebp + 0x14]
        pop    dword ptr [ebp + 0x3c]
        mov    eax, dword ptr [esi]
        mov    eax, 0                        ; -> obj1:0x00DC
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        push   dword ptr [esi]
        call   sub_5_048B
loc_3_0075:
        add    esp, 0x10
        or     eax, eax
        je     loc_3_0089
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        clc    
        ret    
loc_3_0089:
        mov    word ptr [ebp + 0x1c], 0
        mov    word ptr [ebp + 0x14], 0
        stc    
        ret    
loc_3_0097:
        xchg   esi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    eax, dword ptr [esi]
        mov    eax, 0                        ; -> obj1:0x00DC
        lea    eax, [0]                      ; -> obj1:0x00DC
        push   dword ptr [esi + 4]
        push   eax
        call   sub_5_061B
loc_3_00B9:
        add    esp, 8
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_3_00C4:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        jb     loc_3_00DD
        call   sub_3_01B4
        jb     loc_3_00D5
        clc    
        ret    
loc_3_00D5:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_3_00E3
loc_3_00DD:
        mov    word ptr [ebp + 0x1c], 1
loc_3_00E3:
        stc    
        ret    
loc_3_00E5:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_3_011E
        cmp    dword ptr [0], ebx            ; -> loc_1_0108
        je     loc_3_00FD
        mov    word ptr [ebp + 0x1c], 3
        jmp    loc_3_0124
loc_3_00FD:
        call   sub_3_0244
        mov    dword ptr [0], 0              ; -> loc_1_0108
        mov    dword ptr [0], 0              ; -> loc_1_010C
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_3_011E:
        mov    word ptr [ebp + 0x1c], 1
loc_3_0124:
        stc    
        ret    
loc_3_0126:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ax, 1
        jne    loc_3_0173
        mov    ecx, dword ptr [ebp + 0x18]
        cmp    ecx, dword ptr [0]            ; -> loc_1_011C
        jne    loc_3_0173
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_3_0173
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_3_0173
        cmp    ecx, 0x1c
        ja     loc_3_0173
        mov    word ptr [edi + 4], 1
        mov    word ptr [edi + 6], 0x388
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [edi + 0xc], eax
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_3_0173:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
sub_3_017C:
        push   edi
        mov    edi, 0                        ; -> loc_1_00E0
        VMMCall  VMM.Install_Mult_IO_Handlers    ; cd2095000100
        pop    edi
        ret    
loc_3_018A:
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_0108
        cmp    eax, ebx
        je     loc_3_019B
        call   sub_3_01B4
        jb     loc_3_01AE
loc_3_019B:
        pop    eax
        cmp    ecx, 4
        jbe    loc_3_01A7
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_3_01A7:
        je     loc_3_01AC
        in     al, dx
        jmp    loc_3_01AD
loc_3_01AC:
        out    dx, al
loc_3_01AD:
        ret    
loc_3_01AE:
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
        db  090h
sub_3_01B4:
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_0108
        or     eax, eax
        je     loc_3_01C4
        cmp    ebx, eax
        je     loc_3_01F9
        jmp    loc_3_01FC
loc_3_01C4:
        cmp    ebx, dword ptr [0]            ; -> loc_1_010C
        je     loc_3_01D1
        call   sub_3_0265
loc_3_01D1:
        xchg   dword ptr [0], ebx            ; -> loc_1_0108
        mov    dword ptr [0], ebx            ; -> loc_1_010C
        or     ebx, ebx
        je     loc_3_01EE
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_3_01EE
        call   sub_3_0244
loc_3_01EE:
        mov    ebx, dword ptr [0]            ; -> loc_1_0108
        call   sub_3_0223
loc_3_01F9:
        pop    eax
        clc    
        ret    
loc_3_01FC:
        push   esi
        mov    esi, ebx
        jmp    loc_3_021F
        db  060h, 00Fh, 0B7h, 00Dh
        dd  obj2:0x000C              ; fixup
        db  003h, 00Dh
        dd  obj2:0x0004              ; fixup
        db  0B8h, 030h, 000h, 000h, 000h, 033h, 0F6h, 033h, 0FFh, 0CDh, 020h, 004h, 000h, 017h, 000h, 061h
loc_3_021F:
        pop    esi
        pop    eax
        stc    
        ret    
sub_3_0223:
        push   esi
        push   ecx
        push   edx
        mov    esi, 0                        ; -> loc_1_00E0
        movzx  ecx, word ptr [esi]
        add    esi, 2
loc_3_0231:
        movzx  edx, word ptr [esi]
        add    esi, 6
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        loop   loc_3_0231
        pop    edx
        pop    ecx
        pop    esi
        clc    
        ret    
sub_3_0244:
        push   esi
        push   ecx
        push   edx
        mov    esi, 0                        ; -> loc_1_00E0
        movzx  ecx, word ptr [esi]
        add    esi, 2
loc_3_0252:
        movzx  edx, word ptr [esi]
        add    esi, 6
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        loop   loc_3_0252
        pop    edx
        pop    ecx
        pop    esi
        clc    
        ret    
sub_3_0265:
        push   eax
        push   edx
        push   ebx
        push   ecx
        mov    edx, 0x388
        mov    ebx, 0x105
        mov    al, 1
        call   sub_3_0312
        mov    ebx, 4
        mov    al, 0x60
        call   sub_3_0312
        mov    ebx, 0x104
        mov    al, 0x3f
        call   sub_3_0312
        mov    ebx, 8
        xor    al, al
        call   sub_3_0312
        mov    ebx, 0xbd
        mov    al, 0xc0
        call   sub_3_0312
        mov    ecx, 0x15
        mov    ebx, 0x40
        mov    al, 0x3f
loc_3_02B6:
        xor    bh, bh
        call   sub_3_0312
        mov    bh, 1
        call   sub_3_0312
        inc    bl
        loop   loc_3_02B6
        mov    ecx, 8
        mov    ebx, 0xb0
        xor    eax, eax
loc_3_02D4:
        xor    bh, bh
        call   sub_3_0312
        mov    bh, 1
        call   sub_3_0312
        inc    bl
        loop   loc_3_02D4
        mov    ebx, 0x104
        xor    eax, eax
        call   sub_3_0312
        xor    eax, eax
        mov    ebx, 0x105
        call   sub_3_0312
        call   sub_3_032F
        call   sub_3_032F
        call   sub_3_032F
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret    
sub_3_0312:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_3_032F
        pop    eax
        inc    dl
        out    dx, al
        call   sub_3_032F
        pop    edx
        pop    ebx
        ret    
sub_3_032F:
        push   eax
        push   ecx
        inc    edx
        mov    ecx, 0x32
loc_3_0337:
        in     al, dx
        loop   loc_3_0337
        dec    edx
        pop    ecx
        pop    eax
        ret    
loc_3_033E:
        push   eax
        mov    eax, dword ptr [0]            ; -> loc_1_0110
        cmp    eax, ebx
        je     loc_3_0354
        mov    eax, 1
        call   sub_3_0447
        jb     loc_3_0367
loc_3_0354:
        pop    eax
        cmp    ecx, 4
        jbe    loc_3_0360
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_3_0360:
        je     loc_3_0365
        in     al, dx
        jmp    loc_3_0366
loc_3_0365:
        out    dx, al
loc_3_0366:
        ret    
loc_3_0367:
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
sub_3_036C:
        push   edi
        push   eax
        push   ecx
        mov    edi, 0                        ; -> loc_1_00FA
        movzx  ecx, word ptr [edi]
        add    edi, 2
        mov    ax, word ptr [0]              ; -> obj1:0x015A
loc_3_0380:
        mov    word ptr [edi], ax
        add    edi, 6
        inc    ax
        loop   loc_3_0380
        mov    edi, 0                        ; -> loc_1_00FA
        VMMCall  VMM.Install_Mult_IO_Handlers    ; cd2095000100
        pop    ecx
        pop    eax
        pop    edi
        ret    
loc_3_0399:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_3_03BC
        mov    word ptr [ebp + 0x1c], 0
        mov    eax, 1
        call   sub_3_0447
        jb     loc_3_03B4
        ret    
loc_3_03B4:
        mov    word ptr [ebp + 0x1c], 1
        jmp    loc_3_03C2
loc_3_03BC:
        mov    word ptr [ebp + 0x1c], 1
loc_3_03C2:
        stc    
        ret    
loc_3_03C4:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_3_03F0
        mov    ecx, dword ptr [0]            ; -> loc_1_0110
        cmp    ecx, ebx
        je     loc_3_03DE
        mov    word ptr [ebp + 0x1c], 1
        jmp    loc_3_03F6
loc_3_03DE:
        mov    eax, 1
        call   sub_3_0534
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_3_03F0:
        mov    word ptr [ebp + 0x1c], 1
loc_3_03F6:
        stc    
        ret    
loc_3_03F8:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ax, 1
        jne    loc_3_043E
        mov    ecx, dword ptr [ebp + 0x18]
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        mov    word ptr [edi + 4], 0
        movzx  eax, word ptr [0]             ; -> obj1:0x015A
        mov    word ptr [edi + 6], ax
        movzx  eax, word ptr [0]             ; -> obj1:0x0158
        mov    byte ptr [edi + 8], al
        mov    eax, dword ptr [0]            ; -> loc_1_011C
        mov    dword ptr [edi + 0x10], eax
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_3_043E:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
sub_3_0447:
        push   edx
        push   ecx
        push   eax
        test   eax, 1
        je     loc_3_0471
        cmp    dword ptr [0], 0              ; -> loc_1_0110
        je     loc_3_0464
        cmp    dword ptr [0], ebx            ; -> loc_1_0110
        je     loc_3_0495
        jmp    loc_3_0471
loc_3_0464:
        mov    dword ptr [0], ebx            ; -> loc_1_0110
        call   sub_3_049A
        jmp    loc_3_0474
loc_3_0471:
        stc    
        jmp    loc_3_0496
loc_3_0474:
        mov    eax, dword ptr [0]            ; -> loc_1_0114
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 0x10
        je     loc_3_048F
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_3_0495
loc_3_048F:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_3_0495:
        clc    
loc_3_0496:
        pop    eax
        pop    ecx
        pop    edx
        ret    
sub_3_049A:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 1
        xor    ecx, ecx
        movzx  edx, word ptr [0]             ; -> obj1:0x015A
loc_3_04AB:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_3_04BC
        inc    cx
        jmp    loc_3_04AB
loc_3_04BC:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
        db  08Bh, 0C0h
loc_3_04C4:
        push   edi
        push   edx
        push   ebx
        movsx  edx, word ptr [0]             ; -> obj1:0x015A
        or     edx, edx
        js     loc_3_04EF
        mov    ebx, dword ptr [0]            ; -> loc_1_0110
        or     ebx, ebx
        jne    loc_3_04F7
        push   eax
        push   ecx
        mov    ecx, 0x32
loc_3_04E3:
        inc    edx
        in     al, dx
        or     al, al
        js     loc_3_04ED
        dec    edx
        in     al, dx
        loop   loc_3_04E3
loc_3_04ED:
        pop    ecx
        pop    eax
loc_3_04EF:
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        jmp    loc_3_04FD
loc_3_04F7:
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_3_04FD:
        pop    ebx
        pop    edx
        pop    edi
        clc    
        ret    
        db  08Bh, 0C0h
loc_3_0504:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        clc    
        ret    
        db  08Bh, 0C0h
loc_3_0514:
        push   edi
        cmp    dword ptr [0], ebx            ; -> loc_1_0110
        jne    loc_3_052F
        jecxz  loc_3_0527
loc_3_051F:
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_3_0531
loc_3_0527:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jmp    loc_3_0531
loc_3_052F:
        jmp    loc_3_051F
loc_3_0531:
        pop    edi
        clc    
        ret    
sub_3_0534:
        push   eax
        push   edx
        push   esi
        test   eax, 1
        je     loc_3_0575
        cmp    dword ptr [0], ebx            ; -> loc_1_0110
        jne    loc_3_0575
        call   sub_3_057A
        mov    dword ptr [0], 0              ; -> loc_1_0110
        mov    eax, dword ptr [0]            ; -> loc_1_0110
        or     eax, eax
        jne    loc_3_0575
        mov    eax, dword ptr [0]            ; -> loc_1_0114
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
loc_3_0575:
        pop    esi
        pop    edx
        pop    eax
        clc    
        ret    
sub_3_057A:
        push   esi
        push   eax
        push   ecx
        mov    si, 1
        xor    ecx, ecx
        movzx  edx, word ptr [0]             ; -> obj1:0x015A
loc_3_058A:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_3_059B
        inc    cx
        jmp    loc_3_058A
loc_3_059B:
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x561  R-X
        db  060h, 066h, 08Bh, 015h
        dd  loc_1_0130               ; fixup
        db  066h, 0A1h
        dd  loc_1_0132               ; fixup
        db  066h, 08Bh, 00Dh
        dd  obj1:0x0134              ; fixup
        db  0E8h
        dd  sub_1_02AE               ; fixup
        db  00Fh, 082h, 0B2h, 000h, 000h, 000h, 0E8h
        dd  loc_1_02E4               ; fixup
        db  00Fh, 082h, 0A7h, 000h, 000h, 000h, 066h, 0A3h
        dd  obj1:0x0146              ; fixup
        db  0E8h
        dd  loc_1_0304               ; fixup
        db  00Fh, 082h, 096h, 000h, 000h, 000h, 066h, 0A3h
        dd  obj1:0x0148              ; fixup
        db  066h, 08Bh, 00Dh
        dd  loc_1_0132               ; fixup
        db  066h, 089h, 00Dh
        dd  obj6:0x008A              ; fixup
        db  0BFh
        dd  obj6:0x008A              ; fixup
        db  0CDh, 020h, 001h, 000h, 003h, 000h, 072h, 075h, 0A3h
        dd  obj1:0x013A              ; fixup
        db  066h, 08Bh, 00Dh
        dd  obj1:0x0158              ; fixup
        db  066h, 083h, 0F9h, 000h, 074h, 019h, 066h, 089h, 00Dh
        dd  obj6:0x00AA              ; fixup
        db  0BFh
        dd  obj6:0x00AA              ; fixup
        db  0CDh, 020h, 001h, 000h, 003h, 000h, 072h, 04Fh, 0A3h
        dd  loc_1_0114               ; fixup
        db  066h, 0C7h, 005h
        dd  obj1:0x014A              ; fixup
        db  008h, 000h, 0B9h, 008h, 000h, 000h, 000h, 0BEh
        dd  obj6:0x0058              ; fixup
        db  056h, 0BFh
        dd  loc_1_01D6               ; fixup
        db  083h, 0C6h, 002h, 033h, 0C9h, 066h, 08Bh, 014h, 04Dh
        dd  loc_1_01D6               ; fixup
        db  066h, 003h, 015h
        dd  loc_1_0130               ; fixup
        db  066h, 089h, 016h, 083h, 0C6h, 006h, 041h, 081h, 0F9h, 008h, 000h, 000h, 000h, 072h, 0E2h, 05Fh
        db  0CDh, 020h, 095h, 000h, 001h, 000h, 072h, 003h, 061h, 0F8h, 0C3h, 0EBh, 0FBh, 061h, 0F9h, 0C3h
sub_4_00D5:
        push   ebp
        mov    ebp, esp
        push   edi
        cmp    dword ptr [0], 0              ; -> obj1:0x013E
        je     loc_4_00E4
        jmp    loc_4_00EB
loc_4_00E4:
        mov    eax, 0
        jmp    loc_4_00F0
loc_4_00EB:
        mov    eax, 0xffffffff
loc_4_00F0:
        pop    edi
        pop    ebp
        ret    
        db  090h, 0EBh, 006h, 0FFh, 025h
        dd  obj1:0x00D8              ; fixup
loc_4_00FC:
        pushal 
        pushfd 
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_4_0151
        cmp    dword ptr [0], ebx            ; -> obj1:0x013E
        jne    loc_4_011E
        mov    ebx, dword ptr [0]            ; -> obj1:0x013E
        mov    eax, 2
        call   sub_1_03E4
loc_4_011E:
        cmp    dword ptr [0], ebx            ; -> loc_1_0108
        jne    loc_4_0135
        call   sub_3_0244
loc_4_012B:
        mov    dword ptr [0], 0              ; -> loc_1_0108
loc_4_0135:
        cmp    dword ptr [0], ebx            ; -> loc_1_0110
        jne    loc_4_0151
        mov    eax, 1
        call   sub_3_0534
loc_4_0147:
        mov    dword ptr [0], 0              ; -> loc_1_0110
loc_4_0151:
        popfd  
        popal  
        jmp    dword ptr [0]                 ; -> obj1:0x00D8
        db  02Eh, 08Bh, 0C0h
loc_4_015C:
        VxDCall  VDMAD.VDMAD_Default_Handler     ; cd2012800400
        cmp    eax, 0x40
        jle    loc_4_016C
        mov    eax, 0x40
loc_4_016C:
        add    eax, 3
        shr    eax, 2
loc_4_0172:
        mov    ebx, eax
        dec    eax
        bsr    cx, ax
        inc    cl
        mov    eax, 1
        shl    eax, cl
        dec    eax
        mov    ecx, ebx
        push   ecx
        sub    esp, 4
        mov    ebx, esp
        push   0xe
        push   ebx
        push   0xfff
        push   0
        push   eax
        push   0
        push   1
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        pop    ebx
        pop    ecx
        or     eax, eax
        jne    loc_4_01B4
        mov    eax, ecx
        shr    eax, 1
        jne    loc_4_0172
        xor    ecx, ecx
        stc    
        ret    
loc_4_01B4:
        shl    ecx, 0xc
        clc    
        ret    
sub_4_01B9:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> loc_1_011C
        mov    ax, word ptr [ebp + 0x2c]
        mov    word ptr [0], ax              ; -> loc_1_0120
        mov    ax, word ptr [ebp + 0xc]
        mov    word ptr [0], ax              ; -> loc_1_0130
        mov    ax, word ptr [ebp + 0x14]
        mov    word ptr [0], ax              ; -> loc_1_0122
        mov    ax, word ptr [ebp + 0x18]
        mov    word ptr [0], ax              ; -> obj1:0x015A
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [0], ax              ; -> obj1:0x0158
        mov    ax, word ptr [ebp + 0x1c]
        mov    word ptr [0], ax              ; -> loc_1_0132
        mov    ax, word ptr [ebp + 0x20]
        mov    word ptr [0], ax              ; -> obj1:0x0134
        mov    ax, word ptr [ebp + 0x24]
        mov    word ptr [0], ax              ; -> obj1:0x0136
        mov    word ptr [0], 0               ; -> obj1:0x0160
        call   sub_4_03E7
        jb     loc_4_0268
        movzx  eax, word ptr [ebp + 0x20]
        mov    esi, 0                        ; -> loc_4_015C
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        mov    dword ptr [0], eax            ; -> loc_1_0128
        jb     loc_4_0268
        movzx  eax, word ptr [ebp + 0x24]
        cmp    ax, word ptr [ebp + 0x20]
        je     loc_4_025A
        mov    esi, 0                        ; -> loc_4_015C
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        mov    dword ptr [0], eax            ; -> loc_1_012C
        jb     loc_4_0268
loc_4_025A:
        mov    dword ptr [esp + 0x1c], 0xffffffff
loc_4_0262:
        popal  
        add    esp, 4
        pop    ebp
        ret    
loc_4_0268:
        call   sub_4_0277
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_4_0262
sub_4_0277:
        pushal 
        mov    eax, dword ptr [0]            ; -> loc_1_0128
        or     eax, eax
        je     loc_4_0287
        VxDCall  VDMAD.VDMAD_Unvirtualize_Channel ; cd201a000400
loc_4_0287:
        mov    eax, dword ptr [0]            ; -> loc_1_012C
        or     eax, eax
        je     loc_4_0296
        VxDCall  VDMAD.VDMAD_Unvirtualize_Channel ; cd201a000400
loc_4_0296:
        movzx  eax, word ptr [0]             ; -> obj1:0x0156
        or     eax, eax
        je     loc_4_02BE
        push   0
        push   eax
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj1:0x0150
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_4_02BE:
        mov    edi, 0                        ; -> obj6:0x0058
        VMMCall  VMM.Remove_Mult_IO_Handlers     ; cd2017010100
        mov    edi, 0                        ; -> loc_1_00E0
        VMMCall  VMM.Remove_Mult_IO_Handlers     ; cd2017010100
        cmp    word ptr [0], 0               ; -> obj1:0x0158
        je     loc_4_02F8
        mov    edi, 0                        ; -> loc_1_00FA
        VMMCall  VMM.Remove_Mult_IO_Handlers     ; cd2017010100
        mov    eax, dword ptr [0]            ; -> loc_1_0114
        or     eax, eax
        je     loc_4_02F8
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
loc_4_02F8:
        mov    eax, dword ptr [0]            ; -> obj1:0x013A
        or     eax, eax
        je     loc_4_0307
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
loc_4_0307:
        popal  
        ret    
sub_4_0309:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [esp + 0x1c], 0x13
        mov    word ptr [0], 0               ; -> loc_1_0126
        push   dword ptr [ebp + 0x2c]
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_01B9
        add    esp, 0x28
        or     eax, eax
        je     loc_4_0370
        mov    word ptr [0], 0xffff          ; -> loc_1_0126
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [0], 0              ; -> loc_1_0108
        mov    dword ptr [0], 0              ; -> loc_1_010C
loc_4_0370:
        popal  
        add    esp, 4
        pop    ebp
        ret    
        db  0C7h, 044h, 024h, 01Ch, 024h, 000h, 000h, 000h, 0EBh, 0F0h
sub_4_0380:
        push   ebp
        mov    ebp, esp
        push   edi
        pushfd 
        cmp    word ptr [0], -1              ; -> loc_1_0126
        jne    loc_4_03B2
        cmp    dword ptr [0], 0              ; -> obj1:0x013E
        je     loc_4_03A8
        mov    ebx, dword ptr [0]            ; -> obj1:0x013E
        mov    eax, 2
        call   sub_1_03E4
loc_4_03A8:
        call   sub_4_0277
        mov    eax, 0xffffffff
loc_4_03B2:
        popfd  
        pop    edi
        pop    ebp
        ret    
loc_4_03B6:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_0000
loc_4_03CD:
        add    esp, 0x14
        leave  
        clc    
        ret    
loc_4_03D3:
        mov    eax, ebx
        mov    ebx, 0                        ; -> loc_4_03B6
        VxDCall  VFAT.Service_0000h              ; cd2000004a04
        mov    eax, 0
        stc    
        ret    
sub_4_03E7:
        mov    ax, word ptr [0]              ; -> loc_1_0130
        and    ax, 0x70
        mov    bx, word ptr [0]              ; -> obj1:0x015A
        and    bx, 0x30
        shr    bx, 4
        or     bx, ax
        mov    dx, 0x201
        mov    ax, 0xaf
        out    dx, al
        jmp    loc_4_040E
loc_4_040E:
        mov    ax, 0x50
        out    dx, al
        jmp    loc_4_0415
loc_4_0415:
        mov    ax, bx
        out    dx, al
        call   0
        jb     loc_4_055F
        call   sub_3_017C
loc_4_0429:
        cmp    word ptr [0], 0               ; -> obj1:0x015A
        je     loc_4_0438
        call   sub_3_036C
loc_4_0438:
        movzx  ecx, word ptr [0]             ; -> obj1:0x0154
        mov    eax, ecx
        sub    eax, 1
        push   0xf
        push   0                             ; -> obj1:0x014C
        push   0xfff
        push   0x110
        push   eax
        push   0
        push   1
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        mov    ecx, eax
        or     ecx, edx
        je     loc_4_04B1
        mov    dword ptr [0], eax            ; -> obj1:0x0150
        movzx  ecx, word ptr [0]             ; -> obj1:0x0154
        shl    ecx, 0xc
        mov    dword ptr [0], ecx            ; -> obj1:0x0166
        mov    dword ptr [0], edx            ; -> obj1:0x0162
        sub    ecx, 1
        push   0
        push   0
        push   0xf2
        push   ecx
        push   edx
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        mov    word ptr [0], ax              ; -> obj1:0x0156
        jmp    loc_4_04B1
loc_4_04B1:
        mov    ax, word ptr [0]              ; -> obj1:0x0148
        and    ax, 0xf0
        cmp    ax, 0x10
        je     loc_4_04EA
        mov    ax, word ptr [0]              ; -> loc_1_0132
        cmp    al, 7
        jle    loc_4_04D2
        mov    al, 0xff
        jmp    loc_4_055F
loc_4_04D2:
        mov    ax, word ptr [0]              ; -> obj1:0x0134
        cmp    ax, 3
        jle    loc_4_04E2
        mov    al, 0xff
        jmp    loc_4_055F
loc_4_04E2:
        mov    word ptr [0], ax              ; -> obj1:0x0136
        jmp    loc_4_055D
loc_4_04EA:
        xor    ebx, ebx
        mov    bx, word ptr [0]              ; -> obj1:0x0158
        mov    al, byte ptr [ebx]            ; -> obj6:0x0040
        mov    cx, 4
        shl    al, cl
        xor    ebx, ebx
        mov    bx, word ptr [0]              ; -> loc_1_0132
        or     al, byte ptr [ebx]            ; -> obj6:0x0040
        mov    bx, ax
        push   eax
        xor    ebx, ebx
        mov    bx, word ptr [0]              ; -> obj1:0x0136
        and    bx, 7
        mov    al, byte ptr [ebx]            ; -> obj6:0x0050
        xor    ebx, ebx
        mov    bx, word ptr [0]              ; -> obj1:0x0134
        and    bx, 7
        mov    bl, byte ptr [ebx]            ; -> obj6:0x0050
        mov    cx, 4
        shl    ax, cl
        or     bl, al
        mov    al, 0xfb
        mov    dx, word ptr [0]              ; -> loc_1_0130
        call   sub_1_0291
loc_4_054F:
        mov    ax, bx
        call   sub_1_0291
loc_4_0557:
        pop    eax
        call   sub_1_0291
loc_4_055D:
        clc    
        ret    
loc_4_055F:
        stc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0xf49  R-X
sub_5_0000:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x228
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     loc_5_0335
        cmp    eax, 1
        je     loc_5_0042
        cmp    eax, 2
        je     loc_5_02D6
        cmp    eax, 3
        je     loc_5_02E3
        cmp    eax, 4
        je     loc_5_0320
        mov    eax, 1
        jmp    loc_5_0337
loc_5_0042:
        cmp    dword ptr [ebp + 0xc], 1
        jbe    loc_5_0052
        mov    eax, 1
        jmp    loc_5_0337
loc_5_0052:
        push   0
        lea    eax, [ebp - 0x228]
        push   0x100
        mov    esi, dword ptr [ebp + 0x10]
        push   eax
        push   0
        push   esi
        call   sub_5_0E68
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_0337
        push   0
        lea    eax, [ebp - 0x128]
        push   esi
        push   eax
        call   sub_5_0E60
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_0337
        mov    bx, 0x388
        movzx  eax, word ptr [ebp - 0xcc]
        xor    di, di
        cmp    eax, 1
        je     loc_5_00C5
        cmp    eax, 2
        je     loc_5_00CF
        cmp    eax, 3
        je     loc_5_00D9
        cmp    eax, 4
        je     loc_5_00F9
        cmp    eax, 5
        je     loc_5_0141
        mov    eax, 0x13
        jmp    loc_5_0337
loc_5_00C5:
        mov    eax, 0x13
        jmp    loc_5_0337
loc_5_00CF:
        mov    eax, 0x13
        jmp    loc_5_0337
loc_5_00D9:
        cmp    word ptr [ebp - 0x9e], 0x10
        je     loc_5_00ED
        mov    eax, 0x13
        jmp    loc_5_0337
loc_5_00ED:
        mov    cx, word ptr [ebp - 0xc6]
        jmp    loc_5_0180
loc_5_00F9:
        cmp    word ptr [ebp - 0x9e], 0x10
        je     loc_5_010D
        mov    eax, 0x13
        jmp    loc_5_0337
loc_5_010D:
        movzx  eax, word ptr [ebp - 0x9c]
        mov    cx, word ptr [ebp - 0xc6]
        cmp    eax, 2
        je     loc_5_012F
        cmp    eax, 4
        je     loc_5_0138
        mov    eax, 0x13
        jmp    loc_5_0337
loc_5_012F:
        mov    di, word ptr [ebp - 0xc4]
        jmp    loc_5_0180
loc_5_0138:
        mov    bx, word ptr [ebp - 0xc4]
        jmp    loc_5_0180
loc_5_0141:
        cmp    word ptr [ebp - 0x9e], 0x10
        jne    loc_5_02CF
        cmp    word ptr [ebp - 0x9c], 4
        jne    loc_5_02CF
        cmp    word ptr [ebp - 0x9a], 2
        jne    loc_5_02CF
        mov    cx, word ptr [ebp - 0xc6]
        mov    bx, word ptr [ebp - 0xc4]
        mov    di, word ptr [ebp - 0xc2]
loc_5_0180:
        cmp    word ptr [ebp - 0x7a], 1
        jb     loc_5_02C8
        cmp    word ptr [ebp - 0x6a], 2
        jb     loc_5_02C8
        movzx  ax, byte ptr [ebp - 0x78]
        movzx  dx, byte ptr [ebp - 0x68]
        mov    word ptr [ebp - 6], ax
        cmp    word ptr [ebp - 0x6a], 1
        movzx  ax, byte ptr [ebp - 0x77]
        jbe    loc_5_01BF
        mov    word ptr [ebp - 4], dx
        movzx  dx, byte ptr [ebp - 0x67]
        mov    word ptr [ebp - 2], dx
        jmp    loc_5_01C7
loc_5_01BF:
        mov    word ptr [ebp - 2], dx
        mov    word ptr [ebp - 4], dx
loc_5_01C7:
        movzx  edx, cx
        mov    dword ptr [ebp - 0x10], edx
        cmp    edx, -1
        jne    loc_5_01DC
        mov    eax, 0x13
        jmp    loc_5_0337
loc_5_01DC:
        push   0
        push   0x10
        push   dword ptr [ebp - 2]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 6]
        push   eax
        push   edi
        push   ebx
        push   ecx
        push   esi
        call   sub_4_0309
loc_5_01F3:
        add    esp, 0x28
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        jne    loc_5_02B4
        sub    bx, 0xffff
        mov    dword ptr [ebp - 0x28], esi
        mov    dword ptr [ebp - 0x2c], 0     ; -> obj6:0x0030
        cmp    bx, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 0x30], eax
        push   dword ptr [ebp - 0x30]
        push   dword ptr [ebp - 0x2c]
        push   dword ptr [ebp - 0x28]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0001h              ; cd2001004a04
        add    esp, 0xc
        sub    di, 0xffff
        mov    dword ptr [ebp - 0x1c], esi
        mov    dword ptr [ebp - 0x20], 0     ; -> obj6:0x001C
        cmp    di, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 0x24], eax
        push   dword ptr [ebp - 0x24]
        push   dword ptr [ebp - 0x20]
        push   dword ptr [ebp - 0x1c]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0001h              ; cd2001004a04
        add    esp, 0xc
        cmp    word ptr [ebp - 6], 9
        mov    ecx, 2
        je     loc_5_0273
        movzx  ecx, word ptr [ebp - 6]
loc_5_0273:
        movzx  eax, word ptr [ebp - 2]
        movzx  edx, word ptr [ebp - 4]
        push   eax
        push   edx
        push   ecx
        lea    edx, [ebp - 0x50]
        push   dword ptr [ebp - 0x10]
        push   0                             ; -> obj6:0x0008
        push   edx
        call   sub_5_0E58
        add    esp, 0x18
        lea    ecx, [ebp - 0x50]
        mov    dword ptr [ebp - 0x14], 0     ; -> obj6:0x0000
        mov    dword ptr [ebp - 0x18], ecx
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0002h              ; cd2002004a04
        add    esp, 8
loc_5_02B4:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_5_02C3
        push   esi
        call   sub_5_0908
        add    esp, 4
loc_5_02C3:
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_5_0337
loc_5_02C8:
        mov    eax, 0x13
        jmp    loc_5_0337
loc_5_02CF:
        mov    eax, 0x13
        jmp    loc_5_0337
loc_5_02D6:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_5_0320
        mov    eax, 1
        jmp    loc_5_0337
loc_5_02E3:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_02F6
        cmp    eax, 1
        je     loc_5_030B
        mov    eax, 1
        jmp    loc_5_0337
loc_5_02F6:
        call   sub_4_00D5
loc_5_02FB:
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_5_0337
loc_5_030B:
        call   sub_4_00D5
loc_5_0310:
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_5_0337
loc_5_0320:
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        call   sub_5_09B7
        add    esp, 4
        push   esi
        call   sub_4_0380
loc_5_0332:
        add    esp, 4
loc_5_0335:
        xor    eax, eax
loc_5_0337:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh, 0CCh
sub_5_0340:
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083800100
sub_5_0346:
        VMMCall  VMM.End_Nest_Exec               ; cd2086800100
sub_5_034C:
        VMMCall  VMM.Resume_Exec                 ; cd2085800100
sub_5_0352:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        pop    ebp
        ret    
sub_5_0362:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        pop    ebp
        ret    
sub_5_0372:
        movzx  ecx, word ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048800100
sub_5_0381:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        pop    ebp
        ret    
sub_5_038F:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        pop    ebx
        ret    
sub_5_039A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x6c
        mov    eax, 0xffffffff
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     loc_5_0484
        lea    eax, [ebp - 0x6c]
        push   eax
        call   sub_5_0352
        add    esp, 4
        call   sub_5_0340
        call   sub_5_038F
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_0381
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0381
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x10]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_0381
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0381
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x14]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_0381
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0381
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x18]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_0381
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0381
        movzx  ecx, si
        shr    esi, 0x10
        add    esp, 4
        movzx  eax, si
        push   ecx
        push   eax
        call   sub_5_0372
        add    esp, 8
        call   sub_5_034C
        movzx  esi, word ptr [edi + 0x14]
        shl    esi, 0x10
        movzx  eax, word ptr [edi + 0x1c]
        or     esi, eax
        call   sub_5_0346
        lea    ecx, [ebp - 0x6c]
        push   ecx
        call   sub_5_0362
        add    esp, 4
        mov    eax, esi
loc_5_0484:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_048B:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi]
        test   ebp, ebp
        jne    loc_5_04B1
        push   0x15
        push   4
        call   sub_5_0EB4
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_5_04AF
        xor    eax, eax
        jmp    loc_5_0616
loc_5_04AF:
        mov    dword ptr [esi], ebp
loc_5_04B1:
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 8
        jbe    loc_5_04CE
        mov    eax, dword ptr [esp + 0x1c]
        mov    byte ptr [eax + 8], 0
loc_5_04CE:
        push   ebp
        call   sub_5_0EA4
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_05C2
loc_5_04DE:
        mov    ecx, dword ptr [esp + 0x1c]
        lea    edx, [ebx + 0xc]
loc_5_04E5:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    loc_5_0505
        or     al, al
        je     loc_5_0501
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    loc_5_0505
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    loc_5_04E5
loc_5_0501:
        xor    eax, eax
        jmp    loc_5_050A
loc_5_0505:
        sbb    eax, eax
        sbb    eax, -1
loc_5_050A:
        test   eax, eax
        je     loc_5_051B
        push   ebx
        push   ebp
        call   sub_5_0E94
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_04DE
loc_5_051B:
        test   ebx, ebx
        je     loc_5_05C2
        cmp    dword ptr [ebx + 4], 0
        je     loc_5_0530
        xor    eax, eax
        jmp    loc_5_0616
loc_5_0530:
        push   0
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_039A
        add    esp, 0x14
        test   eax, eax
        je     loc_5_055D
        push   ebx
        mov    dword ptr [ebx], 0
        push   esi
        call   sub_5_061B
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_0616
loc_5_055D:
        mov    eax, dword ptr [esp + 0x20]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   ecx
        call   sub_5_039A
        add    esp, 0x14
        test   eax, eax
        je     loc_5_0594
        mov    dword ptr [ebx + 4], 0
        push   ebx
        push   esi
        call   sub_5_061B
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_0616
loc_5_0594:
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   0x10000
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_039A
        add    esp, 0x14
        push   dword ptr [ebx]
        push   dword ptr [ebx + 8]
        push   0x10000
        push   ebx
        push   dword ptr [ebx + 4]
        call   sub_5_039A
        add    esp, 0x14
        jmp    loc_5_0614
loc_5_05C2:
        push   ebp
        call   sub_5_0E80
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_05D2
        xor    eax, eax
        jmp    loc_5_0616
loc_5_05D2:
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x20]
        mov    edi, dword ptr [esp + 0x1c]
        mov    dword ptr [ebx + 8], eax
        mov    edx, dword ptr [ecx + 4]
        sub    eax, eax
        mov    ecx, 0xffffffff
        mov    dword ptr [ebx], edx
        mov    dword ptr [ebx + 4], 0
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [ebx + 0xc]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        push   ebx
        and    ecx, 3
        push   ebp
        rep movsb byte ptr es:[edi], byte ptr [esi]
        call   sub_5_0E70
loc_5_0614:
        mov    eax, ebx
loc_5_0616:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_061B:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        test   edi, edi
        je     loc_5_0667
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     loc_5_0645
        push   0
        push   dword ptr [edi + 8]
        push   0x40000
        push   edi
        push   eax
        call   sub_5_039A
        add    esp, 0x14
        mov    dword ptr [edi], 0
loc_5_0645:
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        je     loc_5_0667
        push   0
        push   dword ptr [edi + 8]
        push   0x40000
        push   edi
        push   eax
        call   sub_5_039A
        mov    dword ptr [edi + 4], 0
        add    esp, 0x14
loc_5_0667:
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_5_0698
        push   edi
        push   eax
        call   sub_5_0EE8
        push   edi
        push   dword ptr [esi]
        call   sub_5_0ED8
        push   dword ptr [esi]
        call   sub_5_0EA4
        test   eax, eax
        jne    loc_5_0698
        push   dword ptr [esi]
        call   sub_5_0EC8
        mov    dword ptr [esi], 0
loc_5_0698:
        pop    edi
        pop    esi
        ret    
loc_5_069B:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0040
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_06D0
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_06DE
loc_5_06D0:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_06DE:
        pop    edi
        pop    esi
        ret    0xc
loc_5_06E3:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        inc    eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
loc_5_06F1:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        dec    eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
loc_5_06FF:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        lea    esi, [edi + 0x104]
        mov    dword ptr [edi], 0xb
        call   sub_5_0C74
        mov    ebx, eax
        push   0xff
        push   0                             ; -> obj6:0x00F4
        push   esi
        call   sub_5_0EFC
        add    esp, 0xc
        lea    eax, [edi + 4]
        push   0xff
        push   ebx
        push   eax
        call   sub_5_0EFC
        mov    eax, dword ptr [esp + 0x1c]
        add    esp, 0xc
        xor    ecx, ecx
        mov    dword ptr [edi + 0x210], ecx
        mov    edx, dword ptr [eax + 4]
        mov    eax, 0xb3
        mov    dword ptr [edi + 0x204], edx
        mov    dword ptr [edi + 0x20c], ecx
        mov    word ptr [edi + 0x208], 0x3202
        cmp    byte ptr [esi], cl
        je     loc_5_0776
loc_5_076B:
        movsx  ecx, byte ptr [esi]
        add    eax, ecx
        inc    esi
        cmp    byte ptr [esi], 0
        jne    loc_5_076B
loc_5_0776:
        mov    ecx, 0xf7
        sub    edx, edx
        div    ecx
        mov    word ptr [edi + 0x20a], dx
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_5_078E:
        push   esi
        push   edi
        mov    esi, 0x8878000a
        mov    edi, dword ptr [esp + 0xc]
        cmp    dword ptr [edi + 0xc], 0
        jne    loc_5_07B3
        push   dword ptr [edi + 4]
        call   sub_5_0D42
        add    esp, 4
        test   eax, eax
        jne    loc_5_07B3
        xor    esi, esi
        inc    dword ptr [edi + 0xc]
loc_5_07B3:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    4
loc_5_07BA:
        mov    eax, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    dword ptr [eax], 0xf
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 0x10], ecx
        mov    dword ptr [eax + 0x14], ecx
        mov    dword ptr [eax + 0x18], ecx
        mov    dword ptr [eax + 0x1c], ecx
        mov    dword ptr [eax + 0x20], ecx
        mov    dword ptr [eax + 0x24], ecx
        mov    dword ptr [eax + 0x28], ecx
        mov    dword ptr [eax + 0x2c], ecx
        mov    dword ptr [eax + 0x30], ecx
        mov    dword ptr [eax + 0x34], ecx
        mov    dword ptr [eax + 0x38], ecx
        mov    dword ptr [eax + 0x3c], ecx
        mov    dword ptr [eax + 0x40], ecx
        mov    dword ptr [eax + 0x44], ecx
        mov    dword ptr [eax + 0x48], ecx
        mov    dword ptr [eax + 0xc], 1
        xor    eax, eax
        ret    8
loc_5_0805:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 0x8007000e
        mov    esi, dword ptr [esp + 0x2c]
        test   byte ptr [esp + 0x1c], 0xf0
        mov    dword ptr [esi], ebx
        je     loc_5_0827
        mov    eax, 0x8878001e
        jmp    loc_5_08C7
loc_5_0827:
        mov    ebp, dword ptr [esp + 0x14]
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_5_08A6
        push   0x201
        push   0x31
        call   sub_5_0F0C
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_089F
        mov    dword ptr [ebx], 0            ; -> obj6:0x0110
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_0D77
        add    esp, 8
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, 0x1000
        jbe    loc_5_08AB
        cmp    eax, 0xffff
        jae    loc_5_08AB
        cmp    dword ptr [ebx + 0xc], 0
        je     loc_5_08AB
        cmp    dword ptr [ebx + 0x10], 0
        je     loc_5_08AB
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x2c]
        push   ebx
        xor    edi, edi
        call   sub_5_0A1F
        mov    eax, dword ptr [esp + 0x24]
        add    esp, 0xc
        movzx  ecx, word ptr [eax + 0xe]
        mov    dword ptr [ebx + 0x24], ecx
        movzx  edx, word ptr [eax + 2]
        mov    dword ptr [ebx + 0x28], edx
        jmp    loc_5_08AB
loc_5_089F:
        mov    edi, 0x8007000e
        jmp    loc_5_08AB
loc_5_08A6:
        mov    edi, 0x8878000a
loc_5_08AB:
        test   edi, edi
        je     loc_5_08C0
        test   ebx, ebx
        je     loc_5_08C5
        push   0
        push   ebx
        call   sub_5_0F04
        add    esp, 8
        jmp    loc_5_08C5
loc_5_08C0:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 0x10]
loc_5_08C5:
        mov    eax, edi
loc_5_08C7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0x1c
loc_5_08CE:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0
        mov    eax, 0x80004001
        ret    0xc
loc_5_08E0:
        mov    eax, 0x80004005
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0xc]
        dec    edx
        mov    dword ptr [ecx + 0xc], edx
        jne    loc_5_0905
        cmp    dword ptr [ecx + 0x10], 0
        jne    loc_5_0905
        push   dword ptr [ecx + 4]
        call   sub_5_0D38
        add    esp, 4
        xor    eax, eax
loc_5_0905:
        ret    4
sub_5_0908:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   0x201
        mov    dword ptr [ebp - 4], ebx
        push   0x14
        call   sub_5_0F0C
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     loc_5_0987
        mov    edi, dword ptr [ebp + 8]
        push   1
        push   0                             ; -> obj6:0x0100
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 0x14]
        push   ecx
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [esi], 0            ; -> obj6:0x00D0
        call   sub_5_0F28
        test   eax, eax
        jne    loc_5_0987
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], ebx
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [ebp - 0xc], esi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        VxDCall  Dev_357E.Service_0001h          ; cd2001007e35
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], ebx
        jne    loc_5_0987
        push   esi
        mov    ebx, 1
        push   edi
        call   sub_5_0E4A
        add    esp, 8
loc_5_0987:
        test   ebx, ebx
        jne    loc_5_09B0
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_09A1
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_0F14
loc_5_09A1:
        test   esi, esi
        je     loc_5_09B0
        push   0
        push   esi
        call   sub_5_0F04
        add    esp, 8
loc_5_09B0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_09B7:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_5_0E40
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     loc_5_0A19
        push   0
        push   edi
        call   sub_5_0E4A
        add    esp, 8
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 8], 0
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        VxDCall  Dev_357E.Service_0002h          ; cd2002007e35
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_0F14
        push   0
        push   esi
        call   sub_5_0F04
        add    esp, 8
loc_5_0A19:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_0A1F:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x14]
        cmp    dword ptr [edx], eax
        jbe    loc_5_0A31
        mov    dword ptr [edx], eax
loc_5_0A31:
        mov    eax, dword ptr [esp + 0xc]
        and    dword ptr [edx], 0xfffc
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax], ecx
        mov    edx, dword ptr [edx]
        mov    dword ptr [esi + 0x1c], edx
        inc    dword ptr [esi + 0x20]
        mov    dword ptr [esi + 8], 0
        pop    esi
        ret    
loc_5_0A51:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 0x20]
        dec    esi
        mov    dword ptr [eax + 0x20], esi
        jne    loc_5_0A76
        cmp    dword ptr [eax + 8], 0
        jne    loc_5_0A76
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 0x10]
        call   sub_5_0F04
        add    esp, 8
loc_5_0A76:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_0A7C:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0050
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_0AB1
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_0ABF
loc_5_0AB1:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_0ABF:
        pop    edi
        pop    esi
        ret    0xc
loc_5_0AC4:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x20]
        inc    eax
        mov    dword ptr [ecx + 0x20], eax
        ret    4
loc_5_0AD2:
        mov    eax, 0x80004001
        ret    0x20
loc_5_0ADA:
        mov    eax, 0x80004001
        ret    0x14
loc_5_0AE2:
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 4]
        movzx  ecx, word ptr [eax + 0xe]
        mov    dword ptr [edx + 0x24], ecx
        movzx  eax, word ptr [eax + 2]
        mov    dword ptr [edx + 0x28], eax
        mov    eax, 0x80004001
        ret    8
loc_5_0B00:
        mov    eax, 0x80004001
        ret    8
loc_5_0B08:
        mov    eax, 0x80004001
        ret    8
loc_5_0B10:
        mov    eax, 0x80004001
        ret    8
loc_5_0B18:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    loc_5_0B5A
        cmp    dword ptr [esi + 0x24], 0x10
        jne    loc_5_0B30
        mov    eax, dword ptr [0]            ; -> obj1:0x016E
        jmp    loc_5_0B35
loc_5_0B30:
        mov    eax, dword ptr [0]            ; -> obj1:0x016A
loc_5_0B35:
        push   eax
        call   sub_5_0C82
        add    esp, 4
        inc    eax
        cmp    dword ptr [esi + 0x24], 0x10
        jne    loc_5_0B47
        add    eax, eax
loc_5_0B47:
        mov    ecx, dword ptr [esi + 0x1c]
        mov    edx, dword ptr [esp + 0x10]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    loc_5_0B6E
loc_5_0B5A:
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_0B6E:
        xor    eax, eax
        pop    esi
        ret    0xc
loc_5_0B74:
        push   ebx
        mov    eax, 0x80004005
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        cmp    dword ptr [edi + 8], 0
        jne    loc_5_0C10
        cmp    dword ptr [edi + 0x24], 0x10
        movzx  esi, word ptr [edi + 0x2e]
        jne    loc_5_0B9C
        mov    ebx, dword ptr [0]            ; -> obj1:0x016E
        jmp    loc_5_0BA2
loc_5_0B9C:
        mov    ebx, dword ptr [0]            ; -> obj1:0x016A
loc_5_0BA2:
        push   ebx
        call   sub_5_0C9D
        add    esp, 4
        push   dword ptr [edi + 0x1c]
        push   dword ptr [edi + 0xc]
        push   ebx
        call   sub_5_0CC1
        add    esp, 0xc
        push   ebx
        call   sub_5_0CE2
        add    esp, 4
        push   ebx
        call   sub_5_0CAB
        mov    bx, word ptr [edi + 0x1c]
        add    esp, 4
        cmp    dword ptr [edi + 0x24], 0x10
        jne    loc_5_0BD9
        shr    bx, 1
loc_5_0BD9:
        cmp    dword ptr [edi + 0x28], 2
        jne    loc_5_0BE2
        shr    bx, 1
loc_5_0BE2:
        dec    bx
        push   0xffff
        push   0xffff
        push   esi
        call   sub_5_0E07
        add    esp, 0xc
        movzx  ecx, bx
        push   dword ptr [edi + 0x24]
        push   ecx
        push   esi
        call   sub_5_0DCC
        add    esp, 0xc
        xor    eax, eax
        mov    dword ptr [edi + 8], 1
loc_5_0C10:
        pop    edi
        pop    esi
        pop    ebx
        ret    0x10
loc_5_0C16:
        push   esi
        mov    eax, 0x80004005
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        cmp    dword ptr [edi + 8], 0
        je     loc_5_0C6F
        movzx  esi, word ptr [edi + 0x2e]
        push   0xd0
        push   0
        push   esi
        call   sub_5_0D06
        add    esp, 0xc
        push   0xda
        push   0
        push   esi
        call   sub_5_0D06
        add    esp, 0xc
        cmp    dword ptr [edi + 0x24], 0x10
        jne    loc_5_0C58
        mov    eax, dword ptr [0]            ; -> obj1:0x016E
        jmp    loc_5_0C5D
loc_5_0C58:
        mov    eax, dword ptr [0]            ; -> obj1:0x016A
loc_5_0C5D:
        push   eax
        call   sub_5_0C9D
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [edi + 8], 0
loc_5_0C6F:
        pop    edi
        pop    esi
        ret    4
sub_5_0C74:
        movzx  eax, word ptr [0xe]           ; -> loc_2_000E
        add    eax, dword ptr [4]            ; -> obj2:0x0004
        ret    
sub_5_0C82:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VDMAD.VDMAD_Get_Phys_Count      ; cd201c000400
        movzx  eax, cx
        pop    ecx
        pop    ebx
        pop    ebp
        ret    
sub_5_0C9D:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VDMAD.VDMAD_Phys_Mask_Channel   ; cd2018000400
        pop    ebp
        ret    
sub_5_0CAB:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VDMAD.VDMAD_Phys_Unmask_Channel ; cd2019000400
        pop    ebx
        pop    ebp
        ret    
sub_5_0CC1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        xor    bx, bx
        mov    bh, 1
        xor    esi, esi
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        VxDCall  VDMAD.VDMAD_Set_Region_Info     ; cd2003000400
        pop    ecx
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_5_0CE2:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edx
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xor    dx, dx
        or     dl, 8
        or     dl, 0x10
        or     dl, 0x40
        VxDCall  VDMAD.VDMAD_Set_Phys_State      ; cd2006000400
        pop    edx
        pop    ebx
        pop    ebp
        ret    
sub_5_0D06:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0xc]
        mov    al, byte ptr [ebp + 0x10]
        call   sub_1_0291
loc_5_0D18:
        xor    eax, eax
        jae    loc_5_0D21
        mov    eax, 1
loc_5_0D21:
        pop    edx
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0xc]
        call   sub_1_0273
loc_5_0D33:
        xor    ah, ah
        pop    edx
        pop    ebp
        ret    
sub_5_0D38:
        mov    word ptr [0], 0               ; -> obj1:0x0160
        ret    
sub_5_0D42:
        push   ebp
        mov    ebp, esp
        push   edi
        push   edx
        push   ebx
        mov    edx, 0xffff
        mov    eax, dword ptr [0]            ; -> obj1:0x013E
        or     eax, eax
        je     loc_5_0D62
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        cmp    eax, ebx
        je     loc_5_0D62
        jmp    loc_5_0D70
loc_5_0D62:
        mov    word ptr [0], 1               ; -> obj1:0x0160
        mov    edx, 0
loc_5_0D70:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret    
sub_5_0D77:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [0]              ; -> loc_1_0130
        mov    word ptr [esi + 0x2e], ax
        mov    ax, word ptr [0]              ; -> obj1:0x0134
        mov    byte ptr [esi + 0x30], al
        mov    eax, dword ptr [0]            ; -> obj1:0x014C
        mov    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x0162
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [0]            ; -> obj1:0x0166
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [0]            ; -> loc_1_0128
        mov    dword ptr [0], eax            ; -> obj1:0x016A
        mov    eax, dword ptr [0]            ; -> loc_1_012C
        mov    dword ptr [0], eax            ; -> obj1:0x016E
        mov    ax, word ptr [0]              ; -> loc_1_0120
        mov    word ptr [esi + 0x2c], ax
        pop    esi
        pop    edi
        pop    ebp
        ret    
sub_5_0DCC:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ecx
        push   edx
        pushfd 
        cli    
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0x48
        call   sub_1_0291
loc_5_0DDE:
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_0291
loc_5_0DE6:
        mov    al, ah
        call   sub_1_0291
loc_5_0DED:
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    ecx, 0x10
        jne    loc_5_0DF9
        mov    al, 0x1c
        jmp    loc_5_0DFB
loc_5_0DF9:
        mov    al, 0x1c
loc_5_0DFB:
        call   sub_1_0291
loc_5_0E00:
        popfd  
        clc    
        pop    edx
        pop    ecx
        pop    eax
        pop    ebp
        ret    
sub_5_0E07:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_5_0E3C
        mov    ebx, eax
        mov    eax, dword ptr [ebp + 0x10]
        call   sub_5_0E3C
        shl    bl, 4
        and    al, 0xf
        or     al, bl
        mov    ah, al
        mov    al, 4
        mov    edx, dword ptr [ebp + 8]
        add    dx, 4
        out    dx, al
        inc    dx
        mov    al, ah
        out    dx, al
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret    
sub_5_0E3C:
        shr    eax, 0xc
        ret    
sub_5_0E40:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [0]            ; -> obj1:0x015C
        pop    ebp
        ret    
sub_5_0E4A:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [0], eax            ; -> obj1:0x015C
        pop    ebp
        ret    
        db  0CCh
sub_5_0E58:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        int3   
        int3   
sub_5_0E60:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
        int3   
        int3   
sub_5_0E68:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d803300
        int3   
        int3   
sub_5_0E70:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_0E80:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Allocate               ; cd209d000100
        mov    esi, ecx
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  0CCh
sub_5_0E94:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_0EA4:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_0EB4:
        pop    edx
        pop    eax
        pop    ecx
        push   esi
        VMMCall  VMM.List_Create                 ; cd209b000100
        mov    eax, esi
        pop    esi
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
sub_5_0EC8:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Destroy                ; cd209c000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_0ED8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_0EE8:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
        db  0CCh, 0CCh
sub_5_0EFC:
        VMMCall  VMM._lstrcpyn                   ; cd207b810100
        int3   
        int3   
sub_5_0F04:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_5_0F0C:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_5_0F14:
        pop    ecx
        pop    eax
        pop    edx
        push   ebx
        mov    ebx, eax
        VxDCall  APM.Service_0002h               ; cd2002002700
        sbb    edx, edx
        and    eax, edx
        pop    ebx
        jmp    ecx
        db  0CCh
sub_5_0F28:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0xc]
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_5_0F46
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_0F46:
        ret    0x10
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x140  RW-
        db  042h, 04Ch, 041h, 053h, 054h, 045h, 052h, 000h, 041h, 025h, 030h, 033h, 058h, 020h, 049h, 025h
        db  064h, 020h, 044h, 025h, 064h, 020h, 048h, 025h, 064h, 000h, 000h, 000h, 06Dh, 069h, 064h, 069h
        db  05Ch, 06Dh, 073h, 06Dh, 070h, 075h, 034h, 030h, 031h, 02Eh, 064h, 072h, 076h, 000h, 000h, 000h
        db  06Dh, 069h, 064h, 069h, 05Ch, 06Dh, 073h, 06Fh, 070h, 06Ch, 02Eh, 064h, 072h, 076h, 000h, 000h
        db  000h, 000h, 002h, 003h, 000h, 001h, 000h, 004h, 000h, 002h, 005h, 000h, 000h, 000h, 000h, 006h
        db  000h, 001h, 000h, 002h, 000h, 003h, 000h, 004h, 008h, 000h, 006h, 000h
        dd  loc_1_04ED               ; fixup
        db  007h, 000h
        dd  loc_1_04ED               ; fixup
        db  00Ah, 000h
        dd  loc_1_04ED               ; fixup
        db  00Bh, 000h
        dd  loc_1_04ED               ; fixup
        db  00Ch, 000h
        dd  loc_1_04ED               ; fixup
        db  00Dh, 000h
        dd  loc_1_04ED               ; fixup
        db  00Eh, 000h
        dd  loc_1_04ED               ; fixup
        db  00Fh, 000h
        dd  loc_1_04ED               ; fixup
        db  007h, 000h, 000h, 000h
        dd  loc_1_0217               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_1_0245               ; fixup
        dd  loc_1_0251               ; fixup
        db  000h, 000h, 000h, 000h, 0F4h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_3_04C4               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_3_0504               ; fixup
        dd  loc_3_0514               ; fixup
        db  000h, 000h, 000h, 000h, 0F4h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h
        dd  loc_5_069B               ; fixup
        dd  loc_5_06E3               ; fixup
        dd  loc_5_06F1               ; fixup
        dd  loc_5_06FF               ; fixup
        dd  loc_5_078E               ; fixup
        dd  loc_5_08E0               ; fixup
        dd  loc_5_07BA               ; fixup
        dd  loc_5_0805               ; fixup
        dd  loc_5_08CE               ; fixup
        db  056h, 04Dh, 056h, 049h, 044h, 02Eh, 056h, 058h, 044h, 000h, 000h, 000h, 064h, 073h, 06Fh, 075h
        db  06Eh, 064h, 02Eh, 076h, 078h, 064h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_5_0A7C               ; fixup
        dd  loc_5_0AC4               ; fixup
        dd  loc_5_0A51               ; fixup
        dd  loc_5_0AD2               ; fixup
        dd  loc_5_0ADA               ; fixup
        dd  loc_5_0AE2               ; fixup
        dd  loc_5_0B00               ; fixup
        dd  loc_5_0B08               ; fixup
        dd  loc_5_0B10               ; fixup
        dd  loc_5_0B18               ; fixup
        dd  loc_5_0B74               ; fixup
        dd  loc_5_0C16               ; fixup
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0x7a  R-X
        db  06Ah, 061h, 07Ah, 07Ah, 02Eh, 064h, 072h, 076h, 000h, 06Ah, 061h, 07Ah, 07Ah, 034h, 030h, 031h
        db  02Eh, 064h, 072h, 076h, 000h, 064h, 069h, 073h, 061h, 062h, 06Ch, 065h, 077h, 061h, 072h, 06Eh
        db  069h, 06Eh, 067h, 000h, 070h, 06Fh, 072h, 074h, 000h, 069h, 06Eh, 074h, 000h, 064h, 06Dh, 061h
        db  063h, 068h, 061h, 06Eh, 06Eh, 065h, 06Ch, 000h, 064h, 06Dh, 061h, 063h, 068h, 061h, 06Eh, 06Eh
        db  065h, 06Ch, 032h, 000h, 064h, 06Dh, 061h, 070h, 061h, 067h, 065h, 073h, 000h, 0CCh, 0CCh, 0CCh
loc_7_0050:
        push   0
        push   4
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        or     eax, eax
        jne    loc_7_0063
        stc    
        ret    
loc_7_0063:
        mov    dword ptr [0], eax            ; -> loc_1_0118
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_4_00FC
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        clc    
        ret    
VxD__LTEXT ENDS
