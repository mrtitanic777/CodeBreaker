; FILESEC -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 3285 instructions recovered by recursive descent

Declare_Virtual_Device FILESEC, 4, 0, FILESEC_Control, 00491h, Undefined_Init_Order
;   DDK version 0400h   init_order 0A0030100h   flags 00000h
;   service table: obj1:0xa4, 17 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x374  R-X
FILESEC_Control:
        sub    eax, 1
        cmp    eax, 0x23
        jae    loc_1_000F
        jmp    dword ptr [eax*4 + 0x11]      ; -> obj1:0x0011
loc_1_000F:
        clc    
        ret    
        ; ---- dispatch table (35 entries) ----
        dd  loc_2_0000
        dd  loc_2_0012
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
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_3_0032
FILESEC_Service_00:
        mov    eax, 0x400
        clc    
        ret    
        dd  FILESEC_Service_00       ; fixup
        dd  FILESEC_Service_01       ; fixup
        dd  FILESEC_Service_02       ; fixup
        dd  FILESEC_Service_03       ; fixup
        dd  FILESEC_Service_04       ; fixup
        dd  FILESEC_Service_05       ; fixup
        dd  FILESEC_Service_06       ; fixup
        dd  FILESEC_Service_07       ; fixup
        dd  FILESEC_Service_08       ; fixup
        dd  FILESEC_Service_09       ; fixup
        dd  FILESEC_Service_10       ; fixup
        dd  FILESEC_Service_11       ; fixup
        dd  FILESEC_Service_12       ; fixup
        dd  FILESEC_Service_13       ; fixup
        dd  FILESEC_Service_14       ; fixup
        dd  FILESEC_Service_15       ; fixup
        dd  FILESEC_Service_16       ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00491h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'FILESEC '                  ; DDB_Name
        dd  0A0030100h                  ; DDB_Init_Order
        dd  FILESEC_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x00A4              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  053h, 045h, 043h, 055h, 052h, 049h, 054h, 059h, 05Ch, 041h, 043h, 043h, 045h, 053h, 053h, 05Ch
        db  000h, 000h, 000h, 000h, 053h, 045h, 043h, 055h, 052h, 049h, 054h, 059h, 05Ch, 041h, 043h, 043h
        db  045h, 053h, 053h, 05Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x188  R-X
loc_2_0000:
        call   sub_2_0100
        call   sub_2_00AD
        or     eax, eax
        jne    loc_2_0010
        clc    
        ret    
loc_2_0010:
        stc    
        ret    
loc_2_0012:
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jae    loc_2_001C
        jmp    loc_2_0046
loc_2_001C:
        VxDCall  Dev_0493.Service_0000h          ; cd2000009304
        jae    loc_2_0026
        jmp    loc_2_0046
loc_2_0026:
        call   sub_2_0070
        or     eax, eax
        je     loc_2_0031
        jmp    loc_2_0046
loc_2_0031:
        VxDCall  VCOMM.VCOMM_Get_Version         ; cd2000003200
        jb     loc_2_0044
        call   sub_2_00D0
        or     eax, eax
        jne    loc_2_0044
        jmp    loc_2_0046
loc_2_0044:
        clc    
        ret    
loc_2_0046:
        stc    
        ret    
sub_2_0048:
        push   esi
        mov    ax, word ptr [esp + 8]
        mov    esi, dword ptr [esp + 0xc]
        VxDCall  VCOMM._VCOMM_Register_Port_Driver ; cd2001003200
        jb     loc_2_0061
        mov    eax, 1
        jmp    loc_2_0063
loc_2_0061:
        xor    eax, eax
loc_2_0063:
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0070:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        lea    eax, [ebp - 0x20]
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [ebp - 4]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  Dev_0493.Service_0005h          ; cd2005009304
        add    esp, 4
        or     eax, eax
        jne    loc_2_00A9
        push   0x2f0                         ; -> obj1:0x02F0
        push   dword ptr [ebp - 0x18]
        call   FILESEC_Service_15
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [0x370], eax        ; -> obj1:0x0370
        sub    eax, eax
loc_2_00A9:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_00AD:
        push   1
        push   0
        call   sub_2_0174
        add    esp, 8
        mov    dword ptr [0x2ec], eax        ; -> obj1:0x02EC
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_00D0:
        push   esi
        mov    esi, 0x134                    ; -> obj4:0x0134
loc_2_00D6:
        push   esi
        push   dword ptr [esi - 4]
        call   sub_2_0048
        add    esp, 8
        or     eax, eax
        je     loc_2_00F8
        add    esi, 0x14
        cmp    esi, 0x1ac                    ; -> obj4:0x01AC
        jb     loc_2_00D6
        mov    eax, 1
loc_2_00F6:
        pop    esi
        ret    
loc_2_00F8:
        sub    eax, eax
        jmp    loc_2_00F6
        db  000h, 000h, 000h, 000h
sub_2_0100:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   edi
        cmp    dword ptr [0x1dc], -1         ; -> obj4:0x01DC
        jne    loc_2_016D
        mov    dword ptr [ebp - 0xc], 5
        mov    dword ptr [ebp - 8], 0
        mov    ebx, 0xffffffff
        mov    ecx, dword ptr [ebp - 0xc]
        mov    edx, 0xffffffff
        lea    edi, [ebp - 0x14]
        mov    eax, 0x6507
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jae    loc_2_0141
        mov    dword ptr [ebp - 8], eax
loc_2_0141:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_016D
        mov    eax, dword ptr [ebp - 0x13]
        mov    dword ptr [ebp - 4], eax
        call   sub_2_017C
        movzx  ecx, word ptr [ebp - 2]
        shl    ecx, 4
        add    ecx, dword ptr [eax + 4]
        movzx  eax, word ptr [ebp - 4]
        cmp    word ptr [ecx + eax], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [0x1dc], eax        ; -> obj4:0x01DC
loc_2_016D:
        pop    edi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h
sub_2_0174:
        VMMCall  VMM._CreateMutex                ; cd2001810100
        add    byte ptr [eax], al
sub_2_017C:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x248f  R-X
FILESEC_Service_01:
        jmp    loc_3_1EED
FILESEC_Service_03:
        jmp    loc_3_1F08
FILESEC_Service_02:
        jmp    loc_3_1F7F
FILESEC_Service_04:
        jmp    loc_3_1F93
FILESEC_Service_05:
        jmp    loc_3_1FD5
FILESEC_Service_06:
        jmp    loc_3_1ED6
FILESEC_Service_07:
        jmp    loc_3_1F27
FILESEC_Service_08:
        jmp    loc_3_1F33
FILESEC_Service_10:
        jmp    loc_3_1F5D
FILESEC_Service_09:
        jmp    loc_3_1FB6
loc_3_0032:
        mov    ebx, dword ptr [esi + 0xc]
        cmp    ebx, 0
        jne    loc_3_0041
        mov    ecx, 0x400
        jmp    loc_3_0046
loc_3_0041:
        cmp    ebx, -1
        jne    loc_3_004A
loc_3_0046:
        xor    eax, eax
        clc    
        ret    
loc_3_004A:
        cmp    ebx, 0x64
        jl     loc_3_0076
        cmp    ebx, 0x6b
        jge    loc_3_0076
        sub    ebx, 0x64
        mov    edi, dword ptr [esi + 0x14]
        mov    eax, edi
        shr    eax, 2
        sub    esp, edi
        mov    edi, esp
        mov    esi, dword ptr [esi + 0x10]
        mov    ecx, eax
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        push   1
        call   sub_4_0000
        clc    
        ret    
loc_3_0076:
        mov    eax, 1
        clc    
        ret    
sub_3_007D:
        movzx  eax, byte ptr [esp + 4]
        VMMCall  VMM.Test_DBCS_Lead_Byte         ; cd20d1000100
        sbb    eax, eax
        inc    eax
        ret    4
sub_3_008E:
        VxDCall  IFSMgr.BCSToUni                 ; cd2042804000
sub_3_0094:
        VxDCall  IFSMgr.UniToBCS                 ; cd2040804000
sub_3_009A:
        VxDCall  IFSMgr.BcsToBcs                 ; cd2070804000
sub_3_00A0:
        VxDCall  IFSMgr.UniToUpper               ; cd2043804000
sub_3_00A6:
        VxDCall  Dev_0493.Service_8001h          ; cd2001809304
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00B0:
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 0x40], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x2c], 0
        mov    si, word ptr [esp + 0x44]
        cmp    si, 1
        je     loc_3_00EC
        cmp    si, 2
        je     loc_3_00E6
        mov    eax, 0x7c
        jmp    loc_3_0322
loc_3_00E6:
        cmp    si, 1
        jne    loc_3_00F9
loc_3_00EC:
        mov    edi, dword ptr [esp + 0x48]
        add    edi, 8
        mov    ebx, dword ptr [esp + 0x34]
        jmp    loc_3_0104
loc_3_00F9:
        mov    ebx, dword ptr [esp + 0x48]
        add    ebx, 8
        mov    edi, dword ptr [esp + 0x38]
loc_3_0104:
        mov    eax, dword ptr [esp + 0x48]
        sub    ecx, ecx
        mov    dx, word ptr [eax + 6]
        cmp    dx, cx
        jle    loc_3_0142
loc_3_0113:
        cmp    si, 1
        jne    loc_3_011F
        mov    ax, word ptr [edi + 0x16]
        jmp    loc_3_0123
loc_3_011F:
        mov    ax, word ptr [ebx + 4]
loc_3_0123:
        movzx  eax, ax
        test   eax, 0xffff7f00
        jne    loc_3_0159
        test   eax, 0xffff7fff
        je     loc_3_0163
        inc    ecx
        add    edi, 0x18
        add    ebx, 6
        movsx  eax, dx
        cmp    eax, ecx
        jg     loc_3_0113
loc_3_0142:
        call   sub_3_2070
        mov    dword ptr [esp + 0x24], eax
        or     eax, eax
        jne    loc_3_016D
        mov    eax, 8
        jmp    loc_3_0322
loc_3_0159:
        mov    eax, 0x57
        jmp    loc_3_0322
loc_3_0163:
        mov    eax, 0x57
        jmp    loc_3_0322
loc_3_016D:
        mov    eax, dword ptr [esp + 0x24]
        add    eax, 0x116
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esp + 0x28]
        or     eax, 0x200
        push   eax
        mov    eax, dword ptr [esp + 0x4c]
        push   dword ptr [esp + 0x28]
        push   dword ptr [eax]
        call   sub_3_1595
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_3_0310
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x28]
        push   0x80000002
        call   sub_3_23FC
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_01FD
        push   0
        push   0
        push   0
        lea    eax, [esp + 0x3c]
        push   0
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x40]
        call   sub_3_2444
        add    esp, 0x30
        or     eax, eax
        jne    loc_3_022B
        cmp    dword ptr [esp + 0x30], 0
        je     loc_3_022B
        push   dword ptr [esp + 0x14]
        call   sub_3_2404
        add    esp, 4
        mov    ebp, 0x8b1
        jmp    loc_3_0310
loc_3_01FD:
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x28]
        push   0x80000002
        call   sub_3_240C
        add    esp, 0xc
        mov    ebp, eax
        or     ebp, ebp
        je     loc_3_0223
        mov    ebp, 0x8b4
        jmp    loc_3_0310
loc_3_0223:
        mov    dword ptr [esp + 0x2c], 1
loc_3_022B:
        cmp    si, 1
        jne    loc_3_023A
        mov    edi, dword ptr [esp + 0x48]
        add    edi, 8
        jmp    loc_3_0241
loc_3_023A:
        mov    ebx, dword ptr [esp + 0x48]
        add    ebx, 8
loc_3_0241:
        mov    eax, dword ptr [esp + 0x48]
        mov    dword ptr [esp + 0x1c], 0
        cmp    word ptr [eax + 6], 0
        jle    loc_3_02B3
loc_3_0254:
        or     ebp, ebp
        jne    loc_3_02B3
        cmp    si, 1
        jne    loc_3_0266
        mov    ecx, edi
        mov    ax, word ptr [edi + 0x16]
        jmp    loc_3_026C
