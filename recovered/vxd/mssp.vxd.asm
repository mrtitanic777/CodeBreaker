; SECPROV -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 3740 instructions recovered by recursive descent

Declare_Virtual_Device SECPROV, 4, 0, SECPROV_Control, 00493h, Undefined_Init_Order
;   DDK version 0400h   init_order 0A002FF00h   flags 00000h
;   service table: obj1:0x1a4, 7 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x264  R-X
SECPROV_Control:
        sub    eax, 1
        cmp    eax, 0x23
        jae    loc_1_000F
        jmp    dword ptr [eax*4 + 0x11]      ; -> obj1:0x0011
loc_1_000F:
        clc    
        ret    
        ; ---- dispatch table (35 entries) ----
        dd  loc_2_0000
        dd  loc_2_000B
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
        dd  loc_3_0007
loc_1_009D:
        push   dword ptr [esp]
        mov    dword ptr [esp + 4], edx
        jmp    sub_3_113F
loc_1_00A9:
        push   dword ptr [esp]
        mov    dword ptr [esp + 4], edx
        jmp    sub_3_11A4
sub_1_00B5:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 0x10]
        VMMCall  VMM.Schedule_Thread_Event       ; cd20fb000100
        mov    eax, esi
        pop    esi
        pop    edi
        leave  
        ret    0xc
sub_1_00D1:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp + 0x1c]
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    eax, esi
        pop    esi
        pop    edi
        pop    ebx
        leave  
        ret    0x18
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0100:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    ebp, 1
        mov    edi, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [esp + 0x1c]
loc_1_0115:
        or     edi, edi
        jne    loc_1_0143
        cmp    dword ptr [esi + 4], ebp
        jb     loc_1_0139
        or     ebx, ebx
        je     loc_1_0154
        mov    eax, esi
        sub    eax, ebx
        cmp    eax, -0x10
        je     loc_1_0139
        lea    eax, [esi + 0x10]
        push   eax
        push   ebx
        call   sub_1_016B
        or     eax, eax
        jne    loc_1_0154
loc_1_0139:
        or     edi, edi
        jne    loc_1_0143
        cmp    dword ptr [esi + 4], 2
        jae    loc_1_015B
loc_1_0143:
        cmp    dword ptr [esi], 0
        je     loc_1_0162
        push   ebp
        push   esi
        call   sub_1_019C
        add    esp, 8
        jmp    loc_1_0115
loc_1_0154:
        mov    eax, 1
        jmp    loc_1_0164
loc_1_015B:
        mov    eax, 2
        jmp    loc_1_0164
loc_1_0162:
        sub    eax, eax
loc_1_0164:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0xc
sub_1_016B:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
loc_1_0173:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    loc_1_0193
        or     dl, dl
        je     loc_1_018F
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    loc_1_0193
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    loc_1_0173
loc_1_018F:
        sub    eax, eax
        jmp    loc_1_0198
loc_1_0193:
        sbb    eax, eax
        sbb    eax, -1
loc_1_0198:
        ret    8
        db  000h
sub_1_019C:
        VMMCall  VMM._BlockOnID                  ; cd20fa800100
        add    byte ptr [eax], al
        dd  SECPROV_Service_00       ; fixup
        dd  SECPROV_Service_01       ; fixup
        dd  SECPROV_Service_02       ; fixup
        dd  SECPROV_Service_03       ; fixup
        dd  SECPROV_Service_04       ; fixup
        dd  SECPROV_Service_05       ; fixup
        dd  SECPROV_Service_06       ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00493h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'SECPROV '                  ; DDB_Name
        dd  0A002FF00h                  ; DDB_Init_Order
        dd  SECPROV_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x01A4              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x3dc  R-X
loc_2_0000:
        call   sub_2_0235
        or     eax, eax
        je     loc_2_000A
        stc    
loc_2_000A:
        ret    
loc_2_000B:
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jae    loc_2_0015
        jmp    loc_2_004F
loc_2_0015:
        VxDCall  Dev_0491.Service_0000h          ; cd2000009104
        jae    loc_2_001F
        jmp    loc_2_004F
loc_2_001F:
        VxDCall  NDIS2SUP.Service_0000h          ; cd2000008004
        jae    loc_2_0029
        jmp    loc_2_004F
loc_2_0029:
        VxDCall  Dev_0481.Service_0000h          ; cd2000008104
        jae    loc_2_0033
        jmp    loc_2_004F
loc_2_0033:
        cmp    word ptr [0x248], 0           ; -> obj1:0x0248
        je     loc_2_0046
        call   sub_2_0299
        or     eax, eax
        jne    loc_2_004F
loc_2_0046:
        call   sub_2_0173
        or     eax, eax
        je     loc_2_0055
loc_2_004F:
        call   sub_2_024A
        stc    
loc_2_0055:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0060:
        sub    esp, 0xc
        push   esi
        lea    eax, [esp + 8]
        push   eax
        push   0x388                         ; -> loc_2_0388
        push   0x80000002
        call   sub_2_02B0
        add    esp, 0xc
        or     eax, eax
        jne    loc_2_016E
        mov    dword ptr [esp + 0xc], 4
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 8]
        push   0x24c                         ; -> obj1:0x024C
        push   eax
        push   0
        push   0x39c                         ; -> obj2:0x039C
        push   dword ptr [esp + 0x1c]
        call   sub_2_02C0
        add    esp, 0x18
        mov    esi, eax
        or     esi, esi
        jne    loc_2_0160
        cmp    dword ptr [esp + 4], 4
        je     loc_2_00D4
        cmp    dword ptr [esp + 4], 3
        jne    loc_2_0157
        cmp    dword ptr [esp + 0xc], 4
        jne    loc_2_0157
loc_2_00D4:
        mov    eax, dword ptr [0x24c]        ; -> obj1:0x024C
        cmp    eax, 1
        jb     loc_2_015B
        cmp    eax, 2
        jbe    loc_2_00E5
        jmp    loc_2_015B
loc_2_00E5:
        mov    eax, dword ptr [0x24c]        ; -> obj1:0x024C
        sub    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [0x248], eax        ; -> obj1:0x0248
        push   0x900                         ; -> loc_4_0900
        lea    eax, [esp + 8]
        push   0x254                         ; -> obj1:0x0254
        push   eax
        push   0
        push   0x3ac                         ; -> obj2:0x03AC
        push   dword ptr [esp + 0x1c]
        call   sub_2_02C0
        add    esp, 0x18
        mov    esi, eax
        or     esi, esi
        jne    loc_2_0160
        cmp    dword ptr [esp + 4], 1
        jne    loc_2_0157
        push   0xa08                         ; -> loc_4_0A08
        lea    eax, [esp + 8]
        push   0x904                         ; -> loc_4_0904
        push   eax
        push   0
        push   0x3b8                         ; -> loc_2_03B8
        push   dword ptr [esp + 0x1c]
        call   sub_2_02C0
        add    esp, 0x18
        mov    esi, eax
        or     esi, esi
        jne    loc_2_0160
        cmp    dword ptr [esp + 4], 1
        je     loc_2_0160
loc_2_0157:
        or     esi, esi
        jne    loc_2_0160
loc_2_015B:
        mov    esi, 0xffffffff
loc_2_0160:
        push   dword ptr [esp + 8]
        call   sub_2_02B8
        add    esp, 4
        mov    eax, esi
loc_2_016E:
        pop    esi
        add    esp, 0xc
        ret    
sub_2_0173:
        sub    esp, 4
        cmp    dword ptr [0x248], 0          ; -> obj1:0x0248
        je     loc_2_0197
        mov    eax, dword ptr [0x250]        ; -> obj1:0x0250
        lea    ecx, [eax + 0x10]
        mov    dword ptr [0xa1c], ecx        ; -> obj4:0x0A1C
        add    eax, 8
        mov    dword ptr [0xa20], eax        ; -> obj4:0x0A20
        jmp    loc_2_01AB
loc_2_0197:
        mov    dword ptr [0xa1c], 0x254      ; -> obj4:0x0A1C
        mov    dword ptr [0xa20], 0x900      ; -> obj4:0x0A20
loc_2_01AB:
        lea    eax, [esp]
        push   eax
        push   4
        push   0x344                         ; -> loc_2_0344
        push   0x2f0                         ; -> loc_2_02F0
        push   0
        push   0x3c8                         ; -> loc_2_03C8
        call   sub_2_02C8
        add    esp, 0x18
        or     eax, eax
        jne    loc_2_01D1
        sub    eax, eax
loc_2_01D1:
        add    esp, 4
        ret    
sub_2_01D5:
        push   esi
        sub    esi, esi
        push   esi
        push   esi
        call   sub_2_02E0
        add    esp, 8
        mov    dword ptr [0x20], eax         ; -> loc_4_0020
        cmp    eax, esi
        jne    loc_2_01F2
        mov    eax, 8
        jmp    loc_2_0233
loc_2_01F2:
        push   0x200
        push   0x78
        call   sub_2_02D0
        add    esp, 8
        mov    dword ptr [0x50], eax         ; -> loc_4_0050
        or     eax, eax
        jne    loc_2_0211
        mov    esi, 8
        jmp    loc_2_021B
loc_2_0211:
        mov    dword ptr [0x54], 0xa         ; -> loc_4_0054
loc_2_021B:
        or     esi, esi
        je     loc_2_0231
        mov    eax, dword ptr [0x20]         ; -> loc_4_0020
        or     eax, eax
        je     loc_2_0231
        push   eax
        call   sub_2_02E8
        add    esp, 4
loc_2_0231:
        mov    eax, esi
loc_2_0233:
        pop    esi
        ret    
sub_2_0235:
        call   sub_2_0060
        or     eax, eax
        jne    loc_2_0249
        call   sub_2_01D5
        or     eax, eax
        jne    loc_2_0249
        sub    eax, eax
loc_2_0249:
        ret    
