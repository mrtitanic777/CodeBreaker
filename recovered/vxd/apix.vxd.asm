; APIX -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 3301 instructions recovered by recursive descent

Declare_Virtual_Device APIX, 3, 10, APIX_Control, 00022h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x480  R-X
sub_1_0000:
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
sub_1_0014:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_001C:
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
APIX_Control:
        sub    eax, 6
        cmp    eax, 0x1e
        jae    loc_1_0043
        jmp    dword ptr [eax*4 + 0x45]      ; -> obj1:0x0045
loc_1_0043:
        clc    
        ret    
        ; ---- dispatch table (30 entries) ----
        dd  loc_1_015A
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_2_0002
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_0043
        dd  loc_1_00BD
loc_1_00BD:
        cmp    ecx, 3
        jne    loc_1_010F
        mov    ecx, dword ptr [esi + 0x10]
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ecx + 0x20], eax
        push   esi
        push   8
        push   ecx
        call   sub_3_1040
        add    esp, 8
        pop    esi
        or     dword ptr [esi + 0x24], 0
        je     loc_1_0152
        cmp    eax, -3
        je     loc_1_00E8
        cmp    eax, -1
        jne    loc_1_014C
loc_1_00E8:
        mov    ebx, dword ptr [esi + 0x24]
        mov    ebx, dword ptr [ebx]
        push   ebx
        mov    ebx, dword ptr [esi + 0x10]
        mov    dword ptr [ebx + 0x1c], 0
        mov    dword ptr [ebx + 0x20], 0
        mov    dword ptr [ebx + 0x2c], 0
        pop    ebx
        VxDCall  NDIS.Service_0001h              ; cd2001002a00
        jmp    loc_1_014C
loc_1_010F:
        cmp    ecx, 1
        jne    loc_1_0140
        mov    ebx, dword ptr [esi + 0x18]
        mov    dword ptr [ebx], 0
        mov    ecx, 8
        mov    al, byte ptr [0x8bf]          ; -> obj5:0x08BF
loc_1_0127:
        shr    al, 1
        adc    dword ptr [ebx], 0
        loop   loc_1_0127
        mov    dword ptr [esi + 0x1c], 4
        mov    ebx, dword ptr [esi + 0x20]
        mov    dword ptr [ebx], 4
        jmp    loc_1_0152
loc_1_0140:
        cmp    ecx, 0
        je     loc_1_0152
        cmp    ecx, 0
        je     loc_1_0152
        jmp    loc_1_0155
loc_1_014C:
        mov    eax, 0xffffffff
        ret    
loc_1_0152:
        xor    eax, eax
        ret    
loc_1_0155:
        mov    ax, 1
        ret    
loc_1_015A:
        call   sub_3_1E80
        jmp    loc_1_0163
        db  0F9h, 0C3h
loc_1_0163:
        clc    
        ret    
sub_1_0165:
        push   edi
        push   esi
        push   ebx
        push   eax
        push   ecx
        mov    edx, dword ptr [esp + 0x18]
        mov    edi, dword ptr [edx + 0x1c]
        mov    ebx, dword ptr [edi + 0x14]
        xor    eax, eax
        mov    ecx, 0x21
        mov    esi, 0x4a9                    ; -> loc_3_04A9
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    ecx
        pop    eax
        pop    ebx
        pop    esi
        pop    edi
        ret    
sub_1_018C:
        push   edi
        push   esi
        push   ebx
        push   eax
        push   ecx
        mov    edx, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [edx + 0x1c]
        xor    eax, eax
        mov    ecx, 0x121
        mov    esi, 0x4a9                    ; -> loc_3_04A9
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    ecx
        pop    eax
        pop    ebx
        pop    esi
        pop    edi
        ret    
loc_1_01B0:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        movzx  eax, word ptr [ebx]
        cmp    ax, 3
        je     loc_1_022D
        cmp    ax, 4
        je     loc_1_02AD
        cmp    ax, 2
        je     loc_1_01DA
        cmp    ax, 1
        je     loc_1_015A
        ret    
loc_1_01DA:
        bts    word ptr [0x8d], 0            ; -> obj5:0x008D
        jb     loc_1_0225
        mov    ecx, dword ptr [0x89]         ; -> obj5:0x0089
        or     ecx, ecx
        je     loc_1_0226
        xor    eax, eax
        or     eax, dword ptr [0x8c0]        ; -> obj5:0x08C0
        jne    loc_1_0225
        sub    edx, edx
        mov    esi, 0xb4                     ; -> loc_3_00B4
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        jb     loc_1_0225
        mov    dword ptr [0x900], eax        ; -> obj5:0x0900
        push   0xa3                          ; -> obj5:0x00A3
        push   0x22
        push   0x2150                        ; -> loc_3_2150
        VxDCall  IFSMgr.IFSMgr_RegisterCFSD      ; cd2062004000
        add    esp, 0xc
        inc    eax
        jne    loc_1_0225
loc_1_0225:
        ret    
loc_1_0226:
        mov    word ptr [ebx + 2], 0xffff
        ret    
loc_1_022D:
        mov    ebx, dword ptr [ebx + 0xc]
        test   byte ptr [ebx + 0x21], 0x80
        je     loc_1_02AC
        cmp    byte ptr [ebx + 0x74], 1
        jne    loc_1_02AC
        mov    eax, dword ptr [0x1c]         ; -> loc_1_001C
        push   dword ptr [eax + 0x50]
        push   ebx
        call   sub_3_1D10
        add    esp, 8
        mov    eax, dword ptr [0x1c]         ; -> loc_1_001C
        test   byte ptr [eax + 0x28], 1
        jne    loc_1_02AC
        cmp    dword ptr [0xb4], 0           ; -> obj5:0x00B4
        jne    loc_1_0277
        mov    eax, dword ptr [0xb4]         ; -> obj5:0x00B4
        cmp    dword ptr [eax], 0x49534353
        jne    loc_1_0277
        cmp    dword ptr [eax + 4], 0x4d41435f
        je     loc_1_02AC
loc_1_0277:
        push   0
        push   8
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        or     eax, eax
        je     loc_1_02AC
        mov    dword ptr [0xb4], eax         ; -> obj5:0x00B4
        mov    dword ptr [eax], 0x49534353
        mov    dword ptr [eax + 4], 0x4d41435f
        mov    eax, 0x4f
        push   esi
        mov    esi, 0x2c                     ; -> loc_3_002C
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        pop    esi
loc_1_02AC:
        ret    
loc_1_02AD:
        mov    ebx, dword ptr [ebx + 0xc]
        test   byte ptr [ebx + 0x21], 0x80
        je     loc_1_02CD
        cmp    byte ptr [ebx + 0x74], 1
        jne    loc_1_02CD
        mov    eax, dword ptr [0x1c]         ; -> loc_1_001C
        push   dword ptr [eax + 0x50]
        push   ebx
        call   sub_3_1DD0
        add    esp, 8
loc_1_02CD:
        ret    
sub_1_02CE:
        mov    eax, dword ptr [esp + 4]
        or     dword ptr [eax + 0x20], 0x80000
        test   dword ptr [eax + 0x20], 0x4000
        jne    loc_1_02E5
        mov    eax, dword ptr [eax + 0x10]
loc_1_02E5:
        or     eax, eax
        je     loc_1_0316
        or     dword ptr [eax + 0x20], 0x100000
        mov    edx, dword ptr [0x1c]         ; -> loc_1_001C
        mov    edx, dword ptr [edx + 0x40]
        cli    
loc_1_02FA:
        or     edx, edx
        je     loc_1_0310
        cmp    dword ptr [edx + 0x28], eax
        jne    loc_1_030B
        or     dword ptr [edx + 4], 2
        or     dword ptr [edx + 4], 1
loc_1_030B:
        mov    edx, dword ptr [edx + 0x24]
        jmp    loc_1_02FA
loc_1_0310:
        sti    
        mov    eax, dword ptr [eax + 0x10]
        jmp    loc_1_02E5
loc_1_0316:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0320:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [0x1c]         ; -> loc_1_001C
        mov    edi, dword ptr [esi + 0x20]
        cmp    dword ptr [eax + 0x54], 0
        je     loc_1_0374
        mov    eax, dword ptr [esi + 0xa0]
        or     eax, eax
        je     loc_1_0374
        mov    cl, byte ptr [edi + 3]
        test   cl, 0x80
        jne    loc_1_034B
        cmp    cl, 0x10
        jne    loc_1_035E
loc_1_034B:
        xor    ecx, ecx
        mov    cl, byte ptr [edi + 0xb]
        push   ecx
        push   eax
        mov    eax, dword ptr [edi + 0x1c]
        push   eax
        call   sub_3_029D
        add    esp, 0xc
loc_1_035E:
        mov    eax, dword ptr [edi + 0x1c]
        push   eax
        call   sub_3_0251
        add    esp, 4
        mov    dword ptr [esi + 0xa0], 0
loc_1_0374:
        test   byte ptr [edi + 3], 0x80
        je     loc_1_039E
        mov    ecx, dword ptr [esi + 8]
        cmp    byte ptr [ecx + 0x40], 0
        jne    loc_1_039E
        test   byte ptr [ecx + 0x20], 4
        je     loc_1_039E
        mov    eax, dword ptr [edi + 0x1c]
        mov    al, byte ptr [eax + 2]
        and    al, 0xf
        cmp    al, 6
        jne    loc_1_039E
        push   ecx
        call   sub_1_02CE
        add    esp, 4
loc_1_039E:
        mov    dword ptr [esi + 0x70], 0x760 ; -> loc_3_0760
        test   byte ptr [esi + 0x94], 8
        push   esi
        je     loc_1_03C0
        call   sub_1_018C
        add    esp, 4
        pop    edi
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_03C0:
        call   sub_1_0165
        add    esp, 4
        pop    edi
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_03D0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        mov    dword ptr [eax + 0x70], 0x11d0 ; -> loc_3_11D0
        call   sub_1_0165
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00022h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'APIX    '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  APIX_Control             ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  APIX_PM_API              ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 00Ah, 003h, 010h, 00Fh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x64  R-X
        db  0F8h, 0C3h
loc_2_0002:
        mov    dword ptr [0x38], 0x1b0       ; -> loc_2_0038
        mov    dword ptr [0x3c], 0           ; -> loc_2_003C
        push   0x2c                          ; -> loc_2_002C
        VxDCall  BLOCKDEV.Service_0007h          ; cd2007001000
        pop    eax
        clc    
        ret    
        db  083h, 0C4h, 010h, 0F8h, 0C3h, 000h, 000h, 000h
loc_2_002C:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
loc_2_0038:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_003C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebx
        push   ebx
        dec    ecx
        and    byte ptr [ecx + 0x50], al
        dec    ecx
        and    byte ptr [eax + 0x4c], bl
        inc    ecx
        push   esp
        dec    edi
        push   edx
        and    byte ptr [eax], al
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

VxD__LTEXT SEGMENT   ; object 3  vsize=0x2280  R-X
        db  08Bh, 044h, 024h, 004h, 0BAh, 000h, 000h, 000h, 000h, 056h, 0BEh
        dd  loc_3_00B4               ; fixup
        db  0CDh, 020h, 009h, 000h, 001h, 000h, 05Eh, 072h, 003h, 033h, 0C0h, 0C3h, 0B8h, 0FFh, 0FFh, 0FFh
        db  0FFh, 0C3h
sub_3_0021:
        mov    eax, dword ptr [esp + 4]
        VMMCall  VMM.Remove_V86_Break_Point      ; cd200a000100
        ret    
loc_3_002C:
        cmp    word ptr [ebp + 0x1c], 0x8100
        jne    loc_3_0066
        call   sub_3_03E9
        or     eax, eax
        je     loc_3_00AC
        push   eax
        call   sub_3_0637
        mov    dword ptr [esp], eax
        call   sub_3_18C0
        add    esp, 4
        or     eax, eax
        jne    loc_3_005A
        mov    word ptr [ebp + 0x1c], 0
        jmp    loc_3_00B2
loc_3_005A:
        cmp    eax, -1
        je     loc_3_00AC
        cmp    eax, -2
        jne    loc_3_00AC
        stc    
        ret    
