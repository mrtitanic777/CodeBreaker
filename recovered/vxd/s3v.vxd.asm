; S3MINI -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 2597 instructions recovered by recursive descent

Declare_Virtual_Device S3MINI, 3, 1, S3MINI_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x484  R-X
S3MINI_Control:
        cmp    eax, 0x24
        jae    loc_1_000C
        jmp    dword ptr [eax*4 + 0xe]       ; -> obj1:0x000E
loc_1_000C:
        clc    
        ret    
        ; ---- dispatch table (36 entries) ----
        dd  loc_2_0000
        dd  loc_2_001B
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_0159
        dd  loc_1_000C
        dd  loc_1_01B1
        dd  loc_1_000C
        dd  loc_1_01ED
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_2_001B
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_000C
        dd  loc_1_009E
loc_1_009E:
        pushal 
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, -1
        je     loc_1_00B8
        cmp    ecx, 0
        je     loc_1_00BA
        cmp    ecx, 0xa
        je     loc_1_00BC
        cmp    ecx, 0xb
        je     loc_1_00D8
        jae    loc_1_0110
loc_1_00B8:
        jmp    loc_1_011D
loc_1_00BA:
        jmp    loc_1_011D
loc_1_00BC:
        mov    edx, dword ptr [esi + 0x10]
        mov    dword ptr [0x452], edx        ; -> loc_1_0452
        mov    edx, dword ptr [esi + 0x18]
        mov    dword ptr [0x456], edx        ; -> loc_1_0456
        mov    ebx, dword ptr [esi + 4]
        call   sub_1_0131
        jmp    loc_1_011D
loc_1_00D8:
        mov    edx, dword ptr [esi + 0x10]
        mov    ebx, dword ptr [edx]
        shr    ebx, 0xc
        mov    ecx, 1
        mov    eax, 0x45a                    ; -> loc_1_045A
        push   0
        push   eax
        push   ecx
        push   ebx
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        or     eax, eax
        je     loc_1_0110
        mov    ebx, 0x45a                    ; -> loc_1_045A
        mov    edx, dword ptr [esi + 0x18]
        mov    ebx, dword ptr [ebx]
        and    ebx, 0xfffff000
        mov    dword ptr [edx], ebx
        jmp    loc_1_011D
loc_1_0110:
        mov    dword ptr [0x44e], 0x32       ; -> loc_1_044E
        stc    
        jmp    loc_1_012A
loc_1_011D:
        mov    dword ptr [0x44e], 0          ; -> loc_1_044E
        clc    
        jmp    loc_1_012A
loc_1_012A:
        popal  
        mov    eax, dword ptr [0x44e]        ; -> loc_1_044E
        ret    
sub_1_0131:
        pushal 
        mov    edx, dword ptr [0x452]        ; -> loc_1_0452
        mov    eax, dword ptr [edx]
        mov    edx, dword ptr [edx + 4]
        push   0
        push   edx
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_0157
        mov    edx, dword ptr [0x456]        ; -> loc_1_0456
        mov    dword ptr [edx], eax
loc_1_0157:
        popal  
        ret    
loc_1_0159:
        cmp    dword ptr [0x42f], 0x883d00   ; -> obj1:0x042F
        je     loc_1_0171
        cmp    dword ptr [0x42f], 0x563100   ; -> obj1:0x042F
        jne    loc_1_01B0
loc_1_0171:
        cmp    dword ptr [0x3b8], 0          ; -> loc_1_03B8
        je     loc_1_01B0
        mov    eax, dword ptr [0x3bc]        ; -> loc_1_03BC
        push   0
        push   eax
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        mov    dword ptr [0x3bc], 0          ; -> loc_1_03BC
        mov    eax, dword ptr [0x3b8]        ; -> loc_1_03B8
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        mov    dword ptr [0x3b8], 0          ; -> loc_1_03B8
loc_1_01B0:
        ret    
loc_1_01B1:
        test   byte ptr [0x438], 1           ; -> loc_1_0438
        je     loc_1_01EC
        mov    byte ptr [0x439], 1           ; -> obj1:0x0439
        inc    byte ptr [0x437]              ; -> obj1:0x0437
        mov    eax, dword ptr [0x3a0]        ; -> obj1:0x03A0
        add    eax, dword ptr [0x3ac]        ; -> obj1:0x03AC
        mov    edx, dword ptr [eax]
        test   edx, 0xf
        je     loc_1_01E2
        mov    dword ptr [0x43c], edx        ; -> loc_1_043C
loc_1_01E2:
        mov    edx, 0x4022
        call   sub_1_0254
loc_1_01EC:
        ret    
loc_1_01ED:
        test   byte ptr [0x438], 1           ; -> loc_1_0438
        je     loc_1_0216
        VxDCall  SHELL.SHELL_GetVMInfo           ; cd2005001700
        test   eax, 4
        jne    loc_1_0216
        mov    edx, 0x4021
        call   sub_1_0254
        mov    byte ptr [0x437], 0           ; -> obj1:0x0437
        jmp    loc_1_0216
loc_1_0216:
        ret    
sub_1_0217:
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], dx
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        ret    
sub_1_0254:
        pushal 
        cmp    ebx, dword ptr [0x378]        ; -> obj1:0x0378
        je     loc_1_0277
        mov    eax, 0x400000
        mov    ebx, dword ptr [0x378]        ; -> obj1:0x0378
        xor    ecx, ecx
        mov    esi, 0x217                    ; -> sub_1_0217
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        jmp    loc_1_027C
loc_1_0277:
        call   sub_1_0217
loc_1_027C:
        popal  
        ret    
loc_1_027E:
        cmp    ebx, dword ptr [0x378]        ; -> obj1:0x0378
        je     loc_1_02DD
        cmp    edx, 0x9ae8
        je     loc_1_0296
        cmp    edx, 0x9ae9
        jne    loc_1_02DD
loc_1_0296:
        cmp    ecx, 0
        je     loc_1_02BE
        cmp    ecx, 8
        jne    loc_1_02DD
        mov    ecx, 0xffff
loc_1_02A5:
        in     ax, dx
        cmp    ax, -1
        je     loc_1_02B9
        test   ah, 2
        je     loc_1_02DA
        call   sub_4_0CA2
        loop   loc_1_02A5
loc_1_02B9:
        and    ah, 0xfd
        jmp    loc_1_02DA
loc_1_02BE:
        cmp    dl, 0xe9
        jne    loc_1_02DD
        mov    ecx, 0xffff
loc_1_02C8:
        in     al, dx
        cmp    al, 0xff
        je     loc_1_02D8
        test   al, 2
        je     loc_1_02DA
        call   sub_4_0CA2
        loop   loc_1_02C8
loc_1_02D8:
        and    al, 0xfd
loc_1_02DA:
        stc    
        jmp    loc_1_02DE
loc_1_02DD:
        clc    
loc_1_02DE:
        ret    
loc_1_02DF:
        cmp    dx, 0x4f00
        je     loc_1_02FC
        cmp    dx, 0x4f02
        jne    loc_1_0321
        cmp    ebx, dword ptr [0x378]        ; -> obj1:0x0378
        je     loc_1_0321
        call   sub_4_05A4
        jmp    loc_1_0321
loc_1_02FC:
        cmp    ebx, dword ptr [0x378]        ; -> obj1:0x0378
        jne    loc_1_0321
        push   eax
        push   edx
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_1_0321:
        ret    
        db  000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'S3MINI  '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  S3MINI_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_03B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_03E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_042A:
        add    byte ptr [eax], al
loc_1_042C:
        add    byte ptr [eax], al
loc_1_042E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0434:
        add    byte ptr [eax], al
loc_1_0436:
        add    byte ptr [eax], al
loc_1_0438:
        add    byte ptr [eax], al
loc_1_043A:
        add    byte ptr [eax], al
loc_1_043C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0440:
        add    byte ptr [eax], al
loc_1_0442:
        add    bh, bh
loc_1_0444:
        add    byte ptr [eax], al
loc_1_0446:
        add    bh, bh
loc_1_0448:
        add    byte ptr [eax], al
loc_1_044A:
        add    byte ptr [eax], al
loc_1_044C:
        add    byte ptr [eax], al
loc_1_044E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0452:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0456:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_045A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0462:
        inc    ebp
        dec    esi
        push   ebp
        dec    ebp
        pop    esp
        push   eax
        inc    ebx
        dec    ecx
        pop    esp
        push   esi
        inc    ebp
        dec    esi
        pop    edi
        cmp    byte ptr [eax], dh
        cmp    byte ptr [esi], dh
        inc    esp
        inc    ebp
        push   esi
        pop    edi
        xor    byte ptr [eax + edi], dh
        add    byte ptr ss:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x6ff  R-X
loc_2_0000:
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        rol    eax, 8
        out    dx, al
        clc    
        ret    
loc_2_001B:
        push   ebx
        mov    dword ptr [0x378], ebx        ; -> obj1:0x0378
        mov    byte ptr [0x436], 0           ; -> loc_1_0436
        pushal 
        mov    eax, 0x20
        mov    esi, 0x6ec                    ; -> loc_2_06EC
        mov    edi, 0x6f4                    ; -> loc_2_06F4
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        cmp    eax, 0x20
        je     loc_2_004E
        cmp    eax, 0x40
        je     loc_2_004E
        mov    eax, 0x20