loc_3_0266:
        mov    ecx, dword ptr [ebx]
        mov    ax, word ptr [ebx + 4]
loc_3_026C:
        mov    word ptr [esp + 0x10], ax
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x1c]
        push   ecx
        push   0
        call   sub_3_1BE2
        lea    ecx, [esp + 0x10]
        push   2
        push   ecx
        push   3
        push   0
        add    edi, 0x18
        push   eax
        add    ebx, 6
        push   dword ptr [esp + 0x28]
        call   sub_3_243C
        add    esp, 0x18
        mov    ebp, eax
        mov    eax, dword ptr [esp + 0x48]
        inc    dword ptr [esp + 0x1c]
        movsx  eax, word ptr [eax + 6]
        cmp    eax, dword ptr [esp + 0x1c]
        jg     loc_3_0254
loc_3_02B3:
        cmp    dword ptr [esp + 0x1c], 0
        jne    loc_3_02E3
        or     ebp, ebp
        jne    loc_3_02E7
        mov    word ptr [esp + 0x12], 0x8000
        lea    eax, [esp + 0x12]
        push   2
        push   eax
        push   3
        push   0
        push   0xf4                          ; -> obj4:0x00F4
        push   dword ptr [esp + 0x28]
        call   sub_3_243C
        add    esp, 0x18
        mov    ebp, eax
loc_3_02E3:
        or     ebp, ebp
        je     loc_3_0304
loc_3_02E7:
        cmp    dword ptr [esp + 0x2c], 0
        je     loc_3_032C
        push   dword ptr [esp + 0x24]
        push   0x80000002
        call   sub_3_2414
        add    esp, 8
loc_3_02FF:
        mov    ebp, 0x8b4
loc_3_0304:
        push   dword ptr [esp + 0x14]
        call   sub_3_2404
        add    esp, 4
loc_3_0310:
        cmp    dword ptr [esp + 0x24], 0
        je     loc_3_0320
        push   dword ptr [esp + 0x24]
        call   sub_3_20D9
loc_3_0320:
        mov    eax, ebp
loc_3_0322:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x2c
        ret    0x10
loc_3_032C:
        mov    esi, 0x64
loc_3_0331:
        mov    dword ptr [esp + 0x20], esi
        push   0
        push   0
        push   0
        lea    eax, [esp + 0x2c]
        push   0
        push   eax
        push   dword ptr [esp + 0x2c]
        push   0
        push   dword ptr [esp + 0x30]
        call   sub_3_242C
        add    esp, 0x20
        or     eax, eax
        jne    loc_3_02FF
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        call   sub_3_2424
        add    esp, 8
        jmp    loc_3_0331
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_3_0370:
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        mov    ax, word ptr [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        push   0
        push   dword ptr [esp + 0x18]
        call   sub_3_0391
        ret    0x14
sub_3_0391:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x18]
        cmp    eax, 1
        sbb    edi, edi
        inc    edi
        mov    ebx, dword ptr [esp + 0x2c]
        mov    ebp, dword ptr [esp + 0x20]
        mov    word ptr [ebx], 5
        mov    esi, dword ptr [esp + 0x1c]
        push   ebp
        push   esi
        push   eax
        call   sub_3_0D8B
        or     eax, eax
        je     loc_3_03CD
        mov    word ptr [esp + 0x12], 0xff
        mov    di, word ptr [esp + 0x28]
        jmp    loc_3_03ED
loc_3_03CD:
        lea    eax, [esp + 0x12]
        push   edi
        mov    di, word ptr [esp + 0x2c]
        push   eax
        push   edi
        push   dword ptr [esp + 0x30]
        push   0x80000002
        push   ebp
        push   esi
        call   sub_3_0950
        or     eax, eax
        jne    loc_3_0401
loc_3_03ED:
        mov    ax, word ptr [esp + 0x12]
        and    ax, di
        cmp    ax, di
        jne    loc_3_03FF
        mov    word ptr [ebx], 0
loc_3_03FF:
        sub    eax, eax
loc_3_0401:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0x18
        db  000h, 000h, 000h, 000h, 000h
sub_3_0410:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [esp + 0x20], 1
        sbb    ebx, ebx
        inc    ebx
        call   sub_3_2070
        mov    esi, eax
        or     esi, esi
        jne    loc_3_0433
        mov    eax, 8
        jmp    loc_3_051A
loc_3_0433:
        push   ebx
        push   esi
        lea    edi, [esi + 0x116]
        push   dword ptr [esp + 0x2c]
        call   sub_3_1595
        or     eax, eax
        je     loc_3_0452
        mov    edi, 0x8ae
        jmp    loc_3_050E
loc_3_0452:
        lea    eax, [esp + 0xc]
        push   eax
        push   esi
        push   0x80000002
        call   sub_3_23FC
        add    esp, 0xc
        or     eax, eax
        je     loc_3_0473
        mov    edi, 0x8ae
        jmp    loc_3_050E
loc_3_0473:
        push   esi
        call   sub_3_0DD3
        push   0
        push   0
        push   0
        lea    eax, [esp + 0x20]
        push   0
        push   eax
        push   0
        lea    eax, [esp + 0x30]
        push   0
        push   eax
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x38]
        call   sub_3_2444
        add    esp, 0x30
        cmp    dword ptr [esp + 0x18], 0
        je     loc_3_0502
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_04E7
        mov    ebx, 0x64
loc_3_04B4:
        mov    dword ptr [esp + 0x10], ebx
        push   0
        push   0
        push   0
        lea    eax, [esp + 0x1c]
        push   0
        push   eax
        push   edi
        push   0
        push   dword ptr [esp + 0x28]
        call   sub_3_242C
        add    esp, 0x20
        or     eax, eax
        jne    loc_3_04E7
        push   edi
        push   dword ptr [esp + 0x10]
        call   sub_3_2424
        add    esp, 8
        jmp    loc_3_04B4
loc_3_04E7:
        push   dword ptr [esp + 0xc]
        call   sub_3_2404
        add    esp, 4
        cmp    dword ptr [esp + 0x14], 1
        sbb    edi, edi
        and    edi, 0x8ae
        jmp    loc_3_050E
loc_3_0502:
        push   dword ptr [esp + 0xc]
        push   esi
        call   sub_3_1B49
        sub    edi, edi
loc_3_050E:
        or     esi, esi
        je     loc_3_0518
        push   esi
        call   sub_3_20D9
loc_3_0518:
        mov    eax, edi
loc_3_051A:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0530:
        cmp    dword ptr [esp + 4], 1
        sbb    eax, eax
        inc    eax
        push   eax
        mov    eax, dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x24]
        push   eax
        mov    ax, word ptr [esp + 0x24]
        push   eax
        mov    eax, dword ptr [esp + 0x24]
        push   eax
        mov    ax, word ptr [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        push   0
        call   sub_3_0565
        ret    0x20
sub_3_0565:
        sub    esp, 0x268
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x1c], 0
        sub    esi, esi
        sub    edi, edi
        cmp    word ptr [esp + 0x288], si
        jl     loc_3_0833
        cmp    word ptr [esp + 0x288], 2
        jg     loc_3_0833
        call   sub_3_2070
        mov    dword ptr [esp + 0x18], eax
        cmp    eax, esi
        jne    loc_3_05AF
        mov    eax, 8
        jmp    loc_3_0838
loc_3_05AF:
        mov    ecx, dword ptr [esp + 0x298]
        cmp    word ptr [esp + 0x284], 0
        mov    word ptr [ecx], 0
        mov    ecx, dword ptr [esp + 0x294]
        mov    eax, dword ptr [esp + 0x280]
        mov    word ptr [ecx], 0
        jne    loc_3_05EC
        or     eax, eax
        je     loc_3_05E2
        cmp    byte ptr [eax], 0
        jne    loc_3_05EC
loc_3_05E2:
        mov    dword ptr [esp + 0x14], 1
        jmp    loc_3_05F4
loc_3_05EC:
        mov    dword ptr [esp + 0x14], 0
loc_3_05F4:
        push   dword ptr [esp + 0x29c]
        push   dword ptr [esp + 0x1c]
        push   eax
        call   sub_3_1595
        or     eax, eax
        je     loc_3_0613
        mov    esi, 0x57
        jmp    loc_3_0815
loc_3_0613:
        lea    ebx, [esp + 0x20]
        push   dword ptr [esp + 0x18]
        call   sub_3_2474
        mov    ecx, dword ptr [esp + 0x1c]
        add    esp, 4
        add    ecx, eax
        mov    dword ptr [esp + 0x20], ecx
        mov    dword ptr [esp + 0x28], 0
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x1c]
        push   0x80000002
        call   sub_3_23FC
        add    esp, 0xc
        or     eax, eax
        je     loc_3_0659
        mov    esi, 0x8ae
        jmp    loc_3_0815
loc_3_0659:
        call   sub_3_2070
        mov    edi, eax
        or     edi, edi
        jne    loc_3_0761
        push   dword ptr [esp + 0x24]
        call   sub_3_2404
        add    esp, 4
        mov    esi, 8
        jmp    loc_3_0815
loc_3_067E:
        push   dword ptr [esp + 0x29c]
        lea    eax, [esp + 0x294]
        push   0
        push   eax
        lea    eax, [esp + 0x298]
        push   eax
        push   dword ptr [esp + 0x298]
        push   dword ptr [esp + 0x2c]
        push   dword ptr [ebx + 4]
        call   sub_3_1851
loc_3_06AA:
        cmp    eax, 3
        je     loc_3_06D4
        mov    ecx, dword ptr [esp + 0x298]
        inc    word ptr [ecx]
        cmp    eax, 2
        je     loc_3_06C8
        mov    ecx, dword ptr [esp + 0x294]
        inc    word ptr [ecx]
loc_3_06C8:
        or     eax, eax
        je     loc_3_06D4
        mov    dword ptr [esp + 0x1c], 1
loc_3_06D4:
        or     ebp, ebp
        je     loc_3_07E1
loc_3_06DC:
        push   0x116
        push   edi
        push   dword ptr [ebx + 8]
        push   dword ptr [ebx + 4]
        call   sub_3_241C
        add    esp, 0x10
        mov    esi, eax
        or     esi, esi
        jne    loc_3_070E
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_070E
        push   0
        push   edi
        call   sub_3_14D0
        or     eax, eax
        je     loc_3_070E
        inc    dword ptr [ebx + 8]
        jmp    loc_3_06DC
loc_3_070E:
        or     ebp, ebp
        je     loc_3_07E1
        or     esi, esi
        jne    loc_3_07E1
        mov    eax, dword ptr [ebx]
        lea    ebp, [ebx + 0xc]
        mov    byte ptr [eax], 0x5c
        push   -1
        push   edi
        mov    eax, dword ptr [ebx]
        inc    eax
        push   eax
        call   sub_3_246C
        add    esp, 0xc
        lea    eax, [ebp + 4]
        push   eax
        push   edi
        push   dword ptr [ebx + 4]
        call   sub_3_23FC
        add    esp, 0xc
        mov    esi, eax
        push   edi
        call   sub_3_2474
        add    esp, 4
        mov    ecx, dword ptr [ebx]
        add    ecx, eax
        inc    ecx
        mov    dword ptr [ebp], ecx
        mov    dword ptr [ebp + 8], 0
        mov    ebx, ebp
loc_3_0761:
        cmp    word ptr [esp + 0x284], 0
        jne    loc_3_0786
        mov    eax, ebx
        lea    ecx, [esp + 0x20]
        sub    eax, ecx
        mov    ecx, 0xc
        cdq    
        idiv   ecx
        cmp    eax, 1
        mov    ebp, 0
        jge    loc_3_078B
loc_3_0786:
        mov    ebp, 1
loc_3_078B:
        cmp    dword ptr [ebx + 8], 0
        jne    loc_3_06D4
        cmp    dword ptr [esp + 0x27c], 0
        je     loc_3_067E
        lea    eax, [esp + 0x12]
        push   dword ptr [esp + 0x29c]
        push   eax
        push   0x40
        push   0
        mov    eax, dword ptr [esp + 0x28c]
        push   dword ptr [ebx + 4]
        push   dword ptr [eax + 4]
        push   dword ptr [eax + 0x14]
        call   sub_3_0950
        or     eax, eax
        jne    loc_3_07D7
        test   byte ptr [esp + 0x12], 0x40
        jne    loc_3_067E
