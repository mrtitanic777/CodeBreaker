; VCDFSD -- reconstructed 32-bit disassembly (VxD / LE format)
; 6 objects, 572 instructions recovered by recursive descent

Declare_Virtual_Device VCDFSD, 3, 0, VCDFSD_Control, 00041h, Undefined_Init_Order
;   DDK version 0400h   init_order 016F00000h   flags 00000h
;   service table: obj1:0x5c, 4 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x19c  R-X
VCDFSD_Control:
        cmp    eax, 2
        jne    loc_1_000A
        call   sub_6_0000
loc_1_000A:
        push   eax
        mov    eax, dword ptr [0xcc]         ; -> loc_1_00CC
        mov    eax, dword ptr [eax + 0x10]
        xchg   dword ptr [esp], eax
        ret    
loc_1_0017:
        mov    eax, dword ptr [0xcc]         ; -> loc_1_00CC
        jmp    dword ptr [eax + 0xc]
loc_1_001F:
        mov    eax, 0xb
        mov    ecx, dword ptr [esp + 4]
        mov    word ptr [ecx + 0x1a], ax
        ret    
VCDFSD_Service_00:
        mov    eax, 0x400
loc_1_0032:
        clc    
        ret    
VCDFSD_Service_01:
        mov    eax, 0xbc                     ; -> obj1:0x00BC
        ret    
loc_1_003A:
        mov    eax, dword ptr [0xcc]         ; -> loc_1_00CC
        jmp    dword ptr [eax + 0x14]
loc_1_0042:
        cmp    byte ptr [ebp + 0x1d], 0x15
        je     loc_1_004A
loc_1_0048:
        stc    
        ret    
loc_1_004A:
        cmp    word ptr [0xc4], 0            ; -> obj1:0x00C4
        jne    loc_1_0048
        jmp    loc_4_0024
        db  000h, 000h, 000h
        dd  VCDFSD_Service_00        ; fixup
        dd  VCDFSD_Service_01        ; fixup
        dd  VCDFSD_Service_02        ; fixup
        dd  VCDFSD_Service_03        ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00041h                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VCDFSD  '                  ; DDB_Name
        dd  016F00000h                  ; DDB_Init_Order
        dd  VCDFSD_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x005C              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 004h, 000h, 0C0h, 000h, 000h, 000h, 022h, 000h
        db  000h, 000h
loc_1_00C6:
        pop    edi
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        db  000h
loc_1_00CC:
        jl     0xcf
        add    byte ptr [eax], al
loc_1_00D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_017C:
        jl     0x17f
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    al, byte ptr [eax]
        lea    eax, [eax]
        add    byte ptr [eax], al
        pop    ds
        add    byte ptr [eax], al
        add    byte ptr [eax + 2], ch        ; -> loc_3_0268
        add    byte ptr [eax], al
        inc    edx
        add    byte ptr [eax], al
        add    bh, cl                        ; -> loc_6_00CF
        add    byte ptr [eax], al
        add    byte ptr [ebp + 1], bl        ; -> loc_6_015D
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x108  R-X
        db  00Ah, 000h, 000h, 000h
loc_2_0004:
        and    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        adc    al, byte ptr [eax]
        inc    esi
        add    byte ptr [esi], bl
        jl     loc_2_0016
loc_2_0016:
        pushfd 
        add    byte ptr [esi - 0x31ff3c00], bh
        add    al, bl
        add    ah, ah
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x44], al
        push   edx
        xor    dword ptr [eax], esi
        xor    dword ptr [edx], edi
        and    byte ptr [esi + 0x6f], cl
        je     0x50
        jb     0x97
        popal  
        jns    loc_2_0036
