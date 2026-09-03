; SPOOLER -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 3216 instructions recovered by recursive descent

Declare_Virtual_Device SPOOLER, 1, 0, SPOOLER_Control, 0002Ch, Undefined_Init_Order
;   DDK version 0400h   init_order 0A0010100h   flags 00000h
;   service table: obj1:0xa0, 17 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x140  R-X
SPOOLER_Control:
        sub    eax, 1
        cmp    eax, 0x23
        jae    loc_1_000F
        jmp    dword ptr [eax*4 + 0x11]      ; -> obj1:0x0011
loc_1_000F:
        clc    
        ret    
        ; ---- dispatch table (35 entries) ----
        dd  loc_2_0000
        dd  loc_2_0023
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
        dd  loc_3_04B4
        db  000h, 000h, 000h
        dd  SPOOLER_Service_00       ; fixup
        dd  SPOOLER_Service_01       ; fixup
        dd  SPOOLER_Service_02       ; fixup
        dd  SPOOLER_Service_03       ; fixup
        dd  SPOOLER_Service_04       ; fixup
        dd  SPOOLER_Service_05       ; fixup
        dd  SPOOLER_Service_06       ; fixup
        dd  SPOOLER_Service_07       ; fixup
        dd  SPOOLER_Service_08       ; fixup
        dd  SPOOLER_Service_09       ; fixup
        dd  SPOOLER_Service_10       ; fixup
        dd  SPOOLER_Service_11       ; fixup
        dd  SPOOLER_Service_12       ; fixup
        dd  SPOOLER_Service_13       ; fixup
        dd  SPOOLER_Service_14       ; fixup
        dd  SPOOLER_Service_15       ; fixup
        dd  SPOOLER_Service_16       ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0002Ch                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'SPOOLER '                  ; DDB_Name
        dd  0A0010100h                  ; DDB_Init_Order
        dd  SPOOLER_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x00A0              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x3ac  R-X
loc_2_0000:
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jb     loc_2_001F
        push   0x16d                         ; -> obj4:0x016D
        push   0x22
        push   0x80                          ; -> loc_3_0080
        VxDCall  IFSMgr.IFSMgr_RegisterCFSD      ; cd2062004000
        add    esp, 0xc
        jae    loc_2_0021
loc_2_001F:
        stc    
        ret    
loc_2_0021:
        clc    
        ret    
loc_2_0023:
        push   0                             ; -> obj4:0x0000
        push   4                             ; -> obj4:0x0004
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        cmp    eax, 0
        jne    loc_2_0096
        push   0x45                          ; -> obj4:0x0045
        push   0x49                          ; -> obj4:0x0049
        push   0x41                          ; -> obj4:0x0041
        push   0
        push   0x34                          ; -> obj4:0x0034
        push   dword ptr [0]                 ; -> obj4:0x0000
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        push   dword ptr [0]                 ; -> obj4:0x0000
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        pop    ecx
        VxDCall  PAGEFILE.PageFile_Get_Version   ; cd2000002100
        jb     loc_2_0096
        cmp    eax, 0x400
        jne    loc_2_0096
        cmp    bl, 3
        jne    loc_2_0096
        call   sub_2_01E9
        or     eax, eax
        jne    loc_2_0096
        mov    byte ptr [0x2db], 1           ; -> obj4:0x02DB
loc_2_0096:
        clc    
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_00A0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x68
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        sub    esi, esi
        cmp    byte ptr [eax], 0
        je     loc_2_01E2
        lea    ecx, [ebp - 0xc]
        mov    dword ptr [ebp - 0x5c], ecx
        mov    dword ptr [ebp - 0x58], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x54], eax
        push   dword ptr [ebp - 0x5c]
        push   dword ptr [ebp - 0x58]
        push   dword ptr [ebp - 0x54]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        cmp    eax, esi
        jne    loc_2_01E2
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0x50], eax
        mov    dword ptr [ebp - 0x4c], 0x330 ; -> loc_2_0330
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x48], eax
        push   dword ptr [ebp - 0x50]
        push   dword ptr [ebp - 0x4c]
        push   dword ptr [ebp - 0x48]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        cmp    eax, esi
        jne    loc_2_01CA
        mov    dword ptr [ebp - 8], 0xa
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x44], eax
        lea    eax, [ebp - 0x68]
        mov    dword ptr [ebp - 0x40], eax
        mov    dword ptr [ebp - 0x3c], esi
        mov    dword ptr [ebp - 0x38], esi
        mov    dword ptr [ebp - 0x34], 0x328 ; -> loc_2_0328
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x30], eax
        push   dword ptr [ebp - 0x44]
        push   dword ptr [ebp - 0x40]
        push   dword ptr [ebp - 0x3c]
        push   dword ptr [ebp - 0x38]
        push   dword ptr [ebp - 0x34]
        push   dword ptr [ebp - 0x30]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        cmp    eax, esi
        jne    loc_2_01B2
        mov    dword ptr [ebp - 8], 0xa
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x2c], eax
        lea    eax, [ebp - 0x68]
        mov    dword ptr [ebp - 0x28], eax
        mov    dword ptr [ebp - 0x24], esi
        mov    dword ptr [ebp - 0x20], esi
        mov    dword ptr [ebp - 0x1c], 0x39c ; -> loc_2_039C
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x18], eax
        push   dword ptr [ebp - 0x2c]
        push   dword ptr [ebp - 0x28]
        push   dword ptr [ebp - 0x24]
        push   dword ptr [ebp - 0x20]
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebp - 0x18]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        cmp    eax, esi
        jne    loc_2_01B2
        mov    esi, 1
loc_2_01B2:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x14], eax
        push   dword ptr [ebp - 0x14]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_2_01CA:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x10], eax
        push   dword ptr [ebp - 0x10]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_2_01E2:
        mov    eax, esi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_01E9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x88
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0x3c
        sub    esi, esi
        lea    eax, [ebp - 0x28]
        mov    dword ptr [ebp - 0x4c], eax
        mov    dword ptr [ebp - 0x48], 0x340 ; -> loc_2_0340
        mov    dword ptr [ebp - 0x44], 0x80000002
        push   dword ptr [ebp - 0x4c]
        push   dword ptr [ebp - 0x48]
        push   dword ptr [ebp - 0x44]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        cmp    eax, esi
        jne    loc_2_0320
        lea    eax, [ebp - 0x2c]
        mov    dword ptr [ebp - 0x40], eax
        mov    dword ptr [ebp - 0x3c], 0x378 ; -> loc_2_0378
        mov    dword ptr [ebp - 0x38], 0x80000002
        push   dword ptr [ebp - 0x40]
        push   dword ptr [ebp - 0x3c]
        push   dword ptr [ebp - 0x38]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        cmp    eax, esi
        jne    loc_2_0308
        sub    edi, edi
loc_2_0267:
        mov    dword ptr [ebp - 4], 0x3c
        mov    ecx, edi
        inc    edi
        mov    dword ptr [ebp - 0x24], 0
        mov    dword ptr [ebp - 0x20], 0
        mov    dword ptr [ebp - 0x1c], 0
        mov    dword ptr [ebp - 0x18], 0
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0x14], eax
        lea    eax, [ebp - 0x88]
        mov    dword ptr [ebp - 0x10], eax
        mov    dword ptr [ebp - 0xc], ecx
        mov    eax, dword ptr [ebp - 0x28]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 0x24]
        push   dword ptr [ebp - 0x20]
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegEnumValue               ; cd2050010100
        add    esp, 0x20
        or     eax, eax
        jne    loc_2_02F0
        lea    eax, [ebp - 0x88]
        push   eax
        push   dword ptr [ebp - 0x2c]
        call   sub_2_00A0
        add    esp, 8
        or     eax, eax
        jne    loc_2_02EB
        jmp    loc_2_0267
loc_2_02EB:
        mov    esi, 1
loc_2_02F0:
        mov    eax, dword ptr [ebp - 0x2c]
        mov    dword ptr [ebp - 0x34], eax
        push   dword ptr [ebp - 0x34]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_2_0308:
        mov    eax, dword ptr [ebp - 0x28]
        mov    dword ptr [ebp - 0x30], eax
        push   dword ptr [ebp - 0x30]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_2_0320:
        mov    eax, esi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0328:
        dec    esi
        je     0x375
        inc    esp
        add    byte ptr [eax], al
        db  000h
loc_2_0330:
        dec    esi
        je     0x3ab
        outsd  dx, dword ptr [esi]
        jb     0x3a2
        push   eax
        jb     0x3a9
        jbe    0x3a5
        jb     loc_2_0340
loc_2_0340:
        push   ebx
        jns    0x3b6
        je     loc_2_03AA
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x3bc
        jb     0x3b1
        outsb  dx, byte ptr [esi]
        je     loc_2_0392
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x3c5
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0x3b5
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x3d0
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    esp
        dec    esi
        je     0x3dc
        outsd  dx, dword ptr [esi]
        jb     0x3d3
        push   eax
        jb     0x3da
        jbe    0x3d6
        jb     0x3cd
        dec    edi
        jb     0x3d8
        jb     0x377
        db  000h
loc_2_0378:
        push   ebx
        jns    0x3ee
        je     0x3e2
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x3f4
        jb     0x3e9
        outsb  dx, byte ptr [esi]
        je     0x3ca
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x3fd
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0x3ed
        push   ebx
loc_2_0392:
        jb     0x40b
        imul   esp, dword ptr [ebx + 0x65], 0x73
loc_2_039C:
        push   edi
        imul   ebp, dword ptr [esi + 0x64], 0x3373776f
        xor    dword ptr [esp + ecx*2 + 0x4c], eax
        add    byte ptr [eax], al
loc_2_03AA:
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x2248  R-X
loc_3_0000:
        push   ebx
        push   ebp
        mov    ebp, dword ptr [esp + 0xc]
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    al, byte ptr [ebp + 4]
        mov    ecx, dword ptr [ebp + 0x28]
        and    word ptr [ecx + 0x2c], 0xfffe
        cmp    al, 0
        je     loc_3_0037
        cmp    al, 1
        je     loc_3_0042
        cmp    al, 6
        je     loc_3_0046
        cmp    al, 7
        je     loc_3_0051
        cmp    al, 0x10
        je     loc_3_005C
        cmp    al, 0xa
        jne    loc_3_0068
        mov    dword ptr [ecx + 0x14], 0x10e0
        xor    eax, eax
        jmp    loc_3_0079
loc_3_0037:
        mov    dword ptr [ecx + 0x14], 0x80e0
        xor    eax, eax
        jmp    loc_3_0079
loc_3_0042:
        xor    eax, eax
        jmp    loc_3_0079
loc_3_0046:
        mov    dword ptr [ecx + 0x1c], 0
        xor    eax, eax
        jmp    loc_3_0079
loc_3_0051:
        mov    dword ptr [ecx + 0x1c], 0xff
        xor    eax, eax
        jmp    loc_3_0079
loc_3_005C:
        mov    eax, dword ptr [ecx + 0x18]
        cmp    ah, 5
        jne    loc_3_0068
        xor    eax, eax
        jmp    loc_3_0079
loc_3_0068:
        or     word ptr [ecx + 0x2c], 1
        mov    dword ptr [ecx + 0x1c], 1
        mov    eax, 0x1f
loc_3_0079:
        mov    word ptr [ebp + 0x1a], ax
        pop    ebp
        pop    ebx
        ret    
loc_3_0080:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cmp    byte ptr [0x2db], 1           ; -> obj4:0x02DB
        jne    loc_3_0109
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_0102
        mov    esi, ebx
        VMMCall  VMM.Get_Crit_Section_Status     ; cd2029000100
        jecxz  loc_3_00AD
        cmp    esi, ebx
        je     loc_3_0102
loc_3_00AD:
        mov    ebx, dword ptr [ebp + 0x28]
        cmp    ebx, 9
        jae    loc_3_0109
        cmp    byte ptr [ebp + 4], 0
        jne    loc_3_0109
        mov    ebx, dword ptr [ebx*4 + 0x4d] ; -> obj4:0x004D
        test   dword ptr [ebx + 0x10], 1
        jne    loc_3_00EA
        mov    eax, dword ptr [ebx + 0x14]
        test   dword ptr [0x49], eax         ; -> obj4:0x0049
        je     loc_3_0102
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebp + 0x1c], ebx
        or     dword ptr [ebx + 0x10], 1
        xor    eax, eax
        jmp    loc_3_00F5
loc_3_00EA:
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp + 0x1c], eax
        mov    eax, 0x25e
loc_3_00F5:
        mov    word ptr [ebp + 0x1a], ax
        mov    dword ptr [ebp + 0x10], 0x201c ; -> loc_3_201C
        jmp    loc_3_0112
