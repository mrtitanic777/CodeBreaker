; PERF -- reconstructed 32-bit disassembly (VxD / LE format)
; 6 objects, 1576 instructions recovered by recursive descent

Declare_Virtual_Device PERF, 4, 0, PERF_Control, 00048h, Undefined_Init_Order
;   DDK version 0400h   init_order 000900000h   flags 00000h
;   service table: obj1:0xd0, 5 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x140  R-X
PERF_Control:
        cmp    eax, 0
        je     loc_3_0000
        cmp    eax, 1
        je     loc_3_0016
        cmp    eax, 4
        je     loc_4_0000
        cmp    eax, 0x24
        je     loc_4_0000
        cmp    eax, 3
        je     loc_4_000C
        clc    
        ret    
        db  000h
sub_1_0030:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0xc]
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_1_004E
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_1_004E:
        ret    0x10
        db  000h, 000h, 000h
sub_1_0054:
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
        db  000h
sub_1_0068:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        add    byte ptr [eax], al
sub_1_0070:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        add    byte ptr [eax], al
sub_1_0078:
        VMMCall  VMM._RegCreateKey               ; cd204a810100
        add    byte ptr [eax], al
sub_1_0080:
        VMMCall  VMM._RegDeleteKey               ; cd204b810100
        add    byte ptr [eax], al
sub_1_0088:
        VMMCall  VMM._RegEnumKey                 ; cd204c810100
        add    byte ptr [eax], al
sub_1_0090:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        add    byte ptr [eax], al
sub_1_0098:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        add    byte ptr [eax], al
sub_1_00A0:
        VMMCall  VMM._RegCreateDynKey            ; cd2076810100
        add    byte ptr [eax], al
sub_1_00A8:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_1_00B0:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_1_00B8:
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    eax, esi
        pop    esi
        ret    8
        db  000h, 000h, 000h
        dd  PERF_Service_00          ; fixup
        dd  PERF_Service_01          ; fixup
        dd  PERF_Service_02          ; fixup
        dd  PERF_Service_03          ; fixup
        dd  PERF_Service_04          ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00048h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'PERF    '                  ; DDB_Name
        dd  000900000h                  ; DDB_Init_Order
        dd  PERF_Control             ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x00D0              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x144  R-X
        db  00Bh, 000h, 000h, 000h
loc_2_0004:
        and    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        or     byte ptr [eax], al
loc_2_0010:
        adc    dword ptr [eax], eax
loc_2_0012:
        and    eax, dword ptr [eax]
loc_2_0014:
        cmp    byte ptr [eax], al
loc_2_0016:
        jo     loc_2_0018
loc_2_0018:
        lodsd  eax, dword ptr [esi]
        add    byte ptr [eax], al
        db  001h
loc_2_001C:
        or     byte ptr [ecx], al
loc_2_001E:
        adc    dword ptr [ecx], eax
loc_2_0020:
        push   ss
        add    dword ptr [eax], esp
        add    dword ptr [ebx + 0x65], ecx
        jb     0x96
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax], al
        push   esp
        push   0x64616572
        jae    loc_2_0034
loc_2_0034:
        add    byte ptr [esi + 0x69], dl
        jb     0xad
        jne    0x9c
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x61], cl
        arpl   word ptr [eax + 0x69], bp
        outsb  dx, byte ptr [esi]
        jae    loc_2_0046
loc_2_0046:
        add    byte ptr [eax + 0x72], dl
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebp + 0x73], sp
        jae    loc_2_00BE
        jb     0x71
        push   ebp
        jae    0xb5
        and    byte ptr gs:[bx + si], ch
        and    eax, 0x54000029
        push   0x756e2065
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebp + 0x73], sp
        jae    0xe1
        jb     loc_2_0094
        je     loc_2_00DE
        jb     loc_2_00DD
        popal  
        jae    0x9c
        jo     0xf0
        jae    loc_2_00E6
        outsb  dx, byte ptr [esi]
        je     loc_2_00A4
        imul   ebp, dword ptr [esi + 0x20], 0x20656874
        jae    0x106
        jae    0x103
        insd   dword ptr es:[edi], dx
        add    byte ptr cs:[eax], al
loc_2_0094:
        push   esp
        push   0x756e2065
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [esi + 0x69], dh
loc_2_00A4:
        jb     loc_2_011A
        jne    loc_2_0109
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x61], ch
        arpl   word ptr [eax + 0x69], bp
        outsb  dx, byte ptr [esi]
        jae    0xd3
        sub    byte ptr [esi + 0x4d], dl
        jae    0xe1
        and    byte ptr [eax + 0x72], dh
        jae    0x123
loc_2_00BE:
        outsb  dx, byte ptr [esi]
        je     0xe1
        imul   ebp, dword ptr [esi + 0x20], 0x20656874
        jae    0x143
        jae    loc_2_0140
        insd   dword ptr es:[edi], dx
        add    byte ptr cs:[eax], al
        push   esp
        push   0x65702065
        jb     loc_2_013C
        outsb  dx, byte ptr gs:[esi]
        je     loc_2_00FD
loc_2_00DD:
        outsd  dx, dword ptr [esi]
loc_2_00DE:
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebp + 0x73], sp
loc_2_00E6:
        jae    0x157
        jb     0x10a
        je     0x155
        insd   dword ptr es:[edi], dx
        and    byte ptr gs:[eax + ebp*2 + 0x61], dh
        je     0x114
        imul   esi, dword ptr [ebx + 0x20], 0x20746f6e
        jae    0x16d
loc_2_00FD:
        outsb  dx, byte ptr gs:[esi]
        je     loc_2_0121
        imul   esp, dword ptr [esp + ebp*2 + 0x65], 0x5420202e
loc_2_0109:
        push   0x76207369
        popal  
        insb   byte ptr es:[edi], dx
        jne    0x177
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ecx + 0x70], ah
        jo     0x18c
loc_2_011A:
        outsd  dx, dword ptr [esi]
        js     0x186
        insd   dword ptr es:[edi], dx
        popal  
        je     0x186
loc_2_0121:
        add    byte ptr cs:[eax], al
        dec    ebx
        inc    ebp
        push   edx
        dec    esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
        push   esp
        push   0x64616572
        jae    loc_2_0134