loc_3_0066:
        cmp    word ptr [ebp + 0x1c], 0x8200
        jne    loc_3_00AC
        cmp    word ptr [ebp + 0x18], 0x8765
        jne    loc_3_00AC
        cmp    word ptr [ebp + 0x14], 0xcba9
        jne    loc_3_00AC
        mov    word ptr [ebp + 0x18], 0x9abc
        mov    word ptr [ebp + 0x14], 0x5678
        mov    eax, dword ptr [0xb4]         ; -> obj5:0x00B4
        mov    ecx, 8
        VMMCall  VMM.Map_Lin_To_VM_Addr          ; cd201d000100
        jb     loc_3_00AC
        mov    word ptr [ebp + 0x38], cx
        mov    word ptr [ebp], dx
        mov    word ptr [ebp + 0x1c], 0
        jmp    loc_3_00B2
loc_3_00AC:
        mov    word ptr [ebp + 0x1c], 0xffff
loc_3_00B2:
        clc    
        ret    
loc_3_00B4:
        push   eax
        call   sub_3_03D3
        or     eax, eax
        je     loc_3_017C
        mov    edx, eax
        push   eax
        call   sub_3_0637
        add    esp, 4
        mov    ecx, eax
        push   edx
        push   ecx
        push   0
        push   eax
        call   sub_3_1040
        add    esp, 8
        pop    ecx
        pop    edx
        or     eax, eax
        jne    loc_3_00EE
        mov    word ptr [ebp + 0x1c], 0
        pop    eax
        jmp    loc_3_0183
loc_3_00EE:
        cmp    eax, -3
        jne    loc_3_0109
        test   byte ptr [ecx + 3], 1
        je     loc_3_0100
        xor    eax, eax
        call   sub_3_05B8
loc_3_0100:
        mov    word ptr [ebp + 0x1c], 0
        pop    eax
        jmp    loc_3_0183
loc_3_0109:
        cmp    eax, -1
        je     loc_3_017C
        cmp    eax, -2
        jne    loc_3_017C
        pop    ebx
        xor    esi, esi
        cmp    dword ptr [0x8c0], 0          ; -> obj5:0x08C0
        je     loc_3_0189
loc_3_011F:
        cmp    dword ptr [esi + 0x8c0], ebx  ; -> obj5:0x08C0
        je     loc_3_012C
        add    esi, 8
        jmp    loc_3_011F
loc_3_012C:
        cmp    dword ptr [esi + 0x8c4], 0    ; -> obj5:0x08C4
        je     loc_3_0189
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        call   sub_3_03D3
        push   eax
        shr    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        pop    eax
        and    eax, 0xffff
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        movzx  ecx, word ptr [esi + 0x8c6]   ; -> obj5:0x08C6
        movzx  edx, word ptr [esi + 0x8c4]   ; -> obj5:0x08C4
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        add    dword ptr [ebp + 0x30], 4
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        jmp    loc_3_0183
loc_3_017C:
        mov    word ptr [ebp + 0x1c], 0xffff
        pop    eax
loc_3_0183:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a800100
loc_3_0189:
        mov    word ptr [ebp + 0x1c], 0xffff
        jmp    loc_3_0183
        db  0F9h, 0C3h
APIX_PM_API:
        movzx  eax, word ptr [ebp + 0x1c]
        or     ah, ah
        jne    loc_3_01A3
        mov    word ptr [ebp + 0x1c], 0x400
        jmp    loc_3_01CD
loc_3_01A3:
        dec    ah
        jne    loc_3_01BF
        mov    byte ptr [ebp + 0x1c], 0
        mov    ecx, 8
        mov    al, byte ptr [0x8bf]          ; -> obj5:0x08BF
loc_3_01B5:
        shr    al, 1
        adc    byte ptr [ebp + 0x1c], 0
        loop   loc_3_01B5
        jmp    loc_3_01CD
loc_3_01BF:
        dec    ah
        jne    loc_3_01C5
        jmp    loc_3_01CD
loc_3_01C5:
        dec    ah
        jne    loc_3_01CB
        jmp    loc_3_01D9
loc_3_01CB:
        jmp    loc_3_01CD
loc_3_01CD:
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
        db  066h, 083h, 04Dh, 02Ch, 001h, 0C3h
loc_3_01D9:
        call   sub_3_03D3
        or     eax, eax
        je     loc_3_017C
        mov    edx, eax
        push   eax
        call   sub_3_0637
        add    esp, 4
        mov    ecx, eax
        push   edx
        push   ecx
        push   4
        push   eax
        call   sub_3_1040
        add    esp, 8
        pop    ecx
        pop    edx
        or     eax, eax
        jne    loc_3_020A
        mov    word ptr [ebp + 0x1c], 0
        jmp    loc_3_022D
loc_3_020A:
        cmp    eax, -3
        jne    loc_3_0227
        test   byte ptr [ecx + 3], 1
        je     loc_3_021F
        mov    eax, 4
        call   sub_3_05B8
loc_3_021F:
        mov    word ptr [ebp + 0x1c], 0
        jmp    loc_3_022D
loc_3_0227:
        mov    word ptr [ebp + 0x1c], 0xffff
loc_3_022D:
        jmp    loc_3_01CD
sub_3_022F:
        mov    eax, dword ptr [esp + 4]
        add    eax, 0x64
        mov    eax, dword ptr [eax + 0x2c]
        sub    esp, 8
        mov    dword ptr [esp + 4], eax
        mov    word ptr [esp], 4
        push   esp
        call   sub_1_0000
        add    esp, 0xc
        ret    
sub_3_0251:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 8
        mov    dword ptr [esp + 4], eax
        mov    word ptr [esp], 4
        push   esp
        call   sub_1_0000
        add    esp, 0xc
        ret    
sub_3_026D:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 0x10
        mov    word ptr [esp + 4], ax
        mov    word ptr [esp + 0xa], 9
        mov    byte ptr [esp + 0xc], 2
        mov    word ptr [esp], 3
        push   esp
        call   sub_1_0000
        add    esp, 4
        mov    eax, dword ptr [esp + 6]
        add    esp, 0x10
        ret    
sub_3_029D:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        ret    
sub_3_02B0:
        lea    eax, [0xbc]                   ; -> obj5:0x00BC
        jmp    loc_3_02C0
sub_3_02B8:
        lea    eax, [0x4bc]                  ; -> obj5:0x04BC
        jmp    loc_3_02C0
loc_3_02C0:
        add    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 8]
        shl    ecx, 3
        add    eax, ecx
        mov    ecx, dword ptr [esp + 4]
        shl    ecx, 7
        add    eax, ecx
        movzx  eax, byte ptr [eax]
        cmp    eax, 0xff
        jne    loc_3_02E4
        xor    eax, eax
        jmp    loc_3_02EB
loc_3_02E4:
        add    eax, 9                        ; -> obj5:0x0009
        mov    eax, dword ptr [eax]
loc_3_02EB:
        ret    
sub_3_02EC:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        movzx  ecx, byte ptr [edx + 0x77]
        shl    ecx, 4
        mov    eax, dword ptr [edx + 4]
        add    ecx, eax
        add    ecx, 0xbc
        add    ecx, 0x50
        add    cx, word ptr [edx + 0x80]
        add    eax, 0x64
        sub    esp, 0x10
        mov    word ptr [esp], 2
        mov    word ptr [esp + 4], cx
        mov    dword ptr [esp + 6], eax
        mov    byte ptr [esp + 0xe], 2
        push   esp
        call   sub_1_0000
        add    esp, 4
        mov    ecx, dword ptr [esp + 0xa]
        add    esp, 0x10
        add    eax, ecx
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 4], 0xf
        mov    word ptr [eax + 6], 0x12
        mov    dword ptr [eax + 8], 0xc400
        mov    dword ptr [eax + 0x40], eax
        sub    dword ptr [eax + 0x40], 0x64
        mov    dword ptr [eax + 0x44], 0
        mov    bl, byte ptr [edx + 0x15]
        mov    byte ptr [eax + 0x4d], bl
        mov    dword ptr [eax + 0x2c], ecx
        mov    dword ptr [eax + 0x48], eax
        add    dword ptr [eax + 0x48], 0x58
        mov    dword ptr [eax + 0x1c], eax
        add    dword ptr [eax + 0x1c], 0x58
        movzx  ecx, byte ptr [edx + 0x77]
        shl    ecx, 3
        add    dword ptr [eax + 0x1c], ecx
        mov    dword ptr [eax + 0x3c], 0
        sub    eax, 0x64
        mov    dword ptr [eax + 8], edx
        mov    dword ptr [eax + 0x20], eax
        add    dword ptr [eax + 0x20], 0xbc
        shl    ecx, 1
        add    dword ptr [eax + 0x20], ecx
        mov    ecx, dword ptr [eax + 0x20]
        mov    dword ptr [ecx], 0x40
        mov    bl, byte ptr [edx + 0x75]
        mov    byte ptr [ecx + 5], bl
        mov    bl, byte ptr [edx + 0x7c]
        mov    byte ptr [ecx + 6], bl
        mov    bl, byte ptr [edx + 0x7d]
        mov    byte ptr [ecx + 7], bl
        mov    dword ptr [ecx + 0x20], 0
        mov    dword ptr [ecx + 0x28], ecx
        add    dword ptr [ecx + 0x28], 0x50
        mov    dword ptr [ecx + 0x40], eax
        mov    dword ptr [ecx + 0x44], 0
        pop    ebx
        ret    
sub_3_03D3:
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        je     loc_3_03E6
        mov    eax, dword ptr [eax + 4]
        ret    
loc_3_03E6:
        xor    eax, eax
        ret    
sub_3_03E9:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        movzx  eax, word ptr [ebx + 0x38]
        shl    eax, 0x10
        add    ax, word ptr [ebx + 0x10]
        pop    ebx
        ret    
sub_3_0400:
        mov    eax, dword ptr [esp + 4]
        sub    eax, 0x64
        push   eax
        call   sub_1_0014
        add    esp, 4
        or     eax, eax
        jne    loc_3_0417
        inc    eax
        ret    
loc_3_0417:
        xor    eax, eax
        ret    
sub_3_041A:
        mov    eax, dword ptr [esp + 4]
        mov    edx, eax
        shr    eax, 0xc
        and    edx, 0xfff
        add    edx, dword ptr [esp + 8]
        dec    edx
        shr    edx, 0xc
        inc    edx
        push   0x40000000
        push   edx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        or     eax, eax
        je     loc_3_0452
        mov    edx, dword ptr [esp + 4]
        and    edx, 0xfff
        add    eax, edx
loc_3_0452:
        ret    
sub_3_0453:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, dword ptr [esp + 8]
        add    eax, dword ptr [ebx + 4]
        pop    ebx
        ret    
sub_3_0463:
        push   edi
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, esi
        sub    edi, 0x64
        mov    edi, dword ptr [edi + 8]
        add    edi, 0xc3
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        pop    esi
        pop    edi
        ret    
sub_3_0480:
        mov    eax, dword ptr [esp + 4]
        mov    edx, eax
        shr    eax, 0xc
        and    edx, 0xfff
        add    edx, dword ptr [esp + 8]
        dec    edx
        shr    edx, 0xc
        inc    edx
        push   0x40000000
        push   edx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        ret    
loc_3_04A9:
        or     edx, edx
        je     loc_3_0523
        push   dword ptr [edx + 0x94]
        push   edx
        call   dword ptr [edx + 0x70]
        pop    edx
        pop    ecx
        test   ecx, 8
        je     loc_3_051B
        mov    ebx, dword ptr [edx + 0x98]
        mov    ebx, dword ptr [ebx + 0x20]
        or     ebx, ebx
        jne    loc_3_04E0
        or     eax, eax
        je     loc_3_05AF
        call   sub_3_05B0
        jmp    loc_3_05AF