loc_3_0102:
        mov    eax, 5
        jmp    loc_3_010E
loc_3_0109:
        mov    eax, 0x1f
loc_3_010E:
        mov    word ptr [ebp + 0x1a], ax
loc_3_0112:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_0117:
        push   ebx
        push   ebp
        mov    ebp, dword ptr [esp + 0xc]
        mov    ebx, dword ptr [ebp + 0x1c]
        cmp    byte ptr [ebp + 4], 3
        jne    loc_3_013E
        push   esi
        push   edi
        mov    esi, dword ptr [ebx + 0x18]
        mov    edi, dword ptr [ebp + 0x10]
        mov    ecx, 2
        add    cx, word ptr [esi]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        xor    eax, eax
        jmp    loc_3_0143
loc_3_013E:
        mov    eax, 0x1f
loc_3_0143:
        mov    word ptr [ebp + 0x1a], ax
        pop    ebp
        pop    ebx
        ret    
loc_3_014A:
        push   ebx
        push   ebp
        mov    ebp, dword ptr [esp + 0xc]
        mov    ebx, dword ptr [ebp + 0x1c]
        and    dword ptr [ebx + 0x10], 0xfffffffe
        xor    eax, eax
        mov    word ptr [ebp + 0x1a], ax
        pop    ebp
        pop    ebx
        ret    
loc_3_0160:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_024B
        mov    esi, ebx
        VMMCall  VMM.Get_Crit_Section_Status     ; cd2029000100
        jecxz  loc_3_018C
        cmp    esi, ebx
        je     loc_3_024B
loc_3_018C:
        mov    ebx, dword ptr [ebp + 0x1c]
        cmp    dword ptr [ebx], 0xbadbeef
        jne    loc_3_0262
        test   dword ptr [ebx + 0x10], 1
        je     loc_3_0262
        lea    edi, [ebp + 0x20]
        mov    esi, dword ptr [ebx + 4]
        push   0
        push   edi
        push   esi
        push   3
        push   0x401
        call   sub_3_11E5
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_0262
        xor    eax, eax
        inc    dword ptr [ebx + 0xc]
        mov    byte ptr [ebp + 4], 1
        mov    dword ptr [ebp], eax
        mov    word ptr [ebp + 0x18], 1
        mov    dword ptr [ebp + 0x24], eax
        mov    word ptr [ebp + 0x28], ax
        mov    word ptr [ebp + 0x2a], ax
        mov    edi, dword ptr [ebp + 0x10]
        mov    dword ptr [edi], 0x2dd        ; -> loc_3_02DD
        mov    dword ptr [edi + 4], 0x2ed    ; -> loc_3_02ED
        mov    dword ptr [edi + 8], 0x205c   ; -> loc_3_205C
        push   0
        push   0x1000
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_3_0232
        mov    esi, dword ptr [ebp + 0x20]
        mov    dword ptr [eax], esi
        mov    dword ptr [eax + 4], 0x14
        mov    dword ptr [ebp + 0x20], eax
        mov    edx, eax
        mov    dword ptr [edx + 8], 1
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [edx + 0x10], eax
        jmp    loc_3_0267
loc_3_0232:
        push   dword ptr [ebp + 0x20]
        push   0x407
        call   sub_3_0566
        pop    ecx
        pop    ecx
        dec    dword ptr [ebx + 0xc]
        mov    eax, 8
        jmp    loc_3_0267
loc_3_024B:
        mov    ebx, dword ptr [ebp + 0x1c]
        cmp    dword ptr [ebx + 0xc], 0
        jne    loc_3_025B
        mov    eax, 0x70b
        jmp    loc_3_0267
loc_3_025B:
        mov    eax, 5
        jmp    loc_3_0267
loc_3_0262:
        mov    eax, 0x37
loc_3_0267:
        mov    word ptr [ebp + 0x1a], ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_0270:
        test   dword ptr [edx + 8], 2
        je     loc_3_0292
        mov    esi, 0x270                    ; -> sub_3_0270
        mov    eax, 0x1388
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [edx + 0xc], esi
        and    dword ptr [edx + 8], 0xfffffffd
        jmp    loc_3_02AA
loc_3_0292:
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        mov    ecx, 0x40
        mov    esi, 0x2ab                    ; -> loc_3_02AB
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [edx + 0x10], esi
loc_3_02AA:
        ret    
loc_3_02AB:
        xor    eax, eax
        mov    dword ptr [edx + 0x10], eax
        test   dword ptr [edx + 8], 2
        je     loc_3_02C0
        call   sub_3_0270
        jmp    loc_3_02DC
loc_3_02C0:
        push   edx
        mov    eax, edx
        call   sub_3_03A1
        pop    edx
        push   edx
        push   dword ptr [edx]
        call   sub_3_1436
        pop    ecx
        pop    edx
        or     dword ptr [edx + 8], 1
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
loc_3_02DC:
        ret    
loc_3_02DD:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        mov    eax, 5
        mov    word ptr [ebp + 0x1a], ax
        pop    ebp
        ret    
loc_3_02ED:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [ebp]
        mov    eax, dword ptr [ebp + 0x20]
        or     dword ptr [eax + 8], 2
        test   dword ptr [eax + 8], 1
        je     loc_3_032E
        push   eax
        mov    esi, dword ptr [eax]
        mov    ebx, 0x27f                    ; -> obj4:0x027F
        push   ebx
        push   2
        push   esi
        call   sub_3_13FA
        add    esp, 0xc
        test   eax, eax
        pop    eax
        jne    loc_3_0393
        and    dword ptr [eax + 8], 0xfffffffe
        mov    edx, eax
        call   sub_3_0270
        mov    eax, edx
loc_3_032E:
        mov    edi, dword ptr [eax + 4]
        add    edi, ecx
        cmp    edi, 0x1000
        jbe    loc_3_0372
        call   sub_3_03A1
        jb     loc_3_0393
        mov    ecx, dword ptr [ebp]
        cmp    ecx, 0xfec
        jbe    loc_3_0372
        mov    esi, dword ptr [eax]
        mov    edi, dword ptr [ebp + 0x14]
        lea    ebx, [eax + 4]
        push   eax
        push   ebx
        push   ecx
        push   edi
        push   esi
        call   sub_3_1416
        add    esp, 0x10
        test   eax, eax
        pop    eax
        mov    dword ptr [eax + 4], 0x14
        jne    loc_3_0393
        xor    eax, eax
        jmp    loc_3_0398
loc_3_0372:
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp]
        mov    edi, dword ptr [eax + 4]
        add    edi, eax
        pushf  
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        popf   
        sub    edi, eax
        mov    dword ptr [eax + 4], edi
        mov    ecx, dword ptr [ebp]
        add    dword ptr [ebp + 0x24], ecx
        xor    eax, eax
        jmp    loc_3_0398
loc_3_0393:
        mov    eax, 0x1d
loc_3_0398:
        mov    word ptr [ebp + 0x1a], ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_03A1:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [eax + 4]
        sub    ecx, 0x14
        cmp    ecx, 0
        je     loc_3_03D2
        lea    edi, [eax + 0x14]
        mov    esi, dword ptr [eax]
        lea    ebx, [eax + 4]
        push   eax
        push   ebx
        push   ecx
        push   edi
        push   esi
        call   sub_3_1416
        add    esp, 0x10
        test   eax, eax
        pop    eax
        mov    dword ptr [eax + 4], 0x14
        clc    
        je     loc_3_03D2
        stc    
loc_3_03D2:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_03D6:
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    edi, edi
        mov    ebp, dword ptr [esp + 0x14]
        mov    eax, dword ptr [ebp + 0x20]
        mov    ebx, dword ptr [ebp + 0x1c]
        cmp    dword ptr [ebx], 0xbadbeef
        jne    loc_3_04A6
        cmp    dword ptr [ebx + 0xc], 0
        je     loc_3_04A6
        xor    esi, esi
        xchg   dword ptr [eax + 0xc], esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        xor    esi, esi
        xchg   dword ptr [eax + 0x10], esi
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    esi, ebx
        VMMCall  VMM.Get_Crit_Section_Status     ; cd2029000100
        jecxz  loc_3_0428
        cmp    esi, ebx
        jne    loc_3_0428
        jmp    loc_3_0450
loc_3_0428:
        mov    eax, dword ptr [ebp + 0x20]
        call   sub_3_03A1
        jae    loc_3_0433
        inc    edi
loc_3_0433:
        test   dword ptr [eax + 8], 1
        jne    loc_3_0450
        push   dword ptr [eax]
        call   sub_3_1436
        pop    ecx
        test   eax, eax
        je     loc_3_0449
        inc    edi
loc_3_0449:
        mov    eax, dword ptr [ebp + 0x20]
        or     dword ptr [eax + 8], 1
loc_3_0450:
        cmp    byte ptr [ebp + 4], 2
        jne    loc_3_049E
        mov    esi, dword ptr [eax]
        mov    dword ptr [ebp + 0x20], esi
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        or     eax, eax
        jne    loc_3_046B
        inc    edi
loc_3_046B:
        push   dword ptr [ebp + 0x20]
        push   0x407
        call   sub_3_0566
        pop    ecx
        pop    ecx
        mov    ebx, dword ptr [ebp + 0x1c]
        dec    dword ptr [ebx + 0xc]
        cmp    dword ptr [ebx + 0xc], 0
        jne    loc_3_049E
        mov    eax, dword ptr [ebx + 0x14]
        test   dword ptr [0x49], eax         ; -> obj4:0x0049
        jne    loc_3_049E
        push   dword ptr [ebx + 8]
        VxDCall  IFSMgr.IFSMgr_FSDUnmountCFSD    ; cd2050004000
        pop    ecx
        jae    loc_3_049E
        inc    edi
loc_3_049E:
        test   edi, edi
        jne    loc_3_04A6
        xor    eax, eax
        jmp    loc_3_04AB
loc_3_04A6:
        mov    eax, 0x1f
loc_3_04AB:
        mov    word ptr [ebp + 0x1a], ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_04B4:
        mov    ecx, dword ptr [esi + 0xc]
        jecxz  loc_3_04D8
        cmp    ecx, -1
        jne    loc_3_04C2
        sub    eax, eax
        jmp    loc_3_04D7
loc_3_04C2:
        cmp    ecx, 5
        ja     loc_3_04D2
        dec    ecx
        jmp    dword ptr [ecx*4 + 0x2dc]     ; -> obj4:0x02DC
loc_3_04D2:
        mov    eax, 0x32
loc_3_04D7:
        ret    
loc_3_04D8:
        xor    eax, eax
        mov    ecx, 0x100
        ret    
SPOOLER_Service_00:
        mov    eax, 0x100
        clc    
        ret    
SPOOLER_Service_01:
        jmp    loc_3_07A0
SPOOLER_Service_02:
        jmp    loc_3_081C
SPOOLER_Service_03:
        jmp    loc_3_0A69
SPOOLER_Service_04:
        jmp    loc_3_0B2B
SPOOLER_Service_05:
        jmp    loc_3_0B90
SPOOLER_Service_06:
        jmp    sub_3_0BA4
SPOOLER_Service_07:
        jmp    loc_3_0C4E
SPOOLER_Service_08:
        jmp    loc_3_0DB6
SPOOLER_Service_09:
        jmp    loc_3_0EB8
SPOOLER_Service_10:
        jmp    loc_3_1053
SPOOLER_Service_11:
        jmp    loc_3_112D
SPOOLER_Service_12:
        jmp    loc_3_12EB
SPOOLER_Service_13:
        jmp    loc_3_13D0
SPOOLER_Service_14:
        jmp    loc_3_13E5
SPOOLER_Service_15:
        jmp    loc_3_144A
SPOOLER_Service_16:
        xor    eax, eax
        cmp    eax, dword ptr [0x424]        ; -> obj4:0x0424
        je     loc_3_053F
        stc    
        jmp    loc_3_0547
loc_3_053F:
        push   eax
        call   sub_3_0584
        pop    ecx
        clc    
loc_3_0547:
        ret    
sub_3_0548:
        push   edx
        mov    edx, dword ptr [0x428]        ; -> obj4:0x0428
        VxDCall  NDIS.Service_0009h              ; cd2009002a00
        pop    edx
        ret    
sub_3_0557:
        push   edx
        mov    edx, dword ptr [0x428]        ; -> obj4:0x0428
        VxDCall  NDIS.Service_000Ah              ; cd200a002a00
        pop    edx
        ret    