loc_2_004E:
        shl    eax, 0xa
        mov    dword ptr [0x374], eax        ; -> obj1:0x0374
        popal  
        push   0
        push   0x10000
        push   0xa0000
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_2_06E2
        mov    dword ptr [0x388], eax        ; -> obj1:0x0388
        mov    dword ptr [0x3a4], 0          ; -> obj1:0x03A4
        mov    dword ptr [0x3a8], 0x9ae8     ; -> obj1:0x03A8
        xor    eax, eax
        mov    edx, 0x3d4
        mov    ax, 0x4838
        out    dx, ax
        mov    al, 0x36
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 3
        mov    byte ptr [0x435], al          ; -> obj1:0x0435
        dec    edx
        mov    al, 0x30
        out    dx, al
        inc    dx
        in     al, dx
        cmp    al, 0xe1
        je     loc_2_0106
        mov    ch, al
        and    ch, 0xf0
        mov    byte ptr [0x42f], ch          ; -> obj1:0x042F
        and    al, 0xf
        mov    byte ptr [0x433], al          ; -> obj1:0x0433
        cmp    ch, 0x80
        je     loc_2_0246
        cmp    ch, 0x90
        je     loc_2_0246
        cmp    ch, 0xb0
        je     loc_2_0246
        cmp    ch, 0xa0
        je     loc_2_0246
        mov    byte ptr [0x434], 0xff        ; -> loc_1_0434
        cmp    ch, 0xc0
        je     loc_2_0246
        cmp    ch, 0xd0
        je     loc_2_0246
        jmp    loc_2_06E2
loc_2_0106:
        mov    byte ptr [0x434], 0xff        ; -> loc_1_0434
        or     dword ptr [0x3a4], 1          ; -> obj1:0x03A4
        xor    eax, eax
        dec    dl
        mov    al, 0x2d
        out    dx, al
        inc    dl
        in     al, dx
        shl    eax, 8
        dec    dl
        mov    al, 0x2e
        out    dx, al
        inc    dl
        in     al, dx
        shl    eax, 8
        dec    dl
        mov    al, 0x2f
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [0x433], al          ; -> obj1:0x0433
        and    al, 0x40
        cmp    eax, 0x881140
        jne    loc_2_0174
        mov    dword ptr [0x42f], eax        ; -> obj1:0x042F
        or     byte ptr [0x438], 1           ; -> loc_1_0438
        mov    dword ptr [0x3ac], 0x81ec     ; -> obj1:0x03AC
        cmp    byte ptr [0x435], 1           ; -> obj1:0x0435
        je     loc_2_023F
        mov    dword ptr [0x3a8], 0x8118     ; -> obj1:0x03A8
        jmp    loc_2_023F
loc_2_0174:
        and    al, 0
        mov    dword ptr [0x42f], eax        ; -> obj1:0x042F
        cmp    eax, 0x881000
        je     loc_2_0189
        cmp    eax, 0x881100
        jne    loc_2_01C6
loc_2_0189:
        cmp    byte ptr [0x433], 0x20        ; -> obj1:0x0433
        je     loc_2_023F
        cmp    byte ptr [0x433], 0x30        ; -> obj1:0x0433
        je     loc_2_023F
        cmp    byte ptr [0x433], 3           ; -> obj1:0x0433
        je     loc_2_023F
        cmp    byte ptr [0x433], 0x12        ; -> obj1:0x0433
        je     loc_2_023F
        cmp    byte ptr [0x433], 0x22        ; -> obj1:0x0433
        je     loc_2_023F
loc_2_01C6:
        mov    dword ptr [0x3a8], 0x8118     ; -> obj1:0x03A8
        cmp    eax, 0x888000
        je     loc_2_023F
        cmp    eax, 0x889000
        je     loc_2_023F
        cmp    eax, 0x88b000
        je     loc_2_023F
        cmp    eax, 0x88f000
        je     loc_2_023F
        or     byte ptr [0x438], 1           ; -> loc_1_0438
        mov    dword ptr [0x3ac], 0x81ec     ; -> obj1:0x03AC
        cmp    eax, 0x881200
        je     loc_2_023F
        cmp    eax, 0x881500
        je     loc_2_023F
        cmp    eax, 0x881400
        je     loc_2_023F
        or     byte ptr [0x438], 5           ; -> loc_1_0438
        mov    dword ptr [0x3a8], 0x8504     ; -> obj1:0x03A8
        mov    dword ptr [0x3ac], 0x8200     ; -> obj1:0x03AC
        cmp    eax, 0x563100
        je     loc_2_023F
        cmp    eax, 0x883d00
        jne    loc_2_06E2
loc_2_023F:
        or     dword ptr [0x3a4], 2          ; -> obj1:0x03A4
loc_2_0246:
        dec    dl
        mov    al, 0x30
        out    dx, al
        inc    dl
        in     al, dx
        inc    al
        mov    ah, al
        out    dx, al
        in     al, dx
        cmp    al, ah
        je     loc_2_06E2
        dec    dl
        mov    eax, 0x3e0                    ; -> loc_1_03E0
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        push   eax
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        pop    eax
        mov    eax, dword ptr [eax + 8]
        cmp    eax, 0x315f3353
        je     loc_2_02F3
        or     eax, 0x20
        cmp    eax, 0x315f3373
        je     loc_2_02F3
        or     eax, 0x20202020
        cmp    eax, 0x642e3373
        je     loc_2_02F3
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_2_06E2
        cmp    dword ptr [0x42f], 0x90       ; -> obj1:0x042F
        je     loc_2_06E2
        cmp    dword ptr [0x42f], 0xb0       ; -> obj1:0x042F
        je     loc_2_06E2
        cmp    dword ptr [0x42f], 0xa0       ; -> obj1:0x042F
        je     loc_2_06E2
        cmp    dword ptr [0x42f], 0xc0       ; -> obj1:0x042F
        je     loc_2_06E2
        cmp    dword ptr [0x42f], 0xd0       ; -> obj1:0x042F
        je     loc_2_06E2
loc_2_02F3:
        mov    byte ptr [0x481], 0           ; -> obj1:0x0481
        mov    eax, 0x462                    ; -> loc_1_0462
        mov    edx, 0x47d                    ; -> obj1:0x047D
        push   edx
        push   eax
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        cmp    eax, 0
        jne    loc_2_032E
        inc    byte ptr [0x481]              ; -> obj1:0x0481
        push   dword ptr [0x47d]             ; -> obj1:0x047D
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_2_032E:
        mov    dx, 0x3d4
        mov    al, 0x36
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
        mov    cl, al
        cmp    dword ptr [0x42f], 0x883d00   ; -> obj1:0x042F
        jne    loc_2_036F
        shr    cl, 5
        and    cl, 3
        shl    cl, 1
        add    cl, 2
        mov    al, 0x37
        out    dx, al
        inc    dx
        in     al, dx
        dec    dx
        shr    al, 5
        and    al, 3
        xor    al, 3
        shl    al, 1
        sub    cl, al
        xor    eax, eax
        mov    al, cl
        shl    eax, 0x14
        jmp    loc_2_03D4
loc_2_036F:
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_2_03C5
        shr    cl, 5
        mov    eax, 0x400000
        or     cl, cl
        je     loc_2_03D4
        mov    eax, 0x300000
        cmp    cl, 2
        je     loc_2_03D4
        mov    eax, 0x800000
        cmp    cl, 3
        je     loc_2_03D4
        mov    eax, 0x200000
        cmp    cl, 4
        je     loc_2_03D4
        mov    eax, 0x100000
        add    eax, 0x80000
        cmp    cl, 5
        je     loc_2_03D4
        mov    eax, 0x100000
        cmp    cl, 6
        je     loc_2_03D4
        mov    eax, 0x80000
        jmp    loc_2_03D4
loc_2_03C5:
        mov    eax, 0x100000
        test   cl, 0x20
        je     loc_2_03D4
        mov    eax, 0x80000
loc_2_03D4:
        mov    dword ptr [0x384], eax        ; -> obj1:0x0384
        push   0
        push   0x10
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0x38c], eax        ; -> obj1:0x038C
        or     eax, eax
        je     loc_2_06E2
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_06E2
        mov    dword ptr [edi], 0            ; -> loc_3_0000
        mov    dword ptr [edi + 4], 0        ; -> loc_4_0000
        mov    dword ptr [edi + 8], 0x171    ; -> loc_4_0171
        mov    dword ptr [edi + 0xc], 0x1ff  ; -> loc_4_01FF
        mov    dword ptr [edi + 0x58], 0x2b4 ; -> loc_4_02B4
        mov    dword ptr [edi + 0x5c], 0x335 ; -> loc_4_0335
        mov    dword ptr [edi + 0x10], 0x425 ; -> loc_4_0425
        mov    dword ptr [edi + 0x14], 0x505 ; -> loc_4_0505
        mov    dword ptr [edi + 0x18], 0x548 ; -> loc_4_0548
        mov    dword ptr [edi + 0x1c], 0x55a ; -> loc_4_055A
        mov    dword ptr [edi + 0x20], 0x5a4 ; -> sub_4_05A4
        mov    dword ptr [edi + 0x24], 0x622 ; -> loc_4_0622
        mov    dword ptr [edi + 0x2c], 0x856 ; -> loc_4_0856
        mov    dword ptr [edi + 0x30], 0x8a6 ; -> loc_4_08A6
        mov    dword ptr [edi + 0x34], 0x937 ; -> loc_4_0937
        mov    dword ptr [edi + 0x38], 0xa36 ; -> sub_4_0A36
        mov    dword ptr [edi + 0x3c], 0xb40 ; -> loc_4_0B40
        mov    dword ptr [edi + 0x40], 0xcae ; -> loc_4_0CAE
        mov    dword ptr [edi + 0x44], 0xd03 ; -> loc_4_0D03
        mov    dword ptr [edi + 0xb8], 0x788 ; -> loc_3_0788
        mov    dword ptr [edi + 0xb4], 0x74e ; -> loc_3_074E
        mov    dword ptr [edi + 0x68], 0x70e ; -> loc_3_070E
        mov    dword ptr [edi + 0x70], 0x106 ; -> loc_3_0106
        mov    dword ptr [edi + 0x74], 0x6e8 ; -> loc_3_06E8
        mov    dword ptr [edi + 0x90], 0xdc3 ; -> loc_4_0DC3
        mov    dword ptr [edi + 0x80], 0xdcb ; -> loc_4_0DCB
        mov    dword ptr [edi + 0x84], 0xdcb ; -> loc_4_0DCB
        mov    dword ptr [edi + 0x94], 0xe1b ; -> loc_4_0E1B
        mov    dword ptr [edi + 0x88], 0xf62 ; -> loc_4_0F62
        mov    dword ptr [edi + 0x9c], 0xe24 ; -> loc_4_0E24
        mov    dword ptr [edi + 0xa0], 0xee6 ; -> loc_4_0EE6
        mov    dword ptr [edi + 0xa8], 0x6e5 ; -> loc_2_06E5
        mov    dword ptr [edi + 0xb0], 0x27e ; -> loc_1_027E
        mov    dword ptr [edi + 0xa4], 0xfd6 ; -> loc_4_0FD6
        mov    dword ptr [edi + 0xbc], 0x2df ; -> loc_1_02DF
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        jne    loc_2_0533
        mov    dword ptr [edi + 0x98], 0xfc5 ; -> loc_4_0FC5