loc_3_07D7:
        mov    eax, 3
        jmp    loc_3_06AA
loc_3_07E1:
        push   dword ptr [ebx + 4]
        call   sub_3_2404
        add    esp, 4
        mov    esi, eax
        lea    eax, [esp + 0x20]
        cmp    ebx, eax
        jbe    loc_3_0807
        mov    eax, dword ptr [ebx - 0xc]
        sub    ebx, 0xc
        mov    byte ptr [eax], 0
        inc    dword ptr [ebx + 8]
        jmp    loc_3_0761
loc_3_0807:
        cmp    dword ptr [esp + 0x1c], 1
        cmc    
        sbb    esi, esi
        and    esi, 0xea
loc_3_0815:
        or     edi, edi
        je     loc_3_081F
        push   edi
        call   sub_3_20D9
loc_3_081F:
        cmp    dword ptr [esp + 0x18], 0
        je     loc_3_082F
        push   dword ptr [esp + 0x18]
        call   sub_3_20D9
loc_3_082F:
        mov    eax, esi
        jmp    loc_3_0838
loc_3_0833:
        mov    eax, 0x7c
loc_3_0838:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x268
        ret    0x24
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0850:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        cmp    dword ptr [esp + 0x18], 1
        sbb    ebp, ebp
        inc    ebp
        mov    si, word ptr [esp + 0x20]
        cmp    si, 0xa
        je     loc_3_0872
        cmp    si, 0xc
        jne    loc_3_087B
loc_3_0872:
        mov    ebx, 1
        sub    si, 0xa
loc_3_087B:
        or     si, si
        jl     loc_3_0932
        cmp    si, 2
        jg     loc_3_0932
        call   sub_3_2070
        mov    edi, eax
        or     edi, edi
        jne    loc_3_08A3
        mov    eax, 8
        jmp    loc_3_0937
loc_3_08A3:
        or     ebx, ebx
        push   ebp
        jne    loc_3_08DB
        push   edi
        push   dword ptr [esp + 0x24]
        call   sub_3_1595
        or     eax, eax
        je     loc_3_08BD
        mov    esi, 0x8ae
        jmp    loc_3_0924
loc_3_08BD:
        lea    eax, [esp + 0x10]
        push   eax
        push   edi
        push   0x80000002
        call   sub_3_23FC
        add    esp, 0xc
        or     eax, eax
        je     loc_3_08F5
        mov    esi, 0x8ae
        jmp    loc_3_0924
loc_3_08DB:
        lea    eax, [esp + 0x14]
        push   eax
        push   edi
        push   dword ptr [esp + 0x28]
        call   sub_3_1781
        or     eax, eax
        je     loc_3_08F5
        mov    esi, 0x8ae
        jmp    loc_3_0924
loc_3_08F5:
        push   ebp
        lea    eax, [esp + 0x2c]
        push   dword ptr [esp + 0x30]
        push   eax
        lea    eax, [esp + 0x30]
        push   eax
        push   esi
        push   edi
        push   dword ptr [esp + 0x28]
        call   sub_3_1851
        mov    esi, eax
        push   dword ptr [esp + 0x10]
        call   sub_3_2404
        add    esp, 4
        mov    esi, dword ptr [esi*4 + 0x80] ; -> loc_4_0080
loc_3_0924:
        or     edi, edi
        je     loc_3_092E
        push   edi
        call   sub_3_20D9
loc_3_092E:
        mov    eax, esi
        jmp    loc_3_0937
loc_3_0932:
        mov    eax, 0x7c
loc_3_0937:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0x18
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0950:
        sub    esp, 0x8c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0xb4]
        mov    dword ptr [esp + 0x1c], 0
        mov    word ptr [edi], 0
        call   sub_3_2070
        mov    esi, eax
        or     esi, esi
        jne    loc_3_0983
        mov    eax, 8
        jmp    loc_3_0CF1
loc_3_0983:
        lea    ebp, [esi + 0x116]
        lea    ebx, [esi + 0x17a]
        cmp    dword ptr [esp + 0xa8], 0x80000002
        je     loc_3_0A82
        push   0
        push   0
        push   0
        lea    eax, [esp + 0x40]
        push   0
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0xd4]
        call   sub_3_2444
        add    esp, 0x30
        or     eax, eax
        jne    loc_3_0A75
        cmp    dword ptr [esp + 0x34], 0
        je     loc_3_0A75
        mov    eax, dword ptr [esp + 0xa8]
        mov    dword ptr [esp + 0x18], eax
loc_3_09E6:
        push   dword ptr [esp + 0xb8]
        push   ebp
        push   dword ptr [esp + 0xac]
        push   dword ptr [esp + 0xac]
        call   sub_3_1BE2
        mov    dword ptr [esp + 0x24], eax
        cmp    dword ptr [esp + 0xa8], 0x80000002
        jne    loc_3_0ABE
        push   1
        push   dword ptr [0x2ec]             ; -> obj1:0x02EC
        call   sub_3_245C
        mov    ax, word ptr [esp + 0xb8]
        add    esp, 8
        and    ax, word ptr [0x168]          ; -> obj1:0x0168
        cmp    ax, word ptr [esp + 0xb0]
        jne    loc_3_0AB0
        push   0
        push   0x170                         ; -> obj1:0x0170
        lea    eax, [esi + 0x10]
        push   eax
        call   FILESEC_Service_13
        or     eax, eax
        jne    loc_3_0AB0
        push   0
        push   0x288                         ; -> obj1:0x0288
        push   dword ptr [esp + 0x2c]
        call   FILESEC_Service_13
        or     eax, eax
        jne    loc_3_0AB0
        mov    ax, word ptr [0x16c]          ; -> obj1:0x016C
        mov    word ptr [edi], ax
        jmp    loc_3_0CBC
loc_3_0A75:
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_3_0CE3
loc_3_0A82:
        lea    eax, [esp + 0x18]
        push   dword ptr [esp + 0xb8]
        push   eax
        push   esi
        push   dword ptr [esp + 0xb8]
        call   sub_3_1781
        or     eax, eax
        je     loc_3_09E6
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_3_0CE3
loc_3_0AB0:
        push   dword ptr [0x2ec]             ; -> obj1:0x02EC
        call   sub_3_2464
        add    esp, 4
loc_3_0ABE:
        mov    dword ptr [esp + 0x14], 2
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x16]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        push   0
        push   dword ptr [esp + 0x34]
        push   dword ptr [esp + 0x2c]
        call   sub_3_2434
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_0AFC
        mov    ax, word ptr [esp + 0x12]
        and    ax, 0x7fff
        mov    word ptr [edi], ax
        jmp    loc_3_0C62
loc_3_0AFC:
        push   0xf4                          ; -> obj4:0x00F4
        push   dword ptr [esp + 0x28]
        call   sub_3_2140
        or     eax, eax
        je     loc_3_0C5D
        mov    dword ptr [esp + 0x14], 2
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x16]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        push   0
        push   0xf4                          ; -> obj4:0x00F4
        push   dword ptr [esp + 0x2c]
        call   sub_3_2434
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_0B61
        mov    ax, word ptr [esp + 0x12]
        mov    word ptr [edi], ax
        mov    cx, word ptr [esp + 0xb0]
        and    cx, ax
        cmp    cx, word ptr [esp + 0xb0]
        je     loc_3_0C5D
loc_3_0B61:
        call   sub_3_2070
        mov    dword ptr [esp + 0x28], eax
        or     eax, eax
        jne    loc_3_0B7B
        mov    dword ptr [esp + 0x1c], 8
        jmp    loc_3_0CCA
loc_3_0B7B:
        mov    dword ptr [esp + 0x30], 0x200
        push   ebx
        push   dword ptr [esp + 0x28]
        call   FILESEC_Service_16
        push   0x5c
        push   ebx
        call   FILESEC_Service_14
        lea    ecx, [esp + 0x30]
        push   ecx
        mov    byte ptr [eax], 0
        push   dword ptr [esp + 0x2c]
        inc    eax
        push   eax
        push   ebx
        call   sub_3_00A6
        mov    dword ptr [esp + 0x2c], eax
        add    esp, 0x10
        or     eax, eax
        jne    loc_3_0C4C
        mov    eax, dword ptr [esp + 0x28]
        mov    ebp, eax
        mov    ebx, dword ptr [eax]
        add    ebp, 4
        mov    eax, ebx
        dec    ebx
        or     eax, eax
        je     loc_3_0C54
        mov    eax, dword ptr [esp + 0xb8]
        or     eax, 0x100
        mov    dword ptr [esp + 0x2c], eax
loc_3_0BDE:
        lea    eax, [esp + 0x38]
        push   dword ptr [esp + 0x2c]
        push   eax
        push   dword ptr [ebp]
        push   dword ptr [esp + 0xac]
        call   sub_3_1BE2
        mov    dword ptr [esp + 0x14], 2
        lea    ecx, [esp + 0x14]
        push   ecx
        lea    ecx, [esp + 0x16]
        push   ecx
        lea    ecx, [esp + 0x28]
        push   ecx
        push   0
        push   eax
        push   dword ptr [esp + 0x2c]
        call   sub_3_2434
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_0C40
        mov    ax, word ptr [edi]
        or     ax, word ptr [esp + 0x12]
        mov    word ptr [edi], ax
        mov    cx, word ptr [esp + 0xb0]
        and    cx, ax
        cmp    cx, word ptr [esp + 0xb0]
        je     loc_3_0C54
loc_3_0C40:
        add    ebp, 4
        mov    eax, ebx
        dec    ebx
        or     eax, eax
        jne    loc_3_0BDE
        jmp    loc_3_0C54
loc_3_0C4C:
        mov    dword ptr [esp + 0x1c], 0
loc_3_0C54:
        push   dword ptr [esp + 0x28]
        call   sub_3_20D9
loc_3_0C5D:
        and    word ptr [edi], 0x7fff
loc_3_0C62:
        cmp    dword ptr [esp + 0xa8], 0x80000002
        jne    loc_3_0CCA
        push   1
        push   dword ptr [0x2ec]             ; -> obj1:0x02EC
        call   sub_3_245C
        add    esp, 8
        push   -1
        lea    eax, [esi + 0x10]
        push   eax
        push   0x170                         ; -> obj1:0x0170
        call   sub_3_246C
        add    esp, 0xc
        push   -1
        push   dword ptr [esp + 0x28]
        push   0x288                         ; -> obj1:0x0288
        call   sub_3_246C
        mov    ax, word ptr [esp + 0xbc]
        add    esp, 0xc
        mov    word ptr [0x168], ax          ; -> obj1:0x0168
        mov    ax, word ptr [edi]
        mov    word ptr [0x16c], ax          ; -> obj1:0x016C
loc_3_0CBC:
        push   dword ptr [0x2ec]             ; -> obj1:0x02EC
        call   sub_3_2464
        add    esp, 4
loc_3_0CCA:
        mov    eax, dword ptr [esp + 0xa8]
        cmp    dword ptr [esp + 0x18], eax
        je     loc_3_0CE3
        push   dword ptr [esp + 0x18]
        call   sub_3_2404
        add    esp, 4
loc_3_0CE3:
        or     esi, esi
        je     loc_3_0CED
        push   esi
        call   sub_3_20D9
loc_3_0CED:
        mov    eax, dword ptr [esp + 0x1c]
loc_3_0CF1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x8c
        ret    0x1c
sub_3_0CFE:
        push   dword ptr [esp + 0x10]
        push   1
        push   0
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   0
        push   dword ptr [esp + 0x1c]
        call   sub_3_0D1C
        ret    0x10
sub_3_0D1C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x14]
        cmp    ecx, 1
        sbb    ebx, ebx
        inc    ebx
        mov    di, word ptr [esp + 0x24]
        or     di, di
        jne    loc_3_0D38
        mov    di, 0xff