sub_3_0566:
        xor    eax, eax
        push   eax
        push   eax
        push   dword ptr [esp + 0x10]
        push   1
        push   dword ptr [esp + 0x14]
        push   dword ptr [0x42c]             ; -> obj4:0x042C
        VxDCall  SHELL._SHELL_PostMessage        ; cd2006001700
        add    esp, 0x18
        ret    
sub_3_0584:
        push   ebx
        mov    al, 0
        xchg   byte ptr [0x2d9], al          ; -> obj4:0x02D9
        or     al, al
        je     loc_3_05A3
        push   0
        push   0
        push   0x200c                        ; -> loc_3_200C
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
loc_3_05A3:
        cmp    dword ptr [esp + 8], 0
        je     loc_3_05E2
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_05E0
        push   5
        push   0x200c                        ; -> loc_3_200C
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        pop    ecx
        pop    ecx
        cmp    byte ptr [0x2da], 0           ; -> obj4:0x02DA
        jne    loc_3_05E0
        cmp    dword ptr [0x424], 0          ; -> obj4:0x0424
        je     loc_3_05A3
        mov    eax, 1
        jmp    loc_3_05E2
loc_3_05E0:
        xor    eax, eax
loc_3_05E2:
        pop    ebx
        ret    
sub_3_05E4:
        push   dword ptr [esp + 4]
        VxDCall  SHELL._SHELL_ShellExecute       ; cd2007001700
        pop    ecx
        cmp    eax, 0x20
        ja     loc_3_0607
        mov    byte ptr [0x2da], 1           ; -> obj4:0x02DA
        push   0x200c                        ; -> loc_3_200C
        VMMCall  VMM._SignalID                   ; cd2027010100
        pop    ecx
loc_3_0607:
        ret    
        db  056h, 057h, 051h, 0FCh, 033h, 0C9h, 049h, 08Bh, 07Ch, 024h, 014h, 08Bh, 074h, 024h, 010h, 08Bh
        db  0D7h, 032h, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 08Bh, 0FAh, 0F3h, 0A6h, 00Fh, 0B6h, 046h, 0FFh, 00Fh
        db  0B6h, 057h, 0FFh, 02Bh, 0C2h, 059h, 05Fh, 05Eh, 0C3h, 056h, 057h, 051h, 0FCh, 08Bh, 074h, 024h
        db  010h, 033h, 0C9h, 049h, 032h, 0C0h, 08Bh, 0FEh, 0F2h, 0AEh, 0F7h, 0D1h, 08Ah, 044h, 024h, 014h
        db  08Bh, 0FEh, 0F2h, 0AEh, 074h, 004h, 033h, 0C0h, 0EBh, 003h, 04Fh, 08Bh, 0C7h, 059h, 05Fh, 05Eh
        db  0C3h, 056h, 057h, 051h, 0FCh, 033h, 0C9h, 049h, 08Bh, 07Ch, 024h, 014h, 08Bh, 074h, 024h, 010h
        db  08Bh, 0D7h, 032h, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 08Bh, 0FAh, 0ACh, 03Ch, 061h, 07Ch, 006h, 03Ch
        db  07Ah, 07Fh, 002h, 02Ch, 020h, 026h, 038h, 007h, 075h, 005h, 047h, 0E2h, 0EDh, 033h, 0C0h, 059h
        db  05Fh, 05Eh, 0C3h, 06Ah, 0FFh, 0FFh, 074h, 024h, 00Ch, 0FFh, 074h, 024h, 00Ch, 0CDh, 020h, 07Bh
        db  001h, 001h, 000h, 083h, 0C4h, 00Ch, 0C3h, 0FFh, 074h, 024h, 004h, 0CDh, 020h, 07Ch, 001h, 001h
        db  000h, 059h, 0C3h, 0FFh, 074h, 024h, 00Ch, 0FFh, 074h, 024h, 00Ch, 0FFh, 074h, 024h, 00Ch, 0CDh
        db  020h, 07Dh, 001h, 001h, 000h, 083h, 0C4h, 00Ch, 0C3h
sub_3_06C1:
        push   edi
        push   ecx
        push   edx
        mov    edi, dword ptr [esp + 0x10]
        VxDCall  IFSMgr.IFSMgr_Get_NetTime       ; cd2006004000
        push   eax
        xor    edx, edx
        mov    ecx, 0x15180
        div    ecx
        add    eax, 4
        xor    edx, edx
        mov    ecx, 7
        div    ecx
        mov    word ptr [edi + 4], dx
        VxDCall  IFSMgr.IFSMgr_GetTimeZoneBias   ; cd2060004000
        shl    eax, 2
        lea    eax, [eax + eax*2]
        lea    eax, [eax + eax*4]
        pop    ecx
        add    eax, ecx
        push   eax
        VxDCall  IFSMgr.IFSMgr_NetToDosTime      ; cd2037004000
        pop    ecx
        mov    ecx, eax
        shr    eax, 0x19
        add    ax, 0x7bc
        mov    word ptr [edi], ax
        mov    eax, ecx
        shr    eax, 0x15
        and    ax, 0xf
        mov    word ptr [edi + 2], ax
        mov    eax, ecx
        shr    eax, 0x10
        and    ax, 0x1f
        mov    word ptr [edi + 6], ax
        mov    eax, ecx
        shr    ax, 0xb
        and    ax, 0x1f
        mov    word ptr [edi + 8], ax
        mov    eax, ecx
        shr    ax, 5
        and    ax, 0x3f
        mov    word ptr [edi + 0xa], ax
        mov    eax, ecx
        and    ax, 0x1f
        shl    ax, 1
        mov    word ptr [edi + 0xc], ax
        pop    edx
        pop    ecx
        pop    edi
        ret    
sub_3_0755:
        push   ebx
        push   ecx
        push   edx
        mov    edx, dword ptr [esp + 0x10]
        mov    al, 0x40
        mov    ah, 0x3d
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_3_079A
        push   eax
        mov    ebx, eax
        xor    ecx, ecx
        xor    edx, edx
        mov    ax, 2
        mov    ah, 0x42
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    ebx
        jae    loc_3_0786
        xor    eax, eax
        xor    edx, edx
loc_3_0786:
        shl    edx, 0x10
        mov    dx, ax
        push   edx
        mov    ah, 0x3e
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    eax
        jmp    loc_3_079C
loc_3_079A:
        xor    eax, eax
loc_3_079C:
        pop    edx
        pop    ecx
        pop    ebx
        ret    
loc_3_07A0:
        push   esi
        push   edi
        sub    edi, edi
        cmp    dword ptr [0x424], edi        ; -> obj4:0x0424
        jne    loc_3_07B3
        mov    eax, 0x871
        jmp    loc_3_0819
loc_3_07B3:
        mov    esi, dword ptr [esp + 0xc]
        or     esi, esi
        je     loc_3_07C7
        cmp    byte ptr [esi], 0
        jne    loc_3_07C7
        mov    eax, 0x7b
        jmp    loc_3_0819
loc_3_07C7:
        call   sub_3_0548
        push   esi
        call   sub_3_1591
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_3_080F
        test   byte ptr [esi + 0x14], 4
        je     loc_3_07E3
        jmp    loc_3_0801
loc_3_07E3:
        or     esi, esi
        je     loc_3_0801
        push   dword ptr [esp + 0x14]
        push   3
        push   esi
        call   sub_3_1542
        mov    ecx, dword ptr [esp + 0x1c]
        add    esp, 0xc
        mov    dword ptr [ecx], eax
        inc    dword ptr [esi + 0xc]
        jmp    loc_3_0806
loc_3_0801:
        mov    edi, 0x7b
loc_3_0806:
        call   sub_3_0557
        mov    eax, edi
        jmp    loc_3_0819
loc_3_080F:
        call   sub_3_0557
        mov    eax, 0x709
loc_3_0819:
        pop    edi
        pop    esi
        ret    
loc_3_081C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x110
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [0x424], 0          ; -> obj4:0x0424
        jne    loc_3_0842
        mov    eax, 0x871
        jmp    loc_3_0A62
loc_3_0842:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi], 0x464d
        je     loc_3_0859
        mov    dword ptr [ebp - 4], 6
        jmp    loc_3_0A5F
loc_3_0859:
        call   sub_3_0548
        mov    esi, dword ptr [esi + 0x10]
        or     esi, esi
        je     loc_3_0A53
        test   byte ptr [esi + 0x18], 2
        jne    loc_3_0A53
        inc    dword ptr [0x370]             ; -> obj4:0x0370
        cmp    dword ptr [0x370], 0x300      ; -> obj4:0x0370
        jbe    loc_3_088F
        mov    dword ptr [0x370], 0x200      ; -> obj4:0x0370
loc_3_088F:
        lea    eax, [ebp - 0x110]
        push   eax
        push   dword ptr [0x370]             ; -> obj4:0x0370
        call   sub_3_166F
        add    esp, 8
        lea    edi, [ebp - 0x110]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ecx, 8
        mov    edi, dword ptr [ebp + 0x14]
        cmp    ecx, edi
        jbe    loc_3_08D0
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [ebp - 4], 0x7a
        mov    dword ptr [eax], ecx
        jmp    loc_3_0A5A
loc_3_08D0:
        mov    dword ptr [ebp - 0x10], 1
        mov    dword ptr [ebp - 0xc], 0xac
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    ebx, dword ptr [ebp - 8]
        or     ebx, ebx
        je     loc_3_0A5A
        mov    eax, ebx
        push   0x34c                         ; -> obj4:0x034C
        mov    dword ptr [eax], 0x494a
        mov    dword ptr [ebx + 4], 0xac
        mov    eax, dword ptr [0x370]        ; -> obj4:0x0370
        mov    dword ptr [ebx + 0x1c], eax
        call   sub_3_1472
        add    esp, 4
        mov    dword ptr [ebx + 0x24], eax
        push   0x34c                         ; -> obj4:0x034C
        call   sub_3_1472
        add    esp, 4
        mov    dword ptr [ebx + 0x20], eax
        push   0x360                         ; -> obj4:0x0360
        call   sub_3_1472
        add    esp, 4
        mov    dword ptr [ebx + 0x10], eax
        push   0x34c                         ; -> obj4:0x034C
        call   sub_3_1472
        add    esp, 4
        mov    dword ptr [ebx + 0x28], eax
        push   0x34c                         ; -> obj4:0x034C
        call   sub_3_1472
        add    esp, 4
        mov    dword ptr [ebx + 0x6c], eax
        lea    eax, [ebx + 0x40]
        push   eax
        call   sub_3_06C1
        add    esp, 4
        push   0x350                         ; -> obj4:0x0350
        call   sub_3_1472
        add    esp, 4
        mov    dword ptr [ebx + 0x38], eax
        inc    dword ptr [esi + 0x60]
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [ebx + 0x2c], eax
        mov    eax, dword ptr [esi + 0x34]
        push   ebx
        inc    dword ptr [eax + 0xc]
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [ebx + 0x34], eax
        mov    eax, dword ptr [esi + 0x24]
        push   esi
        inc    dword ptr [eax + 0xc]
        mov    dword ptr [ebx + 8], 0
        call   sub_3_1719
        add    esp, 8
        mov    dword ptr [ebx + 0x14], 0x5000008
        mov    dword ptr [ebx + 0x3c], 0
        mov    dword ptr [ebx + 0x80], 0
        mov    dword ptr [ebx + 0x8c], 0
        mov    dword ptr [ebx + 0x5c], 0
        mov    dword ptr [ebx + 0x84], 0
        mov    dword ptr [ebx + 0x60], 0
        mov    dword ptr [ebx + 0x88], 0
        mov    dword ptr [ebx + 0x58], 0
        mov    dword ptr [ebx + 0x54], 0
        mov    dword ptr [ebx + 0x30], 0
        mov    edx, dword ptr [ebp + 0x10]
        add    edx, edi
        lea    edi, [ebp - 0x110]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edx, ecx
        lea    edi, [ebp - 0x110]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx], edx
        mov    eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [ecx + 4], eax
        jmp    loc_3_0A5A
loc_3_0A53:
        mov    dword ptr [ebp - 4], 0xc
loc_3_0A5A:
        call   sub_3_0557
loc_3_0A5F:
        mov    eax, dword ptr [ebp - 4]
loc_3_0A62:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_0A69:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        sub    edi, edi
        cmp    dword ptr [0x424], edi        ; -> obj4:0x0424
        jne    loc_3_0A86
        mov    eax, 0x871
        jmp    loc_3_0B24
loc_3_0A86:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi], 0x464d
        je     loc_3_0A9B
        mov    edi, 6
        jmp    loc_3_0B22