loc_2_0533:
        mov    edx, 0x3c2
        mov    ecx, 0xdab                    ; -> loc_4_0DAB
        VxDCall  VDD.VDD_Takeover_VGA_Port       ; cd2013000a00
        mov    dword ptr [0x3b0], ecx        ; -> loc_1_03B0
        VxDCall  VDD.VDD_Get_Special_VM_IDs      ; cd2011000a00
        mov    dword ptr [0x37c], esi        ; -> obj1:0x037C
        mov    dword ptr [0x380], edi        ; -> obj1:0x0380
        cmp    dword ptr [0x42f], 0x883d00   ; -> obj1:0x042F
        je     loc_2_0573
        cmp    dword ptr [0x42f], 0x563100   ; -> obj1:0x042F
        jne    loc_2_05DA
loc_2_0573:
        cmp    dword ptr [0x3b8], 0          ; -> loc_1_03B8
        jne    loc_2_05D5
        push   0x8f
        push   0x3b4                         ; -> loc_1_03B4
        push   0x100
        push   0
        push   0xf
        push   0
        push   1
        push   0x10
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        test   eax, eax
        je     loc_2_05D5
        mov    dword ptr [0x3b8], eax        ; -> loc_1_03B8
        push   1
        push   0
        push   0xf2
        push   0x10000
        push   eax
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        test   eax, eax
        je     loc_2_05D5
        mov    dword ptr [0x3bc], eax        ; -> loc_1_03BC
loc_2_05D5:
        jmp    loc_2_06DA
loc_2_05DA:
        mov    edx, 0x9ae8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xe2e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xbee8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x96e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x86e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x82e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x8ee8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x8ae8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xb6e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xbae8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xaae8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xaee8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xa2e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xa6e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0x92e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    edx, 0xb2e8
        mov    cl, 2
        VxDCall  VDD.VDD_Register_Virtual_Port   ; cd200f000a00
        mov    esi, 0xb1f                    ; -> loc_4_0B1F
        mov    edx, 0x4ae8
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_06E2
        mov    edx, 0x4ae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_2_06E2
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
loc_2_06DA:
        mov    eax, dword ptr [0x42f]        ; -> obj1:0x042F
        clc    
        jmp    loc_2_06E3
loc_2_06E2:
        stc    
loc_2_06E3:
        pop    ebx
        ret    
loc_2_06E5:
        mov    eax, dword ptr [0x42f]        ; -> obj1:0x042F
        ret    
        db  000h
loc_2_06EC:
        inc    esp
        imul   esi, dword ptr [ebx + 0x70], 0x79616c
loc_2_06F4:
        push   esi
        inc    esp
        inc    esp
        push   edx
        jae    0x760
        jb     0x773
        db  065h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x7aa  R-X
loc_3_0000:
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [0x390], eax        ; -> obj1:0x0390
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [0x394], eax        ; -> obj1:0x0394
        mov    ax, 0x3c00
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [0x398], eax        ; -> obj1:0x0398
        mov    ecx, dword ptr [ebp + 0x18]
        mov    al, 0x5a
        mov    ah, cl
        mov    word ptr [0x42a], ax          ; -> loc_1_042A
        mov    al, 0x59
        mov    ah, ch
        mov    word ptr [0x42c], ax          ; -> loc_1_042C
        shr    ecx, 0x10
        mov    byte ptr [0x44c], cl          ; -> loc_1_044C
        mov    byte ptr [0x42e], ch          ; -> loc_1_042E
        mov    eax, dword ptr [ebp + 4]
        shr    eax, 0x10
        or     ax, ax
        je     loc_3_009D
        cmp    ax, word ptr [0x39e]          ; -> obj1:0x039E
        je     loc_3_009D
        mov    word ptr [0x39e], ax          ; -> obj1:0x039E
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
        test   dword ptr [0x3a4], 2          ; -> obj1:0x03A4
        je     loc_3_009D
        movzx  eax, word ptr [0x39e]         ; -> obj1:0x039E
        push   0
        push   eax
        push   dword ptr [0x378]             ; -> obj1:0x0378
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        mov    dword ptr [0x3a0], eax        ; -> obj1:0x03A0
loc_3_009D:
        mov    eax, dword ptr [ebp]
        shr    eax, 0x10
        or     ax, ax
        je     loc_3_00BD
        cmp    ax, word ptr [0x39c]          ; -> obj1:0x039C
        je     loc_3_00BD
        mov    word ptr [0x39c], ax          ; -> obj1:0x039C
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
loc_3_00BD:
        mov    edx, 0x3cc
        in     al, dx
        mov    byte ptr [0x44d], al          ; -> obj1:0x044D
        cmp    dword ptr [0x42f], 0x883d00   ; -> obj1:0x042F
        je     loc_3_00E0
        cmp    dword ptr [0x42f], 0x563100   ; -> obj1:0x042F
        jne    loc_3_0105
loc_3_00E0:
        xor    eax, eax
        mov    ax, word ptr [0x3bc]          ; -> loc_1_03BC
        test   ax, ax
        je     loc_3_00FA
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
        movzx  eax, word ptr [0x3bc]         ; -> loc_1_03BC
loc_3_00FA:
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [0x3b8]        ; -> loc_1_03B8
        mov    dword ptr [ebp + 0x18], eax
loc_3_0105:
        ret    
loc_3_0106:
        mov    word ptr [0x43a], 0           ; -> loc_1_043A
        mov    eax, 0x3e0                    ; -> loc_1_03E0
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        push   eax
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        pop    eax
        test   word ptr [eax + 2], 0x110
        jne    loc_3_06C9
        movzx  edi, word ptr [eax + 0x20]
        or     edi, edi
        je     loc_3_06C9
        movzx  esi, word ptr [eax + 0x18]
        or     esi, esi
        je     loc_3_06C9
        mov    edx, 0x3d4
        in     al, dx
        push   eax
        mov    eax, 0xa539
        out    dx, ax
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_3_0286
        cmp    dword ptr [0x42f], 0xa0       ; -> obj1:0x042F
        je     loc_3_02DF
        cmp    dword ptr [0x42f], 0xa0       ; -> obj1:0x042F
        je     loc_3_02DF
        cmp    dword ptr [0x42f], 0x90       ; -> obj1:0x042F
        je     loc_3_02DF
        cmp    dword ptr [0x42f], 0xb0       ; -> obj1:0x042F
        je     loc_3_02DF
        cmp    dword ptr [0x42f], 0xc0       ; -> obj1:0x042F
        je     loc_3_0340
        cmp    dword ptr [0x42f], 0xd0       ; -> obj1:0x042F
        je     loc_3_0340
        cmp    dword ptr [0x42f], 0x881100   ; -> obj1:0x042F
        je     loc_3_0340
        cmp    dword ptr [0x42f], 0x881140   ; -> obj1:0x042F
        je     loc_3_0340
        cmp    dword ptr [0x42f], 0x881000   ; -> obj1:0x042F
        je     loc_3_0340
        cmp    dword ptr [0x42f], 0x888000   ; -> obj1:0x042F
        je     loc_3_0340
        cmp    dword ptr [0x42f], 0x889000   ; -> obj1:0x042F
        je     loc_3_0340
        cmp    dword ptr [0x42f], 0x88b000   ; -> obj1:0x042F
        je     loc_3_0340
        cmp    dword ptr [0x42f], 0x88f000   ; -> obj1:0x042F
        je     loc_3_0340
        cmp    dword ptr [0x42f], 0x881200   ; -> obj1:0x042F
        je     loc_3_0340
        cmp    dword ptr [0x42f], 0x881500   ; -> obj1:0x042F
        je     loc_3_0340
        cmp    dword ptr [0x42f], 0x881400   ; -> obj1:0x042F
        je     loc_3_0340
        cmp    dword ptr [0x42f], 0x883d00   ; -> obj1:0x042F
        je     loc_3_051D
        cmp    dword ptr [0x42f], 0x563100   ; -> obj1:0x042F
        je     loc_3_051D
loc_3_0286:
        mov    al, 0x41
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0xf8
        cmp    esi, 0x280
        je     loc_3_02CE
        cmp    esi, 0x320
        je     loc_3_02BC
        cmp    esi, 0x400
        je     loc_3_02AA
        mov    ah, 7
        jmp    loc_3_02D7
loc_3_02AA:
        mov    ah, 7
        cmp    edi, 0x46
        jae    loc_3_02D7
        mov    ah, 6
        cmp    edi, 0x3c
        jae    loc_3_02D7
        mov    ah, 5
        jmp    loc_3_02D7
loc_3_02BC:
        mov    ah, 3
        cmp    edi, 0x48
        jae    loc_3_02D7
        mov    ah, 2
        cmp    edi, 0x3c
        jae    loc_3_02D7
        mov    ah, 4
        jmp    loc_3_02D7
loc_3_02CE:
        mov    ah, 1
        cmp    edi, 0x48
        jae    loc_3_02D7
        xor    ah, ah
loc_3_02D7:
        or     al, ah
        out    dx, al
        jmp    loc_3_06C6