loc_3_0D38:
        cmp    dword ptr [esp + 0x28], 0
        je     loc_3_0D5E
        mov    ebp, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [esp + 0x18]
        push   ebp
        push   esi
        push   ecx
        call   sub_3_0D8B
        or     eax, eax
        je     loc_3_0D66
        mov    ebx, dword ptr [esp + 0x2c]
        sub    eax, eax
        mov    word ptr [ebx], di
        jmp    loc_3_0D84
loc_3_0D5E:
        mov    esi, dword ptr [esp + 0x18]
        mov    ebp, dword ptr [esp + 0x1c]
loc_3_0D66:
        push   ebx
        mov    ebx, dword ptr [esp + 0x30]
        push   ebx
        push   edi
        push   dword ptr [esp + 0x2c]
        push   0x80000002
        push   ebp
        push   esi
        call   sub_3_0950
        or     eax, eax
        jne    loc_3_0D84
        and    word ptr [ebx], di
loc_3_0D84:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0x1c
sub_3_0D8B:
        sub    esp, 4
        cmp    dword ptr [esp + 8], 1
        sbb    eax, eax
        inc    eax
        push   eax
        lea    eax, [esp + 6]
        push   eax
        push   1
        push   0xa4                          ; -> loc_4_00A4
        push   0x80000002
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x24]
        call   sub_3_0950
        or     eax, eax
        je     loc_3_0DBD
        sub    eax, eax
        jmp    loc_3_0DCD
loc_3_0DBD:
        mov    ax, word ptr [esp + 2]
        and    ax, 1
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
loc_3_0DCD:
        add    esp, 4
        ret    0xc
sub_3_0DD3:
        push   1
        push   dword ptr [0x2ec]             ; -> obj1:0x02EC
        call   sub_3_245C
        add    esp, 8
        push   0
        mov    eax, dword ptr [esp + 8]
        push   0x170                         ; -> obj1:0x0170
        add    eax, 0x10
        push   eax
        call   FILESEC_Service_13
        or     eax, eax
        jne    loc_3_0E02
        mov    byte ptr [0x170], 0           ; -> obj1:0x0170
loc_3_0E02:
        push   dword ptr [0x2ec]             ; -> obj1:0x02EC
        call   sub_3_2464
        add    esp, 4
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0E20:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x2c]
        cmp    ebx, 0xea
        ja     loc_3_0E38
        mov    ebx, 0xea
loc_3_0E38:
        mov    eax, dword ptr [esp + 0x28]
        push   0x200
        add    ebx, dword ptr [eax]
        push   ebx
        call   sub_3_244C
        add    esp, 8
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_3_0E59
        sub    eax, eax
        jmp    loc_3_0EF7
loc_3_0E59:
        mov    edi, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [esp + 0x18]
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [esi]
        cmp    eax, ecx
        je     loc_3_0E7C
        sub    eax, ecx
        sar    eax, 3
        shl    eax, 3
        push   eax
        push   ecx
        push   ebp
        call   sub_3_247C
        add    esp, 0xc
loc_3_0E7C:
        mov    eax, dword ptr [esp + 0x24]
        mov    edx, ebp
        mov    ecx, dword ptr [eax]
        mov    eax, dword ptr [esp + 0x28]
        mov    eax, dword ptr [eax]
        sub    eax, ecx
        add    eax, dword ptr [esi]
        sub    edx, eax
        add    edx, ebx
        mov    dword ptr [esp + 0x10], edx
        or     eax, eax
        je     loc_3_0EA8
        push   eax
        push   ecx
        push   dword ptr [esp + 0x18]
        call   sub_3_247C
        add    esp, 0xc
loc_3_0EA8:
        cmp    dword ptr [esp + 0x30], 0
        je     loc_3_0EBB
        push   0
        push   dword ptr [esi]
        call   sub_3_2454
        add    esp, 8
loc_3_0EBB:
        mov    edx, ebp
        sub    edx, dword ptr [esi]
        mov    eax, dword ptr [esp + 0x24]
        mov    ecx, dword ptr [esp + 0x10]
        sub    ecx, dword ptr [eax]
        mov    dword ptr [esi], ebp
        mov    eax, dword ptr [esp + 0x20]
        add    dword ptr [edi], edx
        add    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 0x24]
        mov    edx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [esi]
        cmp    dword ptr [edi], eax
        jbe    loc_3_0EF2
loc_3_0EE9:
        add    dword ptr [eax], ecx
        add    eax, 8
        cmp    dword ptr [edi], eax
        ja     loc_3_0EE9
loc_3_0EF2:
        mov    eax, 1
loc_3_0EF7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0x1c
sub_3_0F01:
        sub    esp, 0x54
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x40], 0
        cmp    dword ptr [esp + 0x68], 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 0x50], eax
        mov    dword ptr [esp + 0x20], 0
        mov    dword ptr [esp + 0x58], 0
        cmp    word ptr [esp + 0x70], 0xc
        jne    loc_3_0F5D
        mov    word ptr [esp + 0x70], 2
        mov    dword ptr [esp + 0x40], 1
loc_3_0F43:
        mov    dx, word ptr [esp + 0x7c]
        or     dx, dx
        je     loc_3_0F77
        cmp    dx, 2
        je     loc_3_0F7D
        mov    eax, 0x57
        jmp    loc_3_14BA
loc_3_0F5D:
        cmp    word ptr [esp + 0x70], 1
        je     loc_3_0F43
        cmp    word ptr [esp + 0x70], 2
        je     loc_3_0F43
        mov    eax, 0x7c
        jmp    loc_3_14BA
loc_3_0F77:
        cmp    dx, 2
        jne    loc_3_0F93
loc_3_0F7D:
        cmp    word ptr [esp + 0x78], 2
        jae    loc_3_1031
        mov    eax, 0x84b
        jmp    loc_3_14BA
loc_3_0F93:
        movsx  eax, word ptr [esp + 0x70]
        lea    esi, [eax*4]
        movzx  ebp, word ptr [esp + 0x78]
        mov    ecx, dword ptr [esi + 0xdc]   ; -> loc_4_00DC
        cmp    ecx, ebp
        jbe    loc_3_0FB8
        mov    eax, 0x84b
        jmp    loc_3_14BA
loc_3_0FB8:
        mov    ebx, dword ptr [esp + 0x74]
        mov    esi, dword ptr [esi + 0xe8]   ; -> loc_4_00E8
        mov    ax, word ptr [ebx + 6]
        movsx  edi, ax
        imul   esi, edi
        add    esi, ecx
        cmp    esi, ebp
        jbe    loc_3_0FDC
        mov    eax, 0x84b
        jmp    loc_3_14BA
loc_3_0FDC:
        cmp    word ptr [esp + 0x70], 1
        jne    loc_3_0FF1
        mov    esi, dword ptr [esp + 0x74]
        add    esi, 8
        mov    ebx, dword ptr [esp + 0x5c]
        jmp    loc_3_0FFC
loc_3_0FF1:
        mov    ebx, dword ptr [esp + 0x74]
        add    ebx, 8
        mov    esi, dword ptr [esp + 0x34]
loc_3_0FFC:
        sub    ecx, ecx
        cmp    ax, cx
        jle    loc_3_1031
loc_3_1003:
        cmp    word ptr [esp + 0x70], 1
        jne    loc_3_1011
        mov    ax, word ptr [esi + 0x16]
        jmp    loc_3_1015
loc_3_1011:
        mov    ax, word ptr [ebx + 4]
loc_3_1015:
        movzx  ebp, ax
        test   ebp, 0xffff7f00
        jne    loc_3_104A
        cmp    ax, 0x8000
        je     loc_3_104A
        inc    ecx
        add    esi, 0x18
        add    ebx, 6
        cmp    ecx, edi
        jl     loc_3_1003
loc_3_1031:
        cmp    dx, 2
        jne    loc_3_1054
        mov    ebx, dword ptr [esp + 0x74]
        cmp    word ptr [ebx], 1
        cmc    
        sbb    eax, eax
        and    eax, 0x57
        jmp    loc_3_14BA
loc_3_104A:
        mov    eax, 0x57
        jmp    loc_3_14BA
loc_3_1054:
        call   sub_3_2070
        mov    dword ptr [esp + 0x28], eax
        or     eax, eax
        jne    loc_3_106B
        mov    eax, 8
        jmp    loc_3_14BA
loc_3_106B:
        mov    eax, dword ptr [esp + 0x28]
        add    eax, 0x116
        mov    dword ptr [esp + 0x38], eax
        mov    dword ptr [esp + 0x48], 0xea
        add    eax, 0xea
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [esp + 0x38]
        mov    dword ptr [esp + 0x20], eax
        mov    dword ptr [esp + 0x14], eax
        push   dword ptr [esp + 0x50]
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x74]
        call   sub_3_1595
        or     eax, eax
        je     loc_3_10AF
        jmp    loc_3_146C
loc_3_10AF:
        lea    eax, [esp + 0x2c]
        push   eax
        push   dword ptr [esp + 0x2c]
        push   0x80000002
        call   sub_3_23FC
        add    esp, 0xc
        or     eax, eax
        je     loc_3_10CE
        jmp    loc_3_146C
loc_3_10CE:
        mov    dword ptr [esp + 0x58], 1
        push   0
        push   0
        push   0
        lea    eax, [esp + 0x60]
        push   0
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x58]
        call   sub_3_2444
        add    esp, 0x30
        or     eax, eax
        jne    loc_3_146C
        cmp    dword ptr [esp + 0x54], 0
        je     loc_3_146C
        push   dword ptr [esp + 0x28]
        call   sub_3_0DD3
        sub    esi, esi
        mov    edi, 0x116
        mov    ebx, 2
loc_3_1123:
        mov    dword ptr [esp + 0x24], edi
        mov    dword ptr [esp + 0x4c], ebx
        lea    eax, [esp + 0x4c]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x68]
        push   eax
        lea    eax, [esp + 0x30]
        push   0
        push   eax
        push   dword ptr [esp + 0x3c]
        push   esi
        push   dword ptr [esp + 0x48]
        call   sub_3_242C
        add    esp, 0x20
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_3_121B
        inc    dword ptr [esp + 0x24]
        mov    eax, dword ptr [esp + 0x14]
        sub    eax, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esp + 0x24]
        lea    ecx, [eax + ecx + 8]
        or     ecx, ecx
        jle    loc_3_11B2
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esp + 0x20]
        sub    eax, dword ptr [esp + 0x38]
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        push   eax
        lea    eax, [esp + 0x4c]
        push   ecx
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        lea    eax, [esp + 0x38]
        push   eax
        call   sub_3_0E20
        or     eax, eax
        je     loc_3_126F
loc_3_11B2:
        mov    eax, dword ptr [esp + 0x24]
        sub    dword ptr [esp + 0x1c], eax
        mov    ecx, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esp + 0x1c]
        push   -1
        mov    dword ptr [ecx], eax
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x24]
        call   sub_3_246C
        mov    ax, word ptr [esp + 0x1c]
        mov    ecx, dword ptr [esp + 0x20]
        add    esp, 0xc
        mov    word ptr [ecx + 4], ax
        cmp    dword ptr [esp + 0x40], 0
        je     loc_3_11F9
        mov    ecx, dword ptr [esp + 0x14]
        mov    ax, word ptr [esp + 0x10]
        mov    word ptr [ecx + 6], ax
        jmp    loc_3_1203
loc_3_11F9:
        mov    eax, dword ptr [esp + 0x14]
        mov    word ptr [eax + 6], 0xffff
loc_3_1203:
        mov    eax, dword ptr [esp + 0x14]
        add    eax, 6
        cmp    word ptr [eax], 0x8000
        jne    loc_3_1216
        mov    word ptr [eax], 0xffff
loc_3_1216:
        add    dword ptr [esp + 0x14], 8
loc_3_121B:
        inc    esi
        or     ebp, ebp
        je     loc_3_1123
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x18], eax
        mov    dword ptr [esp + 0x30], 0
        mov    dword ptr [esp + 0x44], 0
        mov    edi, dword ptr [esp + 0x74]
        add    edi, 8
        mov    ebx, dword ptr [esp + 0x74]
        mov    dword ptr [esp + 0x34], edi
        cmp    word ptr [ebx + 6], 0
        jle    loc_3_139B
        mov    esi, 8