sub_2_024A:
        cmp    dword ptr [0x384], 0          ; -> loc_2_0384
        je     loc_2_025E
        push   dword ptr [0x384]             ; -> loc_2_0384
        call   sub_3_10FB
loc_2_025E:
        mov    eax, dword ptr [0x20]         ; -> loc_4_0020
        or     eax, eax
        je     loc_2_027A
        push   eax
        call   sub_2_02E8
        add    esp, 4
        mov    dword ptr [0x20], 0           ; -> loc_4_0020
loc_2_027A:
        mov    eax, dword ptr [0x50]         ; -> loc_4_0050
        or     eax, eax
        je     loc_2_0298
        push   0
        push   eax
        call   sub_2_02D8
        add    esp, 8
        mov    dword ptr [0x50], 0           ; -> loc_4_0050
loc_2_0298:
        ret    
sub_2_0299:
        push   0x384                         ; -> loc_2_0384
        push   1
        push   0
        push   0x254                         ; -> obj1:0x0254
        call   sub_3_1254
        ret    
        db  000h, 000h, 000h
sub_2_02B0:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        add    byte ptr [eax], al
sub_2_02B8:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        add    byte ptr [eax], al
sub_2_02C0:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        add    byte ptr [eax], al
sub_2_02C8:
        VMMCall  VMM._RegCreateDynKey            ; cd2076810100
        add    byte ptr [eax], al
sub_2_02D0:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_2_02D8:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_2_02E0:
        VMMCall  VMM._CreateMutex                ; cd2001810100
        add    byte ptr [eax], al
sub_2_02E8:
        VMMCall  VMM._DestroyMutex               ; cd2002810100
        add    byte ptr [eax], al
loc_2_02F0:
        mov    byte ptr [0xa2000017], al     ; -> loc_3_17A2
        pop    ss
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0308:
        push   eax
        insb   byte ptr es:[edi], dx
        popal  
        je     0x373
        outsd  dx, dword ptr [esi]
        jb     0x37d
        pop    edi
        push   esp
        jns    loc_2_0384
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 0x6f], al
        outsb  dx, byte ptr [esi]
        je     loc_2_037E
        imul   ebp, dword ptr [esi + 0x65], 0x72
loc_2_0324:
        inc    ecx
        jb     0x38e
        jae    loc_2_039E
        pop    edi
        push   ebx
        jb     loc_2_03A6
        jb     loc_2_0333
loc_2_0333:
        add    byte ptr [ecx + 0x64], al
        jb     loc_2_039E
        jae    loc_2_03AE
        pop    edi
        inc    edx
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        imul   eax, dword ptr [eax], 0
        add    byte ptr [eax], al
loc_2_0344:
        or     byte ptr [ebx], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        or     al, 0xa
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        sbb    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    al, 0xa
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        and    al, 3
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    al, 0xa
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        xor    al, 3
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    al, 0xa
loc_2_037E:
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_2_0384:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0388:
        push   ebx
        arpl   word ptr gs:[ebp + 0x72], si
        imul   esi, dword ptr [ecx + edi*2 + 0x5c], 0x766f7250
        imul   esp, dword ptr [ebp + 0x72], 0x50000000
        insb   byte ptr es:[edi], dx
loc_2_039E:
        popal  
        je     0x407
        outsd  dx, dword ptr [esi]
        jb     0x411
        pop    edi
        push   esp
loc_2_03A6:
        jns    0x418
        add    byte ptr gs:[eax], al
        add    byte ptr [ebx + 0x6f], al
loc_2_03AE:
        outsb  dx, byte ptr [esi]
        je     0x412
        imul   ebp, dword ptr [esi + 0x65], 0x72
loc_2_03B8:
        inc    ecx
        jb     0x422
        jae    0x432
        pop    edi
        inc    edx
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        imul   eax, dword ptr [eax], 0
        add    byte ptr [eax], al
loc_2_03C8:
        push   ebx
        arpl   word ptr gs:[ebp + 0x72], si
        imul   esi, dword ptr [ecx + edi*2 + 0x5c], 0x766f7250
        db  069h, 064h, 065h, 072h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x18c3  R-X
SECPROV_Service_00:
        mov    eax, 0x400
        clc    
        ret    
loc_3_0007:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, 0
        jne    loc_3_0016
        mov    ecx, 0x400
        jmp    loc_3_001B
loc_3_0016:
        cmp    ecx, -1
        jne    loc_3_001F
loc_3_001B:
        xor    eax, eax
        clc    
        ret    
loc_3_001F:
        cmp    ecx, 0x64
        jl     loc_3_003E
        cmp    ecx, 0x6c
        jge    loc_3_003E
        sub    ecx, 0x64
        push   dword ptr [esi + 0x14]
        push   dword ptr [esi + 0x10]
        call   sub_4_0000
        add    esp, 8
        clc    
        ret    
loc_3_003E:
        mov    eax, 1
        clc    
        ret    
sub_3_0045:
        movzx  eax, byte ptr [esp + 4]
        VMMCall  VMM.Test_DBCS_Lead_Byte         ; cd20d1000100
        sbb    eax, eax
        inc    eax
        ret    4
sub_3_0056:
        VxDCall  Dev_0481.Service_800Ch          ; cd200c808104
sub_3_005C:
        VxDCall  Dev_0481.Service_800Ah          ; cd200a808104
sub_3_0062:
        VxDCall  Dev_0481.Service_800Bh          ; cd200b808104
sub_3_0068:
        VxDCall  Dev_0481.Service_8009h          ; cd2009808104
sub_3_006E:
        VxDCall  Dev_0481.Service_800Fh          ; cd200f808104
sub_3_0074:
        VxDCall  NDIS2SUP.Service_8001h          ; cd2001808004
sub_3_007A:
        VxDCall  IFSMgr.IFSMgr_MakeMailSlot      ; cd2019804000
        VxDCall  IFSMgr.IFSMgr_DeleteMailSlot    ; cd201a804000
sub_3_0086:
        VxDCall  IFSMgr.BCSToUni                 ; cd2042804000
sub_3_008C:
        VxDCall  IFSMgr.UniToBCS                 ; cd2040804000
sub_3_0092:
        VxDCall  IFSMgr.UniToUpper               ; cd2043804000
sub_3_0098:
        VxDCall  Dev_0491.Service_800Bh          ; cd200b809104
sub_3_009E:
        VxDCall  Dev_0491.Service_800Fh          ; cd200f809104
sub_3_00A4:
        VxDCall  Dev_0491.Service_800Dh          ; cd200d809104
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_00B0:
        push   ebx
        push   esi
        push   edi
        call   sub_3_0098
        or     eax, eax
        jne    loc_3_00DB
        mov    edi, dword ptr [esp + 0x10]
        push   edi
        call   sub_3_18A8
        add    esp, 4
        lea    ebx, [edi + eax - 1]
        cmp    byte ptr [ebx], 0x20
        jne    loc_3_00D8
loc_3_00D2:
        dec    ebx
        cmp    byte ptr [ebx], 0x20
        je     loc_3_00D2
loc_3_00D8:
        inc    ebx
        jmp    loc_3_0113
loc_3_00DB:
        sub    ebx, ebx
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, edi
        cmp    byte ptr [edi], bl
        je     loc_3_010D
loc_3_00E7:
        mov    al, byte ptr [esi]
        push   eax
        call   sub_3_0045
        or     eax, eax
        je     loc_3_00F8
        sub    ebx, ebx
        inc    esi
        jmp    loc_3_0107
loc_3_00F8:
        cmp    byte ptr [esi], 0x20
        je     loc_3_0101
        sub    ebx, ebx
        jmp    loc_3_0107
loc_3_0101:
        or     ebx, ebx
        jne    loc_3_0107
        mov    ebx, esi
loc_3_0107:
        inc    esi
        cmp    byte ptr [esi], 0
        jne    loc_3_00E7
loc_3_010D:
        or     ebx, ebx
        jne    loc_3_0113
        mov    ebx, esi
loc_3_0113:
        mov    esi, dword ptr [esp + 0x14]
        cmp    esi, edi
        je     loc_3_012A
        sub    ebx, edi
        push   ebx
        push   edi
        push   esi
        call   sub_3_18B0
        add    esp, 0xc
        add    ebx, esi
loc_3_012A:
        mov    byte ptr [ebx], 0
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_3_0135:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        or     esi, esi
        je     loc_3_0153
        cmp    byte ptr [esi], 0
        je     loc_3_0153
        push   esi
        call   sub_3_18A8
        add    esp, 4
        mov    ebp, eax
        jmp    loc_3_015F
loc_3_0153:
        mov    esi, 0x254                    ; -> obj1:0x0254
        mov    ebp, dword ptr [0x900]        ; -> loc_4_0900
        dec    ebp
loc_3_015F:
        mov    edi, dword ptr [esp + 0x18]
        or     edi, edi
        jne    loc_3_0169
        jmp    loc_3_01AC
loc_3_0169:
        push   edi
        call   sub_3_18A8
        add    esp, 4
        mov    ebx, eax
        or     ebp, ebp
        je     loc_3_01AC
        cmp    ebp, 0xf
        ja     loc_3_01AC
        or     ebx, ebx
        je     loc_3_01AC
        cmp    ebx, 0x14
        ja     loc_3_01AC
        push   ebp
        push   esi
        inc    ebx
        mov    esi, dword ptr [esp + 0x24]
        push   esi
        call   sub_3_18B0
        add    esp, 0xc
        lea    eax, [esi + ebp]
        push   ebx
        mov    byte ptr [eax], 0x5c
        push   edi
        inc    eax
        push   eax
        call   sub_3_18B0
        add    esp, 0xc
        sub    eax, eax
        jmp    loc_3_01B1
loc_3_01AC:
        mov    eax, 0x57
loc_3_01B1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0xc
sub_3_01B8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    edi, edi
        mov    ebp, dword ptr [0x58]         ; -> loc_4_0058
        dec    ebp
        js     loc_3_01FB
        mov    esi, dword ptr [esp + 0x14]