loc_3_02DF:
        mov    ch, 3
        cmp    esi, 0x280
        je     loc_3_0315
        mov    ch, 0xc
        cmp    esi, 0x320
        je     loc_3_0315
        mov    ch, 0x30
        cmp    esi, 0x400
        je     loc_3_0315
        mov    ch, 0x30
        cmp    esi, 0x480
        je     loc_3_0315
        mov    ch, 0xc0
        cmp    esi, 0x500
        jne    loc_3_06C6
loc_3_0315:
        mov    cl, ch
        not    cl
        mov    ah, 0xff
        cmp    edi, 0x48
        jae    loc_3_0330
        mov    ah, 0xaa
        cmp    edi, 0x3c
        jae    loc_3_0330
        mov    ah, 0x55
        cmp    edi, 0x38
        jae    loc_3_0330
        xor    ah, ah
loc_3_0330:
        mov    al, 0x52
        out    dx, al
        inc    edx
        in     al, dx
        and    ax, cx
        or     al, ah
        out    dx, al
        jmp    loc_3_06C6
loc_3_0340:
        mov    ah, 0x41
        cmp    dword ptr [0x42f], 0x881100   ; -> obj1:0x042F
        je     loc_3_038C
        cmp    dword ptr [0x42f], 0x881000   ; -> obj1:0x042F
        je     loc_3_038C
        cmp    dword ptr [0x42f], 0x881140   ; -> obj1:0x042F
        je     loc_3_038C
        cmp    dword ptr [0x42f], 0x881200   ; -> obj1:0x042F
        je     loc_3_038C
        cmp    dword ptr [0x42f], 0x881500   ; -> obj1:0x042F
        je     loc_3_038C
        cmp    dword ptr [0x42f], 0x881400   ; -> obj1:0x042F
        je     loc_3_038C
        mov    ah, 0x5b
loc_3_038C:
        cmp    esi, 0x280
        jne    loc_3_03E6
        mov    cx, 0x8f30
        mov    word ptr [0x43a], 0x55        ; -> loc_1_043A
        cmp    edi, 0x55
        jae    loc_3_03D7
        mov    cx, 0x8f20
        mov    word ptr [0x43a], 0x4b        ; -> loc_1_043A
        cmp    edi, 0x4b
        jae    loc_3_03D7
        mov    cx, 0x8f10
        mov    word ptr [0x43a], 0x48        ; -> loc_1_043A
        cmp    edi, 0x48
        jae    loc_3_03D7
        mov    cx, 0x8f00
        mov    word ptr [0x43a], 0x3c        ; -> loc_1_043A
loc_3_03D7:
        mov    al, 0x52
        out    dx, al
        inc    edx
        in     al, dx
        and    al, ch
        or     al, cl
        out    dx, al
        jmp    loc_3_06C6
loc_3_03E6:
        cmp    esi, 0x320
        jne    loc_3_045D
        mov    cx, 0x200
        mov    word ptr [0x43a], 0x55        ; -> loc_1_043A
        cmp    edi, 0x55
        jae    loc_3_0443
        mov    cx, 0x180
        mov    word ptr [0x43a], 0x4b        ; -> loc_1_043A
        cmp    edi, 0x4b
        jae    loc_3_0443
        mov    cx, 0x100
        mov    word ptr [0x43a], 0x48        ; -> loc_1_043A
        cmp    edi, 0x48
        jae    loc_3_0443
        mov    cx, 0x80
        mov    word ptr [0x43a], 0x3c        ; -> loc_1_043A
        cmp    edi, 0x3c
        jae    loc_3_0443
        mov    cx, 0
        mov    word ptr [0x43a], 0x38        ; -> loc_1_043A
loc_3_0443:
        mov    al, 0x52
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0x7f
        or     al, cl
        out    dx, al
        dec    edx
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xfc
        or     al, ch
        out    dx, al
        jmp    loc_3_06C6
loc_3_045D:
        cmp    esi, 0x400
        je     loc_3_046D
        cmp    esi, 0x480
        jne    loc_3_04C7
loc_3_046D:
        mov    cl, 0x14
        mov    word ptr [0x43a], 0x55        ; -> loc_1_043A
        cmp    edi, 0x55
        jae    loc_3_04B8
        mov    cl, 0x10
        mov    word ptr [0x43a], 0x4b        ; -> loc_1_043A
        cmp    edi, 0x4b
        jae    loc_3_04B8
        mov    cl, 0xc
        mov    word ptr [0x43a], 0x46        ; -> loc_1_043A
        cmp    edi, 0x46
        jae    loc_3_04B8
        mov    cl, 8
        mov    word ptr [0x43a], 0x3c        ; -> loc_1_043A
        cmp    edi, 0x3c
        jae    loc_3_04B8
        mov    cl, 0
        mov    word ptr [0x43a], 0x2b        ; -> loc_1_043A
loc_3_04B8:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xe3
        or     al, cl
        out    dx, al
        jmp    loc_3_06C6
loc_3_04C7:
        cmp    esi, 0x500
        jne    loc_3_06C6
        mov    cl, 0xc0
        mov    word ptr [0x43a], 0x4b        ; -> loc_1_043A
        cmp    edi, 0x4b
        jae    loc_3_050E
        mov    cl, 0xa0
        mov    word ptr [0x43a], 0x48        ; -> loc_1_043A
        cmp    edi, 0x48
        jae    loc_3_050E
        mov    cl, 0x80
        mov    word ptr [0x43a], 0x3c        ; -> loc_1_043A
        cmp    edi, 0x3c
        jae    loc_3_050E
        mov    cl, 0
        mov    word ptr [0x43a], 0x2b        ; -> loc_1_043A
loc_3_050E:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0x1f
        or     al, cl
        out    dx, al
        jmp    loc_3_06C6
loc_3_051D:
        mov    ah, 0x41
        cmp    esi, 0x280
        jne    loc_3_058B
        mov    cx, 0x8f40
        mov    word ptr [0x43a], 0x55        ; -> loc_1_043A
        cmp    edi, 0x55
        jae    loc_3_057C
        mov    cx, 0x8f30
        mov    word ptr [0x43a], 0x4b        ; -> loc_1_043A
        cmp    edi, 0x4b
        jae    loc_3_057C
        mov    cx, 0x8f20
        mov    word ptr [0x43a], 0x46        ; -> loc_1_043A
        cmp    edi, 0x46
        jae    loc_3_057C
        mov    cx, 0x8f10
        mov    word ptr [0x43a], 0x3c        ; -> loc_1_043A
        cmp    edi, 0x3c
        jae    loc_3_057C
        mov    cx, 0x8f00
        mov    word ptr [0x43a], 0x38        ; -> loc_1_043A
loc_3_057C:
        mov    al, 0x52
        out    dx, al
        inc    edx
        in     al, dx
        and    al, ch
        or     al, cl
        out    dx, al
        jmp    loc_3_06C6
loc_3_058B:
        cmp    esi, 0x320
        jne    loc_3_0602
        mov    cx, 0x200
        mov    word ptr [0x43a], 0x55        ; -> loc_1_043A
        cmp    edi, 0x55
        jae    loc_3_05E8
        mov    cx, 0x180
        mov    word ptr [0x43a], 0x4b        ; -> loc_1_043A
        cmp    edi, 0x4b
        jae    loc_3_05E8
        mov    cx, 0x100
        mov    word ptr [0x43a], 0x46        ; -> loc_1_043A
        cmp    edi, 0x46
        jae    loc_3_05E8
        mov    cx, 0x80
        mov    word ptr [0x43a], 0x3c        ; -> loc_1_043A
        cmp    edi, 0x3c
        jae    loc_3_05E8
        mov    cx, 0
        mov    word ptr [0x43a], 0x38        ; -> loc_1_043A
loc_3_05E8:
        mov    al, 0x52
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0x7f
        or     al, cl
        out    dx, al
        dec    edx
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xfc
        or     al, ch
        out    dx, al
        jmp    loc_3_06C6
loc_3_0602:
        cmp    esi, 0x400
        je     loc_3_0612
        cmp    esi, 0x480
        jne    loc_3_0669
loc_3_0612:
        mov    cl, 0x10
        mov    word ptr [0x43a], 0x55        ; -> loc_1_043A
        cmp    edi, 0x55
        jae    loc_3_065D
        mov    cl, 0xc
        mov    word ptr [0x43a], 0x4b        ; -> loc_1_043A
        cmp    edi, 0x4b
        jae    loc_3_065D
        mov    cl, 8
        mov    word ptr [0x43a], 0x46        ; -> loc_1_043A
        cmp    edi, 0x46
        jae    loc_3_065D
        mov    cl, 4
        mov    word ptr [0x43a], 0x3c        ; -> loc_1_043A
        cmp    edi, 0x3c
        jae    loc_3_065D
        mov    cl, 0
        mov    word ptr [0x43a], 0x2b        ; -> loc_1_043A
loc_3_065D:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xe3
        or     al, cl
        out    dx, al
        jmp    loc_3_06C6
loc_3_0669:
        cmp    esi, 0x500
        jne    loc_3_06C6
        mov    cl, 0x80
        mov    word ptr [0x43a], 0x55        ; -> loc_1_043A
        cmp    edi, 0x55
        jae    loc_3_06BC
        mov    cl, 0x60
        mov    word ptr [0x43a], 0x4b        ; -> loc_1_043A
        cmp    edi, 0x4b
        jae    loc_3_06BC
        mov    cl, 0x40
        mov    word ptr [0x43a], 0x46        ; -> loc_1_043A
        cmp    edi, 0x46
        jae    loc_3_06BC
        mov    cl, 0x20
        mov    word ptr [0x43a], 0x3c        ; -> loc_1_043A
        cmp    edi, 0x3c
        jae    loc_3_06BC
        mov    cl, 0
        mov    word ptr [0x43a], 0x2b        ; -> loc_1_043A
loc_3_06BC:
        mov    al, ah
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0x1f
        or     al, cl
        out    dx, al
loc_3_06C6:
        pop    eax
        dec    edx
        out    dx, al