loc_3_04E0:
        push   eax
        push   edx
        mov    ebx, dword ptr [ebx]
        push   ebx
        mov    ebx, dword ptr [edx + 0x98]
        mov    dword ptr [ebx + 0x1c], 0
        mov    dword ptr [ebx + 0x20], 0
        mov    dword ptr [ebx + 0x2c], 0
        pop    ebx
        VxDCall  NDIS.Service_0001h              ; cd2001002a00
        pop    edx
        pop    eax
        or     eax, eax
        je     loc_3_05AF
        call   sub_3_05B0
        jmp    loc_3_05AF
loc_3_051B:
        or     eax, eax
        je     loc_3_05AF
loc_3_0523:
        push   edx
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        test   byte ptr [edx + 0x94], 4
        je     loc_3_0544
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        jmp    loc_3_054A
loc_3_0544:
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
loc_3_054A:
        mov    ecx, dword ptr [edx + 0x20]
        movzx  eax, word ptr [ecx + 0x26]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        movzx  eax, word ptr [ecx + 0x24]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp + 0x30]
        push   eax
        mov    eax, edx
        add    eax, 0x64
        mov    edx, dword ptr [eax + 0x38]
        mov    ecx, edx
        and    edx, 0xffff
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        pop    eax
        mov    dword ptr [ebp + 0x30], eax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    edx
        call   sub_3_05B0
loc_3_05AF:
        ret    
sub_3_05B0:
        push   edx
        call   dword ptr [edx + 0x70]
        add    esp, 4
        ret    
sub_3_05B8:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        or     eax, eax
        je     loc_3_05D3
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        jmp    loc_3_05D9
loc_3_05D3:
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
loc_3_05D9:
        mov    eax, edx
        ror    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        ror    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [ebp + 0x30]
        push   eax
        mov    dx, word ptr [ecx + 0x1a]
        and    edx, 0xffff
        mov    cx, word ptr [ecx + 0x1c]
        and    ecx, 0xffff
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        pop    eax
        mov    dword ptr [ebp + 0x30], eax
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        VMMCall  VMM.Simulate_Pop                ; cd204e000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        ret    
sub_3_0637:
        push   ebx
        mov    eax, dword ptr [esp + 8]
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ecx, dword ptr [ebx + 8]
        push   dword ptr [ecx + 0x1c]
        mov    dword ptr [ecx + 0x1c], eax
        mov    ax, 0x1e1c
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        pop    dword ptr [ecx + 0x1c]
        pop    ebx
        ret    
sub_3_065A:
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        ret    
        db  033h, 0C0h, 066h, 0B8h, 00Ah, 003h, 0F8h, 0C3h, 000h, 000h, 000h
sub_3_0670:
        mov    edx, dword ptr [esp + 4]
        mov    cl, byte ptr [edx]
        cmp    cl, 7
        jb     loc_3_0681
        mov    eax, 0x60
        ret    
loc_3_0681:
        test   byte ptr [esp + 8], 8
        je     loc_3_06A7
        xor    eax, eax
        mov    al, cl
        mov    al, byte ptr [eax + 0x28]     ; -> loc_4_0028
        and    eax, 0xff
        cmp    cl, 2
        jne    loc_3_06DE
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 0x14]
        add    ecx, 2
        add    eax, ecx
        ret    
loc_3_06A7:
        xor    eax, eax
        mov    al, cl
        mov    al, byte ptr [eax + 0x20]     ; -> loc_4_0020
        and    eax, 0xff
        or     cl, cl
        jne    loc_3_06CB
        cmp    word ptr [edx + 4], 0xaa55
        jne    loc_3_06CB
        xor    ecx, ecx
        mov    cx, word ptr [edx + 6]
        add    eax, ecx
        ret    
loc_3_06CB:
        cmp    cl, 2
        jne    loc_3_06DE
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 0xe]
        add    eax, ecx
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 0x17]
        add    eax, ecx
loc_3_06DE:
        ret    
        db  090h
sub_3_06E0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        je     loc_3_06F7
        mov    esi, dword ptr [eax + 0x98]
        mov    eax, dword ptr [eax + 0x94]
        jmp    loc_3_06FF
loc_3_06F7:
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 0x10]
loc_3_06FF:
        push   eax
        push   esi
        call   sub_3_0670
        add    esp, 8
        push   eax
        push   esi
        call   sub_3_0480
        add    esp, 8
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0720:
        mov    ecx, dword ptr [esp + 4]
        add    ecx, 0x64
        test   byte ptr [ecx + 0x30], 2
        je     loc_3_073E
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    eax, dword ptr [ecx + 0x1c]
        push   eax
        call   sub_3_0480
        add    esp, 8
loc_3_073E:
        ret    
        db  090h
loc_3_0740:
        push   esi
        push   0
        mov    esi, dword ptr [esp + 0xc]
        push   0
        push   esi
        call   sub_3_06E0
        add    esp, 0xc
        push   esi
        call   sub_3_022F
        add    esp, 4
        pop    esi
        ret    
        db  08Dh, 049h, 000h
loc_3_0760:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [ebp + 0x98]
        add    ebp, 0x64
        mov    eax, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [eax + 0x94]
        mov    dword ptr [esp + 0x10], eax
        mov    edi, eax
        and    edi, 8
        je     loc_3_0791
        mov    edx, dword ptr [esi + 0x18]
        mov    ebx, dword ptr [esi + 0xc]
        jmp    loc_3_0797
loc_3_0791:
        mov    edx, dword ptr [esi + 0x1a]
        mov    ebx, dword ptr [esi + 0xa]
loc_3_0797:
        mov    al, byte ptr [edx + 3]
        and    eax, 0x3f
        mov    dword ptr [esp + 0x14], ebx
        mov    al, byte ptr [eax + 0x30]     ; -> obj4:0x0030
        and    al, 0x3f
        mov    cl, byte ptr [esi + 3]
        test   cl, 4
        je     loc_3_07C2
        or     ebx, ebx
        je     loc_3_07C2
        sub    ebx, dword ptr [edx + 0x10]
        cmp    al, 0x12
        jne    loc_3_07D1
        or     edi, edi
        jne    loc_3_07C2
        mov    al, 1
loc_3_07C2:
        cmp    al, 0x12
        jne    loc_3_07D1
        test   cl, 0x18
        je     loc_3_07CF
        or     edi, edi
        jne    loc_3_07D1
loc_3_07CF:
        mov    al, 1
loc_3_07D1:
        test   al, 0x10
        je     loc_3_07E6
        mov    cl, al
        mov    al, 4
        cmp    cl, 0x12
        jne    loc_3_07E8
        or     edi, edi
        je     loc_3_0810
        mov    al, 1
        jmp    loc_3_07E8
loc_3_07E6:
        sub    cl, cl
loc_3_07E8:
        or     edi, edi
        je     loc_3_0810
        mov    byte ptr [esi + 0x16], cl
        mov    dword ptr [esi + 0xc], ebx
        mov    cl, byte ptr [edx + 4]
        mov    byte ptr [esi + 0x17], cl
        mov    ebx, dword ptr [ebp + 0x38]
        mov    dword ptr [esi + 0x18], ebx
        test   byte ptr [esi + 3], 0x40
        je     loc_3_0806
        jmp    loc_3_0822
loc_3_0806:
        mov    byte ptr [esi + 0x24], al
        jmp    loc_3_0825
        db  005h, 000h, 000h, 000h, 000h
loc_3_0810:
        mov    byte ptr [esi + 0x18], cl
        mov    dword ptr [esi + 0xa], ebx
        mov    cl, byte ptr [edx + 4]
        mov    byte ptr [esi + 0x19], cl
        mov    ebx, dword ptr [ebp + 0x38]
        mov    dword ptr [esi + 0x1a], ebx
loc_3_0822:
        mov    byte ptr [esi + 1], al
loc_3_0825:
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x20]
        push   eax
        call   sub_3_0720
        add    esp, 8
        test   byte ptr [esi + 3], 1
        je     loc_3_0841
        or     ebx, ebx
        jne    loc_3_0870
loc_3_0841:
        or     edi, edi
        jne    loc_3_0870
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        call   sub_3_022F
        mov    eax, dword ptr [esp + 0x14]
        add    esp, 4
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_0870:
        mov    dword ptr [ebp + 0xc], 0x740  ; -> loc_3_0740
        mov    eax, 0xffffffff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0890:
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x3c]
        mov    dword ptr [esp + 0x24], ebp
        mov    eax, dword ptr [esp + 0x40]
        push   eax
        call   sub_3_02EC
        mov    dword ptr [esp + 0x20], eax
        add    esp, 4
        mov    esi, eax
        mov    edi, dword ptr [eax + 0x20]
        mov    dword ptr [esi + 0x70], 0x320 ; -> loc_1_0320
        mov    ebx, dword ptr [esp + 0x38]
        mov    dword ptr [esi + 0x98], ebx
        add    esi, 0x64
        mov    byte ptr [edi + 2], 0
        mov    dword ptr [edi + 0xc], 0
        mov    dword ptr [edi + 0x10], 0
        cmp    ebp, 8
        jne    loc_3_0912
        mov    eax, dword ptr [ebx + 0x18]
        mov    dword ptr [esi + 0x38], eax
        mov    dword ptr [ebx + 0x18], edi
        lea    eax, [ebx + 0x30]
        mov    dword ptr [esp + 0x10], eax
        mov    al, byte ptr [ebx + 0x15]
        mov    byte ptr [edi + 0xa], al
        xor    eax, eax
        mov    al, byte ptr [ebx + 0x14]
        mov    dword ptr [esp + 0x14], eax
        lea    eax, [ebx + 0x40]
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [ebx + 0x10]
        jmp    loc_3_0957
loc_3_0912:
        lea    eax, [esi + 0x38]
        mov    eax, dword ptr [ebx + 0x1a]
        mov    dword ptr [esi + 0x38], eax
        mov    dword ptr [ebx + 0x1a], edi
        lea    eax, [ebx + 0x40]
        mov    dword ptr [esp + 0x10], eax
        lea    eax, [edi + 0xa]
        mov    al, byte ptr [ebx + 0x17]
        mov    byte ptr [edi + 0xa], al
        xor    eax, eax
        mov    al, byte ptr [ebx + 0xe]
        mov    dword ptr [esp + 0x14], eax
        xor    eax, eax
        mov    al, byte ptr [ebx + 0x17]
        lea    eax, [ebx + eax + 0x40]
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [ebx + 0xa]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [ebx + 0xf]
        push   eax
        call   sub_3_0637
        add    esp, 4
loc_3_0957:
        mov    dword ptr [esp + 0x28], eax
        mov    cl, byte ptr [ebx + 3]
        mov    al, cl
        and    al, 0x18
        cmp    al, 0x18
        je     loc_3_097C
        test   cl, 8
        je     loc_3_096F
        or     dword ptr [edi + 0xc], 0x40
loc_3_096F:
        test   byte ptr [ebx + 3], 0x10
        je     loc_3_097C
        or     dword ptr [edi + 0xc], 0x80
loc_3_097C:
        sub    ecx, ecx
        cmp    byte ptr [edi + 0xa], cl
        jbe    loc_3_0998
loc_3_0983:
        mov    eax, dword ptr [esp + 0x10]
        mov    al, byte ptr [ecx + eax]
        mov    byte ptr [edi + ecx + 0x30], al
        inc    ecx
        xor    eax, eax
        mov    al, byte ptr [edi + 0xa]
        cmp    eax, ecx
        ja     loc_3_0983
loc_3_0998:
        mov    dword ptr [edi + 0x14], 0
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_09E2
        mov    al, byte ptr [esp + 0x14]
        mov    byte ptr [edi + 0xb], al
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [edi + 0x1c], eax
        mov    eax, dword ptr [0x1c]         ; -> loc_1_001C
        cmp    dword ptr [eax + 0x54], 0
        je     loc_3_09E6
        cmp    dword ptr [esp + 0x14], 0x200
        jae    loc_3_09E2
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        call   sub_3_026D
        add    esp, 4
        mov    dword ptr [edi + 0x1c], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x3c], eax
        jmp    loc_3_09E6
loc_3_09E2:
        or     dword ptr [edi + 0xc], 0x20