loc_3_125B:
        cmp    word ptr [esp + 0x70], 1
        jne    loc_3_127C
        mov    ecx, dword ptr [esp + 0x34]
        mov    eax, ecx
        mov    bx, word ptr [eax + 0x16]
        jmp    loc_3_1282
loc_3_126F:
        mov    dword ptr [esp + 0x30], 8
        jmp    loc_3_1474
loc_3_127C:
        mov    ecx, dword ptr [edi]
        mov    bx, word ptr [edi + 4]
loc_3_1282:
        push   dword ptr [esp + 0x50]
        push   dword ptr [esp + 0x2c]
        push   ecx
        push   0
        call   sub_3_1BE2
        mov    ebp, eax
        cmp    dword ptr [esp + 0x40], 0
        je     loc_3_12A4
        or     bx, bx
        jne    loc_3_12A4
        mov    bx, 0xffff
loc_3_12A4:
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esp + 0x14], eax
        cmp    dword ptr [esp + 0x18], eax
        jbe    loc_3_12DC
loc_3_12B2:
        push   0
        mov    eax, dword ptr [esp + 0x18]
        push   ebp
        push   dword ptr [eax]
        call   FILESEC_Service_13
        or     eax, eax
        je     loc_3_12D4
        add    dword ptr [esp + 0x14], esi
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [esp + 0x18], eax
        ja     loc_3_12B2
        jmp    loc_3_12DC
loc_3_12D4:
        mov    eax, dword ptr [esp + 0x14]
        mov    word ptr [eax + 6], bx
loc_3_12DC:
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [esp + 0x18], eax
        ja     loc_3_137D
        push   ebp
        call   sub_3_2474
        add    esp, 4
        inc    eax
        mov    dword ptr [esp + 0x3c], eax
        sub    eax, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esp + 0x14]
        lea    ecx, [eax + ecx + 8]
        or     ecx, ecx
        jle    loc_3_1346
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [esp + 0x20]
        sub    eax, dword ptr [esp + 0x38]
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        push   eax
        lea    eax, [esp + 0x4c]
        push   ecx
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        lea    eax, [esp + 0x38]
        push   eax
        call   sub_3_0E20
        or     eax, eax
        je     loc_3_13DB
loc_3_1346:
        mov    eax, dword ptr [esp + 0x3c]
        sub    dword ptr [esp + 0x1c], eax
        mov    ecx, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esp + 0x1c]
        push   -1
        push   ebp
        mov    dword ptr [ecx], eax
        push   dword ptr [esp + 0x24]
        call   sub_3_246C
        mov    eax, dword ptr [esp + 0x20]
        add    esp, 0xc
        mov    word ptr [eax + 4], 0xffff
        mov    eax, dword ptr [esp + 0x14]
        mov    word ptr [eax + 6], bx
        add    dword ptr [esp + 0x18], esi
loc_3_137D:
        inc    dword ptr [esp + 0x44]
        add    dword ptr [esp + 0x34], 0x18
        mov    ebx, dword ptr [esp + 0x74]
        add    edi, 6
        movsx  eax, word ptr [ebx + 6]
        cmp    eax, dword ptr [esp + 0x44]
        jg     loc_3_125B
loc_3_139B:
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [esp + 0x14], eax
        cmp    dword ptr [esp + 0x18], eax
        jbe    loc_3_1413
        mov    esi, 8
loc_3_13AE:
        mov    ecx, dword ptr [esp + 0x14]
        mov    ax, word ptr [ecx + 6]
        add    ecx, 6
        mov    edx, dword ptr [esp + 0x14]
        cmp    word ptr [edx + 4], ax
        je     loc_3_1405
        cmp    ax, 0xffff
        jne    loc_3_13E8
        mov    eax, edx
        push   dword ptr [eax]
        push   dword ptr [esp + 0x30]
        call   sub_3_2424
        add    esp, 8
        jmp    loc_3_1401
loc_3_13DB:
        mov    dword ptr [esp + 0x30], 8
        jmp    loc_3_1474
loc_3_13E8:
        push   2
        push   ecx
        push   3
        mov    eax, dword ptr [esp + 0x20]
        push   0
        push   dword ptr [eax]
        push   dword ptr [esp + 0x40]
        call   sub_3_243C
        add    esp, 0x18
loc_3_1401:
        mov    dword ptr [esp + 0x30], eax
loc_3_1405:
        add    dword ptr [esp + 0x14], esi
        mov    eax, dword ptr [esp + 0x14]
        cmp    dword ptr [esp + 0x18], eax
        ja     loc_3_13AE
loc_3_1413:
        push   0
        push   0
        push   0
        lea    eax, [esp + 0x60]
        push   0
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x58]
        call   sub_3_2444
        add    esp, 0x30
        or     eax, eax
        jne    loc_3_1474
        cmp    dword ptr [esp + 0x54], 0
        jne    loc_3_1474
        mov    word ptr [esp + 0x12], 0x8000
        lea    eax, [esp + 0x12]
        push   2
        push   eax
        push   3
        push   0
        push   0xf4                          ; -> obj4:0x00F4
        push   dword ptr [esp + 0x40]
        call   sub_3_243C
        mov    dword ptr [esp + 0x48], eax
        add    esp, 0x18
        jmp    loc_3_1474
loc_3_146C:
        mov    dword ptr [esp + 0x30], 0x8ae
loc_3_1474:
        cmp    dword ptr [esp + 0x20], 0
        je     loc_3_1493
        mov    eax, dword ptr [esp + 0x38]
        cmp    dword ptr [esp + 0x20], eax
        je     loc_3_1493
        push   0
        push   dword ptr [esp + 0x24]
        call   sub_3_2454
        add    esp, 8
loc_3_1493:
        cmp    dword ptr [esp + 0x28], 0
        je     loc_3_14A3
        push   dword ptr [esp + 0x28]
        call   sub_3_20D9
loc_3_14A3:
        cmp    dword ptr [esp + 0x58], 0
        je     loc_3_14B6
        push   dword ptr [esp + 0x2c]
        call   sub_3_2404
        add    esp, 4
loc_3_14B6:
        mov    eax, dword ptr [esp + 0x30]
loc_3_14BA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x54
        ret    0x18
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_14D0:
        mov    eax, dword ptr [esp + 4]
        mov    cl, byte ptr [eax]
        cmp    cl, 0x41
        jb     loc_3_14E0
        cmp    cl, 0x5a
        jbe    loc_3_14EA
loc_3_14E0:
        cmp    cl, 0x61
        jb     loc_3_1504
        cmp    cl, 0x7a
        ja     loc_3_1504
loc_3_14EA:
        cmp    byte ptr [eax + 1], 0x3a
        jne    loc_3_1504
        cmp    dword ptr [esp + 8], 0
        je     loc_3_14FD
        cmp    byte ptr [eax + 2], 0x5c
        jne    loc_3_1504
loc_3_14FD:
        mov    eax, 1
        jmp    loc_3_1506
loc_3_1504:
        sub    eax, eax
loc_3_1506:
        ret    8
sub_3_1509:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 1
        mov    dword ptr [ebp - 0xc], eax
        or     eax, eax
        je     loc_3_1547
        call   sub_3_2484
        mov    ecx, eax
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ecx]
        mov    ecx, eax
        and    ecx, 0x30000
        mov    dword ptr [ebp - 0x10], ecx
        mov    ecx, dword ptr [ebp - 8]
        and    eax, 0xfffcffff
        mov    dword ptr [ecx], eax
loc_3_1547:
        mov    eax, 0x7160
        mov    ecx, 2
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        push   0x21
        xor    ebx, ebx
        xor    edx, edx
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jae    loc_3_1568
        mov    dword ptr [ebp - 4], eax
loc_3_1568:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_3_1578
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0x10]
        or     dword ptr [ecx], eax
        jmp    loc_3_1589
loc_3_1578:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_3_1589
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 0xc]
        call   FILESEC_Service_15
loc_3_1589:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_3_1595:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x1c]
        or     edi, edi
        je     loc_3_15AE
        cmp    byte ptr [edi], 0
        mov    ebx, 0
        jne    loc_3_15B3
loc_3_15AE:
        mov    ebx, 1
loc_3_15B3:
        mov    dword ptr [esp + 0x14], 0
        mov    esi, dword ptr [esp + 0x24]
        mov    ebp, esi
        and    ebp, 0x100
        je     loc_3_15DB
        test   esi, 0x200
        je     loc_3_15DB
        mov    eax, 0x57
        jmp    loc_3_1777
loc_3_15DB:
        or     ebx, ebx
        jne    loc_3_1604
        cmp    byte ptr [edi], 0x5c
        jne    loc_3_15EA
        cmp    byte ptr [edi + 1], 0x5c
        jne    loc_3_1604
loc_3_15EA:
        push   1
        push   edi
        call   sub_3_14D0
        mov    dword ptr [esp + 0x14], eax
        or     eax, eax
        jne    loc_3_1604
        mov    eax, 0x57
        jmp    loc_3_1777
loc_3_1604:
        push   -1
        push   0xc8                          ; -> loc_4_00C8
        push   dword ptr [esp + 0x28]
        call   sub_3_246C
        add    esp, 0xc
        or     ebx, ebx
        jne    loc_3_1746
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_3_1651
        test   esi, 1
        mov    eax, dword ptr [esp + 0x20]
        je     loc_3_1647
        add    eax, 0x10
        push   -1
        inc    edi
        push   edi
        push   eax
        call   sub_3_246C
        add    esp, 0xc
        jmp    loc_3_1746
loc_3_1647:
        add    eax, 0x10
        push   eax
        inc    edi
        jmp    loc_3_1740
loc_3_1651:
        mov    ebx, esi
        and    ebx, 0xff
        mov    eax, dword ptr [esp + 0x20]
        add    eax, 0x10
        mov    dword ptr [esp + 0x10], eax
        push   ebx
        push   dword ptr [esp + 0x14]
        push   edi
        call   sub_3_1509
        or     eax, eax
        je     loc_3_1746
        test   esi, 0x200
        je     loc_3_1689
        mov    eax, 0x8ae
        jmp    loc_3_1777
loc_3_1689:
        or     ebp, ebp
        je     loc_3_1723
        push   edi
        call   sub_3_2474
        add    esp, 4
        mov    ebp, eax
        cmp    ebp, 3
        ja     loc_3_16AB
        mov    eax, 0x8ae
        jmp    loc_3_1777
loc_3_16AB:
        call   sub_3_2070
        mov    esi, eax
        or     esi, esi
        jne    loc_3_16C0
        mov    eax, 8
        jmp    loc_3_1777
loc_3_16C0:
        inc    ebp
        push   ebp
        push   edi
        push   esi
        call   sub_3_247C
        add    esp, 0xc
        push   0x5c
        push   esi
        call   FILESEC_Service_14
        mov    edi, eax
        or     edi, edi
        je     loc_3_1712
loc_3_16DA:
        mov    eax, edi
        sub    eax, esi
        sub    eax, 2
        cmp    eax, 1
        sbb    ebp, ebp
        neg    ebp
        push   ebx
        mov    byte ptr [edi + ebp], 0
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_3_1509
        or     eax, eax
        je     loc_3_1712
        or     ebp, ebp
        mov    edi, 0
        jne    loc_3_170E
        push   0x5c
        push   esi
        call   FILESEC_Service_14
        mov    edi, eax
loc_3_170E:
        or     edi, edi
        jne    loc_3_16DA
loc_3_1712:
        push   esi
        call   sub_3_20D9
        or     edi, edi
        jne    loc_3_1746
        mov    eax, 0x8ae
        jmp    loc_3_1777
loc_3_1723:
        test   esi, 1
        je     loc_3_173C
        push   -1
        push   edi
        push   dword ptr [esp + 0x18]
        call   sub_3_246C
        add    esp, 0xc
        jmp    loc_3_1746
loc_3_173C:
        push   dword ptr [esp + 0x10]
loc_3_1740:
        push   edi
        call   FILESEC_Service_15