loc_3_06C9:
        cmp    word ptr [0x43a], 0           ; -> loc_1_043A
        je     loc_3_06E7
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        mov    cx, word ptr [0x43a]          ; -> loc_1_043A
        mov    word ptr [ebx + 0x18], cx
loc_3_06E7:
        ret    
loc_3_06E8:
        test   byte ptr [0x481], 1           ; -> obj1:0x0481
        je     loc_3_070D
        mov    edx, 0x3d4
        in     al, dx
        push   eax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x66
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf7
        out    dx, al
        pop    eax
        mov    edx, 0x3d4
        out    dx, al
loc_3_070D:
        ret    
loc_3_070E:
        mov    byte ptr [0x440], 0           ; -> loc_1_0440
        cmp    dword ptr [0x42f], 0x883d00   ; -> obj1:0x042F
        je     loc_3_0748
        cmp    dword ptr [0x42f], 0x563100   ; -> obj1:0x042F
        je     loc_3_0748
        call   sub_4_0A36
        mov    edx, 0x4ae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
loc_3_0748:
        call   sub_4_0390
        ret    
loc_3_074E:
        push   eax
        push   edi
        mov    edi, 0x3c0                    ; -> loc_1_03C0
        mov    al, 0x67
        cmp    byte ptr [0x449], 3
        jbe    loc_3_0762
        mov    al, 0x63
loc_3_0762:
        mov    byte ptr [edi + 0xd], al
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 5], 0
        mov    byte ptr [edi + 6], 0
        test   byte ptr [0x438], 1           ; -> loc_1_0438
        je     loc_3_077E
        mov    byte ptr [edi + 9], 0
loc_3_077E:
        mov    byte ptr [0x442], 0xff        ; -> loc_1_0442
        pop    edi
        pop    eax
        ret    
loc_3_0788:
        cld    
        push   edi
        mov    edi, 0x3d0                    ; -> loc_1_03D0
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 5], 0
        mov    byte ptr [edi + 6], 0
        test   byte ptr [0x438], 1           ; -> loc_1_0438
        je     loc_3_07A8
        mov    byte ptr [edi + 9], 0
loc_3_07A8:
        pop    edi
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xffb  R-X
loc_4_0000:
        test   byte ptr [0x438], 2           ; -> loc_1_0438
        je     loc_4_0041
        mov    edx, 0x3d4
        in     al, dx
        push   eax
        mov    ax, 0xa039
        out    dx, ax
        mov    al, 0x67
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf3
        or     al, 0xc
        out    dx, al
        mov    eax, dword ptr [0x3a0]        ; -> obj1:0x03A0
        mov    edx, dword ptr [0x43c]        ; -> loc_1_043C
        add    eax, dword ptr [0x3ac]        ; -> obj1:0x03AC
        mov    dword ptr [eax], edx
        and    byte ptr [0x438], 0xfd        ; -> loc_1_0438
        pop    eax
        mov    edx, 0x3d4
        out    dx, al
loc_4_0041:
        mov    byte ptr [0x440], 0xff        ; -> loc_1_0440
        mov    byte ptr [0x441], 0xff        ; -> obj1:0x0441
        mov    eax, ecx
        shr    eax, 0x10
        or     cx, cx
        je     loc_4_0077
        mov    edx, dword ptr [0x384]        ; -> obj1:0x0384
        shr    edx, 0x10
        dec    dl
        cmp    al, dl
        jae    loc_4_016C
        inc    al
        xor    cx, cx
        add    ecx, 0x10000
loc_4_0077:
        mov    edx, dword ptr [0x384]        ; -> obj1:0x0384
        sub    edx, ecx
        cmp    edx, dword ptr [0x374]        ; -> obj1:0x0374
        mov    edx, dword ptr [0x374]        ; -> obj1:0x0374
        ja     loc_4_0092
        mov    edx, 0x8000
loc_4_0092:
        cmp    byte ptr [0x434], 0           ; -> loc_1_0434
        je     loc_4_0124
        mov    byte ptr [0x444], al          ; -> loc_1_0444
        mov    byte ptr [0x443], 0x3f        ; -> obj1:0x0443
        cmp    dword ptr [0x42f], 0x881400   ; -> obj1:0x042F
        je     loc_4_016E
        cmp    dword ptr [0x42f], 0x881200   ; -> obj1:0x042F
        je     loc_4_016E
        cmp    dword ptr [0x42f], 0x881500   ; -> obj1:0x042F
        je     loc_4_016E
        cmp    dword ptr [0x42f], 0x881140   ; -> obj1:0x042F
        je     loc_4_016E
        cmp    dword ptr [0x42f], 0x881100   ; -> obj1:0x042F
        je     loc_4_016E
        cmp    dword ptr [0x42f], 0x881000   ; -> obj1:0x042F
        je     loc_4_016E
        cmp    dword ptr [0x42f], 0x883d00   ; -> obj1:0x042F
        je     loc_4_016E
        cmp    dword ptr [0x42f], 0x563100   ; -> obj1:0x042F
        je     loc_4_016E
        mov    byte ptr [0x443], 0xff        ; -> obj1:0x0443
        jmp    loc_4_016E
loc_4_0124:
        push   edx
        push   ebx
        mov    bl, al
        mov    bh, al
        and    bl, 0xf
        shr    bh, 4
        mov    edx, 0x3d4
        mov    al, 0x35
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf0
        or     al, bl
        mov    byte ptr [0x444], al          ; -> loc_1_0444
        mov    byte ptr [0x445], 0           ; -> obj1:0x0445
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_0168
        dec    edx
        mov    al, 0x51
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf3
        shl    bh, 2
        or     al, bh
        mov    byte ptr [0x445], al          ; -> obj1:0x0445
loc_4_0168:
        pop    ebx
        pop    edx
        jmp    loc_4_016E
loc_4_016C:
        mov    edx, ecx
loc_4_016E:
        xor    ah, ah
        ret    
loc_4_0171:
        cmp    byte ptr [0x447], 0xff        ; -> obj1:0x0447
        jne    loc_4_01FE
        cmp    byte ptr [0x441], 0           ; -> obj1:0x0441
        je     loc_4_01FE
        push   eax
        push   edx
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x31
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x446], al          ; -> loc_1_0446
        or     al, 1
        out    dx, al
        dec    edx
        cmp    byte ptr [0x434], 0           ; -> loc_1_0434
        je     loc_4_01CA
        mov    al, 0x6a
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [0x447], al          ; -> obj1:0x0447
        mov    al, byte ptr [0x444]          ; -> loc_1_0444
        out    dx, al
        dec    dl
        jmp    loc_4_01F8
loc_4_01CA:
        mov    al, 0x35
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x447], al          ; -> obj1:0x0447
        mov    al, byte ptr [0x444]          ; -> loc_1_0444
        out    dx, al
        dec    edx
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_01F8
        mov    al, 0x51
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x448], al          ; -> loc_1_0448
        mov    al, byte ptr [0x445]          ; -> obj1:0x0445
        out    dx, al
        dec    edx
loc_4_01F8:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_01FE:
        ret    
loc_4_01FF:
        cmp    byte ptr [0x447], 0xff        ; -> obj1:0x0447
        je     loc_4_02B3
        push   eax
        push   edx
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        cmp    byte ptr [0x434], 0           ; -> loc_1_0434
        je     loc_4_0248
        mov    al, 0x6a
        out    dx, al
        inc    dl
        in     al, dx
        and    al, byte ptr [0x443]          ; -> obj1:0x0443
        cmp    al, byte ptr [0x444]          ; -> loc_1_0444
        jne    loc_4_02A1
        mov    al, byte ptr [0x447]          ; -> obj1:0x0447
        out    dx, al
        jmp    loc_4_02A1
loc_4_0248:
        xor    ah, ah
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_025E
        mov    al, 0x51
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        dec    edx
loc_4_025E:
        mov    al, 0x35
        out    dx, al
        inc    edx
        in     al, dx
        mov    dl, byte ptr [0x444]          ; -> loc_1_0444
        mov    dh, byte ptr [0x445]          ; -> obj1:0x0445
        cmp    ax, dx
        jne    loc_4_02A1
        mov    edx, 0x3d5
        mov    al, byte ptr [0x447]          ; -> obj1:0x0447
        out    dx, al
        dec    edx
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_0297
        mov    al, 0x51
        out    dx, al
        inc    edx
        mov    al, byte ptr [0x448]          ; -> loc_1_0448
        out    dx, al
        dec    edx
loc_4_0297:
        mov    al, 0x31
        mov    ah, byte ptr [0x446]          ; -> loc_1_0446
        out    dx, ax
loc_4_02A1:
        mov    byte ptr [0x447], 0xff        ; -> obj1:0x0447
        mov    edx, 0x3d4
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
loc_4_02B3:
        ret    
loc_4_02B4:
        push   eax
        push   edx
        cmp    byte ptr [0x441], 0           ; -> obj1:0x0441
        je     loc_4_0332
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x31
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x449], al          ; -> obj1:0x0449
        or     al, 1
        out    dx, al
        dec    edx
        cmp    byte ptr [0x434], 0           ; -> loc_1_0434
        je     loc_4_0300
        mov    al, 0x6a
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [0x44a], al          ; -> loc_1_044A
        mov    al, byte ptr [0x444]          ; -> loc_1_0444
        out    dx, al
        dec    dl
        jmp    loc_4_032E
loc_4_0300:
        mov    al, 0x35
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x44a], al          ; -> loc_1_044A
        mov    al, byte ptr [0x444]          ; -> loc_1_0444
        out    dx, al
        dec    edx
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_032E
        mov    al, 0x51
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [0x44b], al          ; -> obj1:0x044B
        mov    al, byte ptr [0x445]          ; -> obj1:0x0445
        out    dx, al
        dec    edx
loc_4_032E:
        rol    eax, 8
        out    dx, al
loc_4_0332:
        pop    edx
        pop    eax
        ret    