loc_3_09E6:
        cmp    dword ptr [esp + 0x18], 0
        je     loc_3_0A36
        mov    eax, dword ptr [esp + 0x18]
        push   eax
        mov    eax, dword ptr [esp + 0x2c]
        push   eax
        call   sub_3_041A
        add    esp, 8
        mov    dword ptr [edi + 0x18], eax
        mov    dword ptr [esi + 0x1c], eax
        cmp    dword ptr [edi + 0x18], 0
        je     loc_3_0A30
        mov    eax, ebp
        or     eax, 2
        mov    dword ptr [esp + 0x24], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edi + 0x10], eax
        mov    eax, dword ptr [edi + 0xc]
        test   al, 0xc0
        jne    loc_3_0A3D
        or     eax, 0xc0
        mov    dword ptr [edi + 0xc], eax
        jmp    loc_3_0A3D
        db  08Dh, 064h, 024h, 000h
loc_3_0A30:
        mov    byte ptr [ebx + 1], 0x80
        jmp    loc_3_0A3D
loc_3_0A36:
        and    dword ptr [edi + 0xc], 0xffffff3f
loc_3_0A3D:
        and    ebp, 8
        cmp    ebp, 8
        jne    loc_3_0A50
        mov    dword ptr [edi + 0x24], ebx
        jmp    loc_3_0A58
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_0A50:
        call   sub_3_03D3
        mov    dword ptr [edi + 0x24], eax
loc_3_0A58:
        or     dword ptr [esi + 8], 0x200
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x18], eax
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [esi + 0x30], eax
        cmp    byte ptr [ebx + 1], 0x80
        jne    loc_3_0A94
        cmp    ebp, 8
        mov    eax, dword ptr [esi + 0x38]
        jne    loc_3_0A80
        mov    dword ptr [ebx + 0x18], eax
        jmp    loc_3_0A83
        db  090h
loc_3_0A80:
        mov    dword ptr [ebx + 0x1a], eax
loc_3_0A83:
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x20]
        push   eax
        call   sub_3_0720
        add    esp, 8
loc_3_0A94:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x24
        ret    
        db  08Bh, 0FFh
sub_3_0AA0:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x18], 8
        mov    edx, 8
        xor    eax, eax
        mov    al, byte ptr [0x8bf]          ; -> obj5:0x08BF
        add    eax, 0
loc_3_0AC0:
        mov    esi, 1
        lea    ecx, [edx - 1]
        shl    esi, cl
        test   eax, esi
        jne    loc_3_0AD1
        dec    edx
        jne    loc_3_0AC0
loc_3_0AD1:
        mov    esi, dword ptr [esp + 0x20]
        mov    dword ptr [esp + 0x14], 0xff
        mov    byte ptr [esi + 8], dl
        mov    byte ptr [esi + 9], 7
        mov    edi, 0x10
        mov    dword ptr [esp + 0x10], 0xff
        sub    ebx, ebx
loc_3_0AF3:
        dec    edi
        sub    ebp, ebp
loc_3_0AF6:
        push   ebp
        push   edi
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        push   eax
        call   sub_3_02B0
        add    esp, 0xc
        or     eax, eax
        je     loc_3_0B12
        or     ebx, ebx
        jne    loc_3_0B33
        mov    ebx, eax
        jmp    loc_3_0B33
loc_3_0B12:
        cmp    dword ptr [esp + 0x14], 0xff
        jne    loc_3_0B20
        mov    dword ptr [esp + 0x14], edi
loc_3_0B20:
        cmp    dword ptr [esp + 0x10], 0xff
        jne    loc_3_0B33
        cmp    edi, 7
        jg     loc_3_0B33
        mov    dword ptr [esp + 0x10], edi
loc_3_0B33:
        inc    ebp
        cmp    ebp, 8
        jl     loc_3_0AF6
        or     edi, edi
        jne    loc_3_0AF3
        or     ebx, ebx
        jne    loc_3_0B4A
        mov    al, byte ptr [esp + 0x10]
        jmp    loc_3_0BE4
loc_3_0B4A:
        mov    ebp, dword ptr [ebx + 8]
        or     ebp, ebp
        je     loc_3_0BE7
loc_3_0B55:
        cmp    byte ptr [ebp + 0x13], 0x13
        jb     loc_3_0B89
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     loc_3_0B89
        mov    edi, dword ptr [eax + 0x14]
        or     edi, edi
        je     loc_3_0B89
        lea    eax, [edi + 0x14]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        lea    eax, [eax + eax*4]
        lea    eax, [eax*2 + 0x8c]           ; -> obj4:0x008C
        push   eax
        call   sub_3_1FF0
        add    esp, 8
        or     eax, eax
        jne    loc_3_0B92
loc_3_0B89:
        mov    ebp, dword ptr [ebp + 0xc]
        or     ebp, ebp
        jne    loc_3_0B55
        jmp    loc_3_0BE7
loc_3_0B92:
        movsx  eax, byte ptr [edi + 0x49]
        inc    eax
        mov    dword ptr [esp + 0x18], eax
        test   byte ptr [esp + 0x24], 8
        je     loc_3_0BBB
        mov    dword ptr [esi + 0x2a], 0
        mov    al, byte ptr [edi + 0x49]
        inc    al
        mov    byte ptr [esi + 0x2d], al
        mov    byte ptr [esi + 0x2c], 2
        mov    eax, dword ptr [ebx + 0x50]
        mov    dword ptr [esi + 0x2e], eax
loc_3_0BBB:
        cmp    dword ptr [esp + 0x10], 0xff
        je     loc_3_0BD0
        mov    al, byte ptr [esp + 0x10]
        jmp    loc_3_0BE4
        db  005h, 000h, 000h, 000h, 000h
loc_3_0BD0:
        cmp    byte ptr [edi + 0x49], 7
        jle    loc_3_0BE7
        cmp    dword ptr [esp + 0x14], 0xff
        je     loc_3_0BE7
        mov    al, byte ptr [esp + 0x14]
loc_3_0BE4:
        mov    byte ptr [esi + 9], al
loc_3_0BE7:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 0x24]
        and    ecx, 8
loc_3_0BF0:
        cmp    ecx, 8
        jne    loc_3_0C00
        mov    dl, byte ptr [eax + 0x10]     ; -> loc_4_0010
        jmp    loc_3_0C06
        db  08Dh, 049h, 000h
loc_3_0C00:
        mov    dl, byte ptr [eax]            ; -> loc_4_0000
loc_3_0C06:
        mov    byte ptr [esi + eax + 0xa], dl
        xor    edx, edx
        mov    dl, byte ptr [esi + 2]
        inc    eax
        lea    edx, [edx + edx*4]
        mov    dl, byte ptr [eax + edx*2 + 0x8b] ; -> loc_4_008B
        mov    byte ptr [esi + eax + 0x19], dl
        cmp    eax, 0x10
        jl     loc_3_0BF0
        cmp    word ptr [esi + 4], 0xaa55
        jne    loc_3_0C48
        cmp    word ptr [esi + 6], 2
        jb     loc_3_0C48
        mov    byte ptr [esi + 4], 0xaa
        mov    byte ptr [esi + 5], 0x55
        mov    eax, dword ptr [esp + 0x18]
        shr    eax, 2
        and    al, 4
        or     al, 2
        mov    byte ptr [esi + 0x3a], al
loc_3_0C48:
        mov    byte ptr [esi + 1], 1
        mov    eax, dword ptr [esp + 0x24]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0C70:
        push   esi
        mov    esi, dword ptr [esp + 8]
        xor    eax, eax
        mov    al, byte ptr [esi + 9]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 8]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        push   eax
        call   sub_3_02B0
        add    esp, 0xc
        or     eax, eax
        je     loc_3_0CB7
        test   byte ptr [eax + 0x23], 8
        jne    loc_3_0CB7
        mov    al, byte ptr [eax + 0x40]
        mov    byte ptr [esi + 0xa], al
        mov    byte ptr [esi + 1], 1
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        sub    eax, eax
        pop    esi
        ret    
loc_3_0CB7:
        mov    byte ptr [esi + 1], 0x82
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        sub    eax, eax
        pop    esi
        ret    
        db  090h
sub_3_0CD0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        xor    eax, eax
        mov    al, byte ptr [esi + 9]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 8]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        push   eax
        call   sub_3_02B0
        add    esp, 0xc
        or     eax, eax
        je     loc_3_0D90
        test   byte ptr [eax + 0x23], 8
        jne    loc_3_0D90
        mov    edi, dword ptr [esp + 0x14]
        push   eax
        push   edi
        push   esi
        call   sub_3_0890
        add    esp, 0xc
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_3_0D31
        mov    byte ptr [esi + 1], 0x82
        push   edi
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        mov    eax, 0xfffffffd
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_0D31:
        cmp    byte ptr [esi + 1], 0x80
        je     loc_3_0D53
        push   ebx
        call   sub_3_0400
        add    esp, 4
        or     eax, eax
        je     loc_3_0D53
        push   ebx
        call   sub_3_0463
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_0D53:
        test   edi, 8
        mov    eax, dword ptr [ebx + 0x38]
        je     loc_3_0D63
        mov    dword ptr [esi + 0x18], eax
        jmp    loc_3_0D66
loc_3_0D63:
        mov    dword ptr [esi + 0x1a], eax
loc_3_0D66:
        mov    byte ptr [esi + 1], 0xe6
        push   edi
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        mov    eax, dword ptr [ebx + 0x40]
        push   eax
        call   sub_3_022F
        add    esp, 4
        mov    eax, 0xfffffffd
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_0D90:
        mov    edi, dword ptr [esp + 0x14]
        test   edi, 8
        je     loc_3_0DA2
        mov    byte ptr [esi + 0x24], 0x82
        jmp    loc_3_0DA6
loc_3_0DA2:
        mov    byte ptr [esi + 1], 0x82
loc_3_0DA6:
        push   edi
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        mov    eax, 0xfffffffd
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_3_0DC0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esi + 8]
        push   eax
        call   sub_3_0637
        mov    edi, eax
        add    esp, 4
        mov    eax, dword ptr [edi + 0x1a]
        mov    dword ptr [esp + 0x10], eax
        xor    eax, eax
        mov    al, byte ptr [edi + 9]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [edi + 8]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [edi + 2]
        push   eax
        call   sub_3_02B0
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_0E20
        mov    byte ptr [esi + 1], 1
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_0E20:
        test   byte ptr [eax + 0x23], 8
        je     loc_3_0E44
        mov    byte ptr [esi + 1], 1
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_3_0E44:
        push   eax
        call   sub_3_02EC
        mov    ebx, eax
        add    esp, 4
        mov    ebp, dword ptr [ebx + 0x20]
        or     dword ptr [ebx + 0x6c], 0x100
        mov    dword ptr [ebx + 0x98], esi
        lea    ecx, [ebx + 0x64]
        mov    byte ptr [ebp + 2], 0x10
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebp + 0x20], eax
        mov    dword ptr [ebp + 0xc], 0
        push   ecx
        call   sub_3_0463
        add    esp, 4
        mov    al, byte ptr [ebp + 3]
        and    eax, 0x3f
        mov    al, byte ptr [eax + 0x30]     ; -> obj4:0x0030
        mov    byte ptr [edi + 1], al
        mov    byte ptr [esi + 1], 1
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        push   ebx
        call   sub_3_022F
        add    esp, 4
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0EC0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        xor    eax, eax
        mov    al, byte ptr [esi + 9]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 8]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        push   eax
        call   sub_3_02B0
        add    esp, 0xc
        or     eax, eax
        je     loc_3_0F70
        test   byte ptr [eax + 0x23], 8
        je     loc_3_0F09
        mov    byte ptr [esi + 1], 0x82
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_0F09:
        push   eax
        call   sub_3_02EC
        add    esp, 4
        mov    edi, dword ptr [eax + 0x20]
        mov    dword ptr [eax + 0x70], 0x320 ; -> loc_1_0320
        mov    dword ptr [eax + 0x98], esi
        mov    dword ptr [eax + 0x94], 0
        lea    ebx, [eax + 0x64]
        test   byte ptr [esp + 0x14], 8
        je     loc_3_0F44
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ebx + 0x38], eax
        mov    dword ptr [esi + 0x18], edi
        mov    dword ptr [edi + 0x24], esi
        jmp    loc_3_0F55