loc_3_1746:
        push   dword ptr [esp + 0x20]
        call   sub_3_2474
        add    esp, 4
        mov    edi, eax
        push   0x5c
        mov    esi, edi
        push   dword ptr [esp + 0x24]
        call   FILESEC_Service_14
        sub    esi, eax
        add    esi, dword ptr [esp + 0x20]
        cmp    esi, 1
        jne    loc_3_1775
        mov    eax, dword ptr [esp + 0x20]
        mov    byte ptr [edi + eax - 1], 0
loc_3_1775:
        sub    eax, eax
loc_3_1777:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    0xc
sub_3_1781:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x28]
        or     eax, 0x100
        mov    edi, dword ptr [esp + 0x20]
        push   eax
        push   edi
        sub    esi, esi
        push   dword ptr [esp + 0x24]
        call   sub_3_1595
        cmp    eax, esi
        je     loc_3_17B0
        mov    eax, 0x8ae
        jmp    loc_3_1847
loc_3_17B0:
        lea    ebx, [edi + 0xf]
        push   edi
        call   sub_3_2474
        add    esp, 4
        add    eax, edi
        cmp    eax, ebx
        jbe    loc_3_1832
loc_3_17C2:
        sub    ebp, ebp
        lea    eax, [esp + 0x10]
        push   eax
        push   edi
        push   0x80000002
        call   sub_3_23FC
        add    esp, 0xc
        cmp    eax, ebp
        jne    loc_3_180C
        push   0
        push   0
        push   0
        lea    eax, [esp + 0x20]
        push   0
        mov    ebp, 1
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x3c]
        call   sub_3_2444
        add    esp, 0x30
        cmp    dword ptr [esp + 0x14], 0
        ja     loc_3_182D
loc_3_180C:
        or     ebp, ebp
        je     loc_3_181C
        push   dword ptr [esp + 0x10]
        call   sub_3_2404
        add    esp, 4
loc_3_181C:
        push   0x5c
        push   ebx
        call   FILESEC_Service_14
        mov    byte ptr [eax], 0
        cmp    eax, ebx
        ja     loc_3_17C2
        jmp    loc_3_1832
loc_3_182D:
        mov    esi, 1
loc_3_1832:
        or     esi, esi
        mov    eax, 3
        je     loc_3_1847
        mov    eax, dword ptr [esp + 0x24]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], ecx
        sub    eax, eax
loc_3_1847:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    0x10
sub_3_1851:
        sub    esp, 0x3c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x14], 0
        push   0
        push   0
        lea    eax, [esp + 0x3c]
        push   0
        push   eax
        lea    eax, [esp + 0x30]
        push   eax
        push   0
        push   0
        push   0
        push   0
        push   0
        push   0
        push   dword ptr [esp + 0x7c]
        call   sub_3_2444
        mov    dword ptr [esp + 0x48], eax
        add    esp, 0x30
        or     eax, eax
        jne    loc_3_1B3A
        cmp    dword ptr [esp + 0x20], 0
        je     loc_3_1B3A
        cmp    dword ptr [esp + 0x20], 1
        jne    loc_3_18E5
        mov    dword ptr [esp + 0x30], 2
        lea    eax, [esp + 0x30]
        push   eax
        lea    eax, [esp + 0x16]
        push   eax
        lea    eax, [esp + 0x40]
        push   eax
        push   0
        push   0xf4                          ; -> obj4:0x00F4
        push   dword ptr [esp + 0x64]
        call   sub_3_2434
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_18E5
        cmp    word ptr [esp + 0x12], 0x8000
        jne    loc_3_18E5
        mov    dword ptr [esp + 0x20], 0
loc_3_18E5:
        movsx  eax, word ptr [esp + 0x58]
        shl    eax, 2
        mov    ecx, dword ptr [esp + 0x60]
        mov    edx, dword ptr [eax + 0xe8]   ; -> loc_4_00E8
        imul   dx, word ptr [esp + 0x20]
        add    dx, word ptr [eax + 0xdc]     ; -> loc_4_00DC
        cmp    word ptr [ecx], dx
        jae    loc_3_1922
        mov    dword ptr [esp + 0x14], 2
        cmp    dword ptr [esp + 0x64], 0
        jne    loc_3_1922
        mov    eax, 2
        jmp    loc_3_1B3F
loc_3_1922:
        cmp    dword ptr [esp + 0x64], 0
        je     loc_3_1930
        mov    ecx, dword ptr [esp + 0x64]
        mov    word ptr [ecx], dx
loc_3_1930:
        cmp    dword ptr [esp + 0x14], 2
        je     loc_3_1956
        mov    eax, dword ptr [esp + 0x5c]
        mov    edi, dword ptr [eax]
        movzx  eax, dx
        lea    ecx, [edi + eax]
        mov    dword ptr [esp + 0x28], ecx
        mov    ecx, dword ptr [esp + 0x60]
        movzx  eax, word ptr [ecx]
        add    eax, edi
        mov    dword ptr [esp + 0x2c], eax
        jmp    loc_3_195A
loc_3_1956:
        mov    edi, dword ptr [esp + 0x3c]
loc_3_195A:
        cmp    dword ptr [esp + 0x64], 0
        mov    ebx, dword ptr [esp + 0x54]
        je     loc_3_1988
        lea    ebp, [ebx + 0x10]
        push   ebp
        call   sub_3_2474
        add    esp, 4
        lea    esi, [eax + 1]
        push   1
        push   ebp
        call   sub_3_14D0
        or     eax, eax
        jne    loc_3_1981
        inc    esi
loc_3_1981:
        mov    ecx, dword ptr [esp + 0x64]
        add    word ptr [ecx], si
loc_3_1988:
        call   sub_3_2070
        mov    esi, eax
        or     esi, esi
        jne    loc_3_1998
        jmp    loc_3_1B3A
loc_3_1998:
        cmp    dword ptr [esp + 0x14], 2
        je     loc_3_19F9
        mov    ebp, esi
        add    ebx, 0x10
        push   0
        push   ebx
        call   sub_3_14D0
        or     eax, eax
        jne    loc_3_19B6
        mov    byte ptr [esi], 0x5c
        lea    ebp, [esi + 1]
loc_3_19B6:
        push   -1
        push   ebx
        push   ebp
        call   sub_3_246C
        add    esp, 0xc
        cmp    byte ptr [esi], 0x5c
        je     loc_3_19D5
        cmp    byte ptr [esi + 2], 0
        jne    loc_3_19D5
        mov    byte ptr [esi + 2], 0x5c
        mov    byte ptr [esi + 3], 0
loc_3_19D5:
        lea    eax, [esp + 0x2c]
        push   dword ptr [esp + 0x68]
        push   eax
        mov    dword ptr [edi], esi
        push   dword ptr [esp + 0x30]
        push   edi
        call   sub_3_2000
        add    esp, 0x10
        or     eax, eax
        jne    loc_3_19F9
        mov    dword ptr [esp + 0x14], 1
loc_3_19F9:
        cmp    word ptr [esp + 0x58], 0
        jle    loc_3_1B10
        cmp    dword ptr [esp + 0x14], 2
        je     loc_3_1A21
        mov    word ptr [edi + 4], 0
        mov    eax, dword ptr [esp + 0x20]
        mov    word ptr [edi + 6], ax
        add    edi, 8
        mov    ebp, edi
        jmp    loc_3_1A29
loc_3_1A21:
        mov    ebp, dword ptr [esp + 0x40]
        mov    edi, dword ptr [esp + 0x44]
loc_3_1A29:
        sub    ebx, ebx
        cmp    dword ptr [esp + 0x20], ebx
        jle    loc_3_1B10
loc_3_1A35:
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_3_1B10
        mov    dword ptr [esp + 0x1c], 0x64
        mov    dword ptr [esp + 0x24], 2
        lea    eax, [esp + 0x24]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x50]
        push   eax
        lea    eax, [esp + 0x28]
        push   0
        push   eax
        push   esi
        push   ebx
        push   dword ptr [esp + 0x6c]
        call   sub_3_242C
        mov    dword ptr [esp + 0x38], eax
        add    esp, 0x20
        or     eax, eax
        jne    loc_3_1B05
        cmp    dword ptr [esp + 0x64], 0
        je     loc_3_1A9C
        cmp    word ptr [esp + 0x58], 2
        jne    loc_3_1A9C
        mov    eax, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esp + 0x64]
        inc    ax
        add    word ptr [ecx], ax
loc_3_1A9C:
        cmp    dword ptr [esp + 0x14], 2
        je     loc_3_1B05
        cmp    word ptr [esp + 0x58], 1
        jne    loc_3_1AD5
        test   byte ptr [esp + 0x68], 1
        je     loc_3_1AC0
        push   -1
        push   esi
        push   ebp
        call   sub_3_246C
        add    esp, 0xc
        jmp    loc_3_1AC7
loc_3_1AC0:
        push   ebp
        push   esi
        call   FILESEC_Service_16
loc_3_1AC7:
        mov    ax, word ptr [esp + 0x10]
        mov    word ptr [ebp + 0x16], ax
        add    ebp, 0x18
        jmp    loc_3_1B05
loc_3_1AD5:
        mov    dword ptr [edi], esi
        mov    ax, word ptr [esp + 0x10]
        push   dword ptr [esp + 0x68]
        mov    word ptr [edi + 4], ax
        lea    eax, [esp + 0x30]
        push   eax
        push   dword ptr [esp + 0x30]
        push   edi
        call   sub_3_2000
        add    esp, 0x10
        or     eax, eax
        jne    loc_3_1B02
        mov    dword ptr [esp + 0x14], 1
loc_3_1B02:
        add    edi, 6
loc_3_1B05:
        inc    ebx
        cmp    ebx, dword ptr [esp + 0x20]
        jl     loc_3_1A35
loc_3_1B10:
        cmp    dword ptr [esp + 0x14], 2
        je     loc_3_1B2E
        mov    eax, dword ptr [esp + 0x5c]
        mov    ecx, dword ptr [esp + 0x28]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 0x2c]
        sub    eax, ecx
        mov    ecx, dword ptr [esp + 0x60]
        mov    word ptr [ecx], ax
loc_3_1B2E:
        push   esi
        call   sub_3_20D9
        mov    eax, dword ptr [esp + 0x14]
        jmp    loc_3_1B3F
loc_3_1B3A:
        mov    eax, 3
loc_3_1B3F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x3c
        ret    0x1c
sub_3_1B49:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x1c]
        sub    ebx, ebx
        lea    esi, [edi + 0xf]
loc_3_1B59:
        push   dword ptr [esp + 0x20]
        call   sub_3_2404
        add    esp, 4
        mov    ebp, ebx
        push   edi
        push   0x80000002
        call   sub_3_2414
        add    esp, 8
        push   0x5c
        push   esi
        call   FILESEC_Service_14
        cmp    eax, esi
        jbe    loc_3_1BC6
        mov    byte ptr [eax], bl
        lea    eax, [esp + 0x20]
        push   eax
        push   edi
        push   0x80000002
        call   sub_3_23FC
        add    esp, 0xc
        push   ebx
        push   ebx
        push   ebx
        lea    eax, [esp + 0x1c]
        push   ebx
        mov    ebp, 1
        push   eax
        push   ebx
        lea    eax, [esp + 0x2c]
        push   ebx
        push   eax
        push   ebx
        push   ebx
        push   ebx
        push   dword ptr [esp + 0x4c]
        call   sub_3_2444
        add    esp, 0x30
        cmp    dword ptr [esp + 0x14], ebx
        jne    loc_3_1BC6
        cmp    dword ptr [esp + 0x10], ebx
        je     loc_3_1B59
loc_3_1BC6:
        or     ebp, ebp
        je     loc_3_1BD6
        push   dword ptr [esp + 0x20]
        call   sub_3_2404
        add    esp, 4
loc_3_1BD6:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
sub_3_1BE2:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        or     ebp, ebp
        je     loc_3_1BF9
        cmp    byte ptr [ebp], 0
        mov    edi, 0
        jne    loc_3_1BFE
loc_3_1BF9:
        mov    edi, 1