loc_3_0A9B:
        call   sub_3_0548
        lea    eax, [ebp - 0x10]
        push   eax
        push   dword ptr [ebp + 0xc]
        push   dword ptr [esi + 0x10]
        call   sub_3_1748
        add    esp, 0xc
        or     eax, eax
        je     loc_3_0B18
        mov    dword ptr [ebp - 0xc], 1
        mov    dword ptr [ebp - 8], 0x34
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    ebx, ecx
        add    ebx, 0xc
        mov    eax, ecx
        mov    dword ptr [eax], 0x6060
        mov    dword ptr [ecx + 4], 0
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ecx + 8], eax
        push   dword ptr [ebp + 0xc]
        push   ecx
        push   ebx
        push   2
        call   sub_3_1778
        add    esp, 0x10
        push   ebx
        push   0x41e
        call   sub_3_0566
        add    esp, 8
        jmp    loc_3_0B1D
loc_3_0B18:
        mov    edi, 6
loc_3_0B1D:
        call   sub_3_0557
loc_3_0B22:
        mov    eax, edi
loc_3_0B24:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_0B2B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        sub    edi, edi
        cmp    dword ptr [0x424], edi        ; -> obj4:0x0424
        jne    loc_3_0B44
        mov    eax, 0x871
        jmp    loc_3_0B8A
loc_3_0B44:
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi], 0x464d
        jne    loc_3_0B83
        call   sub_3_0548
        mov    eax, dword ptr [esi + 0x10]
        dec    dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], esi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        call   sub_3_0557
        jmp    loc_3_0B88
loc_3_0B83:
        mov    edi, 6
loc_3_0B88:
        mov    eax, edi
loc_3_0B8A:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_0B90:
        cmp    dword ptr [0x424], 0          ; -> obj4:0x0424
        mov    eax, 0x871
        je     loc_3_0BA3
        mov    eax, 0x78
loc_3_0BA3:
        ret    
sub_3_0BA4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 0x10]
        call   sub_3_14E4
        add    esp, 4
        push   dword ptr [esi + 0x24]
        call   sub_3_14E4
        add    esp, 4
        push   dword ptr [esi + 0x20]
        call   sub_3_14E4
        add    esp, 4
        push   dword ptr [esi + 0x38]
        call   sub_3_14E4
        add    esp, 4
        push   dword ptr [esi + 0x28]
        call   sub_3_14E4
        add    esp, 4
        push   dword ptr [esi + 0x6c]
        call   sub_3_14E4
        add    esp, 4
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], esi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, 1
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_0C1C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_0C4E:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x1c], 0
        cmp    dword ptr [0x424], 0          ; -> obj4:0x0424
        jne    loc_3_0C70
        mov    eax, 0x871
        jmp    loc_3_0DAE
loc_3_0C70:
        mov    esi, dword ptr [esp + 0x30]
        cmp    esi, 2
        je     loc_3_0C83
        mov    eax, 0x7c
        jmp    loc_3_0DAE
loc_3_0C83:
        call   sub_3_0548
        push   dword ptr [esp + 0x24]
        call   sub_3_17A6
        mov    dword ptr [esp + 0x18], eax
        add    esp, 4
        or     eax, eax
        je     loc_3_0D9D
        mov    eax, dword ptr [eax + 0x58]
        or     eax, eax
        je     loc_3_0CBC
        test   byte ptr [eax + 0x14], 0x20
        jne    loc_3_0CBC
        call   sub_3_0557
        mov    eax, 6
        jmp    loc_3_0DAE
loc_3_0CBC:
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x18], 0
        mov    ebx, dword ptr [eax + 0x6c]
        or     ebx, ebx
        mov    edi, dword ptr [esp + 0x28]
        mov    ecx, edi
        je     loc_3_0CE3
loc_3_0CD5:
        mov    eax, ecx
        dec    ecx
        or     eax, eax
        je     loc_3_0CE3
        mov    ebx, dword ptr [ebx + 8]
        or     ebx, ebx
        jne    loc_3_0CD5
loc_3_0CE3:
        mov    ebp, ebx
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [esp + 0x10], eax
        or     ebx, ebx
        je     loc_3_0D21
loc_3_0CF1:
        mov    eax, dword ptr [esp + 0x10]
        dec    dword ptr [esp + 0x10]
        or     eax, eax
        je     loc_3_0D21
        mov    eax, dword ptr [ebp + 0x14]
        test   al, 4
        je     loc_3_0D08
        test   al, 0x10
        je     loc_3_0D1D
loc_3_0D08:
        push   ebp
        push   esi
        push   dword ptr [esp + 0x1c]
        call   sub_3_17C4
        add    dword ptr [esp + 0x24], eax
        add    esp, 0xc
        mov    ebp, dword ptr [ebp + 8]
loc_3_0D1D:
        or     ebp, ebp
        jne    loc_3_0CF1
loc_3_0D21:
        mov    eax, dword ptr [esp + 0x3c]
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 0x38]
        cmp    eax, ecx
        jb     loc_3_0D93
        mov    ebp, dword ptr [esp + 0x34]
        mov    ecx, dword ptr [esp + 0x40]
        add    eax, ebp
        mov    dword ptr [ecx], 0
        mov    ecx, dword ptr [esp + 0x2c]
        mov    dword ptr [esp + 0x10], ecx
        or     ebx, ebx
        je     loc_3_0DA5
loc_3_0D4F:
        mov    ecx, dword ptr [esp + 0x10]
        dec    dword ptr [esp + 0x10]
        or     ecx, ecx
        je     loc_3_0DA5
        mov    ecx, dword ptr [ebx + 0x14]
        test   cl, 4
        je     loc_3_0D68
        test   cl, 0x10
        je     loc_3_0D8D
loc_3_0D68:
        push   eax
        push   ebp
        push   esi
        push   ebx
        inc    edi
        push   dword ptr [esp + 0x24]
        call   sub_3_194E
        add    esp, 0x14
        cmp    esi, 2
        jne    loc_3_0D84
        mov    dword ptr [ebp + 0x3c], edi
        add    ebp, 0x68
loc_3_0D84:
        mov    ecx, dword ptr [esp + 0x40]
        mov    ebx, dword ptr [ebx + 8]
        inc    dword ptr [ecx]
loc_3_0D8D:
        or     ebx, ebx
        jne    loc_3_0D4F
        jmp    loc_3_0DA5
loc_3_0D93:
        mov    dword ptr [esp + 0x1c], 0x7a
        jmp    loc_3_0DA5
loc_3_0D9D:
        mov    dword ptr [esp + 0x1c], 6
loc_3_0DA5:
        call   sub_3_0557
        mov    eax, dword ptr [esp + 0x1c]
loc_3_0DAE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
loc_3_0DB6:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], 0
        sub    esi, esi
        cmp    dword ptr [0x424], esi        ; -> obj4:0x0424
        jne    loc_3_0DD9
        mov    eax, 0x871
        jmp    loc_3_0EB0
loc_3_0DD9:
        mov    edi, dword ptr [esp + 0x24]
        cmp    edi, 2
        je     loc_3_0DEC
        mov    eax, 0x7c
        jmp    loc_3_0EB0
loc_3_0DEC:
        call   sub_3_0548
        mov    eax, dword ptr [esp + 0x1c]
        or     eax, eax
        je     loc_3_0E6E
        push   eax
        call   sub_3_17A6
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_3_0EA4
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [esp + 0x24]
        push   ebp
        call   sub_3_1748
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
        or     ebp, ebp
        je     loc_3_0EA4
loc_3_0E26:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_3_0E9D
        push   dword ptr [esp + 0x10]
        push   edi
        push   ebp
        call   sub_3_17C4
        mov    ecx, dword ptr [esp + 0x3c]
        add    esp, 0xc
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 0x2c]
        cmp    eax, ecx
        ja     loc_3_0E96
        mov    ebx, dword ptr [esp + 0x28]
        lea    eax, [ecx + ebx]
        push   eax
        push   ebx
        push   edi
        push   dword ptr [esp + 0x1c]
        push   ebp
        call   sub_3_194E
        add    esp, 0x14
        cmp    edi, 2
        jne    loc_3_0EA9
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ebx + 0x3c], eax
        jmp    loc_3_0EA9
loc_3_0E6E:
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x14]
        push   dword ptr [esp + 0x24]
        push   eax
        call   sub_3_1B78
        add    esp, 0xc
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_3_0E26
        call   sub_3_0557
        mov    eax, 0x57
        jmp    loc_3_0EB0
loc_3_0E96:
        mov    esi, 0x7a
        jmp    loc_3_0EA9
loc_3_0E9D:
        mov    esi, 0x57
        jmp    loc_3_0EA9
loc_3_0EA4:
        mov    esi, 6
loc_3_0EA9:
        call   sub_3_0557
        mov    eax, esi
loc_3_0EB0:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_0EB8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], 0
        cmp    dword ptr [0x424], 0          ; -> obj4:0x0424
        jne    loc_3_0EE2
        mov    eax, 0x871
        jmp    loc_3_104C
loc_3_0EE2:
        call   sub_3_0548
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     loc_3_0F34
        push   eax
        call   sub_3_17A6
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     loc_3_0F10
        lea    eax, [ebp - 0x18]
        push   eax
        push   dword ptr [ebp + 0xc]
        push   edi
        call   sub_3_1748
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
loc_3_0F10:
        or     edi, edi
        je     loc_3_103D
        mov    eax, dword ptr [edi + 0x58]
        or     eax, eax
        je     loc_3_0F5C
        test   byte ptr [eax + 0x14], 0x20
        jne    loc_3_0F5C
        call   sub_3_0557
        mov    eax, 6
        jmp    loc_3_104C
loc_3_0F34:
        lea    eax, [ebp - 0x18]
        push   eax
        push   dword ptr [ebp + 0xc]
        lea    eax, [ebp - 4]
        push   eax
        call   sub_3_1B78
        add    esp, 0xc
        mov    edi, eax
        or     edi, edi
        jne    loc_3_0F10
        call   sub_3_0557
        mov    eax, 0x57
        jmp    loc_3_104C
loc_3_0F5C:
        cmp    dword ptr [ebp - 4], 0
        je     loc_3_1034
        mov    ebx, dword ptr [ebp + 0x18]
        or     ebx, ebx
        je     loc_3_0F8D
        cmp    ebx, 1
        je     loc_3_0FA3
        cmp    ebx, 2
        je     loc_3_0FAC
        cmp    ebx, 3
        je     loc_3_0FB5
        cmp    ebx, 4
        je     loc_3_0FCE
        mov    dword ptr [ebp - 8], 0x57
        jmp    loc_3_1044
loc_3_0F8D:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp - 4]
        call   sub_3_1BB3
        add    esp, 0xc
        jmp    loc_3_1044
loc_3_0FA3:
        mov    eax, dword ptr [ebp - 4]
        or     dword ptr [eax + 0x14], 1
        jmp    loc_3_0FCE
loc_3_0FAC:
        mov    eax, dword ptr [ebp - 4]
        and    dword ptr [eax + 0x14], 0xfffffffe
        jmp    loc_3_0FCE
loc_3_0FB5:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 0x14]
        add    ecx, 0x14
        test   al, 8
        je     loc_3_0FCE
        and    eax, 0xfffffff7
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp - 4]
        or     dword ptr [eax + 0x14], 4
loc_3_0FCE:
        mov    dword ptr [ebp - 0x14], 1
        mov    dword ptr [ebp - 0x10], 0x34
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        mov    ecx, dword ptr [ebp - 0xc]
        mov    esi, ecx
        add    esi, 0xc
        mov    eax, ecx
        mov    dword ptr [eax], 0x6060
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx + 8], edi
        push   ebx
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   ecx
        push   esi
        push   5
        call   sub_3_1778
        add    esp, 0x1c
        push   esi
        push   0x40a
        call   sub_3_0566
        add    esp, 8
        jmp    loc_3_1044
loc_3_1034:
        mov    dword ptr [ebp - 8], 0x57
        jmp    loc_3_1044
loc_3_103D:
        mov    dword ptr [ebp - 8], 6
loc_3_1044:
        call   sub_3_0557
        mov    eax, dword ptr [ebp - 8]
loc_3_104C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_1053:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        cmp    dword ptr [0x424], 0          ; -> obj4:0x0424
        jne    loc_3_1076
        mov    eax, 0x871
        jmp    loc_3_1126
loc_3_1076:
        cmp    dword ptr [ebp + 0xc], 2
        je     loc_3_1086
        mov    eax, 0x7c
        jmp    loc_3_1126