loc_3_0F44:
        mov    eax, dword ptr [esi + 0x1a]
        mov    dword ptr [ebx + 0x38], eax
        mov    dword ptr [esi + 0x1a], edi
        call   sub_3_03D3
        mov    dword ptr [edi + 0x24], eax
loc_3_0F55:
        mov    byte ptr [edi + 2], 0x13
        mov    dword ptr [edi + 0xc], 0x20
        push   ebx
        call   sub_3_0463
        add    esp, 4
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h
loc_3_0F70:
        mov    eax, dword ptr [esp + 0x14]
        test   al, 8
        je     loc_3_0F80
        mov    byte ptr [esi + 0x24], 0x82
        jmp    loc_3_0F84
        db  08Bh, 0FFh
loc_3_0F80:
        mov    byte ptr [esi + 1], 0x82
loc_3_0F84:
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        mov    eax, 0xfffffffd
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_0FA0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        xor    eax, eax
        mov    al, byte ptr [esi + 9]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 8]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        push   eax
        call   sub_3_02B0
        add    esp, 0xc
        or     eax, eax
        je     loc_3_1020
        test   byte ptr [eax + 0x23], 8
        je     loc_3_0FE1
        mov    byte ptr [esi + 1], 0x82
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        sub    eax, eax
        pop    esi
        ret    
loc_3_0FE1:
        mov    cl, byte ptr [eax + 0x15]
        mov    byte ptr [esi + 0xb], cl
        mov    cl, byte ptr [eax + 0xdf]
        mov    byte ptr [esi + 0xc], cl
        mov    cl, byte ptr [eax + 0xe7]
        mov    byte ptr [esi + 0xd], cl
        test   byte ptr [eax + 0x20], 1
        mov    byte ptr [esi + 0xa], 1
        jne    loc_3_1007
        mov    byte ptr [esi + 0xa], 0
loc_3_1007:
        mov    byte ptr [esi + 1], 1
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        sub    eax, eax
        pop    esi
        ret    
        db  090h
loc_3_1020:
        mov    byte ptr [esi + 1], 0x82
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        push   esi
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        mov    eax, 0xfffffffe
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_3_1040:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   esi
        call   sub_3_0670
        add    esp, 8
        push   eax
        push   esi
        call   sub_3_041A
        add    esp, 8
        test   edi, 8
        jne    loc_3_1068
        mov    eax, esi
loc_3_1068:
        or     eax, eax
        je     loc_3_1120
        mov    byte ptr [eax + 1], 0
        xor    edx, edx
        mov    dl, byte ptr [0x8bf]          ; -> obj5:0x08BF
        mov    esi, 1
        mov    cl, byte ptr [eax + 2]
        shl    esi, cl
        test   esi, edx
        je     loc_3_1104
        cmp    cl, 0x1f
        ja     loc_3_1104
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        cmp    ecx, 6
        ja     loc_3_10A0
        jmp    dword ptr [ecx*4 + 0x1128]    ; -> obj3:0x1128
        db  090h
loc_3_10A0:
        mov    byte ptr [eax + 1], 0x80
        push   edi
        push   eax
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
loc_3_10B5:
        push   edi
        push   eax
        call   sub_3_0AA0
        add    esp, 8
        pop    edi
        pop    esi
        ret    
loc_3_10C2:
        push   edi
        push   eax
        call   sub_3_0C70
        add    esp, 8
        pop    edi
        pop    esi
        ret    
        db  090h
loc_3_10D0:
        push   edi
        push   eax
        call   sub_3_0CD0
        add    esp, 8
        pop    edi
        pop    esi
        ret    
loc_3_10DD:
        push   edi
        push   eax
        call   sub_3_0DC0
        add    esp, 8
        pop    edi
        pop    esi
        ret    
loc_3_10EA:
        push   edi
        push   eax
        call   sub_3_0EC0
        add    esp, 8
        pop    edi
        pop    esi
        ret    
loc_3_10F7:
        push   edi
        push   eax
        call   sub_3_0FA0
        add    esp, 8
        pop    edi
        pop    esi
        ret    
loc_3_1104:
        mov    byte ptr [eax + 1], 0x81
        push   edi
        push   eax
        push   0
        call   sub_3_06E0
        add    esp, 0xc
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h
loc_3_1120:
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        ret    
        ; ---- dispatch table (7 entries) ----
        dd  loc_3_10B5
        dd  loc_3_10C2
        dd  loc_3_10D0
        dd  loc_3_10DD
        dd  loc_3_10EA
        dd  loc_3_10A0
        dd  loc_3_10F7
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1150:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 0x20]
        mov    ebx, dword ptr [esi + 0x94]
        add    esi, 0x64
        test   bl, 1
        je     loc_3_117A
        xor    eax, eax
        mov    al, byte ptr [edi + 0xb]
        push   eax
        mov    eax, dword ptr [edi + 0x1c]
        push   eax
        call   sub_3_0480
        add    esp, 8
loc_3_117A:
        test   bl, 2
        je     loc_3_11B9
        test   bl, 4
        je     loc_3_11A9
        sub    edi, edi
loc_3_1186:
        mov    eax, dword ptr [esi + 0x1c]
        add    eax, edi
        mov    ebx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        or     ebx, ebx
        je     loc_3_11A5
        push   eax
        push   ebx
        call   sub_3_0480
        add    esp, 8
        add    edi, 8
        or     ebx, ebx
        jne    loc_3_1186
loc_3_11A5:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_11A9:
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        mov    eax, dword ptr [esi + 0x1c]
        push   eax
        call   sub_3_0480
        add    esp, 8
loc_3_11B9:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
loc_3_11C0:
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_3_022F
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_3_11D0:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, dword ptr [edi + 0x98]
        lea    ebx, [edi + 0x64]
        mov    ecx, dword ptr [esi + 0x1c]
        push   edi
        mov    al, byte ptr [ecx + 3]
        mov    byte ptr [esi + 7], al
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [esi + 0x34], al
        mov    eax, dword ptr [ebx + 0x38]
        mov    dword ptr [esi + 0x1c], eax
        call   sub_3_1150
        add    esp, 4
        test   byte ptr [esi + 0xc], 8
        jne    loc_3_122A
        cmp    dword ptr [esi + 0x1c], 0
        je     loc_3_122A
        mov    dword ptr [ebx + 0xc], 0x11c0 ; -> loc_3_11C0
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   eax
        push   esi
        call   sub_3_0480
        add    esp, 8
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_122A:
        push   edi
        call   sub_3_022F
        add    esp, 4
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   eax
        push   esi
        call   sub_3_0480
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1250:
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x38]
        mov    eax, dword ptr [esp + 0x38]
        add    esi, 0x64
        mov    ebx, dword ptr [eax + 0x20]
        mov    dword ptr [esp + 0x20], 0
        mov    edi, dword ptr [esp + 0x30]
        mov    dword ptr [esi + 0xc], 0x3d0  ; -> loc_1_03D0
        mov    eax, dword ptr [edi + 0x1c]
        mov    dword ptr [esi + 0x38], eax
        mov    dword ptr [esi + 0x34], edi
        mov    dword ptr [edi + 0x1c], ebx
        mov    byte ptr [ebx + 2], 0
        mov    dword ptr [ebx + 0xc], 0
        test   byte ptr [edi + 0xc], 0x40
        je     loc_3_129C
        mov    dword ptr [ebx + 0xc], 0x40
loc_3_129C:
        test   byte ptr [edi + 0xc], 0x80
        je     loc_3_12A9
        or     dword ptr [ebx + 0xc], 0x80
loc_3_12A9:
        test   byte ptr [edi + 0xc], 0x20
        je     loc_3_12B3
        or     dword ptr [ebx + 0xc], 0x20
loc_3_12B3:
        test   byte ptr [edi + 0xc], 2
        je     loc_3_12BD
        or     dword ptr [ebx + 0xc], 2
loc_3_12BD:
        test   byte ptr [edi + 0xd], 0x80
        je     loc_3_12C7
        or     dword ptr [ebx + 0xc], 4
loc_3_12C7:
        test   byte ptr [edi + 0xd], 0x20
        je     loc_3_12D1
        or     dword ptr [ebx + 0xc], 8
loc_3_12D1:
        test   byte ptr [edi + 0xd], 4
        je     loc_3_12DE
        or     dword ptr [ebx + 0xc], 0x100
loc_3_12DE:
        mov    eax, dword ptr [edi + 0xc]
        test   al, 1
        je     loc_3_1350
        test   eax, 0x400000
        mov    eax, dword ptr [edi + 0x3c]
        push   eax
        je     loc_3_12F7
        call   sub_3_0453
        jmp    loc_3_12FC
loc_3_12F7:
        call   sub_3_0637
loc_3_12FC:
        mov    dword ptr [esp + 0x14], eax
        add    esp, 4
        mov    eax, dword ptr [esp + 0x10]
        push   0xc
        push   eax
        call   sub_3_041A
        add    esp, 8
        or     eax, eax
        je     loc_3_1346
        lea    edx, [ebx + 0x30]
        mov    ecx, 3
        mov    eax, 4
loc_3_1323:
        mov    ebp, dword ptr [esp + 0x10]
        mov    ebp, dword ptr [ebp]
        mov    dword ptr [edx], ebp
        add    dword ptr [esp + 0x10], eax
        add    edx, eax
        dec    ecx
        jne    loc_3_1323
        mov    eax, dword ptr [esp + 0x10]
        push   0xc
        push   eax
        call   sub_3_0480
        add    esp, 8
        jmp    loc_3_1368
loc_3_1346:
        mov    byte ptr [edi + 7], 6
        jmp    loc_3_1368
        db  08Dh, 064h, 024h, 000h
loc_3_1350:
        lea    edx, [edi + 0x3c]
        lea    ecx, [ebx + 0x30]
        mov    eax, 3
loc_3_135B:
        mov    ebp, dword ptr [edx]
        mov    dword ptr [ecx], ebp
        add    edx, 4
        add    ecx, 4
        dec    eax
        jne    loc_3_135B
loc_3_1368:
        mov    al, byte ptr [edi + 0x2d]
        or     al, al
        jne    loc_3_1380
        mov    al, byte ptr [ebx + 0x30]
        shr    al, 5
        and    eax, 0xff
        mov    al, byte ptr [eax + 0x74]     ; -> loc_4_0074
loc_3_1380:
        mov    byte ptr [ebx + 0xa], al
        mov    eax, dword ptr [edi + 0x48]
        mov    dword ptr [ebx + 0x14], eax
        cmp    byte ptr [edi + 0x2c], 0
        je     loc_3_13E6
        mov    eax, dword ptr [edi + 0x28]
        or     eax, eax
        je     loc_3_13E6
        test   byte ptr [edi + 0xe], 0x10
        push   eax
        je     loc_3_13A4
        call   sub_3_0453
        jmp    loc_3_13A9
loc_3_13A4:
        call   sub_3_0637
loc_3_13A9:
        mov    dword ptr [esp + 0x14], eax
        add    esp, 4
        xor    eax, eax
        mov    al, byte ptr [edi + 0x2c]
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        call   sub_3_041A
        add    esp, 8
        or     eax, eax
        je     loc_3_13E0
        mov    dword ptr [esp + 0x20], 1
        mov    al, byte ptr [edi + 0x2c]
        mov    byte ptr [ebx + 0xb], al
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 0x1c], eax
        jmp    loc_3_13EA
        db  08Bh, 0FFh
loc_3_13E0:
        mov    byte ptr [edi + 7], 6
        jmp    loc_3_13EA
loc_3_13E6:
        or     dword ptr [ebx + 0xc], 0x20
loc_3_13EA:
        mov    dword ptr [ebx + 0x10], 0
        cmp    dword ptr [edi + 0x24], 0
        je     loc_3_1558
        mov    eax, dword ptr [edi + 0x20]
        or     eax, eax
        je     loc_3_1558
        test   byte ptr [edi + 0xe], 0x20
        push   eax
        je     loc_3_1414
        call   sub_3_0453
        jmp    loc_3_1419