loc_3_1BFE:
        mov    esi, dword ptr [esp + 0x18]
        push   0xf4                          ; -> obj4:0x00F4
        push   esi
        call   sub_3_2140
        or     eax, eax
        je     loc_3_1C8C
        push   0x5c
        push   esi
        call   FILESEC_Service_14
        or     eax, eax
        jne    loc_3_1C8C
        or     edi, edi
        jne    loc_3_1C2E
        push   ebp
        call   sub_3_2474
        add    esp, 4
        mov    ebx, eax
        jmp    loc_3_1C3A
loc_3_1C2E:
        mov    ebp, 0x2f0                    ; -> obj1:0x02F0
        mov    ebx, dword ptr [0x370]        ; -> obj1:0x0370
        dec    ebx
loc_3_1C3A:
        mov    edi, dword ptr [esp + 0x20]
        test   edi, 1
        jne    loc_3_1C5A
        cmp    ebp, 0x2f0                    ; -> obj1:0x02F0
        je     loc_3_1C5A
        push   dword ptr [esp + 0x1c]
        push   ebp
        call   FILESEC_Service_15
        jmp    loc_3_1C68
loc_3_1C5A:
        push   ebx
        push   ebp
        push   dword ptr [esp + 0x24]
        call   sub_3_247C
        add    esp, 0xc
loc_3_1C68:
        mov    eax, dword ptr [esp + 0x1c]
        add    eax, ebx
        test   edi, 0x101
        mov    byte ptr [eax], 0x5c
        je     loc_3_1C88
        push   -1
        push   esi
        inc    eax
        push   eax
        call   sub_3_246C
        add    esp, 0xc
        jmp    loc_3_1CA2
loc_3_1C88:
        inc    eax
        push   eax
        jmp    loc_3_1C9C
loc_3_1C8C:
        test   dword ptr [esp + 0x20], 0x101
        mov    eax, esi
        jne    loc_3_1CA6
        push   dword ptr [esp + 0x1c]
loc_3_1C9C:
        push   esi
        call   FILESEC_Service_15
loc_3_1CA2:
        mov    eax, dword ptr [esp + 0x1c]
loc_3_1CA6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0x10
        db  000h, 000h, 000h
sub_3_1CB0:
        sub    esp, 4
        mov    eax, dword ptr [esp + 8]
        test   byte ptr [eax + 0x11], 1
        jne    loc_3_1CF3
        mov    ecx, dword ptr [esp + 0xc]
        or     ecx, ecx
        je     loc_3_1CEF
        cmp    byte ptr [ecx], 0
        je     loc_3_1CEF
        lea    edx, [esp + 2]
        push   0
        push   edx
        push   0x40
        push   ecx
        push   0x80000002
        push   dword ptr [eax + 4]
        push   dword ptr [eax + 0x14]
        call   sub_3_0950
        or     eax, eax
        jne    loc_3_1CEF
        test   byte ptr [esp + 2], 0x40
        jne    loc_3_1CF3
loc_3_1CEF:
        sub    eax, eax
        jmp    loc_3_1CF8
loc_3_1CF3:
        mov    eax, 1
loc_3_1CF8:
        add    esp, 4
        ret    8
sub_3_1CFE:
        mov    eax, dword ptr [esp + 4]
        lea    ecx, [eax + 8]
        movsx  eax, word ptr [eax + 6]
        or     eax, eax
        jle    loc_3_1D1A
loc_3_1D0D:
        cmp    dword ptr [ecx], 0
        je     loc_3_1D22
        dec    eax
        add    ecx, 6
        or     eax, eax
        jg     loc_3_1D0D
loc_3_1D1A:
        mov    eax, 1
loc_3_1D1F:
        ret    4
loc_3_1D22:
        sub    eax, eax
        jmp    loc_3_1D1F
loc_3_1D26:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi + 2]
        push   dword ptr [eax]
        push   dword ptr [esp + 0xc]
        call   sub_3_1CB0
        or     eax, eax
        jne    loc_3_1D44
        mov    eax, 5
        jmp    loc_3_1D71
loc_3_1D44:
        cmp    word ptr [esi + 6], 2
        jne    loc_3_1D5E
        push   dword ptr [esi + 2]
        call   sub_3_1CFE
        or     eax, eax
        jne    loc_3_1D5E
        mov    eax, 0x57
        jmp    loc_3_1D71
loc_3_1D5E:
        mov    ax, word ptr [esi]
        push   eax
        push   dword ptr [esi + 2]
        mov    ax, word ptr [esi + 6]
        push   eax
        push   0
        call   sub_3_00B0
loc_3_1D71:
        pop    esi
        ret    
loc_3_1D73:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   dword ptr [esi]
        push   dword ptr [esp + 0xc]
        call   sub_3_1CB0
        or     eax, eax
        mov    eax, 5
        je     loc_3_1D95
        push   dword ptr [esi]
        push   0
        call   sub_3_0410
loc_3_1D95:
        pop    esi
        ret    
loc_3_1D97:
        mov    edx, dword ptr [esp + 4]
        test   byte ptr [edx + 0x11], 1
        je     loc_3_1DA3
        sub    edx, edx
loc_3_1DA3:
        mov    ecx, dword ptr [esp + 8]
        push   0
        push   dword ptr [ecx]
        push   dword ptr [ecx + 4]
        mov    ax, word ptr [ecx + 8]
        push   eax
        push   dword ptr [ecx + 0xa]
        mov    ax, word ptr [ecx + 0xe]
        push   eax
        mov    ax, word ptr [ecx + 0x10]
        push   eax
        push   dword ptr [ecx + 0x12]
        push   edx
        call   sub_3_0565
        ret    
loc_3_1DCA:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   dword ptr [esi + 0xc]
        push   dword ptr [esp + 0xc]
        call   sub_3_1CB0
        or     eax, eax
        jne    loc_3_1DE6
        mov    eax, 5
        jmp    loc_3_1DFF
loc_3_1DE6:
        push   dword ptr [esi]
        mov    ax, word ptr [esi + 4]
        push   eax
        push   dword ptr [esi + 6]
        mov    ax, word ptr [esi + 0xa]
        push   eax
        push   dword ptr [esi + 0xc]
        push   0
        call   sub_3_0850
loc_3_1DFF:
        pop    esi
        ret    
loc_3_1E01:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        push   dword ptr [ebx + 4]
        push   esi
        call   sub_3_1CB0
        or     eax, eax
        jne    loc_3_1E66
        call   sub_3_2070
        mov    edi, eax
        or     edi, edi
        jne    loc_3_1E2C
        mov    eax, 8
        jmp    loc_3_1E75
loc_3_1E2C:
        lea    ebp, [edi + 0x64]
        push   0
        push   edi
        push   dword ptr [esi + 4]
        push   dword ptr [esi + 0x14]
        call   sub_3_1BE2
        mov    esi, eax
        push   0
        push   ebp
        push   dword ptr [ebx + 8]
        push   0
        call   sub_3_1BE2
        push   0
        push   eax
        push   esi
        call   FILESEC_Service_13
        mov    esi, eax
        push   edi
        call   sub_3_20D9
        or     esi, esi
        mov    eax, 5
        jne    loc_3_1E75
loc_3_1E66:
        push   dword ptr [ebx]
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   0
        call   sub_3_0CFE
loc_3_1E75:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_1E7A:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   dword ptr [esi + 0xa]
        push   dword ptr [esp + 0xc]
        call   sub_3_1CB0
        or     eax, eax
        jne    loc_3_1E96
        mov    eax, 5
        jmp    loc_3_1ED4
loc_3_1E96:
        mov    ax, word ptr [esi + 8]
        cmp    ax, 2
        je     loc_3_1EA6
        cmp    ax, 0xc
        jne    loc_3_1EB9
loc_3_1EA6:
        push   dword ptr [esi + 4]
        call   sub_3_1CFE
        or     eax, eax
        jne    loc_3_1EB9
        mov    eax, 0x57
        jmp    loc_3_1ED4
loc_3_1EB9:
        mov    ax, word ptr [esi]
        push   eax
        mov    ax, word ptr [esi + 2]
        push   eax
        push   dword ptr [esi + 4]
        mov    ax, word ptr [esi + 8]
        push   eax
        push   dword ptr [esi + 0xa]
        push   0
        call   sub_3_0F01
loc_3_1ED4:
        pop    esi
        ret    
loc_3_1ED6:
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0xc]
        push   eax
        mov    ax, word ptr [esp + 0xc]
        push   eax
        push   0
        call   sub_3_00B0
        ret    
loc_3_1EED:
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        mov    ax, word ptr [esp + 0x10]
        push   eax
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   0
        call   sub_3_0370
        ret    
loc_3_1F08:
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        mov    ax, word ptr [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        push   0
        call   sub_3_0391
        ret    
loc_3_1F27:
        push   dword ptr [esp + 4]
        push   0
        call   sub_3_0410
        ret    
loc_3_1F33:
        mov    eax, dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x1c]
        push   eax
        mov    ax, word ptr [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x1c]
        push   eax
        mov    ax, word ptr [esp + 0x1c]
        push   eax
        push   dword ptr [esp + 0x1c]
        push   dword ptr [esp + 0x1c]
        push   0
        call   sub_3_0530
        ret    
loc_3_1F5D:
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        mov    ax, word ptr [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x14]
        push   eax
        mov    ax, word ptr [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x14]
        push   0
        call   sub_3_0850
        ret    
loc_3_1F7F:
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        push   0
        call   sub_3_0CFE
        ret    
loc_3_1F93:
        mov    eax, dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x18]
        push   eax
        mov    ax, word ptr [esp + 0x18]
        push   eax
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   0
        call   sub_3_0D1C
        ret    
loc_3_1FB6:
        push   dword ptr [esp + 0x14]
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x14]
        push   eax
        mov    ax, word ptr [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x14]
        push   0
        call   sub_3_0F01
        ret    
loc_3_1FD5:
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        push   0
        call   sub_3_0D8B
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C3h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_2000:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, 1
        mov    eax, dword ptr [esi]
        or     eax, eax
        jne    loc_3_201A
        mov    dword ptr [esi], 0x1a8        ; -> obj4:0x01A8
        jmp    loc_3_2026
loc_3_201A:
        push   eax
        call   sub_3_2474
        add    esp, 4
        lea    edi, [eax + 1]
loc_3_2026:
        mov    ebx, dword ptr [esp + 0x18]
        mov    edx, dword ptr [ebx]
        mov    ecx, edx
        sub    ecx, dword ptr [esp + 0x14]
        cmp    ecx, edi
        jl     loc_3_205F
        sub    edx, edi
        mov    dword ptr [ebx], edx
        test   byte ptr [esp + 0x1c], 1
        je     loc_3_204F
        push   edi
        push   dword ptr [esi]
        push   edx
        call   sub_3_247C
        add    esp, 0xc
        jmp    loc_3_2057
loc_3_204F:
        push   edx
        push   dword ptr [esi]
        call   FILESEC_Service_16
loc_3_2057:
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi], eax
        mov    eax, edi
        jmp    loc_3_2067
loc_3_205F:
        mov    dword ptr [esi], 0
        sub    eax, eax
loc_3_2067:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h
sub_3_2070:
        push   esi
        push   1
        push   dword ptr [0x2ec]             ; -> obj1:0x02EC
        call   sub_3_245C
        add    esp, 8
        cmp    dword ptr [0x1c0], 0          ; -> obj4:0x01C0
        je     loc_3_20B1
        mov    esi, dword ptr [0x1c0]        ; -> obj4:0x01C0
        mov    eax, esi
        push   dword ptr [0x2ec]             ; -> obj1:0x02EC
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x1c0], eax        ; -> obj4:0x01C0
        dec    dword ptr [0x1c4]             ; -> obj4:0x01C4
        call   sub_3_2464
        add    esp, 4
        mov    eax, esi
        jmp    loc_3_20D7
loc_3_20B1:
        push   dword ptr [0x2ec]             ; -> obj1:0x02EC
        call   sub_3_2464
        add    esp, 4
        push   0x200
        push   0x200
        call   sub_3_244C
        add    esp, 8
        or     eax, eax
        jne    loc_3_20D7
        sub    eax, eax
loc_3_20D7:
        pop    esi
        ret    