loc_2_0036:
        inc    ebx
        inc    esp
        push   edx
        xor    dword ptr [eax], esi
        xor    edi, dword ptr [edx]
        and    byte ptr [eax + ebp*2 + 0x69], dl
        jae    0x63
        inc    ebx
        inc    esp
        push   edx
        dec    edi
        dec    ebp
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [edx + 0x65], dh
        arpl   word ptr [edi + 0x72], bp
        and    byte ptr fs:[ecx + 0x6e], ch
        and    byte ptr [ecx + 0x6e], ah
        and    byte ptr [ebp + 0x6e], dh
        imul   ebp, dword ptr [esi + 0x6f], 0x77
        outsb  dx, byte ptr [esi]
        and    byte ptr [esi + 0x6f], ah
        jb     loc_2_00D4
        popal  
        je     loc_2_006A
loc_2_006A:
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        push   eax
        jb     loc_2_00DB
        outsd  dx, dword ptr [esi]
        jb     loc_2_00E5
        popal  
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebp + 0x20], sp
        inc    esp
        popal  
        je     loc_2_00E2
        add    byte ptr [edx + 0x79], al
        je     0xeb
        jae    loc_2_00A8
        push   edx
        popal  
        and    byte ptr fs:[eax], ch
        inc    edx
        jns    loc_2_0105
        jae    loc_2_00B4
        push   eax
        jb     loc_2_00B8
        push   ebx
        arpl   word ptr gs:[edi + 0x6e], bp
        sub    dword ptr fs:[eax], eax
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        and    byte ptr [eax + 0x69], cl
loc_2_00A8:
        je     0x11d
        and    byte ptr [eax], ch
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        jae    loc_2_00D4
loc_2_00B4:
        push   eax
        jb     0xd8
loc_2_00B8:
        push   ebx
        arpl   word ptr gs:[edi + 0x6e], bp
        sub    dword ptr fs:[eax], eax
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        and    byte ptr [ebp + 0x69], cl
        jae    0x13d
        jae    0xed
        sub    byte ptr [ebx + 0x65], dl
        arpl   word ptr [edi + ebp*2 + 0x72], si
loc_2_00D4:
        jae    0xf6
        push   eax
        jb     0xfa
        push   ebx
loc_2_00DB:
        arpl   word ptr gs:[edi + 0x6e], bp
        sub    dword ptr fs:[eax], eax
loc_2_00E2:
        inc    ebx
        inc    esp
        push   edx
loc_2_00E5:
        dec    edi
        dec    ebp
        add    byte ptr [edx + 0x79], al
        je     0x151
        jae    0x140
        popal  
        add    byte ptr fs:[ebx + 0x61], al
        arpl   word ptr [eax + 0x65], bp
        dec    eax
        imul   esi, dword ptr [ebx + esi*2], 0x68636143
        dec    ebp
        db  069h, 073h, 073h
loc_2_0105:
        jae    0x108
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x27c  R-X
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 0C8h, 016h, 000h, 01Bh, 000h, 06Dh, 073h, 063h, 064h, 024h, 024h
        db  024h, 024h, 000h, 000h, 000h, 000h, 0EAh, 000h, 000h, 000h, 000h, 0EAh, 000h, 000h, 000h, 000h
loc_3_0020:
        mov    word ptr [0xc4], dx           ; -> obj1:0x00C4
        clc    
        ret    
loc_3_0029:
        push   ebx
        push   edi
        push   esi
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jb     loc_3_00C4
        cmp    eax, 0x22
        jne    loc_3_00D2
        VxDCall  BLOCKDEV.BlockDev_Get_Version   ; cd2000001000
        jb     loc_3_00C4
        call   sub_3_00F5
        jb     loc_3_00C4
        push   0x204
        push   0x20
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        lea    esp, [esp + 8]
        or     eax, eax
        je     loc_3_00C3
        shr    eax, 4
        shl    eax, 0x10
        mov    dword ptr [0xc8], eax         ; -> obj1:0x00C8
        shr    eax, 0xc
        call   sub_3_0225
        jb     loc_3_00C4
        mov    eax, 0x2f
        mov    esi, 0x3a                     ; -> loc_1_003A
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        jb     loc_3_00C4
        push   0
        push   0x22
        push   0x17                          ; -> loc_1_0017
        VxDCall  IFSMgr.IFSMgr_RegisterMount     ; cd2001004000
        lea    esp, [esp + 0xc]
        cmp    eax, -1
        je     loc_3_00A6
        jmp    loc_3_00B9