loc_3_1414:
        call   sub_3_0637
loc_3_1419:
        mov    dword ptr [esp + 0x14], eax
        add    esp, 4
        test   byte ptr [edi + 0xc], 0x10
        je     loc_3_1435
        xor    eax, eax
        mov    ax, word ptr [edi + 0x2e]
        lea    ebp, [eax*8]
        jmp    loc_3_143F
loc_3_1435:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x1c], eax
        mov    ebp, dword ptr [edi + 0x24]
loc_3_143F:
        mov    eax, dword ptr [esp + 0x10]
        push   ebp
        push   eax
        call   sub_3_041A
        add    esp, 8
        or     eax, eax
        je     loc_3_1554
        or     dword ptr [esp + 0x20], 2
        test   byte ptr [edi + 0xc], 0x10
        je     loc_3_153D
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esp + 0x24], eax
        mov    dword ptr [esp + 0x28], ebp
        mov    dword ptr [esp + 0x1c], 0
        cmp    word ptr [edi + 0x2e], 0
        jbe    loc_3_1509
        mov    dword ptr [esp + 0x14], 0
        mov    ebp, eax
loc_3_1490:
        mov    eax, dword ptr [ebp]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [esp + 0x18], eax
        test   byte ptr [edi + 0xe], 0x20
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        je     loc_3_14B0
        call   sub_3_0453
        jmp    loc_3_14B5
loc_3_14B0:
        call   sub_3_0637
loc_3_14B5:
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esp + 0x1c]
        add    esp, 4
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        call   sub_3_041A
        add    esp, 8
        or     eax, eax
        je     loc_3_1505
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [edx + ecx + 4], eax
        add    ebp, 8
        mov    edx, dword ptr [esi + 0x1c]
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edx + ecx], eax
        add    dword ptr [esp + 0x14], 8
        xor    eax, eax
        inc    dword ptr [esp + 0x1c]
        mov    ax, word ptr [edi + 0x2e]
        cmp    eax, dword ptr [esp + 0x1c]
        ja     loc_3_1490
        jmp    loc_3_1509
loc_3_1505:
        mov    byte ptr [edi + 7], 6
loc_3_1509:
        mov    eax, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [eax + ecx*8 + 4], 0
        mov    eax, dword ptr [esi + 0x1c]
        or     dword ptr [esp + 0x20], 4
        mov    dword ptr [eax + ecx*8], 0
        mov    eax, dword ptr [esp + 0x28]
        push   eax
        mov    eax, dword ptr [esp + 0x28]
        push   eax
        call   sub_3_0480
        add    esp, 8
        or     dword ptr [esi + 8], 2
loc_3_153D:
        mov    eax, dword ptr [edi + 0x24]
        mov    dword ptr [ebx + 0x10], eax
        mov    eax, dword ptr [ebx + 0xc]
        test   al, 0xc0
        jne    loc_3_1558
        or     eax, 0xc0
        mov    dword ptr [ebx + 0xc], eax
        jmp    loc_3_1558
loc_3_1554:
        mov    byte ptr [edi + 7], 6
loc_3_1558:
        call   sub_3_03E9
        mov    dword ptr [ebx + 0x24], eax
        or     dword ptr [esi + 8], 0x200
        mov    eax, dword ptr [ebx + 0x10]
        mov    dword ptr [esi + 0x18], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esi + 0x30], eax
        cmp    byte ptr [edi + 7], 6
        jne    loc_3_158D
        mov    eax, dword ptr [esi + 0x38]
        mov    dword ptr [edi + 0x1c], eax
        mov    eax, dword ptr [esp + 0x38]
        push   eax
        call   sub_3_1150
        add    esp, 4
loc_3_158D:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x1c
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_15A0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        push   esi
        call   sub_3_02EC
        add    esp, 4
        mov    edi, eax
        push   edi
        mov    ebx, dword ptr [esp + 0x14]
        push   esi
        push   ebx
        call   sub_3_1250
        add    esp, 0xc
        mov    esi, eax
        cmp    byte ptr [ebx + 7], 6
        je     loc_3_15E3
        push   esi
        call   sub_3_0400
        add    esp, 4
        or     eax, eax
        je     loc_3_15E3
        push   esi
        call   sub_3_0463
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_15E3:
        mov    byte ptr [ebx + 7], 6
        xor    eax, eax
        mov    ax, word ptr [ebx + 4]
        push   eax
        push   ebx
        call   sub_3_0480
        add    esp, 8
        push   edi
        call   sub_3_022F
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1610:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_3_0637
        mov    ebx, eax
        mov    esi, dword ptr [esp + 0x18]
        add    esp, 4
        mov    al, byte ptr [esi + 0x40]
        mov    byte ptr [edi + 0x14], al
        or     ebx, ebx
        je     loc_3_1669
        push   0x24
        push   ebx
        call   sub_3_041A
        add    esp, 8
        or     eax, eax
        je     loc_3_1665
        sub    ecx, ecx
loc_3_1644:
        mov    al, byte ptr [ecx + esi + 0x82]
        mov    byte ptr [ecx + ebx], al
        inc    ecx
        cmp    ecx, 0x24
        jl     loc_3_1644
        push   0x24
        push   ebx
        call   sub_3_0480
        add    esp, 8
        mov    byte ptr [edi + 7], 1
        jmp    loc_3_1669
loc_3_1665:
        mov    byte ptr [edi + 7], 6
loc_3_1669:
        xor    eax, eax
        mov    ax, word ptr [edi + 4]
        push   eax
        push   edi
        call   sub_3_0480
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_3_1680:
        push   ebx
        mov    ebx, 8
        xor    eax, eax
        mov    al, byte ptr [0x8be]          ; -> obj5:0x08BE
        lea    ecx, [ecx]
loc_3_1690:
        mov    edx, 1
        lea    ecx, [ebx - 1]
        shl    edx, cl
        test   eax, edx
        jne    loc_3_16A1
        dec    ebx
        jne    loc_3_1690
loc_3_16A1:
        mov    edx, dword ptr [esp + 8]
        mov    byte ptr [edx + 0x2c], bl
        cmp    byte ptr [edx + 9], 0xff
        jne    loc_3_16B0
        jmp    loc_3_1702
loc_3_16B0:
        mov    byte ptr [edx + 0x10], 0x30
        mov    byte ptr [edx + 0x11], 0
        mov    byte ptr [edx + 0x12], 0
        mov    byte ptr [edx + 0x13], 0
        sub    eax, eax
        mov    word ptr [edx + 0x14], 0
        mov    dword ptr [edx + 0x24], 0
        mov    dword ptr [edx + 0x28], 0
        mov    byte ptr [edx + 0x2d], 7
        lea    ebx, [ebx]
loc_3_16E0:
        lea    ecx, [edx + eax]
        mov    bl, byte ptr [eax + 0x54]     ; -> loc_4_0054
        mov    byte ptr [ecx + 0x30], bl
        mov    bl, byte ptr [eax + 0x64]     ; -> loc_4_0064
        mov    byte ptr [ecx + 0x40], bl
        inc    eax
        cmp    eax, 0x10
        jl     loc_3_16E0
        mov    dword ptr [edx + 0x50], 0
loc_3_1702:
        mov    byte ptr [edx + 7], 1
        xor    eax, eax
        mov    ax, word ptr [edx + 4]
        push   eax
        push   edx
        call   sub_3_0480
        add    esp, 8
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_3_1720:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        call   sub_3_02EC
        mov    edi, eax
        add    esp, 4
        mov    ebx, dword ptr [edi + 0x20]
        or     dword ptr [edi + 0x6c], 0x100
        lea    eax, [edi + 0x64]
        mov    esi, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x34], esi
        mov    byte ptr [ebx + 2], 4
        mov    dword ptr [ebx + 0xc], 0
        push   eax
        call   sub_3_0463
        add    esp, 4
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [esi + 7], al
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   eax
        push   esi
        call   sub_3_0480
        add    esp, 8
        push   edi
        call   sub_3_022F
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_3_1780:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        push   eax
        call   sub_3_02EC
        mov    ebx, eax
        add    esp, 4
        mov    esi, dword ptr [ebx + 0x20]
        mov    edi, dword ptr [esp + 0x14]
        lea    ebp, [ebx + 0x64]
        mov    eax, dword ptr [edi + 0x10]
        push   eax
        call   sub_3_0637
        add    esp, 4
        mov    eax, dword ptr [eax + 0x1c]
        or     dword ptr [ebp + 8], 0x100
        mov    dword ptr [ebp + 0x34], edi
        mov    byte ptr [esi + 2], 0x10
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [esi + 0xc], 0
        push   ebp
        call   sub_3_0463
        add    esp, 4
        mov    al, byte ptr [esi + 3]
        mov    byte ptr [edi + 7], al
        xor    eax, eax
        mov    ax, word ptr [edi + 4]
        push   eax
        push   edi
        call   sub_3_0480
        add    esp, 8
        push   ebx
        call   sub_3_022F
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1800:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        call   sub_3_02EC
        mov    edi, eax
        add    esp, 4
        mov    ebx, dword ptr [edi + 0x20]
        or     dword ptr [edi + 0x6c], 0x100
        lea    eax, [edi + 0x64]
        mov    esi, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x34], esi
        mov    byte ptr [ebx + 2], 0x12
        mov    dword ptr [ebx + 0xc], 0
        push   eax
        call   sub_3_0463
        add    esp, 4
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [esi + 7], al
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   eax
        push   esi
        call   sub_3_0480
        add    esp, 8
        push   edi
        call   sub_3_022F
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_3_1860:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        call   sub_3_02EC
        mov    edi, eax
        add    esp, 4
        mov    ebx, dword ptr [edi + 0x20]
        or     dword ptr [edi + 0x6c], 0x100
        lea    eax, [edi + 0x64]
        mov    esi, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x34], esi
        mov    byte ptr [ebx + 2], 0x13
        mov    dword ptr [ebx + 0xc], 0
        push   eax
        call   sub_3_0463
        add    esp, 4
        mov    al, byte ptr [ebx + 3]
        mov    byte ptr [esi + 7], al
        xor    eax, eax
        mov    ax, word ptr [esi + 4]
        push   eax
        push   esi
        call   sub_3_0480
        add    esp, 8
        push   edi
        call   sub_3_022F
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 049h, 000h
sub_3_18C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    ax, word ptr [edx + 4]
        push   eax
        push   edx
        call   sub_3_041A
        add    esp, 8
        or     eax, eax
        je     loc_3_1AB6
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [edx + 7], 0
        cmp    byte ptr [edx + 6], 3
        je     loc_3_19A0
        mov    cl, byte ptr [edx + 9]
        xor    eax, eax
        mov    al, byte ptr [0x8be]          ; -> obj5:0x08BE
        mov    edx, 1
        shl    edx, cl
        test   edx, eax
        mov    edx, dword ptr [ebp + 8]
        je     loc_3_1980
        xor    eax, eax
        mov    al, byte ptr [edx + 0xb]
        push   eax
        xor    eax, eax
        mov    al, byte ptr [edx + 0xa]
        push   eax
        xor    eax, eax
        mov    al, cl
        push   eax
        call   sub_3_02B8
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_1950
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    byte ptr [edx + 7], 8
        mov    ax, word ptr [edx + 4]
        push   eax
        push   edx
        call   sub_3_0480
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_1950:
        test   byte ptr [eax + 0x23], 8
        je     loc_3_19E3
        int3   
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [edx + 7], 8
        xor    eax, eax
        mov    ax, word ptr [edx + 4]
        push   eax
        push   edx
        call   sub_3_0480
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_3_1980:
        mov    byte ptr [edx + 7], 7
        xor    eax, eax
        mov    ax, word ptr [edx + 4]
        push   eax
        push   edx
        call   sub_3_0480
        add    esp, 8
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_19A0:
        mov    edx, dword ptr [ebp + 8]
        mov    cl, byte ptr [edx + 9]
        cmp    cl, 0xff
        je     loc_3_19E0
        xor    eax, eax
        mov    al, byte ptr [0x8be]          ; -> obj5:0x08BE
        mov    edx, 1
        shl    edx, cl
        test   edx, eax
        jne    loc_3_19E0
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    byte ptr [edx + 7], 7
        mov    ax, word ptr [edx + 4]
        push   eax
        push   edx
        call   sub_3_0480
        add    esp, 8
        mov    eax, 0xfffffffe
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_19E0:
        mov    eax, dword ptr [ebp - 4]