loc_2_0134:
        add    byte ptr [esi + 0x4d], dl
        jae    loc_2_0139
loc_2_0139:
        add    byte ptr [ebx + 0x50], al
loc_2_013C:
        push   ebp
        push   ebp
        jae    0x1a1
loc_2_0140:
        add    byte ptr gs:[bx + si], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x16c  R-X
loc_3_0000:
        mov    dword ptr [0], 0              ; -> obj5:0x0000
        mov    dword ptr [4], 0              ; -> obj5:0x0004
        clc    
        ret    
loc_3_0016:
        push   8                             ; -> obj5:0x0008
        push   0x20                          ; -> obj5:0x0020
        push   0x80000002
        VMMCall  VMM._RegDeleteKey               ; cd204b010100
        VMMCall  VMM._RegCreateKey               ; cd204a010100
        add    esp, 0xc
        cmp    eax, 1
        cmc    
        jb     loc_3_003F
        call   sub_3_0040
loc_3_003F:
        ret    
sub_3_0040:
        push   esi
        push   edi
        push   ebx
        push   ecx
        push   edx
        movzx  ebx, word ptr [0xc]           ; -> loc_2_000C
        add    ebx, dword ptr [4]            ; -> loc_2_0004
        movzx  ecx, word ptr [0x1a]          ; -> obj2:0x001A
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        VxDCall  Dev_0048.Service_0000h          ; cd2000004800
        or     eax, eax
        je     loc_3_007E
        push   0xe74                         ; -> loc_4_0E74
        push   ecx
        push   ebx
        push   0
        push   0
        push   esp
        VxDCall  Dev_0048.Service_0001h          ; cd2001004800
        add    esp, 0x18
loc_3_007E:
        cmp    eax, 0
        je     loc_3_0164
        mov    dword ptr [0x148], eax        ; -> obj5:0x0148
        movzx  ebx, word ptr [0xe]           ; -> loc_2_000E
        add    ebx, dword ptr [4]            ; -> loc_2_0004
        movzx  ecx, word ptr [0x1c]          ; -> loc_2_001C
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        movzx  edx, word ptr [0x14]          ; -> loc_2_0014
        add    edx, dword ptr [4]            ; -> loc_2_0004
        push   0xed4                         ; -> loc_4_0ED4
        push   edx
        push   0
        push   ecx
        push   ebx
        push   1
        push   0
        push   esp
        push   eax
        VxDCall  Dev_0048.Service_0003h          ; cd2003004800
        add    esp, 0x24
        movzx  ebx, word ptr [0x10]          ; -> loc_2_0010
        add    ebx, dword ptr [4]            ; -> loc_2_0004
        movzx  ecx, word ptr [0x1e]          ; -> loc_2_001E
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        movzx  edx, word ptr [0x16]          ; -> loc_2_0016
        add    edx, dword ptr [4]            ; -> loc_2_0004
        mov    eax, dword ptr [0x148]        ; -> obj5:0x0148
        push   0xef0                         ; -> loc_4_0EF0
        push   edx
        push   0
        push   ecx
        push   ebx
        push   1
        push   0
        push   esp
        push   eax
        VxDCall  Dev_0048.Service_0003h          ; cd2003004800
        add    esp, 0x24
        mov    eax, dword ptr [0x148]        ; -> obj5:0x0148
        movzx  ebx, word ptr [0x12]          ; -> loc_2_0012
        add    ebx, dword ptr [4]            ; -> loc_2_0004
        movzx  ecx, word ptr [0x20]          ; -> loc_2_0020
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        movzx  edx, word ptr [0x18]          ; -> loc_2_0018
        add    edx, dword ptr [4]            ; -> loc_2_0004
        push   0xf0c                         ; -> loc_4_0F0C
        push   edx
        push   0
        push   ecx
        push   ebx
        push   1
        push   0
        push   esp
        push   eax
        VxDCall  Dev_0048.Service_0003h          ; cd2003004800
        add    esp, 0x24
        mov    dword ptr [0x14c], eax        ; -> obj5:0x014C
loc_3_0164:
        pop    edx
        pop    ecx
        pop    ebx
        pop    edi
        pop    esi
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xf88  R-X
loc_4_0000:
        mov    dword ptr [0x18], 1           ; -> obj5:0x0018
        clc    
        ret    
loc_4_000C:
        mov    dword ptr [0x14], 1           ; -> obj5:0x0014
        push   dword ptr [8]                 ; -> obj5:0x0008
        call   sub_4_0BA3
        pop    ecx
        push   dword ptr [4]                 ; -> obj5:0x0004
        push   dword ptr [0]                 ; -> obj5:0x0000
        call   sub_4_0480
        pop    ecx
        pop    ecx
        ret    
sub_4_0036:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   eax
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 8]
        mov    edx, dword ptr [edx + 8]
        or     edx, edx
        je     loc_4_005D
        push   eax
        cmp    ecx, 3
        je     loc_4_0056
        push   0x11
        jmp    loc_4_0058
loc_4_0056:
        push   0x12
loc_4_0058:
        call   edx
        add    esp, 8
loc_4_005D:
        pop    eax
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_4_0062:
        push   ebp
        mov    ebp, esp
        push   ecx
        mov    ecx, dword ptr [ebp + 8]
        test   byte ptr [ecx + 0x14], 1
        jne    loc_4_0076
        mov    eax, dword ptr [ecx + 0xc]
        mov    eax, dword ptr [eax]
        jmp    loc_4_007B
loc_4_0076:
        push   ecx
        call   dword ptr [ecx + 0xc]
        pop    ecx
loc_4_007B:
        test   dword ptr [ecx + 0x14], 0x40
        je     loc_4_008A
        mul    dword ptr [ecx + 0x18]
        clc    
        jmp    loc_4_009C
loc_4_008A:
        test   dword ptr [ecx + 0x14], 0x80
        je     loc_4_009C
        mov    edx, 0
        div    dword ptr [ecx + 0x18]
        clc    
loc_4_009C:
        pop    ecx
        pop    ebp
        ret    
PERF_Service_00:
        mov    eax, 0x400
        clc    
        ret    