loc_3_00A6:
        mov    eax, 0x2f
        mov    esi, 0x3a                     ; -> loc_1_003A
        VMMCall  VMM.Unhook_V86_Int_Chain        ; cd2018010100
        stc    
        jmp    loc_3_00C4
loc_3_00B9:
        mov    word ptr [0xc0], 1            ; -> obj1:0x00C0
        stc    
loc_3_00C3:
        cmc    
loc_3_00C4:
        mov    dword ptr [0x18c], 0x32       ; -> obj1:0x018C
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_3_00D2:
        clc    
        jmp    loc_3_00C3
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_00F5:
        push   ebx
        push   edi
        push   esi
        cmp    word ptr [0xc4], 0x110        ; -> obj1:0x00C4
        jle    loc_3_0214
        push   0
        push   0x82
        VMMCall  VMM._Allocate_Temp_V86_Data_Area ; cd20a9000100
        add    esp, 8
        or     eax, eax
        je     loc_3_021E
        mov    dword ptr [0xd5], eax         ; -> obj3:0x00D5
        push   ebx
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp + 0x1c], 0x1500
        mov    word ptr [ebp + 0x10], 0
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [0xd9], ax           ; -> obj3:0x00D9
        cmp    ax, 0x1a
        ja     loc_3_01F8
        cmp    ax, 0
        je     loc_3_01F8
        mov    word ptr [ebp + 0x1c], 0x150d
        mov    eax, dword ptr [0xd5]         ; -> obj3:0x00D5
        shr    eax, 4
        mov    word ptr [ebp + 0x10], 0
        mov    word ptr [ebp + 0x38], ax
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  ecx, word ptr [0xd9]          ; -> obj3:0x00D9
        mov    esi, dword ptr [0xd5]         ; -> obj3:0x00D5
        mov    edi, 0xdb                     ; -> obj3:0x00DB
loc_3_01A4:
        lodsb  al, byte ptr [esi]
        cmp    al, 0x1a
        jae    loc_3_01F8
        stosb  byte ptr es:[edi], al
        loop   loc_3_01A4
        mov    word ptr [ebp + 0x1c], 0x1501
        mov    eax, dword ptr [0xd5]         ; -> obj3:0x00D5
        shr    eax, 4
        mov    word ptr [ebp + 0x10], 0
        mov    word ptr [ebp + 0x38], ax
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        xor    ecx, ecx
        mov    esi, dword ptr [0xd5]         ; -> obj3:0x00D5
loc_3_01D7:
        movzx  ebx, byte ptr [ecx + 0xdb]    ; -> obj3:0x00DB
        lea    edi, [ebx*4 + 0xd0]           ; -> loc_1_00D0
        lea    edi, [edi + ebx*2]
        lodsb  al, byte ptr [esi]
        mov    ah, 0x28
        stosw  word ptr es:[edi], ax
        movsd  dword ptr es:[edi], dword ptr [esi]
        inc    ecx
        cmp    cx, word ptr [0xd9]           ; -> obj3:0x00D9
        jb     loc_3_01D7
loc_3_01F8:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebx
        VMMCall  VMM._Free_Temp_V86_Data_Area    ; cd20aa000100
loc_3_0214:
        xor    eax, eax
loc_3_0216:
        cmp    eax, 1
        cmc    
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_3_021E:
        mov    eax, 8
        jmp    loc_3_0216