loc_3_01CB:
        lea    eax, [edi + ebp]
        cdq    
        sub    eax, edx
        sar    eax, 1
        mov    ebx, eax
        push   1
        imul   eax, eax, 0xc
        mov    ecx, dword ptr [0x50]         ; -> loc_4_0050
        push   dword ptr [eax + ecx]
        push   esi
        call   sub_3_00A4
        or     eax, eax
        jge    loc_3_01F2
        lea    ebp, [ebx - 1]
        jmp    loc_3_01F7
loc_3_01F2:
        jle    loc_3_020A
        lea    edi, [ebx + 1]
loc_3_01F7:
        cmp    ebp, edi
        jge    loc_3_01CB
loc_3_01FB:
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax], edi
        sub    eax, eax
loc_3_0203:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_3_020A:
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax], ebx
        mov    eax, 1
        jmp    loc_3_0203
sub_3_0217:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 0x18]
        push   eax
        push   dword ptr [esp + 0x34]
        call   sub_3_01B8
        or     eax, eax
        je     loc_3_0246
        mov    eax, dword ptr [esp + 0x18]
        imul   eax, eax, 0xc
        mov    ecx, dword ptr [0x50]         ; -> loc_4_0050
        inc    dword ptr [eax + ecx + 4]
        jmp    loc_3_0451
loc_3_0246:
        push   dword ptr [esp + 0x2c]
        call   sub_3_18A8
        add    esp, 4
        inc    eax
        mov    dword ptr [esp + 0x14], eax
        movzx  eax, word ptr [esp + 0x34]
        mov    dword ptr [esp + 0x10], eax
        mov    ecx, eax
        lea    edi, [ecx*4 + 4]
        push   0x200
        push   edi
        call   sub_3_183C
        mov    dword ptr [esp + 0x2c], eax
        add    esp, 8
        or     eax, eax
        jne    loc_3_0289
        mov    eax, 8
        jmp    loc_3_0453
loc_3_0289:
        mov    eax, dword ptr [esp + 0x24]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], ecx
        sub    ebx, ebx
        cmp    word ptr [esp + 0x34], bx
        je     loc_3_02D8
        mov    esi, dword ptr [esp + 0x38]
        mov    ebp, eax
        add    ebp, 4
loc_3_02A5:
        or     ebx, ebx
        je     loc_3_02B1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp], eax
        jmp    loc_3_02B8
loc_3_02B1:
        mov    dword ptr [ebp], 0
loc_3_02B8:
        push   esi
        call   sub_3_18A8
        mov    ecx, dword ptr [esp + 0x18]
        add    esp, 4
        add    ecx, eax
        inc    ecx
        add    dword ptr [ebp], ecx
        add    esi, 0x15
        add    ebp, 4
        inc    ebx
        cmp    ebx, dword ptr [esp + 0x10]
        jl     loc_3_02A5
loc_3_02D8:
        cmp    word ptr [esp + 0x34], 0
        je     loc_3_02EB
        mov    eax, dword ptr [esp + 0x24]
        mov    ecx, dword ptr [esp + 0x10]
        add    edi, dword ptr [eax + ecx*4]
loc_3_02EB:
        push   dword ptr [esp + 0x30]
        call   sub_3_18A8
        add    esp, 4
        lea    edi, [eax + edi + 1]
        push   0
        push   edi
        push   dword ptr [esp + 0x2c]
        call   sub_3_184C
        mov    dword ptr [esp + 0x28], eax
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_032A
        push   0
        push   dword ptr [esp + 0x28]
        call   sub_3_1844
        add    esp, 8
        mov    eax, 8
        jmp    loc_3_0453
loc_3_032A:
        mov    eax, dword ptr [0x54]         ; -> loc_4_0054
        cmp    dword ptr [0x58], eax         ; -> loc_4_0058
        jne    loc_3_037F
        add    dword ptr [0x54], 0xa         ; -> loc_4_0054
        push   0
        mov    eax, dword ptr [0x54]         ; -> loc_4_0054
        imul   eax, eax, 0xc
        push   eax
        push   dword ptr [0x50]              ; -> loc_4_0050
        call   sub_3_184C
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_037A
        push   0
        push   dword ptr [esp + 0x20]
        call   sub_3_1844
        add    esp, 8
        sub    dword ptr [0x54], 0xa         ; -> loc_4_0054
        mov    eax, 8
        jmp    loc_3_0453
loc_3_037A:
        mov    dword ptr [0x50], eax         ; -> loc_4_0050
loc_3_037F:
        mov    edx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esp + 0x10]
        lea    ecx, [edx + eax*4 + 4]
        mov    dword ptr [esp + 0x20], ecx
        sub    esi, esi
        cmp    word ptr [esp + 0x34], si
        je     loc_3_03D7
        mov    ebx, dword ptr [esp + 0x38]
        mov    edi, edx
loc_3_039E:
        or     esi, esi
        mov    eax, 0
        je     loc_3_03A9
        mov    eax, dword ptr [edi]
loc_3_03A9:
        mov    ebp, dword ptr [esp + 0x20]
        add    ebp, eax
        push   ebp
        push   dword ptr [esp + 0x30]
        call   sub_3_009E
        mov    eax, dword ptr [esp + 0x14]
        add    eax, ebp
        push   eax
        mov    byte ptr [eax - 1], 0x5c
        push   ebx
        call   sub_3_009E
        add    ebx, 0x15
        add    edi, 4
        inc    esi
        cmp    esi, dword ptr [esp + 0x10]
        jl     loc_3_039E
loc_3_03D7:
        cmp    word ptr [esp + 0x34], 0
        mov    eax, 0
        je     loc_3_03EF
        mov    edx, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [edx + ecx*4]
loc_3_03EF:
        mov    ebx, dword ptr [esp + 0x20]
        push   -1
        add    ebx, eax
        push   dword ptr [esp + 0x34]
        push   ebx
        call   sub_3_18A0
        add    esp, 0xc
        mov    ecx, dword ptr [0x58]         ; -> loc_4_0058
        dec    ecx
        cmp    ecx, dword ptr [esp + 0x18]
        jl     loc_3_042E
        mov    edx, ecx
        imul   edx, edx, 0xc
loc_3_0416:
        mov    esi, dword ptr [0x50]         ; -> loc_4_0050
        add    esi, edx
        sub    edx, 0xc
        lea    edi, [esi + 0xc]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        dec    ecx
        cmp    ecx, dword ptr [esp + 0x18]
        jge    loc_3_0416
loc_3_042E:
        mov    eax, dword ptr [esp + 0x18]
        imul   eax, eax, 0xc
        add    eax, dword ptr [0x50]         ; -> loc_4_0050
        inc    dword ptr [0x58]              ; -> loc_4_0058
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 4], 1
        mov    edx, dword ptr [esp + 0x1c]
        mov    dword ptr [eax + 8], edx
loc_3_0451:
        sub    eax, eax
loc_3_0453:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    0x10
sub_3_045D:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   0
        push   dword ptr [esi + 8]
        call   sub_3_1844
        add    esp, 8
        sub    esi, dword ptr [0x50]         ; -> loc_4_0050
        mov    ecx, 0xc
        mov    eax, esi
        cdq    
        idiv   ecx
        mov    ecx, dword ptr [0x58]         ; -> loc_4_0058
        dec    ecx
        cmp    ecx, eax
        jle    loc_3_04AD
        mov    edx, eax
        imul   edx, edx, 0xc
loc_3_0490:
        mov    edi, dword ptr [0x50]         ; -> loc_4_0050
        add    edi, edx
        add    edx, 0xc
        lea    esi, [edi + 0xc]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        inc    eax
        mov    ecx, dword ptr [0x58]         ; -> loc_4_0058
        dec    ecx
        cmp    ecx, eax
        jg     loc_3_0490
loc_3_04AD:
        dec    dword ptr [0x58]              ; -> loc_4_0058
        mov    eax, dword ptr [0x54]         ; -> loc_4_0054
        sub    eax, 0x14
        cmp    eax, dword ptr [0x58]         ; -> loc_4_0058
        jl     loc_3_04F5
        sub    dword ptr [0x54], 0xa         ; -> loc_4_0054
        push   0
        mov    eax, dword ptr [0x54]         ; -> loc_4_0054
        imul   eax, eax, 0xc
        push   eax
        push   dword ptr [0x50]              ; -> loc_4_0050
        call   sub_3_184C
        add    esp, 0xc
        or     eax, eax
        je     loc_3_04EE
        mov    dword ptr [0x50], eax         ; -> loc_4_0050
        jmp    loc_3_04F5
loc_3_04EE:
        add    dword ptr [0x54], 0xa         ; -> loc_4_0054
loc_3_04F5:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    4
sub_3_04FC:
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    ebx, ebx
        sub    edi, edi
        push   1
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1888
        lea    eax, [esp + 0x18]
        add    esp, 8
        push   eax
        push   3
        push   ebx
        push   dword ptr [esp + 0x48]
        call   sub_3_1254
        mov    esi, eax
        cmp    esi, ebx
        je     loc_3_0544
        cmp    esi, 0x4d5
        je     loc_3_0544
        cmp    esi, 0xb7
        jne    loc_3_0778
        jmp    loc_3_0549
loc_3_0544:
        mov    ebx, 1
loc_3_0549:
        push   0
        push   0
        push   dword ptr [esp + 0x18]
        call   sub_1_0100
        or     eax, eax
        jne    loc_3_055F
        jmp    loc_3_0773
loc_3_055F:
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x44]
        add    eax, 0x10
        push   eax
        call   dword ptr [esp + 0x48]
        mov    esi, eax
        or     esi, esi
        je     loc_3_0778
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [eax + 8]
        add    eax, 0x10
        push   eax
        lea    eax, [esp + 0x30]
        push   eax
        call   sub_3_18B0
        mov    eax, dword ptr [esp + 0x1c]
        add    esp, 0xc
        add    eax, 0x10
        push   eax
        push   0
        push   dword ptr [esp + 0x18]
        call   sub_1_0100
        cmp    eax, 2
        jne    loc_3_05CF
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [eax + 0xc]
        add    eax, 0x20
        push   eax
        mov    edi, 1
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_3_18B0
        mov    eax, dword ptr [esp + 0x1c]
        add    esp, 0xc
        mov    ebp, dword ptr [eax + 0xc]
        jmp    loc_3_05D3