PERF_Service_01:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 0xc]
        VMMCall  VMM._lstrlen                    ; cd207c010100
        pop    ecx
        add    eax, 0x1e
        push   0
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_4_015A
        mov    edi, eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 0x14], eax
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 4], 0
        lea    eax, [edi + 0x1c]
        push   -1
        push   dword ptr [esi + 0xc]
        push   eax
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        lea    eax, [edi + 0x10]
        push   eax
        push   dword ptr [esi + 0xc]
        push   dword ptr [8]                 ; -> obj5:0x0008
        VMMCall  VMM._RegCreateKey               ; cd204a010100
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_0156
        push   dword ptr [esi + 8]
        VMMCall  VMM._lstrlen                    ; cd207c010100
        pop    ecx
        inc    eax
        push   eax
        push   dword ptr [esi + 8]
        push   1
        push   0
        push   0x50                          ; -> obj5:0x0050
        push   dword ptr [edi + 0x10]
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_4_015A
        lea    eax, [edi + 0x1c]
        push   eax
        call   sub_4_0DE6
        pop    ecx
        mov    dword ptr [edi + 0x18], eax
        mov    eax, edi
        xchg   dword ptr [0], eax            ; -> obj5:0x0000
        mov    dword ptr [edi], eax
        mov    eax, edi
        inc    dword ptr [0x10]              ; -> obj5:0x0010
loc_4_0156:
        pop    edi
        pop    esi
        leave  
        ret    
loc_4_015A:
        sub    eax, eax
        jmp    loc_4_0156
PERF_Service_02:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cmp    dword ptr [0x18], 0           ; -> obj5:0x0018
        jne    loc_4_01FE
        lea    edi, [0]                      ; -> obj5:0x0000
        mov    esi, dword ptr [ebp + 8]
loc_4_0179:
        mov    edx, edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     loc_4_01FE
        cmp    edi, esi
        jne    loc_4_0179
        push   edx
        mov    dword ptr [0x1c], 0           ; -> obj5:0x001C
loc_4_0190:
        mov    eax, dword ptr [edi + 4]
        or     eax, eax
        je     loc_4_01A3
        push   eax
        VxDCall  Dev_0048.Service_0004h          ; cd2004004800
        add    esp, 4
        jmp    loc_4_0190
loc_4_01A3:
        mov    dword ptr [0x1c], 1           ; -> obj5:0x001C
        pop    esi
        mov    eax, dword ptr [edi]
        mov    dword ptr [esi], eax
        push   dword ptr [edi + 0x10]
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        lea    eax, [edi + 0x1c]
        push   eax
        call   sub_4_0DE6
        pop    ecx
        cmp    eax, 0
        jne    loc_4_01E0
        lea    eax, [edi + 0x1c]
        push   eax
        push   dword ptr [8]                 ; -> obj5:0x0008
        VMMCall  VMM._RegDeleteKey               ; cd204b010100
        add    esp, 8
loc_4_01E0:
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        push   dword ptr [4]                 ; -> obj5:0x0004
        push   dword ptr [0]                 ; -> obj5:0x0000
        call   sub_4_0480
        pop    ecx
        pop    ecx
loc_4_01FE:
        pop    edi
        pop    esi
        leave  
        ret    
PERF_Service_03:
        enter  8, 0
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 0x18]
        test   eax, eax
        je     loc_4_03D7
        push   dword ptr [esi + 0xc]
        VMMCall  VMM._lstrlen                    ; cd207c010100
        pop    ecx
        inc    eax
        mov    dword ptr [ebp - 4], eax
        add    eax, 0x21
        test   dword ptr [esi + 4], 0x100
        je     loc_4_0251
        push   esi
        mov    esi, dword ptr [esi + 0x18]
        mov    dword ptr [ebp - 8], 0
loc_4_023B:
        cmp    dword ptr [esi], 0
        je     loc_4_0249
        add    dword ptr [ebp - 8], 4
        add    esi, 4
        jmp    loc_4_023B
loc_4_0249:
        pop    esi
        add    dword ptr [ebp - 8], 4
        add    eax, dword ptr [ebp - 8]
loc_4_0251:
        push   0
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_4_03D7
        mov    edi, eax
        mov    dword ptr [ebp + 0xc], eax
        lea    eax, [edi + 0x20]
        push   -1
        push   dword ptr [esi + 0xc]
        push   eax
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        mov    dword ptr [edi + 0x1c], 0
        test   dword ptr [esi + 4], 0x100
        je     loc_4_02A4
        lea    eax, [edi + 0x20]
        add    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0x1c], eax
        push   dword ptr [ebp - 8]
        push   dword ptr [esi + 0x18]
        push   eax
        VMMCall  VMM._lmemcpy                    ; cd207d010100
        add    esp, 0xc
loc_4_02A4:
        lea    eax, [edi + 0x10]
        push   eax
        push   dword ptr [esi + 0xc]
        mov    eax, dword ptr [ebp + 8]
        push   dword ptr [eax + 0x10]
        VMMCall  VMM._RegCreateKey               ; cd204a010100
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_03D7
        push   dword ptr [esi + 8]
        VMMCall  VMM._lstrlen                    ; cd207c010100
        pop    ecx
        inc    eax
        push   eax
        push   dword ptr [esi + 8]
        push   1
        push   0
        push   0x5d                          ; -> obj5:0x005D
        push   dword ptr [edi + 0x10]
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        or     eax, eax
        jne    loc_4_03D4
        cmp    dword ptr [esi + 0x14], 0
        je     loc_4_0325
        push   dword ptr [esi + 0x14]
        VMMCall  VMM._lstrlen                    ; cd207c010100
        pop    ecx
        inc    eax
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp + 0xc], 1
        mov    dword ptr [esp + 4], 0x62     ; -> obj5:0x0062
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        or     eax, eax
        jne    loc_4_03D4
loc_4_0325:
        mov    eax, 0x81                     ; -> obj5:0x0081
        test   byte ptr [esi + 4], 2
        je     loc_4_0335
        mov    eax, 0x7c                     ; -> obj5:0x007C
loc_4_0335:
        mov    dword ptr [esp + 0x10], eax
        push   eax
        VMMCall  VMM._lstrlen                    ; cd207c010100
        pop    ecx
        inc    eax
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [esp + 4], 0x6e     ; -> obj5:0x006E
        VMMCall  VMM._RegSetValueEx              ; cd2052010100
        or     eax, eax
        jne    loc_4_03D4
        add    esp, 0x18
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [ecx + 4], edi
        mov    eax, edi
        xchg   dword ptr [4], eax            ; -> obj5:0x0004
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi + 0x14], eax
        sub    eax, eax
        test   dword ptr [esi + 4], 0x100
        jne    loc_4_0385
        mov    eax, dword ptr [esi + 0x18]