sub_3_0225:
        push   eax
        xor    edx, edx
        mov    esi, 0x14                     ; -> loc_4_0014
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        mov    dword ptr [0x17], eax         ; -> obj3:0x0017
        mov    edx, 1
        mov    esi, 0x1c                     ; -> loc_4_001C
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        mov    dword ptr [0x1c], eax         ; -> obj3:0x001C
        lea    esi, [0]                      ; -> obj3:0x0000
        pop    edi
        push   edi
        mov    ecx, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    eax
        push   eax
        xor    ebx, ebx
        VxDCall  DOSMGR.DOSMGR_Add_Device        ; cd2005001500
        pop    eax
        ret    
loc_3_0268:
        cmp    eax, 0
        je     loc_3_0020
        cmp    eax, 1
        je     loc_3_0029
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xb8  R-X
        db  08Dh, 044h, 024h, 004h, 050h, 06Ah, 002h, 0A1h
        dd  loc_1_00CC               ; fixup
        db  0FFh, 050h, 008h, 08Dh, 064h, 024h, 008h, 0C3h
loc_4_0014:
        mov    eax, dword ptr [0xcc]         ; -> loc_1_00CC
        jmp    dword ptr [eax + 0x18]
loc_4_001C:
        mov    eax, dword ptr [0xcc]         ; -> loc_1_00CC
        jmp    dword ptr [eax + 0x1c]
loc_4_0024:
        movzx  eax, byte ptr [ebp + 0x1c]
        cmp    eax, 0x11
        jae    loc_4_0034
        jmp    loc_4_003C
loc_4_0034:
        mov    word ptr [ebp + 0x1c], 1
        jmp    loc_4_00B2
loc_4_003C:
        add    byte ptr [eax], 0
        add    byte ptr [esi - 0x54000000], ah ; -> loc_4_00A6
        add    byte ptr [eax], al
        add    byte ptr [eax + eax + 0xac0000], ch ; -> loc_4_00AC
        add    byte ptr [eax], al
        lodsb  al, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax + eax + 0xac0000], ch ; -> loc_4_00AC
        add    byte ptr [eax], al
        lodsb  al, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax + eax + 0xac0000], ch ; -> loc_4_00AC
        add    byte ptr [eax], al
        mov    es, word ptr [eax]
        add    byte ptr [eax], al
        pushfd 
        add    byte ptr [eax], al
        add    byte ptr [esi - 0x54000000], ah ; -> loc_4_00A6
        add    byte ptr [eax], al
        add    byte ptr [eax + eax + 0xac0000], ch ; -> loc_4_00AC
        add    byte ptr [eax], al
loc_4_0080:
        mov    word ptr [ebp + 0x10], 0
        mov    word ptr [ebp + 0x18], 0
        jmp    loc_4_00A6
loc_4_008E:
        mov    word ptr [ebp + 0x10], 0xadad
        mov    word ptr [ebp + 0x1c], 0
        jmp    loc_4_00A6
loc_4_009C:
        mov    ax, word ptr [0xc6]           ; -> loc_1_00C6
        mov    word ptr [ebp + 0x10], ax
loc_4_00A6:
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_4_00AC:
        mov    word ptr [ebp + 0x1c], 0xf
loc_4_00B2:
        or     word ptr [ebp + 0x2c], 1
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x34  R-X
        db  0B8h, 001h, 000h, 0F7h, 0C3h, 003h, 000h, 075h, 026h, 0B8h, 000h, 015h, 033h, 0DBh, 0CDh, 02Fh
        db  00Bh, 0DBh, 074h, 015h, 0B8h, 00Ch, 015h, 033h, 0DBh, 0CDh, 02Fh, 083h, 0FBh, 000h, 075h, 003h
        db  0BBh, 001h, 001h, 066h, 00Fh, 0B7h, 0D3h, 0EBh, 003h, 066h, 033h, 0D2h, 0B8h, 000h, 000h, 033h
        db  0DBh, 033h, 0F6h, 0C3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x164  R-X