loc_3_1086:
        call   sub_3_0548
        push   dword ptr [ebp + 8]
        call   sub_3_17A6
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        je     loc_3_1117
        mov    edi, dword ptr [esi + 0x3c]
        mov    ebx, dword ptr [esi + 0x38]
        mov    dword ptr [esi + 0x3c], 0
        mov    dword ptr [esi + 0x38], 0
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_3_1DC2
        mov    ecx, dword ptr [ebp + 0x18]
        add    esp, 8
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebp + 0x14]
        cmp    dword ptr [ecx], eax
        jbe    loc_3_10D1
        mov    dword ptr [ebp - 4], 0x7a
        jmp    loc_3_10E4
loc_3_10D1:
        mov    ecx, dword ptr [ebp + 0x10]
        add    eax, ecx
        push   eax
        push   ecx
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_3_1F14
        add    esp, 0x10
loc_3_10E4:
        cmp    dword ptr [esi + 0x38], 0
        je     loc_3_110F
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [esi + 0x3c]
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    dword ptr [ebp - 0x10], eax
loc_3_110F:
        mov    dword ptr [esi + 0x3c], edi
        mov    dword ptr [esi + 0x38], ebx
        jmp    loc_3_111E
loc_3_1117:
        mov    dword ptr [ebp - 4], 6
loc_3_111E:
        call   sub_3_0557
        mov    eax, dword ptr [ebp - 4]
loc_3_1126:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_112D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        cmp    dword ptr [0x424], esi        ; -> obj4:0x0424
        jne    loc_3_114A
        mov    eax, 0x871
        jmp    loc_3_11DE
loc_3_114A:
        call   sub_3_0548
        push   dword ptr [ebp + 8]
        call   sub_3_17A6
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        je     loc_3_11D2
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_3_11CB
        mov    dword ptr [ebp - 0xc], 1
        mov    dword ptr [ebp - 8], 0x34
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    edi, ecx
        add    edi, 0xc
        mov    eax, ecx
        mov    dword ptr [eax], 0x6060
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [ecx + 8], ebx
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   ecx
        push   edi
        push   4
        call   sub_3_1778
        add    esp, 0x18
        push   edi
        push   0x411
        call   sub_3_0566
        add    esp, 8
        jmp    loc_3_11D7
loc_3_11CB:
        mov    esi, 0x32
        jmp    loc_3_11D7
loc_3_11D2:
        mov    esi, 6
loc_3_11D7:
        call   sub_3_0557
        mov    eax, esi
loc_3_11DE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_11E5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0x424], 0          ; -> obj4:0x0424
        jne    loc_3_120F
        push   1
        call   sub_3_0584
        add    esp, 4
        or     eax, eax
        jne    loc_3_120F
        mov    eax, 0x871
        jmp    loc_3_12E4
loc_3_120F:
        mov    dword ptr [ebp - 0x18], 1
        mov    dword ptr [ebp - 0x14], 0x34
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    edi, dword ptr [ebp - 8]
        mov    esi, edi
        add    esi, 0xc
        lea    ecx, [ebp + 0x10]
        mov    edx, esi
        mov    eax, dword ptr [ebp + 0xc]
        dec    dword ptr [ebp + 0xc]
        or     eax, eax
        je     loc_3_1260
loc_3_124C:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        add    ecx, 4
        add    edx, 4
        mov    eax, dword ptr [ebp + 0xc]
        dec    dword ptr [ebp + 0xc]
        or     eax, eax
        jne    loc_3_124C
loc_3_1260:
        mov    ebx, dword ptr [ebp + 8]
        cmp    ebx, 0x415
        je     loc_3_1278
        cmp    ebx, 0x41f
        je     loc_3_1278
        mov    dword ptr [ebp - 4], esi
        jmp    loc_3_128F
loc_3_1278:
        mov    dword ptr [edi], 0x6060
        mov    dword ptr [edi + 4], 0
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [esi], edi
        mov    dword ptr [ebp - 4], edi
loc_3_128F:
        push   esi
        push   ebx
        call   sub_3_0566
        add    esp, 8
        push   5
        push   dword ptr [ebp - 4]
        call   sub_3_2216
        add    esp, 8
        or     eax, eax
        je     loc_3_12BB
        cmp    ebx, 0x415
        jne    loc_3_12B7
        mov    esi, dword ptr [edi + 4]
        jmp    loc_3_12C0
loc_3_12B7:
        mov    esi, dword ptr [esi]
        jmp    loc_3_12C0
loc_3_12BB:
        mov    esi, 5
loc_3_12C0:
        mov    dword ptr [ebp - 0x10], 0
        mov    dword ptr [ebp - 0xc], edi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, esi
loc_3_12E4:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_12EB:
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   6
        push   0x415
        call   sub_3_11E5
        add    esp, 0x20
        ret    
sub_3_1313:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x30
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0x2c], eax
        mov    dword ptr [ebp - 0x28], 0x374 ; -> obj4:0x0374
        mov    dword ptr [ebp - 0x24], 0x80000002
        push   dword ptr [ebp - 0x2c]
        push   dword ptr [ebp - 0x28]
        push   dword ptr [ebp - 0x24]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_13CA
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ebp - 0x20], eax
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x1c], esi
        lea    eax, [ebp - 0x30]
        mov    dword ptr [ebp - 0x18], eax
        mov    dword ptr [ebp - 0x14], 0
        mov    dword ptr [ebp - 0x10], 0x3a4 ; -> obj4:0x03A4
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        push   dword ptr [ebp - 0x20]
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        cmp    edi, 0xea
        je     loc_3_13C5
        or     esi, esi
        jne    loc_3_13C1
        or     edi, edi
        je     loc_3_13C5
loc_3_13C1:
        mov    eax, edi
        jmp    loc_3_13CA
loc_3_13C5:
        mov    eax, 0x7a
loc_3_13CA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_13D0:
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        call   sub_3_1313
        add    esp, 0xc
        ret    
loc_3_13E5:
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        call   sub_3_1313
        add    esp, 0xc
        ret    
sub_3_13FA:
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        push   3
        push   0x402
        call   sub_3_11E5
        add    esp, 0x14
        ret    
sub_3_1416:
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        push   4
        push   0x404
        call   sub_3_11E5
        add    esp, 0x18
        ret    
sub_3_1436:
        push   dword ptr [esp + 4]
        push   1
        push   0x406
        call   sub_3_11E5
        add    esp, 0xc
        ret    
loc_3_144A:
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   dword ptr [esp + 0x18]
        push   6
        push   0x41f
        call   sub_3_11E5
        add    esp, 0x20
        ret    
sub_3_1472:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        or     esi, esi
        jne    loc_3_1485
        sub    eax, eax
        jmp    loc_3_14DE
loc_3_1485:
        mov    dword ptr [ebp - 0xc], 1
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    dword ptr [ebp - 8], ecx
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    edx, dword ptr [ebp - 4]
        or     edx, edx
        je     loc_3_14DC
        mov    edi, esi
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_14DC:
        mov    eax, edx
loc_3_14DE:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_14E4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     loc_3_1518
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_3_151A
loc_3_1518:
        sub    eax, eax
loc_3_151A:
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_151E:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi]
        call   sub_3_14E4
        add    esp, 4
        push   dword ptr [esp + 0xc]
        call   sub_3_1472
        add    esp, 4
        mov    dword ptr [esi], eax
        mov    eax, 1
        pop    esi
        ret    
sub_3_1542:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    dword ptr [ebp - 0xc], 1
        mov    dword ptr [ebp - 8], 0x20
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        or     eax, eax
        je     loc_3_158B
        mov    ecx, eax
        mov    dword ptr [ecx], 0x464d
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [eax + 0x10], ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 0xc], ecx
        jmp    loc_3_158D
loc_3_158B:
        sub    eax, eax
loc_3_158D:
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_1591:
        push   ebx
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        or     edx, edx
        jne    loc_3_15A2
        sub    eax, eax
        jmp    loc_3_166C
loc_3_15A2:
        mov    eax, dword ptr [0x424]        ; -> obj4:0x0424
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     loc_3_15EB
loc_3_15AE:
        mov    ecx, edx
        mov    esi, dword ptr [eax + 0x10]
loc_3_15B3:
        mov    bl, byte ptr [ecx]
        cmp    bl, byte ptr [esi]
        jne    loc_3_15D3
        or     bl, bl
        je     loc_3_15CF
        mov    bl, byte ptr [ecx + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    loc_3_15D3
        add    ecx, 2
        add    esi, 2
        or     bl, bl
        jne    loc_3_15B3
loc_3_15CF:
        sub    ecx, ecx
        jmp    loc_3_15D8
loc_3_15D3:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_3_15D8:
        or     ecx, ecx
        je     loc_3_15E3
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        jne    loc_3_15AE
loc_3_15E3:
        or     eax, eax
        jne    loc_3_166C
loc_3_15EB:
        mov    eax, dword ptr [0x424]        ; -> obj4:0x0424
        mov    eax, dword ptr [eax + 0x1c]
        or     eax, eax
        je     loc_3_1628
        mov    ecx, dword ptr [eax + 0x58]
        mov    esi, edx
        mov    ecx, dword ptr [ecx + 0x10]
loc_3_15FF:
        mov    bl, byte ptr [esi]
        cmp    bl, byte ptr [ecx]
        jne    loc_3_161F
        or     bl, bl
        je     loc_3_161B
        mov    bl, byte ptr [esi + 1]
        cmp    bl, byte ptr [ecx + 1]
        jne    loc_3_161F
        add    esi, 2
        add    ecx, 2
        or     bl, bl
        jne    loc_3_15FF
loc_3_161B:
        sub    ecx, ecx
        jmp    loc_3_1624
loc_3_161F:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_3_1624:
        or     ecx, ecx
        je     loc_3_166C
loc_3_1628:
        mov    eax, dword ptr [0x424]        ; -> obj4:0x0424
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        je     loc_3_166C
loc_3_1634:
        mov    ecx, dword ptr [eax + 0x58]
        mov    esi, edx
        mov    ecx, dword ptr [ecx + 0x10]
loc_3_163C:
        mov    bl, byte ptr [esi]
        cmp    bl, byte ptr [ecx]
        jne    loc_3_165C
        or     bl, bl
        je     loc_3_1658
        mov    bl, byte ptr [esi + 1]
        cmp    bl, byte ptr [ecx + 1]
        jne    loc_3_165C
        add    esi, 2
        add    ecx, 2
        or     bl, bl
        jne    loc_3_163C
loc_3_1658:
        sub    ecx, ecx
        jmp    loc_3_1661
loc_3_165C:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_3_1661:
        or     ecx, ecx
        je     loc_3_166C
        mov    eax, dword ptr [eax + 8]
        or     eax, eax
        jne    loc_3_1634
loc_3_166C:
        pop    esi
        pop    ebx
        ret    
sub_3_166F:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 0x18]
        call   sub_3_16DF
        add    esp, 4
        mov    ecx, eax
        mov    edx, dword ptr [esp + 0x18]
        mov    byte ptr [ecx + edx], 0x5c
        inc    ecx
        lea    ebx, [edx + ecx + 5]
        mov    esi, 0x3ec                    ; -> obj4:0x03EC
        mov    edi, ebx
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    edi, 4
        mov    esi, dword ptr [esp + 0x14]
loc_3_16A0:
        or     esi, esi
        je     loc_3_16BF
        dec    ebx
        mov    ebp, 0xa
        mov    eax, esi
        sub    edx, edx
        div    ebp
        add    dl, 0x30
        mov    byte ptr [ebx], dl
        mov    eax, esi
        sub    edx, edx
        div    ebp
        mov    esi, eax
        jmp    loc_3_16C3
loc_3_16BF:
        dec    ebx
        mov    byte ptr [ebx], 0x30
loc_3_16C3:
        mov    eax, edi
        dec    edi
        or     eax, eax
        jne    loc_3_16A0
loc_3_16CA:
        mov    eax, ecx
        inc    ecx
        mov    edx, dword ptr [esp + 0x18]
        cmp    byte ptr [eax + edx], 0
        jne    loc_3_16CA
        lea    eax, [ecx - 1]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_16DF:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [0x424]        ; -> obj4:0x0424
        sub    eax, eax
        mov    edi, dword ptr [ecx]
        mov    esi, dword ptr [esp + 0x10]
loc_3_16F0:
        mov    ecx, eax
        inc    eax
        mov    dl, byte ptr [edi]
        mov    byte ptr [esi + ecx], dl
        inc    edi
        or     dl, dl
        jne    loc_3_16F0
        mov    byte ptr [esi + eax - 1], 0x5c
        mov    edx, 0x354                    ; -> obj4:0x0354
loc_3_1707:
        mov    ecx, eax
        inc    eax
        mov    bl, byte ptr [edx]
        mov    byte ptr [esi + ecx], bl
        inc    edx
        or     bl, bl
        jne    loc_3_1707
        dec    eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_1719:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax + 0x6c]
        or     edx, edx
        jne    loc_3_172D
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x6c], ecx
        jmp    loc_3_1747