loc_4_0335:
        push   eax
        push   edx
        cmp    byte ptr [0x441], 0           ; -> obj1:0x0441
        je     loc_4_038D
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    al, 0x31
        mov    ah, byte ptr [0x449]          ; -> obj1:0x0449
        out    dx, ax
        cmp    byte ptr [0x434], 0           ; -> loc_1_0434
        je     loc_4_0368
        mov    al, 0x6a
        mov    ah, byte ptr [0x44a]          ; -> loc_1_044A
        out    dx, ax
        jmp    loc_4_0389
loc_4_0368:
        mov    al, 0x35
        mov    ah, byte ptr [0x44a]          ; -> loc_1_044A
        out    dx, ax
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_0389
        mov    al, 0x51
        out    dx, al
        inc    edx
        mov    al, byte ptr [0x44b]          ; -> obj1:0x044B
        out    dx, al
        dec    edx
loc_4_0389:
        rol    eax, 8
        out    dx, al
loc_4_038D:
        pop    edx
        pop    eax
        ret    
sub_4_0390:
        test   byte ptr [0x438], 1           ; -> loc_1_0438
        je     loc_4_0424
        pushal 
        mov    edx, 0x3d4
        in     al, dx
        push   eax
        mov    al, 0x67
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0xc
        cmp    al, 0xc
        jne    loc_4_041F
        or     byte ptr [0x438], 2           ; -> loc_1_0438
        mov    edx, 0x4021
        call   sub_1_0254
        mov    eax, dword ptr [0x3a0]        ; -> obj1:0x03A0
        add    eax, dword ptr [0x3ac]        ; -> obj1:0x03AC
        mov    edx, dword ptr [eax]
        mov    dword ptr [0x43c], edx        ; -> loc_1_043C
        and    edx, 0xfff78000
        test   byte ptr [0x438], 4           ; -> loc_1_0438
        je     loc_4_03EC
        or     edx, 0xc000
        jmp    loc_4_03F2
loc_4_03EC:
        or     edx, 0x3000
loc_4_03F2:
        cmp    byte ptr [0x435], 1           ; -> obj1:0x0435
        jne    loc_4_0401
        or     edx, 0x80000
loc_4_0401:
        mov    dword ptr [eax], edx
        mov    edx, 0x3da
loc_4_0408:
        in     al, dx
        test   al, 8
        jne    loc_4_0408
loc_4_040D:
        in     al, dx
        test   al, 8
        je     loc_4_040D
        mov    dx, 0x3d4
        mov    al, 0x67
        out    dx, al
        inc    dx
        in     al, dx
        and    al, 0xf3
        out    dx, al
loc_4_041F:
        dec    dx
        pop    eax
        out    dx, al
        popal  
loc_4_0424:
        ret    
loc_4_0425:
        or     byte ptr [0x436], 2           ; -> loc_1_0436
        mov    eax, dword ptr [0x394]        ; -> obj1:0x0394
        or     eax, eax
        je     loc_4_04D1
        cmp    word ptr [eax], 2
        je     loc_4_0444
        mov    word ptr [eax], 0
loc_4_0444:
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x40
        out    dx, al
        inc    dl
        in     al, dx
        or     al, 1
        and    al, 0xf7
        out    dx, al
        dec    dl
        rol    eax, 8
        out    dx, al
        test   dword ptr [0x3a4], 2          ; -> obj1:0x03A4
        je     loc_4_04BF
        mov    edx, dword ptr [0x3a0]        ; -> obj1:0x03A0
        add    edx, dword ptr [0x3a8]        ; -> obj1:0x03A8
        mov    ecx, 0x3e8
        cmp    dword ptr [0x42f], 0x883d00   ; -> obj1:0x042F
        je     loc_4_04AC
        cmp    dword ptr [0x42f], 0x563100   ; -> obj1:0x042F
        je     loc_4_04AC
loc_4_0499:
        mov    ax, word ptr [edx]
        test   ax, 0x200
        je     loc_4_04D1
        call   sub_4_0CA2
        dec    ecx
        jne    loc_4_0499
        jmp    loc_4_04D1
loc_4_04AC:
        mov    ax, word ptr [edx]
        test   ax, 0x2000
        jne    loc_4_04D1
        call   sub_4_0CA2
        dec    ecx
        jne    loc_4_04AC
        jmp    loc_4_04D1
loc_4_04BF:
        mov    edx, 0x9ae9
        in     al, dx
        test   al, 2
        je     loc_4_04D1
        call   sub_4_0CA2
        dec    ecx
        jne    loc_4_04BF
loc_4_04D1:
        cmp    dword ptr [0x42f], 0x563100   ; -> obj1:0x042F
        je     loc_4_04FF
        cmp    dword ptr [0x42f], 0x883d00   ; -> obj1:0x042F
        je     loc_4_04FF
        mov    edx, 0x4ae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
loc_4_04FF:
        call   sub_4_0390
        ret    
loc_4_0505:
        cmp    dword ptr [0x42f], 0x883d00   ; -> obj1:0x042F
        je     loc_4_0533
        cmp    dword ptr [0x42f], 0x563100   ; -> obj1:0x042F
        je     loc_4_0533
        mov    edx, 0x4ae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
loc_4_0533:
        mov    byte ptr [0x436], 0           ; -> loc_1_0436
        cmp    ebx, dword ptr [0x380]        ; -> obj1:0x0380
        je     loc_4_0547
        call   sub_4_05A4
loc_4_0547:
        ret    
loc_4_0548:
        mov    dx, 0x3cc
        in     al, dx
        or     al, 1
        mov    dl, 0xc2
        out    dx, al
        or     byte ptr [0x436], 1           ; -> loc_1_0436
        ret    
loc_4_055A:
        mov    byte ptr [0x436], 0           ; -> loc_1_0436
        test   byte ptr [0x438], 2           ; -> loc_1_0438
        je     loc_4_05A3
        push   ebx
        mov    edx, 0x3d4
        mov    ax, 0xa039
        out    dx, ax
        mov    al, 0x67
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xf3
        or     al, 0xc
        add    ebx, dword ptr [0x38c]        ; -> obj1:0x038C
        mov    byte ptr [ebx + 9], al
        mov    eax, dword ptr [0x3a0]        ; -> obj1:0x03A0
        mov    edx, dword ptr [0x43c]        ; -> loc_1_043C
        add    eax, dword ptr [0x3ac]        ; -> obj1:0x03AC
        mov    dword ptr [eax], edx
        and    byte ptr [0x438], 0xfd        ; -> loc_1_0438
        pop    ebx
loc_4_05A3:
        ret    
sub_4_05A4:
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_0621
        cmp    byte ptr [0x441], 0           ; -> obj1:0x0441
        je     loc_4_0621
        push   edx
        push   edi
        mov    edi, ebx
        add    edi, dword ptr [0x38c]        ; -> obj1:0x038C
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x40
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 2], al
        mov    al, 0x42
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 3], al
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_0613
        mov    al, 0x53
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        mov    byte ptr [edi + 5], al
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 6], al
        test   byte ptr [0x438], 1           ; -> loc_1_0438
        je     loc_4_0613
        dec    edx
        mov    al, 0x67
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 9], al
loc_4_0613:
        mov    dl, 0xd4
        rol    eax, 8
        out    dx, al
        mov    dl, 0xcc
        in     al, dx
        mov    byte ptr [edi + 0xd], al
        pop    edi
        pop    edx
loc_4_0621:
        ret    
loc_4_0622:
        push   edx
        cmp    byte ptr [0x440], 0           ; -> loc_1_0440
        je     loc_4_0854
        cmp    byte ptr [0x441], 0           ; -> obj1:0x0441
        je     loc_4_0854
        cmp    ecx, dword ptr [0x380]        ; -> obj1:0x0380
        jne    loc_4_0654
        mov    ecx, 0x3c0                    ; -> loc_1_03C0
        sub    ecx, dword ptr [0x38c]        ; -> obj1:0x038C
        mov    esi, ecx
        jmp    loc_4_0667
loc_4_0654:
        cmp    esi, dword ptr [0x37c]        ; -> obj1:0x037C
        jne    loc_4_0667
        mov    esi, 0x3d0                    ; -> loc_1_03D0
        sub    esi, dword ptr [0x38c]        ; -> obj1:0x038C
loc_4_0667:
        cmp    ecx, dword ptr [0x378]        ; -> obj1:0x0378
        jne    loc_4_0680
        mov    al, byte ptr [0x44d]          ; -> obj1:0x044D
        or     al, al
        je     loc_4_06AC
        mov    edx, 0x3c2
        out    dx, al
        jmp    loc_4_06AC
loc_4_0680:
        add    ecx, dword ptr [0x38c]        ; -> obj1:0x038C
        mov    edx, 0x3c2
        mov    al, byte ptr [ecx + 0xd]
        out    dx, al
        and    al, 0xc
        cmp    al, 0xc
        jne    loc_4_06A6
        mov    dl, 0xd4
        in     al, dx
        ror    eax, 8
        mov    al, 0x42
        mov    ah, byte ptr [ecx + 3]
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_4_06A6:
        sub    ecx, dword ptr [0x38c]        ; -> obj1:0x038C
loc_4_06AC:
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_0854
        cmp    esi, dword ptr [0x378]        ; -> obj1:0x0378
        jne    loc_4_07C2
        test   byte ptr [0x436], 3           ; -> loc_1_0436
        jne    loc_4_07C2
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        push   ecx
        mov    ax, 0xa539
        out    dx, ax
        mov    ax, 0x4838
        out    dx, ax
        mov    al, 0x40
        out    dx, al
        inc    edx
        in     al, dx
        mov    edi, dword ptr [0x394]        ; -> obj1:0x0394
        mov    ah, byte ptr [edi]
        and    ax, 0x3fe
        cmp    ah, 2
        jne    loc_4_0703
        shr    ah, 1