loc_4_0385:
        mov    dword ptr [edi + 0xc], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [edi + 0x18], eax
        test   dword ptr [edi + 0x14], 0xc0
        je     loc_4_03AA
        cmp    dword ptr [edi + 0x18], 0
        jne    loc_4_03AA
        and    dword ptr [edi + 0x14], 0xffffff3f
loc_4_03AA:
        mov    eax, edi
        inc    dword ptr [0xc]               ; -> obj5:0x000C
        cmp    dword ptr [0x14], 0           ; -> obj5:0x0014
        je     loc_4_03D0
        push   eax
        push   dword ptr [4]                 ; -> obj5:0x0004
        push   dword ptr [0]                 ; -> obj5:0x0000
        call   sub_4_0480
        pop    ecx
        pop    ecx
        pop    eax
loc_4_03D0:
        pop    edi
        pop    esi
        leave  
        ret    
loc_4_03D4:
        add    esp, 0x18
loc_4_03D7:
        sub    eax, eax
        jmp    loc_4_03D0
PERF_Service_04:
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [0x18], 0           ; -> obj5:0x0018
        jne    loc_4_045E
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 8]
loc_4_03EF:
        mov    ecx, edx
        mov    edx, dword ptr [edx + 4]
        or     edx, edx
        je     loc_4_0462
        cmp    edx, esi
        jne    loc_4_03EF
        lea    edx, [4]                      ; -> obj5:0x0004
loc_4_0402:
        mov    ebx, edx
        mov    edx, dword ptr [edx]
        or     edx, edx
        je     loc_4_0462
        cmp    edx, esi
        jne    loc_4_0402
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [ebx], eax
        push   dword ptr [esi + 0x10]
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        lea    eax, [esi + 0x20]
        push   eax
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [eax + 0x10]
        VMMCall  VMM._RegDeleteKey               ; cd204b010100
        add    esp, 8
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        cmp    dword ptr [0x1c], 1           ; -> obj5:0x001C
        jne    loc_4_045E
        push   dword ptr [4]                 ; -> obj5:0x0004
        push   dword ptr [0]                 ; -> obj5:0x0000
        call   sub_4_0480
        pop    ecx
        pop    ecx
loc_4_045E:
        pop    ebx
        pop    esi
        leave  
        ret    
loc_4_0462:
        mov    eax, 6
        stc    
        jmp    loc_4_045E
        db  0C3h
loc_4_046B:
        push   edx
        call   sub_4_0A68
        pop    ecx
        ret    
loc_4_0473:
        push   edx
        call   sub_4_0AA1
        pop    ecx
        ret    
        db  000h, 000h, 000h, 000h, 000h
sub_4_0480:
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x40]
        mov    dword ptr [esp + 0x18], 0
        mov    dword ptr [esp + 0x10], 0
        call   sub_4_081E
        push   0
        mov    eax, dword ptr [0xc]          ; -> obj5:0x000C
        shl    eax, 4
        push   eax
        call   sub_1_00A8
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 8
        push   0
        mov    eax, dword ptr [0xc]          ; -> obj5:0x000C
        imul   eax, eax, 0x66
        push   eax
        call   sub_1_00A8
        mov    dword ptr [esp + 0x28], eax
        add    esp, 8
        cmp    dword ptr [esp + 0x14], 0
        je     loc_4_07FF
        or     eax, eax
        je     loc_4_07EA
        mov    ebp, eax
        or     esi, esi
        je     loc_4_0542
        mov    ebx, dword ptr [esp + 0x14]
loc_4_04EB:
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [0xc], eax          ; -> obj5:0x000C
        jbe    loc_4_0542
        mov    eax, dword ptr [esi + 8]
        or     eax, eax
        je     loc_4_0814
        push   ebp
        lea    ecx, [esi + 0x20]
        push   ecx
        add    eax, 0x1c
        push   eax
        call   sub_4_0AD9
        add    esp, 0xc
        mov    dword ptr [ebx], ebp
        mov    dword ptr [ebx + 4], 4
        mov    dword ptr [ebx + 8], esi
        mov    dword ptr [ebx + 0xc], 3
        mov    edi, ebp
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ebp, ecx
        add    ebx, 0x10
        inc    dword ptr [esp + 0x18]
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_4_04EB
loc_4_0542:
        mov    dword ptr [esp + 0x24], 0x91e ; -> loc_4_091E
        mov    dword ptr [esp + 0x28], 0
        mov    dword ptr [esp + 0x2c], 0x91e ; -> loc_4_091E
        mov    dword ptr [esp + 0x30], 0
        sub    esi, esi
        mov    dword ptr [esp + 0x34], 0
        push   0x90                          ; -> obj5:0x0090
        push   dword ptr [esp + 0x1c]
        lea    eax, [esp + 0x2c]
        push   dword ptr [esp + 0x1c]
        push   eax
        push   1
        push   0xa8                          ; -> obj5:0x00A8
        call   sub_1_00A0
        add    esp, 0x18
        cmp    eax, esi
        jne    loc_4_05E0
        push   0x94                          ; -> obj5:0x0094
        push   dword ptr [esp + 0x1c]
        lea    eax, [esp + 0x2c]
        push   dword ptr [esp + 0x1c]
        push   eax
        push   2
        push   0xbc                          ; -> obj5:0x00BC
        call   sub_1_00A0
        add    esp, 0x18
        cmp    eax, esi
        jne    loc_4_05E0
        push   0x98                          ; -> obj5:0x0098
        push   dword ptr [esp + 0x1c]
        lea    eax, [esp + 0x2c]
        push   dword ptr [esp + 0x1c]
        push   eax
        push   3
        push   0xd0                          ; -> obj5:0x00D0
        call   sub_1_00A0
        add    esp, 0x18
        cmp    eax, esi
        jne    loc_4_05E0
        mov    esi, 1
