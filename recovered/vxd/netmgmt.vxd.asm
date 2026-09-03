; NETMGMT -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 321 instructions recovered by recursive descent

Declare_Virtual_Device NETMGMT, 1, 0, NETMGMT_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 0FFFFFFFFh   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xec  R-X
NETMGMT_Control:
        cmp    eax, 2
        jne    loc_1_000E
        call   sub_2_0000
        cmp    eax, 1
        ret    
loc_1_000E:
        cmp    eax, 0x1b
        jne    loc_1_001C
        call   sub_2_0000
        cmp    eax, 1
        ret    
loc_1_001C:
        cmp    eax, 0x1c
        jne    loc_1_002A
        call   sub_3_0199
        cmp    eax, 1
        ret    
loc_1_002A:
        cmp    eax, 0x23
        jne    loc_1_003C
        push   esi
        push   edx
        push   ebx
        push   ecx
        call   sub_3_0020
        cmp    eax, 1
        ret    
loc_1_003C:
        clc    
        ret    
        db  000h, 000h
loc_1_0040:
        mov    ecx, dword ptr [esp + 8]
        cmp    ecx, 0xc0                     ; -> obj1:0x00C0
        jne    loc_1_005E
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [0xe8], eax         ; -> obj1:0x00E8
        mov    dword ptr [ecx + 0x10], 0xffffffff
        jmp    loc_1_006D
loc_1_005E:
        push   dword ptr [esp + 0xc]
        push   ecx
        push   dword ptr [esp + 0xc]
        call   sub_4_0014
loc_1_006D:
        ret    0xc
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'NETMGMT '                  ; DDB_Name
        dd  0FFFFFFFFh                  ; DDB_Init_Order
        dd  NETMGMT_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x20  R-X
sub_2_0000:
        push   1
        push   0
        call   sub_5_0000
        add    esp, 8
        mov    dword ptr [0x10], eax         ; -> obj4:0x0010
        or     eax, eax
        mov    eax, 0
        je     loc_2_001F
        mov    eax, 1
loc_2_001F:
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x359  R-X
        db  0CDh, 020h, 055h, 080h, 028h, 000h, 0CDh, 020h, 030h, 080h, 028h, 000h
sub_3_000C:
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_0020:
        mov    ecx, dword ptr [esp + 4]
        or     ecx, ecx
        jne    loc_3_002C
        sub    eax, eax
        jmp    loc_3_0055
loc_3_002C:
        cmp    ecx, -1
        jne    loc_3_0038
        call   sub_3_01AD
        jmp    loc_3_0055
loc_3_0038:
        cmp    ecx, 4
        mov    eax, 0x32
        ja     loc_3_0055
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        call   dword ptr [ecx*4 - 4]         ; -> obj4:0xFFFFFFFC
loc_3_0055:
        ret    0x10
loc_3_0058:
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [eax + 0x18]
        cmp    dword ptr [eax + 0x1c], 4
        jae    loc_3_006C
        mov    eax, 0xea
        jmp    loc_3_007D
loc_3_006C:
        or     ecx, ecx
        mov    eax, 0x57
        je     loc_3_007D
        mov    dword ptr [ecx], 0x100
        sub    eax, eax
loc_3_007D:
        ret    0xc
loc_3_0080:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x1c]
        mov    ebx, 0xc0000001
        mov    ebp, dword ptr [edi + 0x10]
        or     ebp, ebp
        je     loc_3_00F0
        cmp    dword ptr [edi + 0x14], 0x10
        jb     loc_3_00F0
        cmp    dword ptr [edi + 0x18], 0
        je     loc_3_00F0
        mov    esi, dword ptr [edi + 0x1c]
        or     esi, esi
        je     loc_3_00F0
        push   dword ptr [ebp + 4]
        call   sub_3_0319
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_3_00BC
        mov    eax, 0xc0010006
        jmp    loc_3_00F2