loc_4_0703:
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 0x53
        out    dx, al
        inc    edx
        in     al, dx
        mov    edi, dword ptr [0x398]        ; -> obj1:0x0398
        mov    ah, byte ptr [edi]
        mov    cl, byte ptr [edi + 1]
        mov    ch, cl
        not    cl
        and    ax, cx
        or     al, ah
        out    dx, al
        dec    edx
        cmp    byte ptr [0x44c], 0           ; -> loc_1_044C
        je     loc_4_0734
        mov    al, 0x31
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xfe
        out    dx, al
        dec    edx
loc_4_0734:
        mov    ax, word ptr [0x42a]          ; -> loc_1_042A
        out    dx, ax
        mov    ax, word ptr [0x42c]          ; -> loc_1_042C
        out    dx, ax
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [0x44c]          ; -> loc_1_044C
        and    ax, 0x3fc
        or     al, ah
        mov    edi, dword ptr [0x394]        ; -> obj1:0x0394
        mov    ah, byte ptr [edi]
        and    ah, 1
        not    ah
        and    ax, 0x1ef
        shl    ah, 4
        or     al, ah
        out    dx, al
        dec    edx
        pop    ecx
        test   byte ptr [0x438], 1           ; -> loc_1_0438
        je     loc_4_07B4
        mov    edi, ecx
        add    edi, dword ptr [0x38c]        ; -> obj1:0x038C
        mov    al, 0x67
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [edi + 9]
        cmp    al, ah
        je     loc_4_07B4
        mov    cl, al
        and    cl, 0xc
        cmp    cl, 0xc
        je     loc_4_07B4
        and    ax, 0xcf3
        or     al, ah
        mov    ecx, eax
        mov    edx, 0x3da
loc_4_07A2:
        in     al, dx
        test   al, 8
        jne    loc_4_07A2
loc_4_07A7:
        in     al, dx
        test   al, 8
        je     loc_4_07A7
        mov    edx, 0x3d5
        mov    eax, ecx
        out    dx, al
loc_4_07B4:
        mov    edx, 0x3d4
        rol    eax, 8
        out    dx, al
        jmp    loc_4_0854
loc_4_07C2:
        add    esi, dword ptr [0x38c]        ; -> obj1:0x038C
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x40
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [esi + 2]
        and    ax, 0x9f6
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 0x53
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [esi + 5]
        mov    edi, dword ptr [0x398]        ; -> obj1:0x0398
        mov    cl, byte ptr [edi + 1]
        mov    ch, cl
        not    cl
        and    ax, cx
        or     al, ah
        out    dx, al
        dec    edx
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [esi + 6]
        and    ax, 0x10ef
        or     al, ah
        out    dx, al
        dec    edx
        test   byte ptr [0x438], 1           ; -> loc_1_0438
        je     loc_4_0850
        cmp    byte ptr [0x439], 0           ; -> obj1:0x0439
        je     loc_4_0850
        mov    byte ptr [0x439], 0           ; -> obj1:0x0439
        mov    edx, 0x3da
loc_4_0831:
        in     al, dx
        test   al, 8
        jne    loc_4_0831
loc_4_0836:
        in     al, dx
        test   al, 8
        je     loc_4_0836
        mov    edx, 0x3d4
        mov    al, 0x67
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [esi + 9]
        and    ax, 0xcf3
        or     al, ah
        out    dx, al
        dec    edx
loc_4_0850:
        rol    eax, 8
        out    dx, al
loc_4_0854:
        pop    edx
        ret    
loc_4_0856:
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_08A5
        cmp    byte ptr [0x441], 0           ; -> obj1:0x0441
        je     loc_4_08A5
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0xa539
        out    dx, ax
        cmp    dword ptr [0x42f], 0xd0       ; -> obj1:0x042F
        ja     loc_4_0892
        mov    ax, 0xa5a
        out    dx, ax
        mov    ax, 0x59
        out    dx, ax
loc_4_0892:
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xec
        out    dx, al
        dec    edx
        mov    ax, 0x39
        out    dx, ax
        rol    eax, 8
        out    dx, al
loc_4_08A5:
        ret    
loc_4_08A6:
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_0936
        cmp    byte ptr [0x441], 0           ; -> obj1:0x0441
        je     loc_4_0936
        cmp    byte ptr [0x42e], 0           ; -> loc_1_042E
        jne    loc_4_0936
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x31
        out    dx, al
        inc    edx
        in     al, dx
        and    al, 0xfe
        out    dx, al
        dec    edx
        cmp    dword ptr [0x42f], 0xd0       ; -> obj1:0x042F
        ja     loc_4_0902
        mov    ax, word ptr [0x42a]          ; -> loc_1_042A
        out    dx, ax
        mov    ax, word ptr [0x42c]          ; -> loc_1_042C
        out    dx, ax
loc_4_0902:
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, byte ptr [0x44c]          ; -> loc_1_044C
        and    ax, 0x3fc
        or     al, ah
        or     al, 0x10
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        cmp    byte ptr [0x437], 1           ; -> obj1:0x0437
        jb     loc_4_0936
        ja     loc_4_0930
        mov    edx, 0x4023
        call   sub_1_0254
loc_4_0930:
        dec    byte ptr [0x437]              ; -> obj1:0x0437
loc_4_0936:
        ret    
loc_4_0937:
        cmp    byte ptr [0x441], 0           ; -> obj1:0x0441
        je     loc_4_0A1F
        cmp    dword ptr [0x42f], 0x883d00   ; -> obj1:0x042F
        je     loc_4_0A35
        cmp    dword ptr [0x42f], 0x563100   ; -> obj1:0x042F
        je     loc_4_0A35
        mov    edx, 0x9ae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x9ae9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xe2e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xbee8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x96e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x86e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x82e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x8ee8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x8ae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xb6e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xbae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xaae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xaee8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xa2e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xa6e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x92e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0xb2e8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
loc_4_0A1F:
        mov    edx, 0x4ae8
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    edx, 0x4ae9
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
loc_4_0A35:
        ret    
sub_4_0A36:
        cmp    byte ptr [0x441], 0           ; -> obj1:0x0441
        je     loc_4_0B1E
        cmp    dword ptr [0x42f], 0x883d00   ; -> obj1:0x042F
        je     loc_4_0B1E
        cmp    dword ptr [0x42f], 0x563100   ; -> obj1:0x042F
        je     loc_4_0B1E
        mov    edx, 0x9ae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x9ae9
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xe2e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xbee8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x96e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x86e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x82e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x8ee8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x8ae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xb6e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xbae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xaae8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xaee8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xa2e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xa6e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0x92e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    edx, 0xb2e8
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
loc_4_0B1E:
        ret    
loc_4_0B1F:
        push   edi
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    edi, dword ptr [0x378]        ; -> obj1:0x0378
        jne    loc_4_0B33
        cmp    ebx, edi
        jne    loc_4_0B3D
loc_4_0B33:
        cmp    ecx, 4
        je     loc_4_0B3C
        out    dx, ax
        jmp    loc_4_0B3D
loc_4_0B3C:
        out    dx, al
loc_4_0B3D:
        pop    esi
        pop    edi
        ret    
loc_4_0B40:
        push   edx
        cmp    eax, dword ptr [0x378]        ; -> obj1:0x0378
        jne    loc_4_0CA0
        cmp    byte ptr [0x441], 0           ; -> obj1:0x0441
        je     loc_4_0CA0
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x40
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        test   ah, 1
        je     loc_4_0CA0
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_0BA8
        test   ah, 8
        jne    loc_4_0CA0
        ror    eax, 8
        mov    al, 0x58
        out    dx, al
        inc    edx
        in     al, dx
        dec    edx
        rol    eax, 8
        out    dx, al
        test   ah, 0x10
        jne    loc_4_0CA0
loc_4_0BA8:
        mov    eax, dword ptr [0x390]        ; -> obj1:0x0390
        cmp    byte ptr [eax], 0
        jne    loc_4_0CA0
        mov    eax, dword ptr [0x398]        ; -> obj1:0x0398
        cmp    byte ptr [eax], 0
        je     loc_4_0CA0
        xor    eax, eax
        cmp    word ptr [0x42c], 0xa59       ; -> loc_1_042C
        jne    loc_4_0BDB
        cmp    word ptr [0x42a], 0x5a        ; -> loc_1_042A
        je     loc_4_0C09
loc_4_0BDB:
        dec    eax
        mov    edx, 0x3d4
        in     al, dx
        push   eax
        mov    al, 0x59
        mov    ah, al
        out    dx, al
        inc    edx
        in     al, dx
        xchg   al, ah
        dec    edx
        push   eax
        mov    al, 0x5a
        mov    ah, al
        out    dx, al
        inc    edx
        in     al, dx
        xchg   al, ah
        push   eax
        dec    edx
        mov    ax, word ptr [0x42a]          ; -> loc_1_042A
        out    dx, ax
        mov    ax, word ptr [0x42c]          ; -> loc_1_042C
        out    dx, ax
loc_4_0C09:
        test   dword ptr [0x3a4], 2          ; -> obj1:0x03A4
        je     loc_4_0C64
        mov    edx, dword ptr [0x3a0]        ; -> obj1:0x03A0
        add    edx, dword ptr [0x3a8]        ; -> obj1:0x03A8
        mov    ecx, 0x3e8
        cmp    dword ptr [0x42f], 0x883d00   ; -> obj1:0x042F
        je     loc_4_0C51
        cmp    dword ptr [0x42f], 0x563100   ; -> obj1:0x042F
        je     loc_4_0C51
loc_4_0C3E:
        mov    ax, word ptr [edx]
        test   ax, 0x200
        je     loc_4_0C76
        call   sub_4_0CA2
        dec    ecx
        jne    loc_4_0C3E
        jmp    loc_4_0C76
loc_4_0C51:
        mov    ax, word ptr [edx]
        test   ax, 0x2000
        jne    loc_4_0C76
        call   sub_4_0CA2
        dec    ecx
        jne    loc_4_0C51
        jmp    loc_4_0C76
loc_4_0C64:
        mov    edx, 0x9ae9
        in     al, dx
        test   al, 2
        je     loc_4_0C76
        call   sub_4_0CA2
        dec    ecx
        jne    loc_4_0C64