loc_4_05E0:
        push   0
        push   dword ptr [esp + 0x18]
        call   sub_1_00B0
        add    esp, 8
        push   0
        push   dword ptr [esp + 0x24]
        call   sub_1_00B0
        add    esp, 8
        or     esi, esi
        jne    loc_4_060A
        call   sub_4_081E
        jmp    loc_4_0814
loc_4_060A:
        mov    eax, dword ptr [0x10]         ; -> obj5:0x0010
        add    eax, dword ptr [0x8c]         ; -> obj5:0x008C
        mov    dword ptr [esp + 0x1c], eax
        push   0
        shl    eax, 4
        push   eax
        call   sub_1_00A8
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 8
        mov    eax, dword ptr [esp + 0x1c]
        push   0
        imul   eax, eax, 0x66
        push   eax
        call   sub_1_00A8
        mov    dword ptr [esp + 0x28], eax
        add    esp, 8
        cmp    dword ptr [esp + 0x14], 0
        je     loc_4_07CE
        or     eax, eax
        je     loc_4_07B9
        mov    ebp, dword ptr [esp + 0x3c]
        mov    ebx, eax
        or     ebp, ebp
        je     loc_4_06C1
        mov    edx, dword ptr [esp + 0x14]
loc_4_0662:
        mov    eax, dword ptr [esp + 0x10]
        cmp    eax, dword ptr [esp + 0x1c]
        jae    loc_4_06C1
        mov    dword ptr [edx], ebx
        mov    dword ptr [edx + 4], 4
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [edx + 8], eax
        lea    edi, [ebp + 0x1c]
        mov    dword ptr [edx + 0xc], 3
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ebx, ecx
        add    edx, 0x10
        inc    dword ptr [esp + 0x10]
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    loc_4_0662
loc_4_06C1:
        mov    edx, dword ptr [0xa4]         ; -> obj5:0x00A4
        or     edx, edx
        je     loc_4_073A
        mov    ecx, dword ptr [esp + 0x10]
        shl    ecx, 4
        mov    ebp, dword ptr [esp + 0x14]
        add    ebp, ecx
loc_4_06D8:
        mov    eax, dword ptr [esp + 0x10]
        cmp    eax, dword ptr [esp + 0x1c]
        jae    loc_4_073A
        cmp    dword ptr [edx + 0xc], 0
        jne    loc_4_0734
        mov    dword ptr [ebp], ebx
        mov    dword ptr [ebp + 4], 4
        mov    dword ptr [ebp + 8], edx
        mov    dword ptr [ebp + 0xc], 3
        mov    ecx, 0xffffffff
        mov    edi, dword ptr [edx + 8]
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ebx, ecx
        add    ebp, 0x10
        inc    dword ptr [esp + 0x10]
loc_4_0734:
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    loc_4_06D8
loc_4_073A:
        push   0x9c                          ; -> obj5:0x009C
        sub    esi, esi
        push   dword ptr [esp + 0x14]
        lea    eax, [esp + 0x2c]
        push   dword ptr [esp + 0x1c]
        push   eax
        push   4
        push   0xe4                          ; -> obj5:0x00E4
        call   sub_1_00A0
        add    esp, 0x18
        cmp    eax, esi
        jne    loc_4_078B
        push   0xa0                          ; -> obj5:0x00A0
        push   dword ptr [esp + 0x14]
        lea    eax, [esp + 0x2c]
        push   dword ptr [esp + 0x1c]
        push   eax
        push   5
        push   0xf8                          ; -> obj5:0x00F8
        call   sub_1_00A0
        add    esp, 0x18
        cmp    eax, esi
        jne    loc_4_078B
        mov    esi, 1
loc_4_078B:
        push   0
        push   dword ptr [esp + 0x18]
        call   sub_1_00B0
        add    esp, 8
        push   0
        push   dword ptr [esp + 0x24]
        call   sub_1_00B0
        add    esp, 8
        or     esi, esi
        jne    loc_4_07B2
        call   sub_4_081E
        jmp    loc_4_0814
loc_4_07B2:
        mov    eax, 1
        jmp    loc_4_0816
loc_4_07B9:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_4_07CE
        push   0
        push   dword ptr [esp + 0x18]
        call   sub_1_00B0
        add    esp, 8
loc_4_07CE:
        cmp    dword ptr [esp + 0x20], 0
        je     loc_4_07E3
        push   0
        push   dword ptr [esp + 0x24]
        call   sub_1_00B0
        add    esp, 8
loc_4_07E3:
        call   sub_4_081E
        jmp    loc_4_0814
loc_4_07EA:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_4_07FF
        push   0
        push   dword ptr [esp + 0x18]
        call   sub_1_00B0
        add    esp, 8
loc_4_07FF:
        cmp    dword ptr [esp + 0x20], 0
        je     loc_4_0814
        push   0
        push   dword ptr [esp + 0x24]
        call   sub_1_00B0
        add    esp, 8
loc_4_0814:
        sub    eax, eax
loc_4_0816:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    
sub_4_081E:
        cmp    dword ptr [0x90], 0           ; -> obj5:0x0090
        je     loc_4_0851
        push   dword ptr [0x90]              ; -> obj5:0x0090
        call   sub_1_0070
        add    esp, 4
        push   0xa8                          ; -> obj5:0x00A8
        push   0x80000006
        call   sub_1_0080
        add    esp, 8
        mov    dword ptr [0x90], 0           ; -> obj5:0x0090
loc_4_0851:
        cmp    dword ptr [0x94], 0           ; -> obj5:0x0094
        je     loc_4_0884
        push   dword ptr [0x94]              ; -> obj5:0x0094
        call   sub_1_0070
        add    esp, 4
        push   0xbc                          ; -> obj5:0x00BC
        push   0x80000006
        call   sub_1_0080
        add    esp, 8
        mov    dword ptr [0x94], 0           ; -> obj5:0x0094
loc_4_0884:
        cmp    dword ptr [0x98], 0           ; -> obj5:0x0098
        je     loc_4_08B7
        push   dword ptr [0x98]              ; -> obj5:0x0098
        call   sub_1_0070
        add    esp, 4
        push   0xd0                          ; -> obj5:0x00D0
        push   0x80000006
        call   sub_1_0080
        add    esp, 8
        mov    dword ptr [0x98], 0           ; -> obj5:0x0098