loc_3_05CF:
        mov    ebp, dword ptr [esp + 0x14]
loc_3_05D3:
        or     ebx, ebx
        jne    loc_3_0601
        lea    eax, [esp + 0x10]
        push   eax
        push   0
        push   0
        mov    ebx, 2
        push   dword ptr [esp + 0x48]
        call   sub_3_1254
        mov    esi, eax
        or     esi, esi
        je     loc_3_060A
        cmp    esi, 0x4d5
        je     loc_3_060A
        jmp    loc_3_0778
loc_3_0601:
        or     edi, edi
        jne    loc_3_060E
        jmp    loc_3_0773
loc_3_060A:
        or     edi, edi
        je     loc_3_061D
loc_3_060E:
        lea    eax, [esp + 0x18]
        push   dword ptr [esp + 0x44]
        push   eax
        call   dword ptr [esp + 0x48]
        mov    esi, eax
loc_3_061D:
        cmp    ebx, 2
        jne    loc_3_063E
        lea    eax, [esp + 0x18]
        or     edi, edi
        jne    loc_3_062E
        lea    eax, [esp + 0x28]
loc_3_062E:
        push   eax
        push   0
        push   dword ptr [esp + 0x18]
        call   sub_1_0100
        mov    ebx, eax
        jmp    loc_3_0640
loc_3_063E:
        sub    ebx, ebx
loc_3_0640:
        or     edi, edi
        je     loc_3_06D7
        or     esi, esi
        jne    loc_3_06D7
        mov    eax, dword ptr [0x250]        ; -> obj1:0x0250
        cmp    dword ptr [esp + 0x10], eax
        jne    loc_3_0666
        lea    eax, [esp + 0x18]
        push   ebp
        push   eax
        call   sub_3_1720
loc_3_0666:
        cmp    ebx, 1
        jne    loc_3_06A6
        push   0
        push   1
        push   dword ptr [esp + 0x18]
        call   sub_1_0100
        cmp    ebx, 1
        jne    loc_3_06A6
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [eax + 8]
        add    eax, 0x10
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        add    eax, 0x20
        push   eax
        call   sub_3_18B0
        add    esp, 0xc
loc_3_06A6:
        mov    eax, dword ptr [esp + 0x10]
        push   ebp
        mov    dword ptr [eax + 8], ebp
        lea    eax, [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        add    eax, 0x10
        push   eax
        call   sub_3_18B0
        add    esp, 0xc
        cmp    ebx, 1
        sbb    eax, eax
        add    eax, 2
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 4], eax
        jmp    loc_3_0778
loc_3_06D7:
        or     ebx, ebx
        jne    loc_3_06E0
        jmp    loc_3_0768
loc_3_06E0:
        cmp    ebx, 2
        mov    eax, dword ptr [esp + 0x10]
        jne    loc_3_06EE
        add    eax, 0x20
        jmp    loc_3_06F1
loc_3_06EE:
        add    eax, 0x10
loc_3_06F1:
        push   dword ptr [esp + 0x44]
        push   eax
        call   dword ptr [esp + 0x48]
        mov    esi, eax
        or     esi, esi
        jne    loc_3_0756
        cmp    ebx, 2
        jne    loc_3_0739
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [eax + 0xc]
        add    eax, 0x20
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        add    eax, 0x10
        push   eax
        call   sub_3_18B0
        mov    eax, dword ptr [esp + 0x1c]
        add    esp, 0xc
        mov    dword ptr [eax + 4], 1
loc_3_0739:
        mov    eax, dword ptr [0x250]        ; -> obj1:0x0250
        cmp    dword ptr [esp + 0x10], eax
        jne    loc_3_0778
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [eax + 8]
        add    eax, 0x10
        push   eax
        call   sub_3_1720
        jmp    loc_3_0778
loc_3_0756:
        cmp    ebx, 1
        jne    loc_3_0768
        push   0
        push   1
        push   dword ptr [esp + 0x18]
        call   sub_1_0100
loc_3_0768:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 4], 0
loc_3_0773:
        mov    esi, 0x51f
loc_3_0778:
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1890
        add    esp, 4
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x28
        ret    0xc
SECPROV_Service_01:
        sub    esp, 0x34
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 0x1c]
        push   eax
        push   dword ptr [esp + 0x50]
        push   dword ptr [esp + 0x50]
        call   sub_3_0135
        mov    dword ptr [esp + 0x10], eax
        or     eax, eax
        je     loc_3_07B8
        jmp    loc_3_08EB
loc_3_07B8:
        push   1
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1888
        lea    eax, [esp + 0x20]
        add    esp, 8
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_3_01B8
        or     eax, eax
        jne    loc_3_07FC
        mov    ecx, dword ptr [esp + 0x54]
        mov    ebx, dword ptr [esp + 0x50]
        mov    dword ptr [ecx], 4
        mov    dword ptr [ebx], 0
        mov    dword ptr [esp + 0x10], 0x525
        jmp    loc_3_08D9
loc_3_07FC:
        mov    eax, dword ptr [esp + 0x18]
        imul   eax, eax, 0xc
        mov    ecx, dword ptr [0x50]         ; -> loc_4_0050
        mov    ebp, dword ptr [eax + ecx + 8]
        mov    ecx, dword ptr [esp + 0x54]
        mov    esi, dword ptr [ebp]
        or     esi, esi
        jne    loc_3_082D
        mov    ebx, dword ptr [esp + 0x50]
        mov    dword ptr [ecx], 4
        mov    dword ptr [ebx], 0
        jmp    loc_3_08D9
loc_3_082D:
        mov    eax, dword ptr [ecx]
        sub    eax, 4
        dec    esi
        js     loc_3_0844
loc_3_0835:
        mov    edx, dword ptr [ebp + esi*4 + 4]
        lea    edx, [edx + esi*4 + 4]
        cmp    edx, eax
        jbe    loc_3_0844
        dec    esi
        jns    loc_3_0835
loc_3_0844:
        or     esi, esi
        jge    loc_3_0862
        mov    ebx, dword ptr [esp + 0x50]
        mov    dword ptr [ecx], 4
        mov    dword ptr [ebx], 0
        mov    dword ptr [esp + 0x10], 0xea
        jmp    loc_3_08D9
loc_3_0862:
        mov    ebx, dword ptr [esp + 0x50]
        lea    eax, [esi + 1]
        mov    dword ptr [ebx], eax
        lea    eax, [eax*4 + 4]
        add    eax, dword ptr [ebp + esi*4 + 4]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [ebx]
        cmp    eax, 1
        jle    loc_3_0897
        lea    eax, [eax*4 - 4]
        push   eax
        lea    eax, [ebp + 4]
        push   eax
        lea    eax, [ebx + 8]
        push   eax
        call   sub_3_18B0
        add    esp, 0xc
loc_3_0897:
        lea    edi, [ebx + 4]
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [ebx]
        lea    eax, [ebx + eax*4 + 4]
        mov    dword ptr [esp + 0x14], eax
        push   dword ptr [ebp + esi*4 + 4]
        mov    eax, dword ptr [ebp]
        lea    eax, [ebp + eax*4]
        add    eax, 4
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_3_18B0
        add    esp, 0xc
        sub    ecx, ecx
        cmp    dword ptr [ebx], ecx
        jle    loc_3_08D9
        mov    eax, dword ptr [esp + 0x14]
loc_3_08CF:
        add    dword ptr [edi], eax
        add    edi, 4
        inc    ecx
        cmp    dword ptr [ebx], ecx
        jg     loc_3_08CF
loc_3_08D9:
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1890
        mov    eax, dword ptr [esp + 0x14]
        add    esp, 4
loc_3_08EB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x34
        ret    
sub_3_08F3:
        push   esi
        push   edi
        push   1
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1888
        mov    edi, dword ptr [esp + 0x18]
        add    esp, 8
        push   0x24                          ; -> loc_4_0024
        push   dword ptr [edi + 4]
        push   dword ptr [edi]
        push   dword ptr [esp + 0x18]
        call   sub_3_005C
        add    esp, 0x10
        mov    esi, eax
        or     esi, esi
        jne    loc_3_094D
        call   sub_3_1880
        mov    dword ptr [0x28], eax         ; -> loc_4_0028
        mov    eax, dword ptr [edi + 4]
        push   dword ptr [eax]
        push   dword ptr [edi]
        push   0x2c                          ; -> loc_4_002C
        call   sub_3_18B0
        add    esp, 0xc
        mov    eax, dword ptr [edi + 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x4c], eax         ; -> loc_4_004C
loc_3_094D:
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1890
        add    esp, 4
        mov    eax, esi
        pop    edi
        pop    esi
        ret    8
SECPROV_Service_02:
        sub    esp, 8
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [esp], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esp + 4], eax
        lea    eax, [esp]
        cmp    dword ptr [0x248], 0          ; -> obj1:0x0248
        push   eax
        je     loc_3_0994
        push   0x8f3                         ; -> sub_3_08F3
        push   0x254                         ; -> obj1:0x0254
        call   sub_3_04FC
        jmp    loc_3_09A9
loc_3_0994:
        push   0x254                         ; -> obj1:0x0254
        call   sub_3_08F3
        cmp    eax, 1
        cmc    
        sbb    eax, eax
        and    eax, 0x51f
loc_3_09A9:
        add    esp, 8
        ret    
loc_3_09AD:
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [edx + 0x14], eax
        push   dword ptr [ecx]
        call   sub_3_006E
        add    esp, 4
        ret    8
SECPROV_Service_06:
        sub    esp, 0x80
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x94]
        or     ebp, ebp
        je     loc_3_09E7
        cmp    byte ptr [ebp], 0
        mov    esi, 0
        jne    loc_3_09EC