loc_4_0C76:
        test   eax, 0xffff0000
        je     loc_4_0CA0
        mov    edx, 0x3d4
        pop    eax
        out    dx, ax
        pop    eax
        out    dx, ax
        pop    eax
        out    dx, al
        jmp    loc_4_0CA0
        db  0BAh, 0E8h, 09Ah, 000h, 000h, 066h, 0EDh, 0F6h, 0C4h, 002h, 074h, 008h, 0E8h, 005h, 000h, 000h
        db  000h, 049h, 075h, 0ECh
loc_4_0CA0:
        pop    edx
        ret    
sub_4_0CA2:
        push   ecx
        mov    ecx, 0x3e8
loc_4_0CA8:
        push   ecx
        pop    ecx
        loop   loc_4_0CA8
        pop    ecx
        ret    
loc_4_0CAE:
        cmp    cl, 0x30
        jae    loc_4_0CB6
        clc    
        jmp    loc_4_0D02
loc_4_0CB6:
        test   byte ptr [0x436], 2           ; -> loc_1_0436
        je     loc_4_0CCA
        dec    dl
        mov    al, cl
        out    dx, al
        inc    dl
        in     al, dx
        stc    
        jmp    loc_4_0D02
loc_4_0CCA:
        dec    dl
        in     al, dx
        mov    ch, al
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        cmp    cl, 0x40
        jne    loc_4_0CE8
        mov    al, 0x40
        out    dx, al
        inc    dl
        in     al, dx
        jmp    loc_4_0CF3
loc_4_0CE8:
        cmp    cl, 0x30
        jne    loc_4_0CF8
        mov    al, 0x30
        out    dx, al
        inc    dl
        in     al, dx
loc_4_0CF3:
        dec    dl
        stc    
        jmp    loc_4_0CF9
loc_4_0CF8:
        clc    
loc_4_0CF9:
        xchg   ch, al
        out    dx, al
        mov    dl, 0xd5
        mov    al, ch
        mov    ch, 0
loc_4_0D02:
        ret    
loc_4_0D03:
        cmp    cl, 0x30
        cmc    
        jae    loc_4_0DAA
        cmp    byte ptr [0x442], 0xff        ; -> loc_1_0442
        jb     loc_4_0DAA
        test   byte ptr [0x436], 2           ; -> loc_1_0436
        je     loc_4_0D2E
        dec    dl
        mov    ah, al
        mov    al, cl
        out    dx, ax
        stc    
        jmp    loc_4_0DAA
loc_4_0D2E:
        dec    dl
        push   eax
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    eax
        inc    dl
        cmp    cl, 0x40
        jne    loc_4_0D5A
        mov    edi, ebx
        add    edi, dword ptr [0x38c]        ; -> obj1:0x038C
        mov    byte ptr [edi + 2], al
        jmp    loc_4_0DA9
loc_4_0D5A:
        cmp    cl, 0x42
        jne    loc_4_0D6C
        mov    edi, ebx
        add    edi, dword ptr [0x38c]        ; -> obj1:0x038C
        mov    byte ptr [edi + 3], al
        jmp    loc_4_0DA9
loc_4_0D6C:
        cmp    cl, 0x53
        jne    loc_4_0D7E
        mov    edi, ebx
        add    edi, dword ptr [0x38c]        ; -> obj1:0x038C
        mov    byte ptr [edi + 5], al
        jmp    loc_4_0DA9
loc_4_0D7E:
        cmp    cl, 0x58
        jne    loc_4_0D90
        mov    edi, ebx
        add    edi, dword ptr [0x38c]        ; -> obj1:0x038C
        mov    byte ptr [edi + 6], al
        jmp    loc_4_0DA9
loc_4_0D90:
        cmp    cl, 0x67
        jne    loc_4_0DA9
        test   byte ptr [0x438], 1           ; -> loc_1_0438
        je     loc_4_0DA9
        mov    edi, ebx
        add    edi, dword ptr [0x38c]        ; -> obj1:0x038C
        mov    byte ptr [edi + 9], al
loc_4_0DA9:
        clc    
loc_4_0DAA:
        ret    
loc_4_0DAB:
        pushfd 
        test   cl, 4
        je     loc_4_0DBC
        push   ebx
        add    ebx, dword ptr [0x38c]        ; -> obj1:0x038C
        mov    byte ptr [ebx + 0xd], al
        pop    ebx
loc_4_0DBC:
        popfd  
        jmp    loc_1_03B0
loc_4_0DC3:
        mov    ecx, dword ptr [0x384]        ; -> obj1:0x0384
        stc    
        ret    
loc_4_0DCB:
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        cmp    byte ptr [0x434], 0           ; -> loc_1_0434
        je     loc_4_0DE9
        mov    al, 0x6a
        out    dx, al
        inc    dl
        in     al, dx
        dec    dl
        or     al, al
        jne    loc_4_0E10
loc_4_0DE9:
        xor    ah, ah
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_0E01
        mov    al, 0x51
        out    dx, al
        inc    dl
        in     al, dx
        mov    ah, al
        dec    dl
loc_4_0E01:
        mov    al, 0x35
        out    dx, al
        inc    dl
        in     al, dx
        and    ax, 0xc0f
        shl    ah, 2
        or     al, ah
loc_4_0E10:
        mov    dl, 0xd4
        rol    eax, 8
        out    dx, al
        movzx  edx, ah
        stc    
        ret    
loc_4_0E1B:
        mov    edx, 0x10000
        xor    eax, eax
        stc    
        ret    
loc_4_0E24:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        add    esi, dword ptr [0x38c]        ; -> obj1:0x038C
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x31
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi], al
        or     al, 1
        out    dx, al
        dec    dl
        mov    al, 0x35
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 1], al
        dec    dl
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_0EDD
        mov    al, 0x51
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 4], al
        out    dx, al
        dec    dl
        cmp    byte ptr [0x434], 0           ; -> loc_1_0434
        je     loc_4_0E88
        mov    al, 0x6a
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 0xa], al
        dec    dl
loc_4_0E88:
        mov    al, 0x40
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 2], al
        and    al, 0xfe
        or     al, 8
        out    dx, al
        dec    dl
        mov    al, 0x53
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 5], al
        mov    edi, dword ptr [0x398]        ; -> obj1:0x0398
        mov    cl, byte ptr [edi + 1]
        not    cl
        and    al, cl
        out    dx, al
        dec    dl
        mov    al, 0x59
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 7], al
        xor    al, al
        out    dx, al
        dec    dl
        mov    al, 0x5a
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 8], al
        mov    al, 0xa
        out    dx, al
        dec    dl
        mov    al, 0x58
        out    dx, al
        inc    dl
        in     al, dx
        mov    byte ptr [esi + 6], al
        and    al, 0xfc
        or     al, 0x10
        out    dx, al
        dec    dl
loc_4_0EDD:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc    
        ret    
loc_4_0EE6:
        push   esi
        push   eax
        push   edx
        mov    esi, ebx
        add    esi, dword ptr [0x38c]        ; -> obj1:0x038C
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x31
        mov    ah, byte ptr [esi]
        out    dx, ax
        mov    al, 0x35
        mov    ah, byte ptr [esi + 1]
        out    dx, ax
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_0F59
        mov    al, 0x51
        mov    ah, byte ptr [esi + 4]
        out    dx, ax
        cmp    byte ptr [0x434], 0           ; -> loc_1_0434
        je     loc_4_0F36
        mov    al, 0x6a
        mov    ah, byte ptr [esi + 0xa]
        out    dx, ax
loc_4_0F36:
        mov    al, 0x40
        mov    ah, byte ptr [esi + 2]
        out    dx, ax
        mov    al, 0x53
        mov    ah, byte ptr [esi + 5]
        out    dx, ax
        mov    al, 0x59
        mov    ah, byte ptr [esi + 7]
        out    dx, ax
        mov    al, 0x5a
        mov    ah, byte ptr [esi + 8]
        out    dx, ax
        mov    al, 0x58
        mov    ah, byte ptr [esi + 6]
        out    dx, ax
loc_4_0F59:
        rol    eax, 8
        out    dx, al
        pop    edx
        pop    eax
        pop    esi
        stc    
        ret    
loc_4_0F62:
        push   edx
        push   eax
        push   ecx
        mov    cl, dl
        mov    edx, 0x3d4
        in     al, dx
        ror    eax, 8
        mov    ax, 0x4838
        out    dx, ax
        mov    ax, 0xa539
        out    dx, ax
        mov    al, 0x35
        mov    ah, cl
        and    ah, 0xf
        out    dx, ax
        cmp    dword ptr [0x42f], 0x80       ; -> obj1:0x042F
        je     loc_4_0FBC
        mov    al, 0x51
        out    dx, al
        inc    dl
        in     al, dx
        and    al, 0xf3
        mov    ah, cl
        and    ah, 0x30
        shr    ah, 2
        or     al, ah
        out    dx, al
        dec    dl
        cmp    byte ptr [0x434], 0           ; -> loc_1_0434
        je     loc_4_0FBC
        mov    al, 0x6a
        mov    ah, cl
        test   cl, 0xc0
        jne    loc_4_0FBA
        xor    ah, ah
loc_4_0FBA:
        out    dx, ax
loc_4_0FBC:
        rol    eax, 8
        out    dx, al
        pop    ecx
        pop    eax
        pop    edx
        stc    
        ret    
loc_4_0FC5:
        push   eax
        mov    eax, 0x86
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    eax
        clc    
        ret    
loc_4_0FD6:
        cmp    ebx, dword ptr [0x378]        ; -> obj1:0x0378
        je     loc_4_0FF9
        cmp    ax, 0x4f02
        jne    loc_4_0FF9
        test   byte ptr [ebp + 0x11], 0x80
        jne    loc_4_0FF9
        push   eax
        mov    eax, 0x13
        push   0x10
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    eax
loc_4_0FF9:
        clc    
        ret    
VxD__LTEXT ENDS