loc_4_08B7:
        cmp    dword ptr [0x9c], 0           ; -> obj5:0x009C
        je     loc_4_08EA
        push   dword ptr [0x9c]              ; -> obj5:0x009C
        call   sub_1_0070
        add    esp, 4
        push   0xe4                          ; -> obj5:0x00E4
        push   0x80000006
        call   sub_1_0080
        add    esp, 8
        mov    dword ptr [0x9c], 0           ; -> obj5:0x009C
loc_4_08EA:
        cmp    dword ptr [0xa0], 0           ; -> obj5:0x00A0
        je     loc_4_091D
        push   dword ptr [0xa0]              ; -> obj5:0x00A0
        call   sub_1_0070
        add    esp, 4
        push   0xf8                          ; -> obj5:0x00F8
        push   0x80000006
        call   sub_1_0080
        add    esp, 8
        mov    dword ptr [0xa0], 0           ; -> obj5:0x00A0
loc_4_091D:
        ret    
loc_4_091E:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, esi
        cmp    eax, 1
        je     loc_4_0955
        cmp    eax, 2
        jb     loc_4_0A64
        cmp    eax, 3
        jbe    loc_4_09C3
        cmp    eax, 4
        jb     loc_4_0A64
        cmp    eax, 5
        jbe    loc_4_0A1A
        jmp    loc_4_0A64
loc_4_0955:
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [ecx], 4
        jae    loc_4_096F
        mov    eax, 0xea
        jmp    loc_4_0A64
loc_4_096F:
        mov    dword ptr [ecx], 4
        test   byte ptr [eax + 0x15], 1
        jne    loc_4_098F
        push   eax
        call   sub_4_0062
        mov    edi, dword ptr [esp + 0x20]
        add    esp, 4
        mov    dword ptr [edi], eax
        jmp    loc_4_0A62
loc_4_098F:
        mov    edi, dword ptr [esp + 0x1c]
        mov    dword ptr [edi], 0
        mov    esi, dword ptr [eax + 0x1c]
        or     esi, esi
        je     loc_4_0A62
loc_4_09A4:
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_4_0A62
        push   eax
        call   sub_4_0062
        add    esp, 4
        add    dword ptr [edi], eax
        add    esi, 4
        jne    loc_4_09A4
        jmp    loc_4_0A62
loc_4_09C3:
        mov    eax, dword ptr [esp + 0x14]
        mov    edi, dword ptr [eax + 4]
        test   byte ptr [edi + 0x15], 1
        jne    loc_4_09DC
        push   esi
        push   edi
        call   sub_4_0036
        add    esp, 8
        jmp    loc_4_09F8
loc_4_09DC:
        mov    ebx, dword ptr [edi + 0x1c]
        or     ebx, ebx
        je     loc_4_09F8
loc_4_09E3:
        mov    eax, dword ptr [ebx]
        or     eax, eax
        je     loc_4_09F8
        push   esi
        push   eax
        call   sub_4_0036
        add    esp, 8
        add    ebx, 4
        jne    loc_4_09E3
loc_4_09F8:
        mov    eax, dword ptr [edi + 8]
        mov    eax, dword ptr [eax + 0x18]
        or     eax, eax
        je     loc_4_0A62
        push   eax
        mov    eax, 0x46b                    ; -> loc_4_046B
        cmp    esi, 2
        je     loc_4_0A12
        mov    eax, 0x473                    ; -> loc_4_0473
loc_4_0A12:
        push   eax
        call   sub_1_00B8
        jmp    loc_4_0A62
loc_4_0A1A:
        mov    ecx, dword ptr [esp + 0x20]
        cmp    dword ptr [ecx], 4
        jae    loc_4_0A2A
        mov    eax, 0xea
        jmp    loc_4_0A64
loc_4_0A2A:
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ecx], 4
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        jne    loc_4_0A3D
        jmp    loc_4_0A58
loc_4_0A3D:
        cmp    esi, 4
        push   eax
        jne    loc_4_0A4A
        push   0x46b                         ; -> loc_4_046B
        jmp    loc_4_0A4F
loc_4_0A4A:
        push   0x473                         ; -> loc_4_0473
loc_4_0A4F:
        call   sub_1_00B8
        or     eax, eax
        je     loc_4_0A62
loc_4_0A58:
        mov    edi, dword ptr [esp + 0x1c]
        mov    dword ptr [edi], 1
loc_4_0A62:
        sub    eax, eax
loc_4_0A64:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_4_0A68:
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   1
        inc    dword ptr [esi + 0xc]
        lea    eax, [esp + 8]
        push   dword ptr [esi + 4]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_1_0030
        or     eax, eax
        jne    loc_4_0A99
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 9]
        mov    word ptr [esi + 0x10], ax
        jmp    loc_4_0A9C
loc_4_0A99:
        dec    dword ptr [esi + 0xc]
loc_4_0A9C:
        pop    esi
        add    esp, 8
        ret    
sub_4_0AA1:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ecx, dword ptr [esi + 0xc]
        or     ecx, ecx
        je     loc_4_0AD7
        mov    ax, word ptr [esi + 0x10]
        or     ax, ax
        je     loc_4_0AD7
        dec    ecx
        mov    dword ptr [esi + 0xc], ecx
        push   0
        push   eax
        call   sub_1_0054
        or     eax, eax
        jne    loc_4_0AD4
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_4_0AD7
        mov    word ptr [esi + 0x10], 0
        jmp    loc_4_0AD7
loc_4_0AD4:
        inc    dword ptr [esi + 0xc]
loc_4_0AD7:
        pop    esi
        ret    
sub_4_0AD9:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x14]
        or     edi, edi
        je     loc_4_0B76
        mov    ebx, dword ptr [esp + 0x18]
        or     ebx, ebx
        mov    edx, dword ptr [esp + 0x1c]
        je     loc_4_0B7A
        or     edx, edx
        je     loc_4_0B7A
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
        mov    edi, 0x140                    ; -> obj5:0x0140
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    ebp, ecx
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    ebx, ecx
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        jmp    loc_4_0B99
loc_4_0B76:
        mov    edx, dword ptr [esp + 0x1c]