loc_3_00BC:
        push   0x10
        push   dword ptr [0x10]              ; -> obj4:0x0010
        call   sub_5_0010
        add    esp, 8
        mov    eax, dword ptr [edi + 0x20]
        mov    dword ptr [eax], esi
        push   dword ptr [edi + 0x20]
        push   dword ptr [edi + 0x18]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_0245
        mov    ebx, eax
        push   dword ptr [0x10]              ; -> obj4:0x0010
        call   sub_5_0018
        add    esp, 4
loc_3_00F0:
        mov    eax, ebx
loc_3_00F2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0xc
loc_3_00F9:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x14]
        mov    edi, 0xc0000001
        mov    esi, dword ptr [eax + 0x10]
        or     esi, esi
        je     loc_3_0156
        cmp    dword ptr [eax + 0x14], 0x10
        jb     loc_3_0156
        push   dword ptr [esi + 4]
        call   sub_3_0319
        mov    edi, eax
        or     edi, edi
        jne    loc_3_0126
        mov    eax, 0xc0010006
        jmp    loc_3_0158
loc_3_0126:
        push   0x10
        push   dword ptr [0x10]              ; -> obj4:0x0010
        call   sub_5_0010
        add    esp, 8
        push   dword ptr [esi + 0xc]
        lea    eax, [esi + 0x10]
        push   eax
        push   dword ptr [esi + 8]
        push   edi
        call   sub_3_02BC
        mov    edi, eax
        push   dword ptr [0x10]              ; -> obj4:0x0010
        call   sub_5_0018
        add    esp, 4
loc_3_0156:
        mov    eax, edi
loc_3_0158:
        pop    edi
        pop    esi
        ret    0xc
loc_3_015D:
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, dword ptr [eax + 0x18]
        cmp    dword ptr [eax + 0x1c], 9
        jae    loc_3_0172
        mov    eax, 0xea
        jmp    loc_3_0195
loc_3_0172:
        mov    eax, dword ptr [eax + 0x10]
        push   dword ptr [eax]
        call   sub_3_0319
        or     eax, eax
        jne    loc_3_0187
        mov    eax, 0xc0010006
        jmp    loc_3_0195
loc_3_0187:
        mov    cl, byte ptr [eax + 0x3c]
        mov    byte ptr [esi + 4], cl
        mov    eax, dword ptr [eax + 0x74]
        mov    dword ptr [esi + 5], eax
        sub    eax, eax
loc_3_0195:
        pop    esi
        ret    0xc
sub_3_0199:
        push   dword ptr [0x10]              ; -> obj4:0x0010
        call   sub_5_0008
        add    esp, 4
        mov    eax, 1
        ret    
sub_3_01AD:
        mov    eax, 1
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_01C0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esi]
        mov    dword ptr [0xe8], eax         ; -> obj1:0x00E8
        mov    eax, dword ptr [esp + 0x18]
        mov    edi, dword ptr [eax + 0x18]
        or     edi, edi
        jne    loc_3_01E2
        mov    dword ptr [esi], 0xc0000001
        jmp    loc_3_023E
loc_3_01E2:
        mov    eax, dword ptr [edi + 0x1c]
        mov    ecx, dword ptr [eax + 0x50]
        add    eax, 0x50
        mov    dword ptr [0x14], ecx         ; -> sub_4_0014
        mov    dword ptr [eax], 0x40         ; -> loc_1_0040
        mov    ebp, dword ptr [esp + 0x1c]
        push   ebp
        push   edi
        push   esi
        call   6
        mov    ebx, dword ptr [esi]
        cmp    ebx, 0x103
        jne    loc_3_0233
        mov    ebp, dword ptr [ebp + 0x10]
        cmp    ebp, -1
        je     loc_3_0224
loc_3_0215:
        push   0x2710
        call   sub_3_000C
        cmp    ebp, -1
        jne    loc_3_0215