loc_3_172D:
        cmp    dword ptr [edx + 8], 0
        je     loc_3_173D
        sub    eax, eax
loc_3_1735:
        mov    edx, dword ptr [edx + 8]
        cmp    dword ptr [edx + 8], eax
        jne    loc_3_1735
loc_3_173D:
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx + 0x18], edx
        mov    dword ptr [edx + 8], ecx
loc_3_1747:
        ret    
sub_3_1748:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 0xc]
        mov    eax, dword ptr [eax + 0x6c]
        mov    dword ptr [edx], 1
        or     eax, eax
        je     loc_3_176F
        mov    ecx, dword ptr [esp + 8]
loc_3_1761:
        cmp    dword ptr [eax + 0x1c], ecx
        je     loc_3_1777
        mov    eax, dword ptr [eax + 8]
        inc    dword ptr [edx]
        or     eax, eax
        jne    loc_3_1761
loc_3_176F:
        mov    dword ptr [edx], 0
        sub    eax, eax
loc_3_1777:
        ret    
sub_3_1778:
        push   esi
        push   edi
        lea    edi, [esp + 0x14]
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, esi
        dec    esi
        or     eax, eax
        je     loc_3_17A3
        mov    edx, 4
loc_3_178E:
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edi]
        mov    dword ptr [ecx], eax
        add    edi, edx
        add    dword ptr [esp + 0x10], edx
        mov    eax, esi
        dec    esi
        or     eax, eax
        jne    loc_3_178E
loc_3_17A3:
        pop    edi
        pop    esi
        ret    
sub_3_17A6:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     loc_3_17C1
        cmp    dword ptr [eax], 0x464d
        jne    loc_3_17C1
        mov    eax, dword ptr [eax + 0x10]
        cmp    dword ptr [eax], 0x4951
        je     loc_3_17C3
loc_3_17C1:
        sub    eax, eax
loc_3_17C3:
        ret    
sub_3_17C4:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 0x2c], 2
        je     loc_3_17D9
        sub    eax, eax
        jmp    loc_3_1906
loc_3_17D9:
        mov    edx, dword ptr [esp + 0x30]
        mov    edi, dword ptr [edx + 0x28]
        or     edi, edi
        je     loc_3_17F5
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    dword ptr [esp + 0x20], ecx
        jmp    loc_3_17FD
loc_3_17F5:
        mov    dword ptr [esp + 0x20], 0
loc_3_17FD:
        mov    edi, dword ptr [edx + 0x24]
        or     edi, edi
        je     loc_3_1815
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    dword ptr [esp + 0x1c], ecx
        jmp    loc_3_181D
loc_3_1815:
        mov    dword ptr [esp + 0x1c], 0
loc_3_181D:
        mov    edi, dword ptr [edx + 0x10]
        or     edi, edi
        je     loc_3_1835
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    dword ptr [esp + 0x18], ecx
        jmp    loc_3_183D
loc_3_1835:
        mov    dword ptr [esp + 0x18], 0
loc_3_183D:
        mov    edi, dword ptr [edx + 0x20]
        or     edi, edi
        je     loc_3_1855
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    dword ptr [esp + 0x14], ecx
        jmp    loc_3_185D
loc_3_1855:
        mov    dword ptr [esp + 0x14], 0
loc_3_185D:
        mov    edi, dword ptr [edx + 0x38]
        or     edi, edi
        je     loc_3_1875
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    dword ptr [esp + 0x10], ecx
        jmp    loc_3_187D
loc_3_1875:
        mov    dword ptr [esp + 0x10], 0
loc_3_187D:
        mov    edi, dword ptr [edx + 0x3c]
        or     edi, edi
        je     loc_3_1893
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    ebx, ecx
        jmp    loc_3_1895
loc_3_1893:
        sub    ebx, ebx
loc_3_1895:
        mov    edi, dword ptr [edx + 0x6c]
        or     edi, edi
        je     loc_3_18AB
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    esi, ecx
        jmp    loc_3_18AD
loc_3_18AB:
        sub    esi, esi
loc_3_18AD:
        mov    eax, dword ptr [edx + 0x34]
        mov    ecx, 0xffffffff
        mov    edi, dword ptr [eax + 0x10]
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edx, dword ptr [esp + 0x28]
        lea    ebp, [ecx - 1]
        mov    eax, dword ptr [edx + 0x34]
        mov    ecx, 0xffffffff
        mov    edi, dword ptr [eax + 0x10]
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    ebp, [ebp + ecx - 1]
        mov    edi, dword ptr [edx + 0x10]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    ebp, [ebp + ecx - 1]
        add    ebp, dword ptr [esp + 0x20]
        add    ebp, dword ptr [esp + 0x1c]
        add    ebp, dword ptr [esp + 0x18]
        add    ebp, dword ptr [esp + 0x14]
        add    ebp, dword ptr [esp + 0x10]
        add    ebp, ebx
        lea    eax, [esi + ebp + 0x6b]
loc_3_1906:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
sub_3_190E:
        sub    esp, 0x100
        push   esi
        mov    esi, dword ptr [esp + 0x108]
        test   byte ptr [esi + 0x14], 8
        je     loc_3_1946
        lea    eax, [esp + 4]
        push   eax
        push   dword ptr [esi + 0x1c]
        call   sub_3_166F
        lea    eax, [esp + 0xc]
        add    esp, 8
        push   eax
        call   sub_3_0755
        add    esp, 4
        or     eax, eax
        je     loc_3_1946
        mov    dword ptr [esi + 0x5c], eax
loc_3_1946:
        pop    esi
        add    esp, 0x100
        ret    
sub_3_194E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        cmp    esi, 2
        je     loc_3_1967
        mov    eax, dword ptr [ebp + 0x18]
        jmp    loc_3_1AF8
loc_3_1967:
        mov    ebx, dword ptr [ebp + 0xc]
        sub    ecx, ecx
        mov    eax, dword ptr [ebx + 0x14]
        and    eax, 0xff
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [0x2f0], -1         ; -> obj4:0x02F0
        je     loc_3_198E
        mov    eax, 0x2f0                    ; -> obj4:0x02F0
loc_3_1985:
        add    eax, 4
        inc    ecx
        cmp    dword ptr [eax], -1
        jne    loc_3_1985
loc_3_198E:
        mov    dword ptr [ebp - 0x1c], 1
        lea    eax, [ecx*4]
        mov    dword ptr [ebp - 0x18], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebp - 0x18]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, ecx
        mov    dword ptr [ebp - 8], eax
        or     ecx, ecx
        je     loc_3_1AF5
        cmp    esi, 2
        je     loc_3_19D4
        mov    eax, dword ptr [ebp + 0x18]
        jmp    loc_3_1AF8
loc_3_19D4:
        mov    eax, dword ptr [ebp + 8]
        add    ecx, 4
        mov    edx, dword ptr [eax + 0x10]
        mov    eax, dword ptr [ebp - 4]
        add    ecx, 4
        mov    dword ptr [eax], edx
        mov    eax, dword ptr [ebx + 0x28]
        mov    dword ptr [ecx - 4], eax
        mov    eax, dword ptr [ebx + 0x24]
        mov    dword ptr [ecx], eax
        add    ecx, 4
        mov    eax, dword ptr [ebx + 0x10]
        mov    dword ptr [ecx], eax
        add    ecx, 4
        mov    eax, dword ptr [ebx + 0x20]
        mov    dword ptr [ecx], eax
        add    ecx, 4
        mov    eax, dword ptr [ebx + 0x38]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 0x34]
        add    ecx, 4
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ecx], eax
        add    ecx, 4
        mov    eax, dword ptr [ebx + 0x3c]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 0x2c]
        add    ecx, 4
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx + 0x6c]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 0x28], 0
        mov    eax, dword ptr [ebx + 0x8c]
        mov    dword ptr [ecx + 0x38], eax
        mov    dword ptr [ecx + 0x3c], 0
        mov    eax, dword ptr [ebx + 0x54]
        mov    dword ptr [ecx + 0x40], eax
        mov    eax, dword ptr [ebx + 0x58]
        mov    dword ptr [ecx + 0x44], eax
        mov    eax, dword ptr [ebx + 0x84]
        mov    dword ptr [ecx + 0x48], eax
        mov    eax, dword ptr [ebx + 0x88]
        or     eax, eax
        jne    loc_3_1A77
        test   byte ptr [ebx + 0x17], 1
        je     loc_3_1A77
        mov    dword ptr [ecx + 0x48], 0
        mov    eax, dword ptr [ebx + 0x60]
        jmp    loc_3_1A7A
loc_3_1A77:
        mov    ecx, dword ptr [ebp + 0x14]
loc_3_1A7A:
        mov    dword ptr [ecx + 0x64], eax
        test   dword ptr [ebx + 0x14], 0x4800000
        je     loc_3_1A8F
        push   ebx
        call   sub_3_190E
        add    esp, 4
loc_3_1A8F:
        mov    eax, dword ptr [ebx + 0x5c]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 0x4c], eax
        lea    esi, [ebx + 0x40]
        mov    edi, ecx
        add    edi, 0x50
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 0x60], 0
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx + 0x34], eax
        mov    eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [ecx], eax
        push   dword ptr [ebp + 0x18]
        push   0x2f0                         ; -> obj4:0x02F0
        push   ecx
        push   dword ptr [ebp - 8]
        call   sub_3_1AFF
        add    esp, 0x10
        mov    dword ptr [ebp + 0x18], eax
        mov    dword ptr [ebp - 0x10], 0
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    dword ptr [ebp - 0x20], eax
loc_3_1AF5:
        mov    eax, dword ptr [ebp + 0x18]
loc_3_1AF8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_1AFF:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x1c]
        cmp    dword ptr [ebx], -1
        je     loc_3_1B6D
        mov    edx, dword ptr [esp + 0x14]
        mov    ebp, dword ptr [esp + 0x20]
loc_3_1B14:
        mov    edi, dword ptr [edx]
        or     edi, edi
        je     loc_3_1B53
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    ebp, ecx
        mov    edi, dword ptr [edx]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + ecx], ebp
        jmp    loc_3_1B60
loc_3_1B53:
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + ecx], 0
loc_3_1B60:
        add    ebx, 4
        add    edx, 4
        cmp    dword ptr [ebx], -1
        jne    loc_3_1B14
        jmp    loc_3_1B71
loc_3_1B6D:
        mov    ebp, dword ptr [esp + 0x20]
loc_3_1B71:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_1B78:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [0x424]        ; -> obj4:0x0424
        mov    ebp, dword ptr [eax + 4]
        or     ebp, ebp
        je     loc_3_1BAC
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [esp + 0x1c]
loc_3_1B94:
        push   ebx
        push   edi
        push   ebp
        call   sub_3_1748
        add    esp, 0xc
        mov    dword ptr [esi], eax
        or     eax, eax
        jne    loc_3_1BAC
        mov    ebp, dword ptr [ebp + 8]
        or     ebp, ebp
        jne    loc_3_1B94
loc_3_1BAC:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_1BB3:
        push   esi
        push   edi
        cmp    dword ptr [esp + 0x10], 2
        je     loc_3_1BC6
        mov    eax, 0x7c
        jmp    loc_3_1C50
loc_3_1BC6:
        mov    edi, dword ptr [esp + 0x14]
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edi + 0x38]
        cmp    eax, 0x63
        jb     loc_3_1BDC
        mov    dword ptr [esi + 0x8c], eax
loc_3_1BDC:
        mov    eax, dword ptr [edi + 0x40]
        cmp    eax, 0x5a0
        jae    loc_3_1BF8
        cmp    dword ptr [edi + 0x44], 0x5a0
        jae    loc_3_1BF8
        mov    dword ptr [esi + 0x54], eax
        mov    eax, dword ptr [edi + 0x44]
        mov    dword ptr [esi + 0x58], eax
loc_3_1BF8:
        push   dword ptr [edi + 0x14]
        lea    eax, [esi + 0x20]
        push   eax
        call   sub_3_151E
        add    esp, 8
        push   dword ptr [edi + 0x2c]
        lea    eax, [esi + 0x6c]
        push   eax
        call   sub_3_151E
        add    esp, 8
        push   dword ptr [edi + 0x10]
        lea    eax, [esi + 0x10]
        push   eax
        call   sub_3_151E
        add    esp, 8
        push   dword ptr [edi + 8]
        lea    eax, [esi + 0x28]
        push   eax
        call   sub_3_151E
        add    esp, 8
        push   dword ptr [edi + 0xc]
        lea    eax, [esi + 0x24]
        push   eax
        call   sub_3_151E
        add    esp, 8
        mov    eax, dword ptr [edi + 0x34]
        and    eax, 0xff
        or     dword ptr [esi + 0x14], eax
        sub    eax, eax