loc_4_0B7A:
        mov    edi, 0x144                    ; -> obj5:0x0144
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
loc_4_0B99:
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_4_0BA3:
        sub    esp, 0x218
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        lea    eax, [esp + 0x1c]
        push   eax
        push   0x10c                         ; -> obj5:0x010C
        push   0x80000002
        call   sub_1_0068
        add    esp, 0xc
        cmp    eax, edi
        jne    loc_4_0D11
        lea    eax, [esp + 0x20]
        push   0xff
        push   eax
        push   edi
        push   dword ptr [esp + 0x28]
        call   sub_1_0088
        add    esp, 0x10
        cmp    eax, edi
        jne    loc_4_0D05
        mov    esi, dword ptr [esp + 0x22c]
        sub    ebx, ebx
        mov    ebp, 0x105
loc_4_0BFB:
        mov    dword ptr [esp + 0x14], ebx
        mov    dword ptr [esp + 0x10], ebx
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        push   esi
        call   sub_1_0078
        add    esp, 0xc
        cmp    eax, ebx
        jne    loc_4_0CC1
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x24]
        call   sub_1_0068
        add    esp, 0xc
        cmp    eax, ebx
        jne    loc_4_0CC1
        mov    dword ptr [esp + 0x18], ebp
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x124]
        push   eax
        push   ebx
        push   ebx
        push   0x50                          ; -> obj5:0x0050
        push   dword ptr [esp + 0x24]
        call   sub_1_0090
        add    esp, 0x18
        cmp    eax, ebx
        jne    loc_4_0C84
        lea    eax, [esp + 0x120]
        push   dword ptr [esp + 0x18]
        push   eax
        push   1
        push   ebx
        push   0x50                          ; -> obj5:0x0050
        push   dword ptr [esp + 0x28]
        call   sub_1_0098
        add    esp, 0x18
loc_4_0C84:
        mov    dword ptr [esp + 0x18], ebp
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x124]
        push   eax
        push   ebx
        push   ebx
        push   0x55                          ; -> obj5:0x0055
        push   dword ptr [esp + 0x24]
        call   sub_1_0090
        add    esp, 0x18
        cmp    eax, ebx
        jne    loc_4_0CC1
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x124]
        push   eax
        call   sub_4_0D1C
        add    esp, 8
loc_4_0CC1:
        cmp    dword ptr [esp + 0x10], ebx
        je     loc_4_0CD3
        push   dword ptr [esp + 0x10]
        call   sub_1_0070
        add    esp, 4
loc_4_0CD3:
        cmp    dword ptr [esp + 0x14], ebx
        je     loc_4_0CE5
        push   dword ptr [esp + 0x14]
        call   sub_1_0070
        add    esp, 4
loc_4_0CE5:
        inc    edi
        lea    eax, [esp + 0x20]
        push   0xff
        push   eax
        push   edi
        push   dword ptr [esp + 0x28]
        call   sub_1_0088
        add    esp, 0x10
        cmp    eax, ebx
        je     loc_4_0BFB
loc_4_0D05:
        push   dword ptr [esp + 0x1c]
        call   sub_1_0070
        add    esp, 4
loc_4_0D11:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x218
        ret    
sub_4_0D1C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    ebx, dword ptr [esp + 0x14]
        lea    edx, [ecx - 1]
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [edx + ecx + 0x13]
        push   0
        push   eax
        call   sub_1_00A8
        add    esp, 8
        mov    edx, eax
        or     edx, edx
        mov    eax, 0
        je     loc_4_0DE1
        mov    eax, dword ptr [0xa4]         ; -> obj5:0x00A4
        mov    dword ptr [edx], eax
        mov    dword ptr [0xa4], edx         ; -> obj5:0x00A4
        lea    ebp, [edx + 0x12]
        mov    dword ptr [edx + 4], ebp
        mov    edi, ebx
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
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ebp, ecx
        mov    dword ptr [edx + 8], ebp
        mov    edi, dword ptr [esp + 0x18]
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
        mov    dword ptr [edx + 0xc], 0
        mov    eax, dword ptr [0x8c]         ; -> obj5:0x008C
        inc    eax
        mov    dword ptr [0x8c], eax         ; -> obj5:0x008C
        mov    eax, 1
loc_4_0DE1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_4_0DE6:
        push   ebx
        push   esi
        mov    eax, dword ptr [0xa4]         ; -> obj5:0x00A4
        or     eax, eax
        je     loc_4_0E29
        mov    esi, dword ptr [esp + 0xc]
loc_4_0DF5:
        mov    ecx, esi
        mov    edx, dword ptr [eax + 8]
loc_4_0DFA:
        mov    bl, byte ptr [ecx]
        cmp    bl, byte ptr [edx]
        jne    loc_4_0E1A
        or     bl, bl
        je     loc_4_0E16
        mov    bl, byte ptr [ecx + 1]
        cmp    bl, byte ptr [edx + 1]
        jne    loc_4_0E1A
        add    ecx, 2
        add    edx, 2
        or     bl, bl
        jne    loc_4_0DFA
loc_4_0E16:
        sub    ecx, ecx
        jmp    loc_4_0E1F
loc_4_0E1A:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_4_0E1F:
        or     ecx, ecx
        je     loc_4_0E2B
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_4_0DF5
loc_4_0E29:
        sub    eax, eax
loc_4_0E2B:
        pop    esi
        pop    ebx
        ret    
sub_4_0E2E:
        mov    ecx, dword ptr [esp + 8]
        or     ecx, ecx
        mov    eax, 0
        je     loc_4_0E49
        mov    eax, dword ptr [esp + 4]
        imul   eax, eax, 0x3e8
        sub    edx, edx
        div    ecx
loc_4_0E49:
        ret    
sub_4_0E4A:
        push   esi
        mov    ecx, dword ptr [esp + 0xc]
        or     ecx, ecx
        je     loc_4_0E6D
        mov    eax, dword ptr [esp + 8]
        cmp    ecx, eax
        jb     loc_4_0E6D
        mov    esi, 0x64
        imul   eax, esi
        sub    edx, edx
        div    ecx
        sub    esi, eax
        mov    eax, esi
        jmp    loc_4_0E72
loc_4_0E6D:
        mov    eax, 0x64
loc_4_0E72:
        pop    esi
        ret    