loc_3_19E3:
        mov    edx, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [edx + 6]
        dec    ecx
        cmp    ecx, 0x11
        ja     loc_3_19F8
        jmp    dword ptr [ecx*4 + 0x1ac4]    ; -> obj3:0x1AC4
loc_3_19F8:
        mov    edx, dword ptr [ebp + 8]
        xor    eax, eax
        mov    byte ptr [edx + 7], 0x16
        mov    ax, word ptr [edx + 4]
        push   eax
        push   edx
        call   sub_3_0480
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_1A18:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_3_15A0
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_3_1A30:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_3_1610
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_1A46:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_3_1680
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_1A5C:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_3_1720
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_1A72:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_3_1780
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_1A88:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_3_1800
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_3_1AA0:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   edx
        call   sub_3_1860
        add    esp, 8
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_1AB6:
        mov    eax, 0xffffffff
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
        ; ---- dispatch table (18 entries) ----
        dd  loc_3_1A18
        dd  loc_3_1A30
        dd  loc_3_1A46
        dd  loc_3_1A5C
        dd  loc_3_19F8
        dd  loc_3_19F8
        dd  loc_3_19F8
        dd  loc_3_19F8
        dd  loc_3_19F8
        dd  loc_3_19F8
        dd  loc_3_19F8
        dd  loc_3_19F8
        dd  loc_3_19F8
        dd  loc_3_19F8
        dd  loc_3_19F8
        dd  loc_3_1A72
        dd  loc_3_1A88
        dd  loc_3_1AA0
        db  08Dh, 064h, 024h, 000h
sub_3_1B10:
        push   esi
        push   edi
        xor    eax, eax
        mov    al, byte ptr [esp + 0xc]
        cmp    eax, 1
        je     loc_3_1B25
        cmp    eax, 3
        je     loc_3_1B42
        pop    edi
        pop    esi
        ret    
loc_3_1B25:
        mov    esi, 0xbc                     ; -> obj5:0x00BC
        inc    byte ptr [0x8bd]              ; -> obj5:0x08BD
        mov    edi, dword ptr [esp + 0x10]
        mov    al, 1
        mov    cl, byte ptr [edi]
        shl    al, cl
        or     byte ptr [0x8bf], al          ; -> obj5:0x08BF
        jmp    loc_3_1B5D
loc_3_1B42:
        mov    esi, 0x4bc                    ; -> obj5:0x04BC
        inc    byte ptr [0x8bc]              ; -> obj5:0x08BC
        mov    edi, dword ptr [esp + 0x10]
        mov    al, 1
        mov    cl, byte ptr [edi]
        shl    al, cl
        or     byte ptr [0x8be], al          ; -> obj5:0x08BE
loc_3_1B5D:
        mov    eax, dword ptr [0x80]         ; -> obj4:0x0080
        cmp    dword ptr [0x89], eax         ; -> obj5:0x0089
        jne    loc_3_1B8C
        push   eax
        call   sub_3_1BB0
        mov    ecx, dword ptr [esp + 0x18]
        add    esp, 4
        mov    dword ptr [eax + 9], ecx      ; -> obj5:0x0009
        cmp    dword ptr [0x80], eax         ; -> obj4:0x0080
        jne    loc_3_1B8C
        add    dword ptr [0x89], 4           ; -> obj5:0x0089
loc_3_1B8C:
        xor    edx, edx
        mov    dl, byte ptr [edi + 1]
        xor    ecx, ecx
        mov    cl, byte ptr [edi]
        shl    ecx, 4
        add    edx, ecx
        xor    ecx, ecx
        mov    cl, byte ptr [edi + 2]
        pop    edi
        lea    edx, [ecx + edx*8]
        mov    byte ptr [esi + edx], al
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_3_1BB0:
        sub    eax, eax
        mov    ecx, dword ptr [esp + 4]
        cmp    ecx, eax
        jle    loc_3_1BCD
        mov    edx, 9                        ; -> obj5:0x0009
        nop    
loc_3_1BC0:
        cmp    dword ptr [edx + eax], 0
        je     loc_3_1BCD
        add    eax, 4
        cmp    eax, ecx
        jl     loc_3_1BC0
loc_3_1BCD:
        ret    
        db  08Bh, 0FFh
sub_3_1BD0:
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [eax], 0
        je     loc_3_1BF9
        mov    edx, dword ptr [esp + 8]
        cmp    byte ptr [edx], 0
        je     loc_3_1BF9
        mov    ecx, dword ptr [eax]
        cmp    dword ptr [edx], ecx
        jne    loc_3_1BF6
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [edx + 4], eax
        jne    loc_3_1BF6
        mov    eax, 1
        ret    
loc_3_1BF6:
        sub    eax, eax
        ret    
loc_3_1BF9:
        mov    eax, 0xffffffff
        ret    
        db  090h
sub_3_1C00:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [esp + 0x10], 0
        mov    edi, dword ptr [eax + 5]
        or     edi, edi
        je     loc_3_1CC5
        mov    ebp, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [esp + 0x20]
loc_3_1C26:
        or     ebp, ebp
        je     loc_3_1C35
        mov    eax, dword ptr [ebp]
        cmp    dword ptr [edi], eax
        jne    loc_3_1CBA
loc_3_1C35:
        cmp    byte ptr [edi + 4], 0
        jne    loc_3_1C90
        lea    eax, [esi + 0xa6]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        add    eax, 0x15
        push   eax
        call   sub_3_1BD0
        add    esp, 8
        or     eax, eax
        je     loc_3_1CBA
        mov    al, byte ptr [edi + 1]
        cmp    byte ptr [esi + 0x7c], al
        jne    loc_3_1CBA
        mov    al, byte ptr [esi + 0x7d]
        cmp    byte ptr [edi + 2], al
        jne    loc_3_1CBA
        xor    eax, eax
        mov    ax, word ptr [edi + 0xc]
        cmp    eax, dword ptr [0x84]         ; -> obj4:0x0084
        jne    loc_3_1CBA
        mov    al, byte ptr [edi + 0xf]
        cmp    byte ptr [esi + 0x82], al
        jne    loc_3_1CBA
        mov    al, byte ptr [edi + 0xe]
        cmp    byte ptr [esi + 0x7e], al
        je     loc_3_1CD8
        mov    dword ptr [esp + 0x10], edi
        jmp    loc_3_1CBA
        db  08Dh, 049h, 000h
loc_3_1C90:
        mov    ebx, dword ptr [0x7c]         ; -> obj4:0x007C
        or     ebx, ebx
        je     loc_3_1CBA
loc_3_1C9A:
        mov    al, byte ptr [edi + 4]
        cmp    byte ptr [ebx], al
        jne    loc_3_1CB3
        push   esi
        lea    eax, [edi + 8]
        push   eax
        push   ebx
        call   sub_3_1C00
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_1CE2
loc_3_1CB3:
        mov    ebx, dword ptr [ebx + 1]
        or     ebx, ebx
        jne    loc_3_1C9A
loc_3_1CBA:
        mov    edi, dword ptr [edi + 0x10]
        or     edi, edi
        jne    loc_3_1C26
loc_3_1CC5:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_1CFD
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_3_1CD8:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_3_1CE2:
        push   esi
        push   eax
        mov    al, byte ptr [ebx]
        push   eax
        call   sub_3_1B10
        add    esp, 0xc
        or     dword ptr [ebx + 0x11], 1
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_3_1CFD:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1D10:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [0x89]         ; -> obj5:0x0089
        lea    edx, [esi + 0x82]
        mov    dword ptr [0x80], eax         ; -> obj4:0x0080
        mov    edi, dword ptr [esp + 0x18]
        cmp    dword ptr [0x7c], edi         ; -> obj4:0x007C
        je     loc_3_1D3C
        mov    dword ptr [0x7c], edi         ; -> obj4:0x007C
loc_3_1D3C:
        mov    dword ptr [0x84], 0           ; -> obj4:0x0084
        mov    eax, 0x24
loc_3_1D4B:
        xor    ecx, ecx
        mov    cl, byte ptr [edx]
        inc    edx
        add    dword ptr [0x84], ecx         ; -> obj4:0x0084
        dec    eax
        jne    loc_3_1D4B
        mov    ebx, 1
        or     edi, edi
        je     loc_3_1D8C
loc_3_1D62:
        push   esi
        push   0
        push   edi
        call   sub_3_1C00
        add    esp, 0xc
        or     eax, eax
        je     loc_3_1D85
        or     dword ptr [edi + 0x11], 1
        push   esi
        push   eax
        mov    al, byte ptr [edi]
        sub    ebx, ebx
        push   eax
        call   sub_3_1B10
        add    esp, 0xc
loc_3_1D85:
        mov    edi, dword ptr [edi + 1]
        or     edi, edi
        jne    loc_3_1D62
loc_3_1D8C:
        push   esi
        call   sub_3_1F20
        add    esp, 4
        or     ebx, ebx
        je     loc_3_1DC0
        mov    byte ptr [esp + 0xc], al
        mov    al, byte ptr [esi + 0x7c]
        mov    byte ptr [esp + 0xd], al
        mov    al, byte ptr [esi + 0x7d]
        mov    byte ptr [esp + 0xe], al
        mov    byte ptr [esp + 0xf], 0
        lea    eax, [esp + 0xc]
        push   esi
        push   eax
        push   1
        call   sub_3_1B10
        add    esp, 0xc
loc_3_1DC0:
        call   sub_3_1EB0
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h
sub_3_1DD0:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   0xbc                          ; -> obj5:0x00BC
        push   esi
        call   sub_3_1E30
        add    esp, 8
        mov    bl, al
        push   0x4bc                         ; -> obj5:0x04BC
        push   esi
        call   sub_3_1E30
        add    esp, 8
        cmp    bl, 0xff
        je     loc_3_1DFF
        dec    byte ptr [0x8bd]              ; -> obj5:0x08BD
loc_3_1DFF:
        cmp    al, 0xff
        je     loc_3_1E09
        dec    byte ptr [0x8bc]              ; -> obj5:0x08BC
loc_3_1E09:
        cmp    bl, 0xff
        jne    loc_3_1E10
        mov    bl, al
loc_3_1E10:
        xor    eax, eax
        mov    al, bl
        pop    esi
        mov    dword ptr [eax + 9], 0        ; -> obj5:0x0009
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1E30:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dl, 0xff
        sub    esi, esi
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x18]
loc_3_1E40:
        lea    edi, [ecx + esi]
        mov    bl, byte ptr [edi]
        cmp    bl, 0xff
        je     loc_3_1E5F
        mov    ebp, ebx
        and    ebp, 0xff
        cmp    dword ptr [ebp + 9], eax      ; -> obj5:0x0009
        jne    loc_3_1E5F
        mov    dl, bl
        mov    byte ptr [edi], 0xff
loc_3_1E5F:
        inc    esi
        cmp    esi, 0x400
        jl     loc_3_1E40
        xor    eax, eax
        mov    al, dl
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1E80:
        push   esi
        mov    esi, dword ptr [0x7c]         ; -> obj4:0x007C
        or     esi, esi
        je     loc_3_1EAC
loc_3_1E8B:
        cmp    byte ptr [esi], 1
        jne    loc_3_1EA5
        mov    eax, dword ptr [esi + 0x11]
        and    al, 0x11
        cmp    al, 0x11
        jne    loc_3_1EA5
        mov    eax, dword ptr [esi + 9]
        push   eax
        call   sub_3_0021
        add    esp, 4
loc_3_1EA5:
        mov    esi, dword ptr [esi + 1]
        or     esi, esi
        jne    loc_3_1E8B
loc_3_1EAC:
        sub    eax, eax
        pop    esi
        ret    