loc_3_09E7:
        mov    esi, 1
loc_3_09EC:
        mov    eax, dword ptr [esp + 0xa4]
        or     eax, eax
        je     loc_3_0A01
        cmp    byte ptr [eax], 0
        mov    edi, 0
        jne    loc_3_0A06
loc_3_0A01:
        mov    edi, 1
loc_3_0A06:
        mov    dword ptr [esp + 0x1c], 0
        mov    ecx, dword ptr [esp + 0xc0]
        mov    dword ptr [esp + 0x18], 0
        cmp    dword ptr [ecx], 0
        je     loc_3_0A2C
        mov    eax, 0x57
        jmp    loc_3_0CF7
loc_3_0A2C:
        mov    ebx, dword ptr [esp + 0x9c]
        or     ebx, ebx
        je     loc_3_0CE9
        cmp    byte ptr [ebx], 0
        je     loc_3_0CE9
        or     edi, edi
        jne    loc_3_0A53
        lea    ecx, [esp + 0x24]
        push   ecx
        push   eax
        call   sub_3_00B0
loc_3_0A53:
        or     esi, esi
        jne    loc_3_0A6C
        or     edi, edi
        jne    loc_3_0A71
        lea    eax, [esp + 0x24]
        push   1
        push   eax
        push   ebp
        call   sub_3_00A4
        or     eax, eax
        jne    loc_3_0A71
loc_3_0A6C:
        mov    ebp, 0x254                    ; -> obj1:0x0254
loc_3_0A71:
        lea    eax, [esp + 0x68]
        push   eax
        push   ebx
        push   ebp
        call   sub_3_0135
        or     eax, eax
        je     loc_3_0A8F
        cmp    eax, 0x57
        jne    loc_3_0CF7
        jmp    loc_3_0CE9
loc_3_0A8F:
        push   1
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1888
        add    esp, 8
        cmp    dword ptr [0x24], 0           ; -> loc_4_0024
        jne    loc_3_0AAD
        jmp    loc_3_0CD2
loc_3_0AAD:
        mov    eax, dword ptr [esp + 0xbc]
        cmp    dword ptr [0x4c], eax         ; -> loc_4_004C
        jne    loc_3_0CD2
        mov    esi, 0x2c                     ; -> loc_4_002C
        mov    ecx, eax
        shr    ecx, 2
        mov    edi, dword ptr [esp + 0xb8]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_3_0CD2
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_3_0CD2
        cmp    dword ptr [0x248], 0          ; -> obj1:0x0248
        je     loc_3_0B17
        push   1
        push   0x254                         ; -> obj1:0x0254
        push   ebp
        call   sub_3_00A4
        or     eax, eax
        je     loc_3_0B17
        mov    dword ptr [esp + 0x1c], 1
        lea    eax, [esp + 0x18]
        push   eax
        push   2
        push   0
        push   ebp
        call   sub_3_1254
loc_3_0B17:
        mov    word ptr [esp + 0x12], 0
        mov    word ptr [esp + 0x14], 0
        lea    eax, [esp + 0x14]
        mov    dword ptr [esp + 0x4c], eax
        lea    eax, [esp + 0x12]
        mov    dword ptr [esp + 0x50], eax
        cmp    dword ptr [0x248], 0          ; -> obj1:0x0248
        mov    word ptr [esp + 0x54], 0x834
        mov    dword ptr [esp + 0x56], 0x5c  ; -> loc_4_005C
        mov    word ptr [esp + 0x5a], 0
        mov    dword ptr [esp + 0x5c], ebx
        je     loc_3_0B66
        mov    eax, dword ptr [0x250]        ; -> obj1:0x0250
        add    eax, 0x10
        mov    dword ptr [esp + 0x60], eax
        jmp    loc_3_0B6E
loc_3_0B66:
        mov    dword ptr [esp + 0x60], 0x254 ; -> obj1:0x0254
loc_3_0B6E:
        mov    dword ptr [esp + 0x64], ebp
        lea    eax, [esp + 0x4c]
        push   eax
        push   dword ptr [esp + 0xb4]
        push   dword ptr [esp + 0xb0]
        push   dword ptr [0x24]              ; -> loc_4_0024
        call   sub_3_0068
        add    esp, 0x10
        mov    esi, eax
        or     esi, esi
        je     loc_3_0BB4
        cmp    esi, 5
        je     loc_3_0BB4
        cmp    esi, 8
        je     loc_3_0BC2
        cmp    esi, 0xea
        je     loc_3_0BB2
        mov    esi, 0x51f
        jmp    loc_3_0BC2
loc_3_0BB2:
        sub    esi, esi
loc_3_0BB4:
        mov    eax, dword ptr [esp + 0xc4]
        mov    word ptr [eax], si
        sub    esi, esi
        jmp    loc_3_0BC9
loc_3_0BC2:
        mov    eax, dword ptr [esp + 0xc4]
loc_3_0BC9:
        or     esi, esi
        jne    loc_3_0CB5
        cmp    word ptr [eax], 0
        jne    loc_3_0CB5
        mov    di, word ptr [esp + 0x12]
        cmp    dword ptr [esp + 0x1c], 0
        mov    ax, word ptr [esp + 0x14]
        mov    word ptr [esp + 0x16], ax
        je     loc_3_0C5C
        mov    word ptr [esp + 0x12], 0
        mov    word ptr [esp + 0x14], 0
        lea    eax, [esp + 0x14]
        mov    dword ptr [esp + 0x34], eax
        lea    eax, [esp + 0x12]
        mov    dword ptr [esp + 0x38], eax
        mov    word ptr [esp + 0x3c], 0x834
        mov    dword ptr [esp + 0x3e], 0x5c  ; -> loc_4_005C
        mov    word ptr [esp + 0x42], 0
        lea    eax, [esp + 0x34]
        mov    dword ptr [esp + 0x44], ebx
        mov    dword ptr [esp + 0x20], eax
        lea    eax, [esp + 0x20]
        push   eax
        push   0x9ad                         ; -> loc_3_09AD
        push   ebp
        call   sub_3_04FC
        or     eax, eax
        je     loc_3_0C5C
        cmp    word ptr [esp + 0x14], 0
        jne    loc_3_0C5C
        mov    ax, word ptr [esp + 0x16]
        mov    word ptr [esp + 0x14], ax
        mov    word ptr [esp + 0x12], di
loc_3_0C5C:
        push   0x5c                          ; -> loc_4_005C
        lea    eax, [esp + 0x6c]
        push   dword ptr [esp + 0x16]
        push   eax
        push   ebp
        call   sub_3_0217
        mov    ecx, dword ptr [esp + 0xc0]
        mov    eax, dword ptr [esp + 0x98]
        mov    dword ptr [ecx], 0
        cmp    eax, ebp
        je     loc_3_0C98
        or     eax, eax
        je     loc_3_0C98
        push   -1
        push   ebp
        push   eax
        call   sub_3_18A0
        add    esp, 0xc
loc_3_0C98:
        mov    eax, dword ptr [esp + 0xa0]
        cmp    eax, ebx
        je     loc_3_0CD7
        or     eax, eax
        je     loc_3_0CD7
        push   -1
        push   ebx
        push   eax
        call   sub_3_18A0
        add    esp, 0xc
        jmp    loc_3_0CD7
loc_3_0CB5:
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_3_0CD7
        cmp    dword ptr [esp + 0x18], 0
        je     loc_3_0CD7
        push   0
        push   1
        push   dword ptr [esp + 0x20]
        call   sub_1_0100
        jmp    loc_3_0CD7
loc_3_0CD2:
        mov    esi, 0x57
loc_3_0CD7:
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1890
        add    esp, 4
        mov    eax, esi
        jmp    loc_3_0CF7
loc_3_0CE9:
        mov    eax, dword ptr [esp + 0xc4]
        mov    word ptr [eax], 5
        sub    eax, eax
loc_3_0CF7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x80
        ret    
SECPROV_Service_03:
        sub    esp, 4
        mov    dword ptr [esp], 0
        lea    eax, [esp]
        push   dword ptr [esp + 0x24]
        push   eax
        push   dword ptr [esp + 0x28]
        push   dword ptr [esp + 0x28]
        push   0
        push   0
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x30]
        push   dword ptr [esp + 0x30]
        push   0
        push   dword ptr [esp + 0x34]
        push   0
        push   dword ptr [esp + 0x38]
        call   SECPROV_Service_06
        add    esp, 0x38
        ret    
SECPROV_Service_04:
        sub    esp, 0x2c
        push   esi
        mov    eax, dword ptr [esp + 0x38]
        cmp    byte ptr [eax], 0
        jne    loc_3_0D5A
        mov    eax, 0x57
        jmp    loc_3_0E04
loc_3_0D5A:
        lea    ecx, [esp + 8]
        push   ecx
        push   eax
        push   dword ptr [esp + 0x3c]
        call   sub_3_0135
        mov    esi, eax
        or     esi, esi
        je     loc_3_0D74
        jmp    loc_3_0E02
loc_3_0D74:
        push   1
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1888
        lea    eax, [esp + 0xc]
        add    esp, 8
        push   eax
        lea    eax, [esp + 0xc]
        push   eax
        call   sub_3_01B8
        or     eax, eax
        jne    loc_3_0D9E
        mov    esi, 0x525
        jmp    loc_3_0DF4
loc_3_0D9E:
        mov    ecx, dword ptr [esp + 4]
        imul   ecx, ecx, 0xc
        add    ecx, dword ptr [0x50]         ; -> loc_4_0050
        mov    eax, dword ptr [ecx + 4]
        dec    eax
        mov    dword ptr [ecx + 4], eax
        jne    loc_3_0DBA
        push   ecx
        call   sub_3_045D