loc_4_0E74:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [0x14c]        ; -> obj5:0x014C
        cmp    dword ptr [ebp + 0xc], eax
        jne    loc_4_0EB4
        cmp    dword ptr [ebp + 8], 0x11
        jne    loc_4_0EA7
        cmp    dword ptr [0x150], 0          ; -> obj5:0x0150
        jne    loc_4_0E9F
        mov    dword ptr [0x15c], 0          ; -> obj5:0x015C
        call   sub_4_0F5B
loc_4_0E9F:
        inc    dword ptr [0x150]             ; -> obj5:0x0150
        jmp    loc_4_0EB4
loc_4_0EA7:
        dec    dword ptr [0x150]             ; -> obj5:0x0150
        jne    loc_4_0EB4
        call   sub_4_0F72
loc_4_0EB4:
        leave  
        ret    
        db  056h, 057h, 0A3h
        dd  obj5:0x0148              ; fixup
        db  0A1h
        dd  obj5:0x0148              ; fixup
        db  083h, 0F8h, 000h, 074h, 00Ah, 050h, 0CDh, 020h, 002h, 000h, 048h, 000h, 083h, 0C4h, 004h, 05Fh
        db  05Eh, 0C3h
loc_4_0ED4:
        push   esi
        push   edi
        xor    eax, eax
        VMMCall  VMM.Get_Sys_Thread_Handle       ; cd200a010100
loc_4_0EDE:
        inc    eax
        VMMCall  VMM.Get_Next_Thread_Handle      ; cd2013010100
        VMMCall  VMM.Test_Sys_Thread_Handle      ; cd200b010100
        jne    loc_4_0EDE
        pop    edi
        pop    esi
        ret    
loc_4_0EF0:
        push   esi
        push   edi
        xor    eax, eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
loc_4_0EFA:
        inc    eax
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_4_0EFA
        pop    edi
        pop    esi
        ret    
loc_4_0F0C:
        push   esi
        push   edi
        push   ebx
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    ecx, eax
        mov    ebx, dword ptr [0x15c]        ; -> obj5:0x015C
        sub    eax, dword ptr [0x158]        ; -> obj5:0x0158
        mov    dword ptr [0x158], ecx        ; -> obj5:0x0158
        mov    dword ptr [0x15c], 0          ; -> obj5:0x015C
        push   eax
        push   ebx
        call   sub_4_0E2E
        pop    ecx
        pop    ecx
        cmp    eax, dword ptr [0x154]        ; -> obj5:0x0154
        jle    loc_4_0F49
        mov    dword ptr [0x154], eax        ; -> obj5:0x0154
loc_4_0F49:
        push   dword ptr [0x154]             ; -> obj5:0x0154
        push   eax
        call   sub_4_0E4A
        pop    ecx
        pop    ecx
        pop    ebx
        pop    edi
        pop    esi
        ret    
sub_4_0F5B:
        mov    esi, 0xf7e                    ; -> loc_4_0F7E
        VMMCall  VMM.Call_When_Idle              ; cd203a000100
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [0x158], eax        ; -> obj5:0x0158
        ret    
sub_4_0F72:
        mov    esi, 0xf7e                    ; -> loc_4_0F7E
        VMMCall  VMM.Cancel_Call_When_Idle       ; cd202b010100
        ret    
loc_4_0F7E:
        inc    dword ptr [0x15c]             ; -> obj5:0x015C
        clc    
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x160  RW-
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  053h, 079h, 073h, 074h, 065h, 06Dh, 05Ch, 043h, 075h, 072h, 072h, 065h, 06Eh, 074h, 043h, 06Fh
        db  06Eh, 074h, 072h, 06Fh, 06Ch, 053h, 065h, 074h, 05Ch, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch
        db  05Ch, 050h, 065h, 072h, 066h, 053h, 074h, 061h, 074h, 073h, 05Ch, 045h, 06Eh, 075h, 06Dh, 000h
        db  04Eh, 061h, 06Dh, 065h, 000h, 056h, 078h, 044h, 04Eh, 061h, 06Dh, 065h, 000h, 04Eh, 061h, 06Dh
        db  065h, 000h, 044h, 065h, 073h, 063h, 072h, 069h, 070h, 074h, 069h, 06Fh, 06Eh, 000h, 044h, 069h
        db  066h, 066h, 065h, 072h, 065h, 06Eh, 074h, 069h, 061h, 074h, 065h, 000h, 054h, 052h, 055h, 045h
        db  000h, 046h, 041h, 04Ch, 053h, 045h, 000h, 053h, 054h, 041h, 054h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 050h, 065h, 072h, 066h, 053h, 074h, 061h, 074h
        db  073h, 05Ch, 053h, 074h, 061h, 074h, 044h, 061h, 074h, 061h, 000h, 000h, 050h, 065h, 072h, 066h
        db  053h, 074h, 061h, 074h, 073h, 05Ch, 053h, 074h, 061h, 072h, 074h, 053h, 074h, 061h, 074h, 000h
        db  050h, 065h, 072h, 066h, 053h, 074h, 061h, 074h, 073h, 05Ch, 053h, 074h, 06Fh, 070h, 053h, 074h
        db  061h, 074h, 000h, 000h, 050h, 065h, 072h, 066h, 053h, 074h, 061h, 074h, 073h, 05Ch, 053h, 074h
        db  061h, 072h, 074h, 053h, 072h, 076h, 000h, 000h, 050h, 065h, 072h, 066h, 053h, 074h, 061h, 074h
        db  073h, 05Ch, 053h, 074h, 06Fh, 070h, 053h, 072h, 076h, 000h, 000h, 000h, 053h, 079h, 073h, 074h
        db  065h, 06Dh, 05Ch, 043h, 075h, 072h, 072h, 065h, 06Eh, 074h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh
        db  06Ch, 053h, 065h, 074h, 05Ch, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 05Ch, 050h, 065h, 072h
        db  066h, 053h, 074h, 061h, 074h, 073h, 05Ch, 04Fh, 074h, 068h, 065h, 072h, 000h, 000h, 000h, 000h
        db  05Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x18  R-X
        db  033h, 0F6h, 0F7h, 0C3h, 003h, 000h, 075h, 009h, 033h, 0DBh, 066h, 033h, 0D2h, 0B8h, 000h, 000h
        db  0C3h, 033h, 0DBh, 0B8h, 001h, 080h, 0C3h, 021h
VxD__LTEXT ENDS