sub_3_20D9:
        push   1
        push   dword ptr [0x2ec]             ; -> obj1:0x02EC
        call   sub_3_245C
        add    esp, 8
        cmp    dword ptr [0x1c4], 4          ; -> obj4:0x01C4
        jl     loc_3_2110
        push   dword ptr [0x2ec]             ; -> obj1:0x02EC
        call   sub_3_2464
        add    esp, 4
        push   0
        push   dword ptr [esp + 8]
        call   sub_3_2454
        add    esp, 8
        jmp    loc_3_2135
loc_3_2110:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [0x1c0]        ; -> obj4:0x01C0
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x1c0], eax        ; -> obj4:0x01C0
        inc    dword ptr [0x1c4]             ; -> obj4:0x01C4
        push   dword ptr [0x2ec]             ; -> obj1:0x02EC
        call   sub_3_2464
        add    esp, 4
loc_3_2135:
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_2140:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
loc_3_2148:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    loc_3_2168
        or     dl, dl
        je     loc_3_2164
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    loc_3_2168
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    loc_3_2148
loc_3_2164:
        sub    eax, eax
        jmp    loc_3_216D
loc_3_2168:
        sbb    eax, eax
        sbb    eax, -1
loc_3_216D:
        ret    8
        db  056h, 057h, 08Bh, 04Ch, 024h, 014h, 08Bh, 074h, 024h, 00Ch, 02Bh, 0C0h, 08Bh, 07Ch, 024h, 010h
        db  0F3h, 0A6h, 074h, 005h, 01Bh, 0C0h, 083h, 0D8h, 0FFh, 05Fh, 05Eh, 0C2h, 00Ch, 000h
FILESEC_Service_11:
        mov    eax, dword ptr [0x1dc]        ; -> obj4:0x01DC
        ret    
FILESEC_Service_15:
        push   0xffff
        push   1
        push   0
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x18]
        call   sub_3_009A
        add    esp, 0x14
        mov    eax, 1
        ret    8
FILESEC_Service_16:
        push   0xffff
        push   0
        push   1
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x18]
        call   sub_3_009A
        add    esp, 0x14
        mov    eax, 1
        ret    8
FILESEC_Service_12:
        sub    esp, 0x84
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 0x9c], 1
        sbb    ebp, ebp
        inc    ebp
        mov    edi, dword ptr [esp + 0x98]
        push   edi
        call   sub_3_2474
        add    esp, 4
        mov    esi, eax
        and    eax, 0x7fffffff
        cmp    eax, 0x40
        jbe    loc_3_2226
        push   0x200
        lea    eax, [esi*2]
        push   eax
        call   sub_3_244C
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_3_222A
        sub    eax, eax
        jmp    loc_3_226E
loc_3_2226:
        lea    ebx, [esp + 0x14]
loc_3_222A:
        push   ebp
        push   esi
        push   edi
        push   ebx
        call   sub_3_008E
        mov    dword ptr [esp + 0x20], eax
        add    esp, 0x10
        push   eax
        push   ebx
        push   ebx
        call   sub_3_00A0
        add    esp, 0xc
        push   ebp
        push   esi
        push   dword ptr [esp + 0x18]
        push   ebx
        push   edi
        call   sub_3_0094
        add    esp, 0x14
        mov    byte ptr [eax + edi], 0
        lea    eax, [esp + 0x14]
        cmp    ebx, eax
        je     loc_3_226C
        push   0
        push   ebx
        call   sub_3_2454
        add    esp, 8
loc_3_226C:
        mov    eax, edi
loc_3_226E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x84
        ret    8
FILESEC_Service_13:
        sub    esp, 0x84
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    ebx, [esp + 0x14]
        push   dword ptr [esp + 0x98]
        call   sub_3_2474
        add    esp, 4
        lea    esi, [eax + 1]
        push   dword ptr [esp + 0x9c]
        call   sub_3_2474
        add    esp, 4
        inc    eax
        mov    dword ptr [esp + 0x10], eax
        cmp    eax, esi
        je     loc_3_22C8
        mov    ebp, 1
        jb     loc_3_2366
        mov    ebp, 0xffffffff
        jmp    loc_3_2366
loc_3_22C8:
        lea    eax, [esi*4]
        cmp    eax, 0x80
        jbe    loc_3_22F1
        push   0x200
        push   eax
        call   sub_3_244C
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_3_22F1
        mov    ebp, 0x80000000
        jmp    loc_3_2366
loc_3_22F1:
        cmp    dword ptr [esp + 0xa0], 1
        sbb    ebp, ebp
        inc    ebp
        push   ebp
        push   esi
        lea    edi, [ebx + esi*2]
        push   dword ptr [esp + 0xa0]
        push   ebx
        call   sub_3_008E
        add    esp, 0x10
        push   ebp
        mov    esi, eax
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0xa4]
        push   edi
        call   sub_3_008E
        mov    dword ptr [esp + 0x20], eax
        add    esp, 0x10
        cmp    eax, esi
        je     loc_3_233E
        mov    ebp, 1
        jb     loc_3_2366
        mov    ebp, 0xffffffff
        jmp    loc_3_2366
loc_3_233E:
        push   esi
        push   ebx
        push   ebx
        call   sub_3_00A0
        add    esp, 0xc
        push   dword ptr [esp + 0x10]
        push   edi
        push   edi
        call   sub_3_00A0
        add    esp, 0xc
        mov    ecx, esi
        mov    esi, ebx
        sub    ebp, ebp
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_3_2366
        sbb    ebp, ebp
        sbb    ebp, -1
loc_3_2366:
        lea    eax, [esp + 0x14]
        cmp    ebx, eax
        je     loc_3_237D
        or     ebx, ebx
        je     loc_3_237D
        push   0
        push   ebx
        call   sub_3_2454
        add    esp, 8
loc_3_237D:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x84
        ret    0xc
FILESEC_Service_14:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0x1dc], 0          ; -> obj4:0x01DC
        jne    loc_3_23C5
        mov    esi, dword ptr [esp + 0x10]
        push   esi
        call   sub_3_2474
        add    esp, 4
        lea    eax, [esi + eax - 1]
        cmp    eax, esi
        jb     loc_3_23C1
        mov    edi, dword ptr [esp + 0x14]
loc_3_23B1:
        movzx  ecx, byte ptr [eax]
        cmp    ecx, edi
        je     loc_3_23BD
        dec    eax
        cmp    eax, esi
        jae    loc_3_23B1
loc_3_23BD:
        cmp    eax, esi
        jae    loc_3_23F3
loc_3_23C1:
        sub    eax, eax
        jmp    loc_3_23F3
loc_3_23C5:
        mov    esi, dword ptr [esp + 0x10]
        sub    ebx, ebx
        cmp    byte ptr [esi], bl
        je     loc_3_23F1
        mov    edi, dword ptr [esp + 0x14]
loc_3_23D3:
        mov    al, byte ptr [esi]
        push   eax
        call   sub_3_007D
        or     eax, eax
        jne    loc_3_23E8
        movzx  ecx, byte ptr [esi]
        cmp    ecx, edi
        jne    loc_3_23E8
        mov    ebx, esi
loc_3_23E8:
        lea    esi, [eax + esi + 1]
        cmp    byte ptr [esi], 0
        jne    loc_3_23D3
loc_3_23F1:
        mov    eax, ebx
loc_3_23F3:
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  000h, 000h, 000h
sub_3_23FC:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        add    byte ptr [eax], al
sub_3_2404:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        add    byte ptr [eax], al
sub_3_240C:
        VMMCall  VMM._RegCreateKey               ; cd204a810100
        add    byte ptr [eax], al
sub_3_2414:
        VMMCall  VMM._RegDeleteKey               ; cd204b810100
        add    byte ptr [eax], al
sub_3_241C:
        VMMCall  VMM._RegEnumKey                 ; cd204c810100
        add    byte ptr [eax], al
sub_3_2424:
        VMMCall  VMM._RegDeleteValue             ; cd204f810100
        add    byte ptr [eax], al
sub_3_242C:
        VMMCall  VMM._RegEnumValue               ; cd2050810100
        add    byte ptr [eax], al
sub_3_2434:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        add    byte ptr [eax], al
sub_3_243C:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        add    byte ptr [eax], al
sub_3_2444:
        VMMCall  VMM._RegQueryInfoKey            ; cd205e810100
        add    byte ptr [eax], al
sub_3_244C:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_3_2454:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_3_245C:
        VMMCall  VMM._EnterMutex                 ; cd202e810100
        add    byte ptr [eax], al
sub_3_2464:
        VMMCall  VMM._LeaveMutex                 ; cd202f810100
        add    byte ptr [eax], al
sub_3_246C:
        VMMCall  VMM._lstrcpyn                   ; cd207b810100
        add    byte ptr [eax], al
sub_3_2474:
        VMMCall  VMM._lstrlen                    ; cd207c810100
        add    byte ptr [eax], al
sub_3_247C:
        VMMCall  VMM._lmemcpy                    ; cd207d810100
        add    byte ptr [eax], al
sub_3_2484:
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    eax, edi
        pop    edi
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x1e0  RW-
sub_4_0000:
        mov    al, 0
        add    byte ptr [eax], al
        jo     9
        add    byte ptr [eax], al
        adc    byte ptr [eax + eax], al
        add    byte ptr [eax], dh            ; -> sub_3_0530
        add    eax, 0x8500000                ; -> sub_3_0850
        add    byte ptr [eax], al
        dec    byte ptr [eax + eax]
        add    byte ptr [ecx], al            ; -> sub_3_0F01
        sldt   word ptr [eax]
        push   ebx
        inc    ebp
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebp
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebp
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebp
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebp
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0080:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        ljmp   8:0x4b000000
        add    byte ptr [esi + 0x53000008], ch
        inc    ebp
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00A4:
        pop    esp
        inc    ecx
        inc    esp
        dec    ebp
        dec    ecx
        dec    esi
        pop    esp
        push   edx
        inc    ebp
        dec    ebp
        dec    edi
        push   esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x45], dl
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00C8:
        push   ebx
        inc    ebp
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00DC:
        add    al, 0
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_00E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [edx], ch
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x45], dl
        inc    ebx
        push   ebp
        push   edx
        dec    ecx
        push   esp
        pop    ecx
        pop    esp
        inc    ecx
        inc    ebx
        inc    ebx
        inc    ebp
        push   ebx
        push   ebx
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_010C:
        push   edi
        jae    loc_4_0163
        add    byte ptr [edx], bh
        add    byte ptr [eax], al
loc_4_0114:
        jp     loc_4_016D
        push   edi
        jb     loc_4_0165
        push   0x72577a00
        dec    esp
        push   0x7a000000
        jp     0x18f
        add    byte ptr [edx + 0x57], bh
        jae    loc_4_0180
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [0x26000000], ch     ; -> loc_3_1D26
        sbb    eax, 0x10c0000                ; -> loc_4_010C
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr cs:[eax], al
        add    byte ptr [ebx + 0x1d], dh     ; -> loc_3_1D73
        add    byte ptr [eax], al
        adc    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Ah, 000h, 000h, 000h
        dd  loc_3_1D97               ; fixup
        dd  loc_4_0114               ; fixup
loc_4_0163:
        add    byte ptr [eax], al
loc_4_0165:
        add    byte ptr [eax], al
        cmp    bh, 0xff
        db  0FFh, 0FFh, 02Bh
loc_4_016D:
        add    byte ptr [eax], al
        add    dl, cl                        ; -> loc_3_1DCA
        sbb    eax, 0x11c0000                ; -> obj4:0x011C
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_4_0180:
        imul   eax, dword ptr [eax], 0x1e010000 ; -> loc_3_1E01
        add    byte ptr [eax], al
        and    al, 1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 02Ch, 000h, 000h, 000h
        dd  loc_3_1E7A               ; fixup
        dd  obj4:0x0128              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 000h, 000h, 000h, 000h, 053h, 045h, 043h, 055h
        db  052h, 049h, 054h, 059h, 05Ch, 041h, 043h, 043h, 045h, 053h, 053h, 05Ch, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 053h, 045h, 043h, 055h, 052h, 049h, 054h, 059h
        db  05Ch, 041h, 043h, 043h, 045h, 053h, 053h, 05Ch, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh
VxD__LDATA ENDS