loc_3_0DBA:
        cmp    dword ptr [0x58], 0           ; -> loc_4_0058
        jne    loc_3_0DF4
        call   sub_3_1880
        cmp    eax, dword ptr [0x28]         ; -> loc_4_0028
        jbe    loc_3_0DF4
        sub    eax, dword ptr [0x28]         ; -> loc_4_0028
        cmp    eax, 0xea60
        jb     loc_3_0DF4
        push   0x24                          ; -> loc_4_0024
        call   sub_3_0062
        add    esp, 4
        mov    dword ptr [0x24], 0           ; -> loc_4_0024
loc_3_0DF4:
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1890
        add    esp, 4
loc_3_0E02:
        mov    eax, esi
loc_3_0E04:
        pop    esi
        add    esp, 0x2c
        ret    
SECPROV_Service_05:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0x24c]        ; -> obj1:0x024C
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [0x900]        ; -> loc_4_0900
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], 0x254    ; -> obj1:0x0254
        cmp    dword ptr [0x248], 0          ; -> obj1:0x0248
        je     loc_3_0E36
        mov    eax, dword ptr [0x250]        ; -> obj1:0x0250
        mov    eax, dword ptr [eax + 8]
        jmp    loc_3_0E3B
loc_3_0E36:
        mov    eax, dword ptr [0x900]        ; -> loc_4_0900
loc_3_0E3B:
        mov    dword ptr [ecx + 0xc], eax
        cmp    dword ptr [0x248], 0          ; -> obj1:0x0248
        je     loc_3_0E54
        mov    eax, dword ptr [0x250]        ; -> obj1:0x0250
        add    eax, 0x10
        mov    dword ptr [ecx + 0x10], eax
        jmp    loc_3_0E5B
loc_3_0E54:
        mov    dword ptr [ecx + 0x10], 0x254 ; -> obj1:0x0254
loc_3_0E5B:
        mov    eax, dword ptr [0xa08]        ; -> loc_4_0A08
        mov    dword ptr [ecx + 0x14], eax
        mov    dword ptr [ecx + 0x18], 0x904 ; -> loc_4_0904
        sub    eax, eax
        ret    
loc_3_0E6D:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   SECPROV_Service_02
        add    esp, 8
        ret    
loc_3_0E7F:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, 0x32
        cmp    dword ptr [esi + 0x1c], 2
        jne    loc_3_0EBD
        lea    eax, [esp + 6]
        push   eax
        push   dword ptr [esi + 0x18]
        push   dword ptr [esi + 0x14]
        push   dword ptr [esi + 0x10]
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   SECPROV_Service_03
        movzx  ecx, word ptr [esp + 0x26]
        add    esp, 0x20
        mov    edx, dword ptr [esi + 0x20]
        mov    dword ptr [edx], ecx
loc_3_0EBD:
        pop    esi
        add    esp, 4
        ret    
loc_3_0EC2:
        sub    esp, 4
        push   esi
        lea    eax, [esp + 6]
        mov    esi, dword ptr [esp + 0xc]
        push   eax
        push   dword ptr [esi + 0x2c]
        push   dword ptr [esi + 0x28]
        push   dword ptr [esi + 0x24]
        push   dword ptr [esi + 0x20]
        push   dword ptr [esi + 0x1c]
        push   dword ptr [esi + 0x18]
        push   dword ptr [esi + 0x14]
        push   dword ptr [esi + 0x10]
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   SECPROV_Service_06
        movzx  ecx, word ptr [esp + 0x3a]
        add    esp, 0x34
        mov    edx, dword ptr [esi + 0x30]
        pop    esi
        mov    dword ptr [edx], ecx
        add    esp, 4
        ret    
loc_3_0F09:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   SECPROV_Service_04
        add    esp, 0xc
        ret    
loc_3_0F1E:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx], 2
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [0x24c]        ; -> obj1:0x024C
        mov    dword ptr [eax], ecx
        sub    eax, eax
        ret    
loc_3_0F38:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [0x900]        ; -> loc_4_0900
        cmp    dword ptr [eax], ecx
        mov    eax, 0x6f
        jb     loc_3_0F70
        push   dword ptr [0x900]             ; -> loc_4_0900
        push   0x254                         ; -> obj1:0x0254
        push   dword ptr [esi]
        call   sub_3_18B0
        add    esp, 0xc
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [0x900]        ; -> loc_4_0900
        mov    dword ptr [ecx], eax
        sub    eax, eax
loc_3_0F70:
        pop    esi
        ret    
loc_3_0F72:
        sub    esp, 4
        push   esi
        push   edi
        mov    dword ptr [esp + 8], 0
        push   1
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1888
        lea    eax, [esp + 0x10]
        add    esp, 8
        push   eax
        push   2
        mov    esi, dword ptr [esp + 0x18]
        push   0
        push   dword ptr [esi]
        call   sub_3_1254
        mov    edi, eax
        or     edi, edi
        je     loc_3_0FB1
        cmp    edi, 0x4d5
        jne    loc_3_0FBE
loc_3_0FB1:
        push   0
        push   1
        push   dword ptr [esp + 0x10]
        call   sub_1_0100
loc_3_0FBE:
        cmp    dword ptr [esp + 8], 0
        je     loc_3_103D
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 4]
        or     eax, eax
        jne    loc_3_0FD7
        mov    edi, 0x51f
        jmp    loc_3_103D
loc_3_0FD7:
        mov    ecx, dword ptr [esp + 8]
        mov    ecx, dword ptr [ecx + 8]
        inc    ecx
        cmp    eax, 2
        jb     loc_3_0FEB
        mov    eax, dword ptr [esp + 8]
        add    ecx, dword ptr [eax + 0xc]
loc_3_0FEB:
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [eax], ecx
        jae    loc_3_0FFB
        mov    dword ptr [eax], ecx
        mov    edi, 0x6f
        jmp    loc_3_103D
loc_3_0FFB:
        mov    esi, dword ptr [esi + 4]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [eax + 8]
        add    eax, 0x10
        push   eax
        push   esi
        call   sub_3_18B0
        mov    eax, dword ptr [esp + 0x14]
        add    esp, 0xc
        add    esi, dword ptr [eax + 8]
        cmp    dword ptr [eax + 4], 2
        jb     loc_3_1038
        push   dword ptr [eax + 0xc]
        add    eax, 0x20
        push   eax
        push   esi
        call   sub_3_18B0
        mov    eax, dword ptr [esp + 0x14]
        add    esp, 0xc
        add    esi, dword ptr [eax + 0xc]
loc_3_1038:
        mov    byte ptr [esi], 0
        sub    edi, edi
loc_3_103D:
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1890
        add    esp, 4
        mov    eax, edi
        pop    edi
        pop    esi
        add    esp, 4
        ret    
loc_3_1053:
        mov    eax, 1
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_1060:
        push   esi
        push   edi
        mov    edi, dword ptr [0x898]        ; -> loc_4_0898
        or     edi, edi
        je     loc_3_1086
        mov    esi, dword ptr [esp + 0xc]
loc_3_1070:
        push   1
        lea    eax, [edi + 4]
        push   eax
        push   esi
        call   sub_3_00A4
        or     eax, eax
        je     loc_3_1086
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_3_1070
loc_3_1086:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
        db  0FFh, 074h, 024h, 004h, 0E8h, 0CAh, 0FFh, 0FFh, 0FFh, 00Bh, 0C0h, 074h, 005h, 08Bh, 040h, 018h
        db  0EBh, 002h, 02Bh, 0C0h, 0C2h, 004h, 000h
sub_3_10A4:
        push   esi
        push   0
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1888
        mov    esi, dword ptr [esp + 0x10]
        add    esp, 8
        mov    eax, dword ptr [esi + 0x24]
        or     eax, eax
        je     loc_3_10CD
        push   eax
        call   sub_3_1870
        mov    dword ptr [esi + 0x24], 0
loc_3_10CD:
        call   sub_3_1880
        mov    dword ptr [esi + 0x20], eax
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [eax], 0
        push   dword ptr [esi + 0x18]
        call   sub_3_1898
        add    esp, 4
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1890
        add    esp, 4
        pop    esi
        ret    4
sub_3_10FB:
        push   0
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1888
        add    esp, 8
        mov    ecx, dword ptr [0x898]        ; -> loc_4_0898
        or     ecx, ecx
        je     loc_3_112E
        mov    eax, dword ptr [esp + 4]
loc_3_1119:
        cmp    dword ptr [ecx + 0x18], eax
        je     loc_3_1124
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        jne    loc_3_1119
loc_3_1124:
        or     ecx, ecx
        je     loc_3_112E
        push   ecx
        call   sub_3_10A4
loc_3_112E:
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1890
        add    esp, 4
        ret    4
sub_3_113F:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x18]
        cmp    dword ptr [eax], 0
        je     loc_3_11A0
        cmp    dword ptr [esi + 0x28], 3
        je     loc_3_1193
        cmp    dword ptr [eax + 4], 2
        je     loc_3_1193
        push   esi
        push   0x1388
        push   0x15a9                        ; -> loc_3_15A9
        call   sub_3_1854
        mov    dword ptr [esi + 0x24], eax
        mov    eax, dword ptr [esi + 0x28]
        inc    eax
        mov    dword ptr [esi + 0x28], eax
        mov    word ptr [esi + 0x36], ax
        push   0x1c
        push   dword ptr [esi + 0x30]
        lea    eax, [esi + 0x34]
        push   eax
        push   dword ptr [esi + 0x14]
        add    esi, 0x197
        push   esi
        call   sub_3_0056
        add    esp, 0x14
        jmp    loc_3_11A0
loc_3_1193:
        mov    dword ptr [esi + 0x24], 0
        push   esi
        call   sub_3_10A4
loc_3_11A0:
        pop    esi
        ret    4