sub_6_0000:
        push   eax
        cmp    word ptr [0xc4], 0x110        ; -> obj1:0x00C4
        jg     loc_6_0050
        push   0
        push   0x65
        movzx  eax, word ptr [0xc]           ; -> loc_2_000C
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        VxDCall  IFSMgr.IFSMgr_SetErrString      ; cd2026004000
        lea    esp, [esp + 0xc]
        push   0
        push   0x67
        movzx  eax, word ptr [0xe]           ; -> loc_2_000E
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        VxDCall  IFSMgr.IFSMgr_SetErrString      ; cd2026004000
        lea    esp, [esp + 0xc]
loc_6_0050:
        clc    
        pop    eax
        ret    
VCDFSD_Service_02:
        cmp    cl, byte ptr [0xce]           ; -> loc_6_00CE
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 6], 2
        jb     loc_6_0094
        cmp    word ptr [eax + 4], 0x20
        jb     loc_6_0094
        push   eax
        push   0
        call   dword ptr [eax + 8]
        lea    esp, [esp + 8]
        or     eax, eax
        jne    loc_6_0094
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [0xcc]         ; -> loc_1_00CC
        mov    dword ptr [eax], ecx
        mov    dword ptr [0xcc], eax         ; -> loc_1_00CC
loc_6_008A:
        xor    eax, eax
        ret    
loc_6_008D:
        cmp    dword ptr [esp + 4], 2
        jbe    loc_6_008A
loc_6_0094:
        mov    eax, 5
        ret    
VCDFSD_Service_03:
        cmp    cl, byte ptr [0xce]           ; -> loc_6_00CE
        mov    eax, dword ptr [esp + 4]
        lea    edx, [0xcc]                   ; -> loc_1_00CC
        jmp    loc_6_00AE
loc_6_00AC:
        mov    edx, dword ptr [edx]
loc_6_00AE:
        cmp    dword ptr [edx], edx
        je     loc_6_0094
        cmp    dword ptr [edx], eax
        jne    loc_6_00AC
        push   eax
        push   edx
        push   eax
        push   1
        call   dword ptr [eax + 8]
        lea    esp, [esp + 8]
        or     eax, eax
        pop    edx
        pop    eax
        jne    loc_6_0094
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        xor    eax, eax
loc_6_00CE:
        ret    
loc_6_00CF:
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        movzx  ecx, byte ptr [eax + 2]
        cmp    cl, 0x80
        je     loc_6_014D
        ja     loc_6_00FC
        cmp    cl, 3
        je     loc_6_010D
        cmp    cl, 7
        je     loc_6_014D
        cmp    cl, 0xc
        je     loc_6_010D
        jb     loc_6_0155
        cmp    cl, 0xe
        jbe    loc_6_014D
        jmp    loc_6_0155
loc_6_00FC:
        cmp    cl, 0x81
        je     loc_6_0155
        cmp    cl, 0x85
        jbe    loc_6_014D
        cmp    cl, 0x88
        je     loc_6_014D
        jmp    loc_6_0155
loc_6_010D:
        push   ecx
        movzx  edx, word ptr [eax + 0x10]
        movzx  ecx, word ptr [eax + 0xe]
        shl    edx, 4
        add    edx, ecx
        add    edx, dword ptr [ebx + 4]
        pop    ecx
        cmp    cl, 3
        movzx  ecx, byte ptr [edx]
        je     loc_6_012E
        cmp    cl, 5
        jbe    loc_6_014D
        jmp    loc_6_0155
loc_6_012E:
        jecxz  loc_6_013C
        cmp    cl, 2
        je     loc_6_0155
        cmp    cl, 0xf
        jbe    loc_6_014D
        jmp    loc_6_0155
loc_6_013C:
        mov    ecx, dword ptr [0xc8]         ; -> obj1:0x00C8
        mov    dword ptr [edx + 1], ecx
        mov    word ptr [eax + 3], 0x100
        jmp    loc_6_015D
loc_6_014D:
        mov    word ptr [eax + 3], 0x8101
        jmp    loc_6_015D
loc_6_0155:
        mov    word ptr [eax + 3], 0x8103
        jmp    loc_6_015D
loc_6_015D:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        ret    
VxD__LTEXT ENDS