loc_3_0224:
        cmp    ebx, 0x103
        jne    loc_3_0233
        mov    eax, dword ptr [0xe8]         ; -> obj1:0x00E8
        mov    dword ptr [esi], eax
loc_3_0233:
        mov    ecx, dword ptr [edi + 0x1c]
        mov    eax, dword ptr [0x14]         ; -> sub_4_0014
        mov    dword ptr [ecx + 0x50], eax
loc_3_023E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0xc
sub_3_0245:
        sub    esp, 4
        push   esi
        push   edi
        mov    dword ptr [esp + 8], 0
        mov    edi, 0xc0                     ; -> obj1:0x00C0
        sub    eax, eax
        mov    ecx, 0xa
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [0xd0], 8           ; -> obj1:0x00D0
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [0xd4], eax         ; -> obj1:0x00D4
        mov    eax, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x1c]
        mov    dword ptr [0xd8], eax         ; -> obj1:0x00D8
        mov    eax, dword ptr [esi]
        push   0xc0                          ; -> obj1:0x00C0
        mov    dword ptr [0xdc], eax         ; -> obj1:0x00DC
        lea    eax, [esp + 0xc]
        push   dword ptr [esp + 0x14]
        push   eax
        call   sub_3_01C0
        cmp    dword ptr [esp + 8], 0
        jne    loc_3_02AA
        mov    eax, dword ptr [0xe0]         ; -> obj1:0x00E0
        mov    dword ptr [esi], eax
        jmp    loc_3_02B0
loc_3_02AA:
        mov    dword ptr [esi], 0
loc_3_02B0:
        mov    eax, dword ptr [esp + 8]
        pop    edi
        pop    esi
        add    esp, 4
        ret    0x10
sub_3_02BC:
        sub    esp, 4
        push   edi
        mov    dword ptr [esp + 4], 0
        mov    edi, 0xc0                     ; -> obj1:0x00C0
        sub    eax, eax
        mov    ecx, 0xa
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [0xd0], 1           ; -> obj1:0x00D0
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [0xd4], eax         ; -> obj1:0x00D4
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [0xd8], eax         ; -> obj1:0x00D8
        mov    eax, dword ptr [esp + 0x18]
        push   0xc0                          ; -> obj1:0x00C0
        mov    dword ptr [0xdc], eax         ; -> obj1:0x00DC
        lea    eax, [esp + 8]
        push   dword ptr [esp + 0x10]
        push   eax
        call   sub_3_01C0
        mov    eax, dword ptr [esp + 4]
        pop    edi
        add    esp, 4
        ret    0x10
sub_3_0319:
        sub    esp, 4
        lea    eax, [esp]
        push   0
        push   eax
        call   0
        mov    ecx, dword ptr [esp + 0x10]
        add    esp, 8
        sub    edx, edx
loc_3_0331:
        cmp    dword ptr [esp], edx
        je     loc_3_0351
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp], eax
        cmp    dword ptr [eax], 0x5044414e
        jne    loc_3_0331
        cmp    dword ptr [eax + 0x38], ecx
        jne    loc_3_0331
        jmp    loc_3_0353
loc_3_0351:
        sub    eax, eax
loc_3_0353:
        add    esp, 4
        ret    4
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x18  RW-
        dd  loc_3_0058               ; fixup
        dd  loc_3_0080               ; fixup
        dd  loc_3_00F9               ; fixup
        dd  loc_3_015D               ; fixup
        db  000h, 000h, 000h, 000h
sub_4_0014:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x1e  R-X
sub_5_0000:
        VMMCall  VMM._CreateMutex                ; cd2001810100
        add    byte ptr [eax], al
sub_5_0008:
        VMMCall  VMM._DestroyMutex               ; cd2002810100
        add    byte ptr [eax], al
sub_5_0010:
        VMMCall  VMM._EnterMutex                 ; cd202e810100
        add    byte ptr [eax], al
sub_5_0018:
        VxDCall  Dev_2501.Service_812Fh          ; cd202f810125
VxD__LTEXT ENDS