loc_3_1C50:
        pop    edi
        pop    esi
        ret    
sub_3_1C53:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x68
        push   esi
        push   edi
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x68], eax
        mov    dword ptr [ebp - 0x64], 0x3f4 ; -> obj4:0x03F4
        mov    dword ptr [ebp - 0x60], 0x80000002
        push   dword ptr [ebp - 0x68]
        push   dword ptr [ebp - 0x64]
        push   dword ptr [ebp - 0x60]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_1DBC
        lea    eax, [ebp - 4]
        mov    dword ptr [ebp - 0x5c], eax
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ebp - 0x58], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0x54], eax
        push   dword ptr [ebp - 0x5c]
        push   dword ptr [ebp - 0x58]
        push   dword ptr [ebp - 0x54]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_1DA4
        lea    edi, [esi + 0x38]
        mov    dword ptr [ebp - 0x50], edi
        mov    dword ptr [ebp - 0x4c], 0
        lea    eax, [ebp - 0x2c]
        mov    dword ptr [ebp - 0x48], eax
        mov    dword ptr [ebp - 0x44], 0
        mov    dword ptr [ebp - 0x40], 0x3ac ; -> obj4:0x03AC
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x3c], eax
        push   dword ptr [ebp - 0x50]
        push   dword ptr [ebp - 0x4c]
        push   dword ptr [ebp - 0x48]
        push   dword ptr [ebp - 0x44]
        push   dword ptr [ebp - 0x40]
        push   dword ptr [ebp - 0x3c]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_3_1D85
        cmp    dword ptr [edi], 0
        je     loc_3_1D85
        mov    dword ptr [ebp - 0x38], 1
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 0x34], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x38]
        push   dword ptr [ebp - 0x34]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [ebp - 0x30], eax
        mov    eax, dword ptr [ebp - 0x30]
        mov    dword ptr [esi + 0x3c], eax
        mov    dword ptr [ebp - 0x28], edi
        mov    eax, dword ptr [esi + 0x3c]
        mov    dword ptr [ebp - 0x24], eax
        lea    eax, [ebp - 0x2c]
        mov    dword ptr [ebp - 0x20], eax
        mov    dword ptr [ebp - 0x1c], 0
        mov    dword ptr [ebp - 0x18], 0x3ac ; -> obj4:0x03AC
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x14], eax
        push   dword ptr [ebp - 0x28]
        push   dword ptr [ebp - 0x24]
        push   dword ptr [ebp - 0x20]
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        jmp    loc_3_1D8C
loc_3_1D85:
        mov    dword ptr [esi + 0x3c], 0
loc_3_1D8C:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], eax
        push   dword ptr [ebp - 0x10]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_3_1DA4:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        push   dword ptr [ebp - 0xc]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_3_1DBC:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_1DC2:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 0x28], 2
        je     loc_3_1DD7
        sub    edi, edi
        jmp    loc_3_1F0A
loc_3_1DD7:
        mov    esi, dword ptr [esp + 0x24]
        mov    edi, dword ptr [esi + 0x1c]
        or     edi, edi
        je     loc_3_1DF3
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    dword ptr [esp + 0x1c], ecx
        jmp    loc_3_1DFB
loc_3_1DF3:
        mov    dword ptr [esp + 0x1c], 0
loc_3_1DFB:
        mov    edi, dword ptr [esi + 0x30]
        or     edi, edi
        je     loc_3_1E13
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    dword ptr [esp + 0x18], ecx
        jmp    loc_3_1E1B
loc_3_1E13:
        mov    dword ptr [esp + 0x18], 0
loc_3_1E1B:
        mov    edi, dword ptr [esi + 0x54]
        or     edi, edi
        je     loc_3_1E33
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    dword ptr [esp + 0x14], ecx
        jmp    loc_3_1E3B
loc_3_1E33:
        mov    dword ptr [esp + 0x14], 0
loc_3_1E3B:
        mov    edi, dword ptr [esi + 0x50]
        or     edi, edi
        je     loc_3_1E53
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    dword ptr [esp + 0x10], ecx
        jmp    loc_3_1E5B
loc_3_1E53:
        mov    dword ptr [esp + 0x10], 0
loc_3_1E5B:
        mov    edi, dword ptr [esi + 0x28]
        or     edi, edi
        je     loc_3_1E71
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    ebx, ecx
        jmp    loc_3_1E73
loc_3_1E71:
        sub    ebx, ebx
loc_3_1E73:
        mov    edi, dword ptr [esi + 0x2c]
        or     edi, edi
        je     loc_3_1E89
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edx, ecx
        jmp    loc_3_1E8B
loc_3_1E89:
        sub    edx, edx
loc_3_1E8B:
        mov    eax, dword ptr [esi + 0x24]
        mov    ecx, 0xffffffff
        mov    edi, dword ptr [eax + 0x10]
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [esi + 0x34]
        lea    ebp, [ecx - 1]
        mov    edi, dword ptr [eax + 0x10]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    eax, dword ptr [esi + 0x58]
        lea    ebp, [ebp + ecx - 1]
        mov    edi, dword ptr [eax + 0x10]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    ebp, [ebp + ecx - 1]
        mov    edi, dword ptr [esi + 0x10]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    ebp, [ebp + ecx - 1]
        add    ebp, dword ptr [esp + 0x1c]
        add    ebp, dword ptr [esp + 0x18]
        add    ebp, dword ptr [esp + 0x14]
        add    ebp, dword ptr [esp + 0x10]
        add    ebp, ebx
        lea    edi, [edx + ebp + 0x58]
        push   esi
        call   sub_3_1C53
        add    esp, 4
        cmp    dword ptr [esi + 0x3c], 0
        je     loc_3_1F0A
        mov    eax, dword ptr [esi + 0x38]
        lea    edi, [edi + eax + 3]
        and    edi, 0xfffffffc
loc_3_1F0A:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_3_1F14:
        sub    esp, 0x54
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x6c]
        cmp    eax, 2
        je     loc_3_1F29
        jmp    loc_3_2000
loc_3_1F29:
        mov    ebx, dword ptr [esp + 0x68]
        mov    dword ptr [esp + 0x10], 0
        mov    eax, dword ptr [ebx + 0x10]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [ebx + 0x58]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [ebx + 0x34]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [ebx + 0x30]
        mov    dword ptr [esp + 0x24], eax
        mov    eax, dword ptr [ebx + 0x54]
        mov    dword ptr [esp + 0x28], eax
        mov    eax, dword ptr [ebx + 0x50]
        mov    dword ptr [esp + 0x2c], eax
        mov    eax, dword ptr [ebx + 0x24]
        push   dword ptr [esp + 0x74]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esp + 0x34], eax
        mov    eax, dword ptr [ebx + 0x28]
        mov    dword ptr [esp + 0x38], eax
        mov    eax, dword ptr [ebx + 0x2c]
        mov    dword ptr [esp + 0x3c], eax
        mov    ebp, dword ptr [esp + 0x74]
        push   0x31c                         ; -> obj4:0x031C
        lea    eax, [esp + 0x18]
        push   ebp
        push   eax
        call   sub_3_1AFF
        add    esp, 0x10
        cmp    dword ptr [ebx + 0x3c], 0
        je     loc_3_1FC6
        sub    eax, dword ptr [ebx + 0x38]
        and    eax, 0xfffffffc
        mov    dword ptr [ebp + 0x1c], eax
        mov    edx, dword ptr [ebx + 0x38]
        mov    edi, eax
        mov    esi, dword ptr [ebx + 0x3c]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_3_1FCD
loc_3_1FC6:
        mov    dword ptr [ebp + 0x1c], 0
loc_3_1FCD:
        mov    ecx, dword ptr [ebx + 0x18]
        mov    dword ptr [ebp + 0x34], ecx
        mov    ecx, dword ptr [ebx + 0x40]
        mov    dword ptr [ebp + 0x38], ecx
        mov    dword ptr [ebp + 0x3c], 0
        mov    ecx, dword ptr [ebx + 0x48]
        mov    dword ptr [ebp + 0x40], ecx
        mov    ecx, dword ptr [ebx + 0x4c]
        mov    dword ptr [ebp + 0x44], ecx
        mov    ecx, dword ptr [ebx + 0x60]
        mov    dword ptr [ebp + 0x4c], ecx
        mov    ecx, dword ptr [ebx + 0x14]
        mov    dword ptr [ebp + 0x48], ecx
        mov    ecx, dword ptr [ebx + 0x64]
        mov    dword ptr [ebp + 0x50], ecx
        jmp    loc_3_2004
loc_3_2000:
        mov    eax, dword ptr [esp + 0x74]
loc_3_2004:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x54
        ret    
loc_3_200C:
        push   0x3bc                         ; -> obj4:0x03BC
        call   sub_3_05E4
        add    esp, 4
        sub    eax, eax
        ret    
loc_3_201C:
        or     al, byte ptr [ebx]
        adc    byte ptr [edi], cl
        pop    ss
        and    dword ptr [eax], eax
        add    byte ptr [edi], dl            ; -> loc_3_0117
        add    dword ptr [eax], eax
        add    byte ptr [0x83000021], al     ; -> loc_3_2105
        and    byte ptr [eax], al
        add    byte ptr [edx + 0x60000020], cl ; -> loc_3_208A
        add    dword ptr [eax], eax
        add    byte ptr [ecx - 0x67ffffe0], dl ; -> loc_3_2091
        and    byte ptr [eax], al
        add    byte ptr [edi + 0x4a000020], bl ; -> loc_3_209F
        add    dword ptr [eax], eax
        add    byte ptr [esi + 0x20], ah     ; -> loc_3_20A6
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x20bb0000], dh ; -> loc_3_20B4
        add    byte ptr [eax], al
        ret    0x20
        db  000h
loc_3_205C:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
        pop    ss
        and    dword ptr [eax], eax
        add    dh, dl                        ; -> loc_3_03D6
        add    eax, dword ptr [eax]
        add    al, dl                        ; -> loc_3_20D0
        and    byte ptr [eax], al
        add    bh, dl                        ; -> loc_3_20D7
        and    byte ptr [eax], al
        add    dh, bl                        ; -> loc_3_20DE
        and    byte ptr [eax], al
        add    ch, ah                        ; -> loc_3_20E5
        and    byte ptr [eax], al
        add    ah, ch                        ; -> loc_3_20EC
        and    byte ptr [eax], al
        add    byte ptr [eax + 1], bh
        jmp    loc_3_20F3
loc_3_2083:
        mov    eax, 2
        jmp    loc_3_20F3
loc_3_208A:
        mov    eax, 3
        jmp    loc_3_20F3
loc_3_2091:
        mov    eax, 4
        jmp    loc_3_20F3
loc_3_2098:
        mov    eax, 5
        jmp    loc_3_20F3
loc_3_209F:
        mov    eax, 6
        jmp    loc_3_20F3
loc_3_20A6:
        mov    eax, 7
        jmp    loc_3_20F3
        db  0B8h, 008h, 000h, 000h, 000h, 0EBh, 03Fh
loc_3_20B4:
        mov    eax, 9
        jmp    loc_3_20F3
loc_3_20BB:
        mov    eax, 0xa
        jmp    loc_3_20F3
loc_3_20C2:
        mov    eax, 0xb
        jmp    loc_3_20F3
        db  0B8h, 00Ch, 000h, 000h, 000h, 0EBh, 023h
loc_3_20D0:
        mov    eax, 0xd
        jmp    loc_3_20F3
loc_3_20D7:
        mov    eax, 0xe
        jmp    loc_3_20F3
loc_3_20DE:
        mov    eax, 0xf
        jmp    loc_3_2117
loc_3_20E5:
        mov    eax, 0x10
        jmp    loc_3_20F3
loc_3_20EC:
        mov    eax, 0x11
        jmp    loc_3_20F3
loc_3_20F3:
        push   ebp
        push   edi
        mov    ebp, dword ptr [esp + 0xc]
        mov    eax, 0x1f
        mov    word ptr [ebp + 0x1a], ax
        pop    edi
        pop    ebp
        ret    
loc_3_2105:
        push   ebp
        push   edi
        mov    ebp, dword ptr [esp + 0xc]
        mov    eax, 2
        mov    word ptr [ebp + 0x1a], ax
        pop    edi
        pop    ebp
        ret    
loc_3_2117:
        push   ebp
        mov    ebp, dword ptr [esp + 8]
        xor    eax, eax
        mov    word ptr [ebp + 0x1a], ax
        pop    ebp
        ret    