sub_3_11A4:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi]
        mov    edi, dword ptr [eax + 0x18]
        cmp    dword ptr [edi], 0
        je     loc_3_123D
        mov    eax, dword ptr [edi + 4]
        or     eax, eax
        jne    loc_3_11F8
        lea    ebx, [esi + 4]
        push   dword ptr [esi + 0x14]
        push   ebx
        lea    eax, [edi + 0x10]
        push   eax
        call   sub_3_18B0
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [edi + 8], eax
        inc    dword ptr [edi + 4]
        push   edi
        call   sub_3_1898
        add    esp, 4
        mov    eax, dword ptr [esi]
        test   byte ptr [eax + 0x2c], 1
        je     loc_3_1230
        push   dword ptr [esi + 0x14]
        push   ebx
        call   sub_3_1720
        jmp    loc_3_1230
loc_3_11F8:
        cmp    eax, 1
        jne    loc_3_1230
        lea    ebx, [esi + 4]
        push   ebx
        lea    eax, [edi + 0x10]
        push   eax
        call   sub_1_016B
        or     eax, eax
        je     loc_3_1230
        push   dword ptr [esi + 0x14]
        push   ebx
        lea    eax, [edi + 0x20]
        push   eax
        call   sub_3_18B0
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [edi + 0xc], eax
        inc    dword ptr [edi + 4]
        push   edi
        call   sub_3_1898
        add    esp, 4
loc_3_1230:
        cmp    dword ptr [edi + 4], 2
        jne    loc_3_123D
        push   dword ptr [esi]
        call   sub_3_10A4
loc_3_123D:
        cmp    dword ptr [esi + 0x18], 0
        je     loc_3_124E
        push   0
        push   esi
        call   sub_3_1844
        add    esp, 8
loc_3_124E:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_3_1254:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   1
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1888
        add    esp, 8
        cmp    dword ptr [0x890], 0          ; -> loc_4_0890
        je     loc_3_1328
        mov    dword ptr [0x890], 0          ; -> loc_4_0890
        mov    dword ptr [0x894], 0x210      ; -> loc_4_0894
        mov    eax, dword ptr [0x894]        ; -> loc_4_0894
        mov    word ptr [0x214], 0xa         ; -> obj1:0x0214
        movzx  eax, word ptr [eax + 4]
        push   eax
        push   0x89c                         ; -> loc_4_089C
        mov    eax, dword ptr [0x894]        ; -> loc_4_0894
        add    eax, 6
        push   eax
        call   sub_3_18B0
        mov    eax, dword ptr [0x894]        ; -> loc_4_0894
        add    esp, 0xc
        mov    word ptr [eax + 2], 4
        mov    eax, dword ptr [0x894]        ; -> loc_4_0894
        mov    cx, word ptr [eax + 2]
        add    cx, word ptr [eax + 4]
        mov    word ptr [eax], cx
        call   sub_3_0074
        mov    esi, eax
        push   dword ptr [esi + 4]
        call   sub_3_18A8
        add    esp, 4
        inc    eax
        mov    dword ptr [0x244], eax        ; -> obj1:0x0244
        push   1
        push   eax
        push   dword ptr [esi + 4]
        push   0x224                         ; -> obj1:0x0224
        call   sub_3_0086
        add    esp, 0x10
        mov    dword ptr [0x244], eax        ; -> obj1:0x0244
        push   eax
        push   0x224                         ; -> obj1:0x0224
        push   0x224                         ; -> obj1:0x0224
        call   sub_3_0092
        add    esp, 0xc
        push   0x220                         ; -> obj1:0x0220
        push   dword ptr [0x894]             ; -> loc_4_0894
        push   0x15df                        ; -> loc_3_15DF
        call   sub_3_007A
        add    esp, 0xc
loc_3_1328:
        mov    edi, dword ptr [esp + 0x14]
        push   edi
        call   sub_3_1060
        mov    esi, eax
        or     esi, esi
        jne    loc_3_142D
        push   0x101
        push   0x1be
        call   sub_3_183C
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     loc_3_1378
        push   1
        push   0x30
        call   sub_3_183C
        add    esp, 8
        mov    dword ptr [esi + 0x18], eax
        or     eax, eax
        jne    loc_3_1374
        push   0
        push   esi
        call   sub_3_1844
        add    esp, 8
        sub    esi, esi
loc_3_1374:
        or     esi, esi
        jne    loc_3_1390
loc_3_1378:
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1890
        add    esp, 4
        mov    eax, 8
        jmp    loc_3_15A2
loc_3_1390:
        push   edi
        call   sub_3_18A8
        add    esp, 4
        mov    dword ptr [esi + 0x14], eax
        inc    eax
        push   eax
        push   edi
        lea    eax, [esi + 4]
        push   eax
        call   sub_3_18B0
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [eax + 8], 1
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [eax + 0xc], 1
        lea    eax, [esi + 0x199]
        mov    byte ptr [esi + 0x198], 0x5c
        mov    byte ptr [esi + 0x197], 0x5c
        push   dword ptr [esi + 0x14]
        push   edi
        push   eax
        call   sub_3_18B0
        add    esp, 0xc
        push   0x16
        push   0x8a8                         ; -> loc_4_08A8
        mov    eax, dword ptr [esi + 0x14]
        add    eax, esi
        add    eax, 0x199
        push   eax
        call   sub_3_18B0
        add    esp, 0xc
        cmp    dword ptr [0x898], 0          ; -> loc_4_0898
        jne    loc_3_1417
        mov    dword ptr [0x898], esi        ; -> loc_4_0898
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [0x250], eax        ; -> obj1:0x0250
        mov    edi, dword ptr [esp + 0x1c]
        jmp    loc_3_1488
loc_3_1417:
        mov    eax, dword ptr [0x898]        ; -> loc_4_0898
        mov    edi, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [eax]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [0x898]        ; -> loc_4_0898
        mov    dword ptr [eax], esi
        jmp    loc_3_1488
loc_3_142D:
        mov    edi, dword ptr [esp + 0x1c]
        test   edi, 2
        je     loc_3_1463
        mov    eax, dword ptr [esi + 0x18]
        cmp    dword ptr [eax + 4], 0
        jbe    loc_3_1463
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1890
        add    esp, 4
        mov    eax, dword ptr [esi + 0x18]
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [ecx], eax
        mov    eax, 0xb7
        jmp    loc_3_15A2
loc_3_1463:
        mov    eax, dword ptr [esi + 0x18]
        cmp    dword ptr [eax], 0
        jne    loc_3_1586
        mov    ebx, dword ptr [esi + 0x20]
        or     ebx, ebx
        je     loc_3_1488
        call   sub_3_1880
        sub    eax, ebx
        cmp    eax, 0xafc8
        jb     loc_3_1586
loc_3_1488:
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [eax], 1
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [eax + 4], 0
        call   sub_3_18B8
        mov    dword ptr [esi + 0x1c], eax
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1890
        add    esp, 4
        mov    word ptr [esi + 0x34], 0x12
        lea    ebx, [esi + 0x38]
        push   dword ptr [0x244]             ; -> obj1:0x0244
        push   0x224                         ; -> obj1:0x0224
        push   ebx
        call   sub_3_18B0
        add    esp, 0xc
        mov    eax, dword ptr [0x244]        ; -> obj1:0x0244
        and    eax, 0xfffffffe
        add    ebx, eax
        mov    ebp, dword ptr [esp + 0x18]
        or     ebp, ebp
        je     loc_3_14FB
        push   ebp
        call   sub_3_18A8
        add    esp, 4
        inc    eax
        push   1
        push   eax
        push   ebp
        push   ebx
        call   sub_3_0086
        add    esp, 0x10
        mov    ebp, eax
        jmp    loc_3_150F
loc_3_14FB:
        mov    ebp, 0x1c
        push   ebp
        push   0x8c0                         ; -> obj4:0x08C0
        push   ebx
        call   sub_3_18B0
        add    esp, 0xc
loc_3_150F:
        shr    ebp, 1
        lea    ebx, [ebx + ebp*2]
        push   0x8dc                         ; -> loc_4_08DC
        call   sub_3_18A8
        add    esp, 4
        lea    ebp, [eax + 1]
        push   ebp
        push   0x8ec                         ; -> loc_4_08EC
        push   ebx
        call   sub_3_18B0
        add    esp, 0xc
        lea    eax, [ebx + ebp]
        mov    dword ptr [eax], 0x10
        add    eax, 4
        mov    dword ptr [eax], 0
        add    eax, 4
        mov    dword ptr [eax], 1
        add    eax, 4
        mov    word ptr [eax], 0xffff
        add    eax, 2
        mov    word ptr [eax], 0xffff
        add    eax, 2
        sub    eax, esi
        sub    eax, 0x34
        mov    dword ptr [esi + 0x30], eax
        mov    dword ptr [esi + 0x28], 0
        mov    dword ptr [esi + 0x2c], edi
        push   esi
        call   sub_3_113F
        mov    ecx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        jmp    loc_3_15A2
loc_3_1586:
        push   dword ptr [0x20]              ; -> loc_4_0020
        call   sub_3_1890
        add    esp, 4
        mov    eax, dword ptr [esi + 0x18]
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [ecx], eax
        mov    eax, 0x4d5
loc_3_15A2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0x10
loc_3_15A9:
        sub    esp, 4
        mov    dword ptr [esp], edx
        call   sub_3_18B8
        mov    ecx, dword ptr [esp]
        cmp    eax, dword ptr [ecx + 0x1c]
        jne    loc_3_15C6
        push   ecx
        call   sub_3_113F
        jmp    loc_3_15DB
loc_3_15C6:
        push   dword ptr [esp]
        mov    eax, dword ptr [esp + 4]
        push   0x9d                          ; -> loc_1_009D
        push   dword ptr [eax + 0x1c]
        call   sub_1_00B5
loc_3_15DB:
        add    esp, 4
        ret    
loc_3_15DF:
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x3c]
        mov    ax, word ptr [esi]
        cmp    ax, 0x13
        je     loc_3_1602
        cmp    ax, 0x15
        je     loc_3_1602
        mov    eax, 1
        jmp    loc_3_1718
loc_3_1602:
        sub    ecx, ecx
        cmp    word ptr [esi + 2], cx
        lea    eax, [esi + 2]
        je     loc_3_1617