sub_3_1EB0:
        push   esi
        push   edi
        mov    edi, dword ptr [0x7c]         ; -> obj4:0x007C
        or     edi, edi
        je     loc_3_1EF4
        mov    esi, 0x8c0                    ; -> obj5:0x08C0
loc_3_1EC1:
        cmp    byte ptr [edi], 1
        jne    loc_3_1EED
        mov    eax, dword ptr [edi + 0x11]
        and    al, 0x11
        cmp    al, 1
        jne    loc_3_1EDF
        mov    eax, dword ptr [edi + 9]
        push   eax
        call   0
        add    esp, 4
        or     dword ptr [edi + 0x11], 0x10
loc_3_1EDF:
        mov    eax, dword ptr [edi + 9]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edi + 0xd]
        mov    dword ptr [esi + 4], eax
        add    esi, 8
loc_3_1EED:
        mov    edi, dword ptr [edi + 1]
        or     edi, edi
        jne    loc_3_1EC1
loc_3_1EF4:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Bh, 044h, 024h, 004h, 00Bh, 0C0h, 074h, 00Ch, 080h
        db  038h, 001h, 074h, 00Dh, 08Bh, 040h, 001h, 00Bh, 0C0h, 075h, 0F4h, 0B8h, 001h, 000h, 000h, 000h
        db  0C3h, 02Bh, 0C0h, 0C3h, 08Dh, 049h, 000h
sub_3_1F20:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        cmp    dword ptr [0x88], edi         ; -> obj4:0x0088
        jle    loc_3_1F80
        mov    esi, dword ptr [esp + 0x14]
        mov    ebp, 0x8c                     ; -> obj4:0x008C
        lea    ebx, [esi + 0xa6]
loc_3_1F3D:
        push   ebx
        push   ebp
        call   sub_3_1FD0
        add    esp, 8
        or     eax, eax
        je     loc_3_1F65
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0x7e]
        movsx  eax, byte ptr [ebp + 8]
        cmp    eax, ecx
        jne    loc_3_1F65
        xor    ecx, ecx
        mov    cl, byte ptr [esi + 0x75]
        movsx  eax, byte ptr [ebp + 9]
        cmp    eax, ecx
        je     loc_3_1F73
loc_3_1F65:
        add    ebp, 0xa
        inc    edi
        cmp    edi, dword ptr [0x88]         ; -> obj4:0x0088
        jl     loc_3_1F3D
        jmp    loc_3_1F84
loc_3_1F73:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_1F80:
        mov    esi, dword ptr [esp + 0x14]
loc_3_1F84:
        inc    dword ptr [0x88]              ; -> obj4:0x0088
        lea    eax, [edi + edi*4]
        lea    ebx, [eax*2]
        lea    eax, [esi + 0xa6]
        push   eax
        lea    eax, [ebx + 0x8c]             ; -> obj4:0x008C
        push   eax
        call   sub_3_2030
        add    esp, 8
        mov    al, byte ptr [esi + 0x7e]
        mov    byte ptr [ebx + 0x94], al     ; -> obj4:0x0094
        mov    al, byte ptr [esi + 0x75]
        mov    byte ptr [ebx + 0x95], al     ; -> loc_4_0095
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_1FD0:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        cmp    dword ptr [edx], ecx
        jne    loc_3_1FEC
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [edx + 4], eax
        jne    loc_3_1FEC
        mov    eax, 1
        ret    
loc_3_1FEC:
        sub    eax, eax
        ret    
        db  090h
sub_3_1FF0:
        push   ebx
        sub    edx, edx
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0xc]
loc_3_1FFB:
        mov    bl, byte ptr [edx + eax]
        or     bl, bl
        jne    loc_3_2006
        or     edx, edx
        jg     loc_3_2018
loc_3_2006:
        cmp    byte ptr [edx + ecx], bl
        jne    loc_3_2020
        inc    edx
        cmp    edx, 8
        jl     loc_3_1FFB
        mov    eax, 1
        pop    ebx
        ret    
loc_3_2018:
        mov    eax, 1
        pop    ebx
        ret    
        db  090h
loc_3_2020:
        sub    eax, eax
        pop    ebx
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_2030:
        mov    eax, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [edx], ecx
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [edx + 4], eax
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_3_2050:
        mov    dword ptr [0x444], 0x2270     ; -> obj1:0x0444
        mov    dword ptr [0x448], 0x2270     ; -> obj1:0x0448
        mov    dword ptr [0x44c], 0x2270     ; -> obj1:0x044C
        mov    dword ptr [0x450], 0x2270     ; -> obj1:0x0450
        mov    dword ptr [0x454], 0x2270     ; -> obj1:0x0454
        mov    dword ptr [0x458], 0x21b0     ; -> obj1:0x0458
        mov    dword ptr [0x45c], 0x2270     ; -> obj1:0x045C
        mov    dword ptr [0x460], 0x2270     ; -> obj1:0x0460
        mov    dword ptr [0x464], 0x2270     ; -> obj1:0x0464
        mov    dword ptr [0x468], 0x2270     ; -> obj1:0x0468
        mov    dword ptr [0x46c], 0x2270     ; -> obj1:0x046C
        mov    dword ptr [0x470], 0x2200     ; -> obj1:0x0470
        mov    dword ptr [0x474], 0x2270     ; -> obj1:0x0474
        mov    dword ptr [0x478], 0x2270     ; -> obj1:0x0478
        mov    dword ptr [0x47c], 0x2270     ; -> obj1:0x047C
        mov    dword ptr [0xe0], 0x2270      ; -> loc_4_00E0
        mov    dword ptr [0xe4], 0x2250      ; -> loc_4_00E4
        mov    dword ptr [0xe8], 0x2270      ; -> loc_4_00E8
        mov    dword ptr [0xec], 0x2270      ; -> loc_4_00EC
        mov    dword ptr [0xf0], 0x2270      ; -> loc_4_00F0
        mov    dword ptr [0xf4], 0x2270      ; -> loc_4_00F4
        mov    dword ptr [0xf8], 0x2270      ; -> loc_4_00F8
        mov    dword ptr [0xfc], 0x2270      ; -> loc_4_00FC
        mov    dword ptr [0x10c], 0xffffffff ; -> loc_4_010C
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2150:
        cmp    dword ptr [0x10c], 0          ; -> loc_4_010C
        jne    loc_3_215E
        call   sub_3_2050
loc_3_215E:
        mov    ecx, dword ptr [esp + 4]
        xor    eax, eax
        mov    al, byte ptr [ecx + 4]
        or     eax, eax
        je     loc_3_2177
        lea    eax, [ecx + 0x1a]
        mov    word ptr [eax], 1
        movsx  eax, word ptr [eax]
        ret    
loc_3_2177:
        mov    eax, dword ptr [ecx + 0x10]
        mov    dword ptr [0x100], eax        ; -> loc_4_0100
        mov    eax, dword ptr [ecx + 0x28]
        mov    dword ptr [0x104], eax        ; -> loc_4_0104
        mov    eax, dword ptr [ecx + 0x20]
        mov    dword ptr [0x108], eax        ; -> loc_4_0108
        lea    eax, [ecx + 0x1a]
        mov    dword ptr [ecx + 0x1c], 0xf090a0b
        mov    word ptr [eax], 0
        mov    dword ptr [ecx + 0x10], 0x440 ; -> obj1:0x0440
        mov    word ptr [ecx + 6], 0
        movsx  eax, word ptr [eax]
        ret    
        db  090h
loc_3_21B0:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x10]
        mov    dword ptr [ecx], 0x2270       ; -> loc_3_2270
        mov    dword ptr [ecx + 4], 0x2270   ; -> loc_3_2270
        mov    dword ptr [ecx + 8], 0xdc     ; -> loc_4_00DC
        mov    dword ptr [eax + 0x20], 0xf090a0b
        mov    word ptr [eax + 0x18], 1
        mov    word ptr [eax + 0x28], 0
        mov    word ptr [eax + 0x2a], 0
        mov    dword ptr [eax + 0x24], 0
        mov    dword ptr [eax], 0
        mov    word ptr [eax + 0x1a], 0
        sub    eax, eax
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_2200:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esi + 0x28]
        cmp    byte ptr [esi + 4], 2
        jne    loc_3_2237
        cmp    word ptr [edi + 0x18], 4
        jb     loc_3_2237
        call   sub_3_065A
        mov    ecx, dword ptr [0x900]        ; -> obj5:0x0900
        mov    dword ptr [eax], ecx
        add    esi, 0x1a
        and    word ptr [edi + 0x2c], 0xfffe
        mov    word ptr [esi], 0
        movsx  eax, word ptr [esi]
        pop    edi
        pop    esi
        ret    
loc_3_2237:
        add    esi, 0x1a
        mov    word ptr [esi], 1
        or     word ptr [edi + 0x2c], 1
        movsx  eax, word ptr [esi]
        pop    edi
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_3_2250:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x1a], 0
        mov    dword ptr [eax + 0x24], 0
        sub    eax, eax
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_3_2270:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0x1a], 1
        mov    eax, 1
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x110  R-X
loc_4_0000:
        inc    ecx
        push   ebx
        push   eax
        dec    ecx
        and    byte ptr [esi + 0x6f], ah
        jb     0x29
        push   edi
        db  069h, 06Eh, 064h, 06Fh, 077h, 073h
loc_4_0010:
        inc    ecx
        push   ebx
        push   eax
        dec    ecx
        and    byte ptr [esi + 0x6f], ah
        jb     0x39
        push   edi
        dec    ecx
        dec    esi
        xor    esi, dword ptr [edx]
        and    byte ptr [eax], ah
loc_4_0020:
        cmp    cl, byte ptr [ebx]
        inc    eax
        or     al, 0x40
        sbb    byte ptr [eax], bl
        db  000h
loc_4_0028:
        cmp    al, 0xc
        inc    eax
        or     al, 0x40
        sbb    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    dword ptr [edx], eax
        add    dword ptr [esp + eax], eax
        add    byte ptr [ecx + 0x4110482], 0
        add    al, 4
        add    al, 4
        add    al, 0x12
        adc    edx, dword ptr [eax + eax*4]
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  080h, 080h, 080h, 004h
loc_4_0054:
        dec    ebp
        dec    ecx
        inc    ebx
        push   edx
        dec    edi
        push   ebx
        dec    edi
        inc    esi
        push   esp
        sub    al, 0x20
        inc    ebx
        dec    edi
        push   edx
        push   eax
        db  000h
loc_4_0064:
        dec    edi
        inc    ebp
        dec    ebp
        and    byte ptr [ebx + 0x43], dl
        push   ebx
        dec    ecx
        and    byte ptr [eax + 0x42], cl
        inc    ecx
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
loc_4_0074:
        push   es
        or     cl, byte ptr [edx]
        or     al, 0xc
        or     al, 0xa
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_008B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0095:
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
loc_4_00DC:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
loc_4_00E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0100:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0104:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0108:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_010C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x904  RW-
        db  053h, 043h, 053h, 049h, 04Dh, 047h, 052h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 012h
        db  000h, 053h, 000h, 043h, 000h, 053h, 000h, 049h, 000h, 04Dh, 000h, 047h, 000h, 052h, 000h, 024h
        db  000h, 000h, 000h
        dd  obj5:0x008F              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 022h, 041h, 050h, 049h, 058h, 020h, 056h, 069h, 072h, 074h, 075h
        db  061h, 06Ch, 020h, 044h, 065h, 076h, 069h, 063h, 065h, 020h, 020h, 028h, 056h, 065h, 072h, 073h
        db  069h, 06Fh, 06Eh, 020h, 034h, 02Eh, 030h, 029h, 000h, 000h, 008h, 041h, 050h, 049h, 058h, 05Fh
        db  044h, 044h, 042h, 001h, 000h, 000h, 0FFh, 010h, 000h, 0FFh, 001h, 000h, 030h, 000h, 0E0h, 001h
        db  000h, 000h, 0E0h, 001h, 034h, 000h, 056h, 053h, 05Fh, 056h, 045h, 052h, 053h
VxD__LDATA ENDS