loc_3_2124:
        push   ebx
        push   esi
        mov    ecx, dword ptr [esi + 0x10]
        mov    eax, dword ptr [0x49]         ; -> obj4:0x0049
        mov    edx, eax
        xor    eax, ecx
        and    eax, edx
        mov    dword ptr [0x49], ecx         ; -> obj4:0x0049
        je     loc_3_2175
        mov    ecx, 9
        mov    edx, 0x4d                     ; -> obj4:0x004D
loc_3_2146:
        mov    ebx, dword ptr [edx]
        test   dword ptr [ebx + 0x14], eax
        je     loc_3_216F
        test   dword ptr [ebx + 0x10], 1
        je     loc_3_216F
        mov    esi, dword ptr [ebx + 0xc]
        test   esi, esi
        jne    loc_3_216F
        push   ecx
        push   edx
        push   dword ptr [ebx + 8]
        VxDCall  IFSMgr.IFSMgr_FSDUnmountCFSD    ; cd2050004000
        pop    ecx
        pop    edx
        pop    ecx
        or     eax, eax
        je     loc_3_216F
loc_3_216F:
        add    edx, 4
        dec    ecx
        jne    loc_3_2146
loc_3_2175:
        push   0                             ; -> obj4:0x0000
        push   4                             ; -> obj4:0x0004
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        cmp    eax, 0
        jne    loc_3_21BE
        push   4
        push   0x49                          ; -> obj4:0x0049
        push   3
        push   0
        push   0x34                          ; -> obj4:0x0034
        push   dword ptr [0]                 ; -> obj4:0x0000
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
        push   dword ptr [0]                 ; -> obj4:0x0000
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        pop    ecx
loc_3_21BE:
        mov    eax, 1
        pop    esi
        pop    ebx
        ret    
loc_3_21C6:
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [0x424], eax        ; -> obj4:0x0424
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [0x428], eax        ; -> obj4:0x0428
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [0x42c], eax        ; -> obj4:0x042C
        cmp    byte ptr [0x2d9], 0           ; -> obj4:0x02D9
        jne    loc_3_21F3
        push   0x200c                        ; -> loc_3_200C
        VMMCall  VMM._SignalID                   ; cd2027010100
        pop    ecx
loc_3_21F3:
        ret    
loc_3_21F4:
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_3_0BA4
        pop    ecx
        ret    
loc_3_21FF:
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        call   sub_3_0C1C
        pop    ecx
        ret    
loc_3_220A:
        mov    eax, dword ptr [esi + 0x10]
        push   eax
        VMMCall  VMM._SignalID                   ; cd2027010100
        pop    ecx
        ret    
sub_3_2216:
        push   ebx
        push   esi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    esi, ebx
        VMMCall  VMM.Get_Crit_Section_Status     ; cd2029000100
        jecxz  loc_3_222C
        cmp    esi, ebx
        je     loc_3_2243
loc_3_222C:
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        pop    ecx
        pop    ecx
        mov    eax, 1
        jmp    loc_3_2245
loc_3_2243:
        xor    eax, eax
loc_3_2245:
        pop    esi
        pop    ebx
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x430  RW-
        db  000h, 000h, 000h, 000h, 053h, 079h, 073h, 074h, 065h, 06Dh, 05Ch, 043h, 075h, 072h, 072h, 065h
        db  06Eh, 074h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 053h, 065h, 074h, 05Ch, 043h, 06Fh, 06Eh
        db  074h, 072h, 06Fh, 06Ch, 05Ch, 050h, 072h, 069h, 06Eh, 074h, 05Ch, 050h, 072h, 069h, 06Eh, 074h
        db  065h, 072h, 073h, 000h, 050h, 072h, 069h, 06Eh, 074h, 065h, 072h, 073h, 04Dh, 061h, 073h, 06Bh
        db  000h, 003h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  obj4:0x0071              ; fixup
        dd  obj4:0x00A9              ; fixup
        dd  obj4:0x00C5              ; fixup
        dd  obj4:0x00E1              ; fixup
        dd  obj4:0x008D              ; fixup
        dd  obj4:0x00FD              ; fixup
        dd  obj4:0x0119              ; fixup
        dd  obj4:0x0135              ; fixup
        dd  obj4:0x0151              ; fixup
        db  0EFh, 0BEh, 0ADh, 00Bh
        dd  obj4:0x02A9              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  obj4:0x01FF              ; fixup
        db  0EFh, 0BEh, 0ADh, 00Bh
        dd  obj4:0x02A9              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        dd  obj4:0x01FF              ; fixup
        db  0EFh, 0BEh, 0ADh, 00Bh
        dd  obj4:0x02AF              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        dd  obj4:0x020F              ; fixup
        db  0EFh, 0BEh, 0ADh, 00Bh
        dd  obj4:0x02B5              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        dd  obj4:0x021F              ; fixup
        db  0EFh, 0BEh, 0ADh, 00Bh
        dd  obj4:0x02BB              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 008h, 000h, 000h, 000h
        dd  obj4:0x022F              ; fixup
        db  0EFh, 0BEh, 0ADh, 00Bh
        dd  obj4:0x02C1              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h
        dd  obj4:0x023F              ; fixup
        db  0EFh, 0BEh, 0ADh, 00Bh
        dd  obj4:0x02C7              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h
        dd  obj4:0x024F              ; fixup
        db  0EFh, 0BEh, 0ADh, 00Bh
        dd  obj4:0x02CD              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 040h, 000h, 000h, 000h
        dd  obj4:0x025F              ; fixup
        db  0EFh, 0BEh, 0ADh, 00Bh
        dd  obj4:0x02D3              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 080h, 000h, 000h, 000h
        dd  obj4:0x026F              ; fixup
        dd  obj4:0x0195              ; fixup
        dd  obj4:0x01A1              ; fixup
        dd  obj4:0x01AD              ; fixup
        dd  obj4:0x01B9              ; fixup
        dd  obj4:0x01C5              ; fixup
        dd  obj4:0x01CF              ; fixup
        dd  obj4:0x01DB              ; fixup
        dd  obj4:0x01E7              ; fixup
        dd  obj4:0x01F3              ; fixup
        db  000h, 000h, 000h, 000h, 00Ah, 000h, 04Ch, 000h, 050h, 000h, 054h, 000h, 031h, 000h, 000h, 000h
        db  00Ah, 000h, 04Ch, 000h, 050h, 000h, 054h, 000h, 032h, 000h, 000h, 000h, 00Ah, 000h, 04Ch, 000h
        db  050h, 000h, 054h, 000h, 033h, 000h, 000h, 000h, 00Ah, 000h, 04Ch, 000h, 050h, 000h, 054h, 000h
        db  034h, 000h, 000h, 000h, 008h, 000h, 050h, 000h, 052h, 000h, 04Eh, 000h, 000h, 000h, 00Ah, 000h
        db  043h, 000h, 04Fh, 000h, 04Dh, 000h, 031h, 000h, 000h, 000h, 00Ah, 000h, 043h, 000h, 04Fh, 000h
        db  04Dh, 000h, 032h, 000h, 000h, 000h, 00Ah, 000h, 043h, 000h, 04Fh, 000h, 04Dh, 000h, 033h, 000h
        db  000h, 000h, 00Ah, 000h, 043h, 000h, 04Fh, 000h, 04Dh, 000h, 034h, 000h, 000h, 000h, 00Eh, 000h
        db  004h, 000h, 00Ah, 000h, 04Ch, 000h, 050h, 000h, 054h, 000h, 031h, 000h, 000h, 000h, 00Eh, 000h
        db  004h, 000h, 00Ah, 000h, 04Ch, 000h, 050h, 000h, 054h, 000h, 032h, 000h, 000h, 000h, 00Eh, 000h
        db  004h, 000h, 00Ah, 000h, 04Ch, 000h, 050h, 000h, 054h, 000h, 033h, 000h, 000h, 000h, 00Eh, 000h
        db  004h, 000h, 00Ah, 000h, 04Ch, 000h, 050h, 000h, 054h, 000h, 034h, 000h, 000h, 000h, 00Eh, 000h
        db  004h, 000h, 00Ah, 000h, 043h, 000h, 04Fh, 000h, 04Dh, 000h, 031h, 000h, 000h, 000h, 00Eh, 000h
        db  004h, 000h, 00Ah, 000h, 043h, 000h, 04Fh, 000h, 04Dh, 000h, 032h, 000h, 000h, 000h, 00Eh, 000h
        db  004h, 000h, 00Ah, 000h, 043h, 000h, 04Fh, 000h, 04Dh, 000h, 033h, 000h, 000h, 000h, 00Eh, 000h
        db  004h, 000h, 00Ah, 000h, 043h, 000h, 04Fh, 000h, 04Dh, 000h, 034h, 000h, 000h, 000h
        dd  obj4:0x0297              ; fixup
        db  000h, 000h, 000h, 000h
        dd  obj4:0x02A5              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 044h, 04Fh, 053h, 020h
        db  070h, 072h, 069h, 06Eh, 074h, 020h, 06Ah, 06Fh, 062h, 000h, 052h, 041h, 057h, 000h, 04Ch, 050h
        db  054h, 031h, 03Ah, 000h, 04Ch, 050h, 054h, 032h, 03Ah, 000h, 04Ch, 050h, 054h, 033h, 03Ah, 000h
        db  04Ch, 050h, 054h, 034h, 03Ah, 000h, 043h, 04Fh, 04Dh, 031h, 03Ah, 000h, 043h, 04Fh, 04Dh, 032h
        db  03Ah, 000h, 043h, 04Fh, 04Dh, 033h, 03Ah, 000h, 043h, 04Fh, 04Dh, 034h, 03Ah, 000h, 001h, 000h
        db  000h
        ; ---- dispatch table (5 entries) ----
        dd  loc_3_21C6
        dd  loc_3_21F4
        dd  loc_3_21FF
        dd  loc_3_220A
        dd  loc_3_2124
        db  004h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 010h, 000h, 000h, 000h
        db  014h, 000h, 000h, 000h, 018h, 000h, 000h, 000h, 01Ch, 000h, 000h, 000h, 020h, 000h, 000h, 000h
        db  024h, 000h, 000h, 000h, 02Ch, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  004h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 00Ch, 000h, 000h, 000h, 010h, 000h, 000h, 000h
        db  014h, 000h, 000h, 000h, 018h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 024h, 000h, 000h, 000h
        db  028h, 000h, 000h, 000h, 02Ch, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  052h, 041h, 057h, 000h, 050h, 052h, 049h, 04Eh, 054h, 045h, 052h, 053h, 000h, 000h, 000h, 000h
        db  055h, 06Eh, 074h, 069h, 074h, 06Ch, 065h, 064h, 000h, 000h, 000h, 000h
        dd  obj4:0x034C              ; fixup
        db  000h, 002h, 000h, 000h, 053h, 04Fh, 046h, 054h, 057h, 041h, 052h, 045h, 05Ch, 04Dh, 069h, 063h
        db  072h, 06Fh, 073h, 06Fh, 066h, 074h, 05Ch, 057h, 069h, 06Eh, 064h, 06Fh, 077h, 073h, 05Ch, 043h
        db  075h, 072h, 072h, 065h, 06Eh, 074h, 056h, 065h, 072h, 073h, 069h, 06Fh, 06Eh, 05Ch, 053h, 045h
        db  054h, 055h, 050h, 000h, 053h, 079h, 073h, 044h, 069h, 072h, 000h, 000h, 044h, 065h, 066h, 061h
        db  075h, 06Ch, 074h, 020h, 044h, 065h, 076h, 04Dh, 06Fh, 064h, 065h, 000h, 02Dh, 000h, 000h, 000h
        db  020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 073h, 070h, 06Fh, 06Fh
        db  06Ch, 033h, 032h, 02Eh, 065h, 078h, 065h, 000h, 000h, 000h, 000h, 000h, 02Eh, 053h, 050h, 04Ch
        db  000h, 000h, 000h, 000h, 053h, 079h, 073h, 074h, 065h, 06Dh, 05Ch, 043h, 075h, 072h, 072h, 065h
        db  06Eh, 074h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 053h, 065h, 074h, 05Ch, 043h, 06Fh, 06Eh
        db  074h, 072h, 06Fh, 06Ch, 05Ch, 050h, 072h, 069h, 06Eh, 074h, 05Ch, 050h, 072h, 069h, 06Eh, 074h
        db  065h, 072h, 073h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x16  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 008h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0C3h, 033h, 0DBh
        db  033h, 0F6h, 0B8h, 001h, 000h, 0C3h
VxD__LTEXT ENDS