loc_3_160D:
        add    eax, 2
        inc    ecx
        cmp    word ptr [eax], 0
        jne    loc_3_160D
loc_3_1617:
        lea    edi, [ecx*2 - 4]
        lea    ebx, [esi + 6]
        push   edi
        push   ebx
        push   ebx
        call   sub_3_0092
        add    esp, 0xc
        push   1
        push   0xf
        push   edi
        lea    eax, [esp + 0x2c]
        push   ebx
        push   eax
        call   sub_3_008C
        mov    dword ptr [esp + 0x44], eax
        add    esp, 0x14
        mov    byte ptr [esp + eax + 0x20], 0
        inc    dword ptr [esp + 0x30]
        lea    edx, [esi + edi + 8]
loc_3_1650:
        mov    eax, edx
        add    edx, 2
        cmp    word ptr [eax], 0
        jne    loc_3_1650
        sub    ecx, ecx
        cmp    word ptr [edx], cx
        je     loc_3_166E
        mov    eax, edx
loc_3_1664:
        add    eax, 2
        inc    ecx
        cmp    word ptr [eax], 0
        jne    loc_3_1664
loc_3_166E:
        push   1
        push   0xf
        lea    eax, [ecx*2]
        push   eax
        lea    eax, [esp + 0x18]
        push   edx
        push   eax
        call   sub_3_008C
        add    esp, 0x14
        mov    byte ptr [esp + eax + 0xc], 0
        lea    eax, [esp + 0xc]
        push   eax
        call   sub_3_1060
        mov    dword ptr [esp + 0x1c], eax
        or     eax, eax
        jne    loc_3_16A6
        mov    eax, 2
        jmp    loc_3_1718
loc_3_16A6:
        mov    eax, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [eax + 0x18]
        cmp    dword ptr [eax], 0
        jne    loc_3_16B4
        jmp    loc_3_1716
loc_3_16B4:
        call   sub_3_18B8
        mov    ecx, dword ptr [esp + 0x1c]
        sub    eax, dword ptr [ecx + 0x1c]
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [esp + 0x34], eax
        or     eax, eax
        jne    loc_3_16DA
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_3_11A4
        jmp    loc_3_1716
loc_3_16DA:
        push   0x100
        push   0x1c
        call   sub_3_183C
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        mov    eax, 8
        je     loc_3_1718
        lea    eax, [esp + 0x1c]
        push   0x1c
        push   eax
        push   esi
        call   sub_3_18B0
        add    esp, 0xc
        push   esi
        mov    eax, dword ptr [esp + 0x20]
        push   0xa9                          ; -> loc_1_00A9
        push   dword ptr [eax + 0x1c]
        call   sub_1_00B5
loc_3_1716:
        sub    eax, eax
loc_3_1718:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x2c
        ret    
        db  000h
sub_3_1720:
        mov    eax, dword ptr [esp + 8]
        push   eax
        mov    dword ptr [0xa3c], eax        ; -> loc_4_0A3C
        push   dword ptr [esp + 8]
        push   0xa2c                         ; -> loc_4_0A2C
        call   sub_3_18B0
        add    esp, 0xc
        push   0
        push   0x1754                        ; -> loc_3_1754
        push   0
        push   0x40
        push   0
        push   0
        call   sub_1_00D1
        sub    eax, eax
        ret    8
loc_3_1754:
        sub    esp, 4
        lea    eax, [esp]
        push   eax
        push   0xa40                         ; -> loc_4_0A40
        push   0x80000002
        call   sub_3_1824
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_179E
        push   dword ptr [0xa3c]             ; -> loc_4_0A3C
        push   0xa2c                         ; -> loc_4_0A2C
        push   1
        push   0
        push   0xa54                         ; -> obj4:0x0A54
        push   dword ptr [esp + 0x14]
        call   sub_3_1834
        add    esp, 0x18
        push   dword ptr [esp]
        call   sub_3_182C
        add    esp, 4
loc_3_179E:
        add    esp, 4
        ret    
loc_3_17A2:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x28]
        mov    dword ptr [esp + 0x10], 0
        mov    ebx, dword ptr [esi]
        mov    dword ptr [esi], 0
        mov    edi, dword ptr [esp + 0x1c]
        mov    ebp, dword ptr [esp + 0x20]
        or     ebp, ebp
        je     loc_3_1810
loc_3_17C9:
        mov    ecx, dword ptr [edi + 4]
        mov    eax, dword ptr [ecx + 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi], eax
        add    dword ptr [esi], eax
        mov    eax, dword ptr [edi]
        cmp    ebx, eax
        jb     loc_3_17FB
        push   eax
        push   dword ptr [ecx]
        push   dword ptr [esp + 0x2c]
        call   sub_3_18B0
        mov    eax, dword ptr [esp + 0x30]
        add    esp, 0xc
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [edi]
        add    dword ptr [esp + 0x24], eax
        sub    ebx, dword ptr [edi]
        jmp    loc_3_180A
loc_3_17FB:
        mov    dword ptr [edi + 8], 0
        mov    dword ptr [esp + 0x10], 0xea
loc_3_180A:
        add    edi, 0xc
        dec    ebp
        jne    loc_3_17C9
loc_3_1810:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 0C3h, 000h, 000h, 000h
sub_3_1824:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        add    byte ptr [eax], al
sub_3_182C:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        add    byte ptr [eax], al
sub_3_1834:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        add    byte ptr [eax], al
sub_3_183C:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_3_1844:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_3_184C:
        VMMCall  VMM._HeapReAllocate             ; cd2050800100
        add    byte ptr [eax], al
sub_3_1854:
        push   esi
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    0xc
        db  000h, 000h, 000h
sub_3_1870:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    4
        db  000h
sub_3_1880:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf800100
        add    byte ptr [eax], al
sub_3_1888:
        VMMCall  VMM._EnterMutex                 ; cd202e810100
        add    byte ptr [eax], al
sub_3_1890:
        VMMCall  VMM._LeaveMutex                 ; cd202f810100
        add    byte ptr [eax], al
sub_3_1898:
        VMMCall  VMM._SignalID                   ; cd2027810100
        add    byte ptr [eax], al
sub_3_18A0:
        VMMCall  VMM._lstrcpyn                   ; cd207b810100
        add    byte ptr [eax], al
sub_3_18A8:
        VMMCall  VMM._lstrlen                    ; cd207c810100
        add    byte ptr [eax], al
sub_3_18B0:
        VMMCall  VMM._lmemcpy                    ; cd207d810100
        add    byte ptr [eax], al
sub_3_18B8:
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    eax, edi
        pop    edi
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0xa64  RW-
sub_4_0000:
        insd   dword ptr es:[edi], dx
        push   cs
        add    byte ptr [eax], al
        jg     loc_4_0014
        add    byte ptr [eax], al
        or     dword ptr [edi], ecx
        add    byte ptr [eax], al
        push   ds
        sldt   word ptr [eax]
        cmp    byte ptr [edi], cl
        add    byte ptr [eax], al
loc_4_0014:
        push   ebx
        adc    byte ptr [eax], al
        add    dl, al                        ; -> loc_3_0EC2
        push   cs
        add    byte ptr [eax], al
        jb     0x2d
        add    byte ptr [eax], al
loc_4_0020:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0024:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0028:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_002C:
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
loc_4_004C:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0050:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0054:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0058:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_005C:
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
loc_4_0890:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_4_0894:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0898:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_089C:
        dec    ebp
        add    byte ptr [ebx], dl
        push   ebx
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_08A8:
        pop    esp
        dec    ebp
        inc    ecx
        dec    ecx
        dec    esp
        push   ebx
        dec    esp
        dec    edi
        push   esp
        pop    esp
        dec    esi
        inc    ebp
        push   esp
        pop    esp
        dec    esi
        push   esp
        dec    esp
        dec    edi
        inc    edi
        dec    edi
        dec    esi
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr fs:[ebp], ch
        imul   eax, dword ptr [eax], 0x69006e
        jae    loc_4_08CE
loc_4_08CE:
        je     loc_4_08D0
loc_4_08D0:
        jb     loc_4_08D2
loc_4_08D2:
        popal  
        add    byte ptr [eax + eax + 0x6f], dh
        add    byte ptr [edx], dh
        add    byte ptr [eax], al
loc_4_08DC:
        pop    esp
        dec    ebp
        inc    ecx
        dec    ecx
        dec    esp
        push   ebx
        dec    esp
        dec    edi
        push   esp
        pop    esp
        dec    ebp
        push   ebx
        push   ebx
        push   eax
        add    byte ptr [eax], al
loc_4_08EC:
        pop    esp
        dec    ebp
        inc    ecx
        dec    ecx
        dec    esp
        push   ebx
        dec    esp
        dec    edi
        push   esp
        pop    esp
        dec    ebp
        push   ebx
        push   ebx
        push   eax
        add    byte ptr [eax], al
loc_4_08FC:
        add    al, 0
        add    byte ptr [eax], al
loc_4_0900:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0904:
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
loc_4_0A08:
        add    al, 1
        add    byte ptr [eax], al
loc_4_0A0C:
        dec    esp
        add    al, byte ptr [eax]
        add    ah, bh                        ; -> loc_4_08FC
        or     byte ptr [eax], al
        add    byte ptr [edx + eax], dl      ; -> obj1:0x0254
        add    byte ptr [eax], al            ; -> loc_4_0900
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx + ecx], al      ; -> loc_4_0904
        add    byte ptr [eax], al
        or     byte ptr [edx], cl
        add    byte ptr [eax], al
loc_4_0A2C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0A3C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0A40:
        push   ebx
        arpl   word ptr gs:[ebp + 0x72], si
        imul   esi, dword ptr [ecx + edi*2 + 0x5c], 0x766f7250
        imul   esp, dword ptr [ebp + 0x72], 0x41000000
        jb     0xabe
        jae    0xace
        pop    edi
        push   ebx
        jb     0xad6
        jb     0xa90
        push   eax
VxD__LDATA ENDS
