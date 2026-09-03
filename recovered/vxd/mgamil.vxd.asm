; MGAMIL -- reconstructed 32-bit disassembly (VxD / LE format)
; 6 objects, 1184 instructions recovered by recursive descent

Declare_Virtual_Device MGAMIL, 3, 1, MGAMIL_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xa18  R-X
MGAMIL_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_000F
        jmp    dword ptr [eax*4 + 0x11]      ; -> obj1:0x0011
loc_1_000F:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_2_0000
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_6_0000
        dd  sub_1_007D
        dd  loc_1_000F
        dd  loc_4_0000
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_5_0000
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
        dd  loc_2_0000
sub_1_007D:
        pushal 
        test   dword ptr [0x80c], 0x4000     ; -> obj1:0x080C
        jne    loc_1_00A5
        mov    eax, dword ptr [0x810]        ; -> obj1:0x0810
        or     eax, eax
        je     loc_1_00A2
        mov    edi, eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        call   sub_1_00A8
        jmp    loc_1_00A6
loc_1_00A2:
        stc    
        jmp    loc_1_00A6
loc_1_00A5:
        clc    
loc_1_00A6:
        popal  
        ret    
sub_1_00A8:
        push   edx
        call   sub_1_015F
        cmp    word ptr [0x818], 0           ; -> obj1:0x0818
        je     loc_1_00DB
        mov    dx, 0xcf8
        mov    al, 0x80
        out    dx, al
        jmp    loc_1_00C1
loc_1_00C1:
        mov    dx, word ptr [0x818]          ; -> obj1:0x0818
        in     al, dx
        jmp    loc_1_00CB
loc_1_00CB:
        and    al, 0xef
        or     al, 1
        out    dx, al
        jmp    loc_1_00D2
loc_1_00D2:
        mov    dx, 0xcf8
        mov    al, 0
        out    dx, al
        jmp    loc_1_00DB
loc_1_00DB:
        mov    dx, 0x3de
        mov    al, 3
        out    dx, al
        jmp    loc_1_00E4
loc_1_00E4:
        inc    dx
        in     al, dx
        jmp    loc_1_00E9
loc_1_00E9:
        and    al, 0x7f
        out    dx, al
        jmp    loc_1_00EE
loc_1_00EE:
        mov    edi, dword ptr [0x810]        ; -> obj1:0x0810
        mov    byte ptr [edi + 0x3c00], 0x1d
        mov    byte ptr [edi + 0x3c0a], 0
        mov    byte ptr [edi + 0x3c00], 0x1a
        mov    byte ptr [edi + 0x3c0a], 0
        mov    byte ptr [edi + 0x3c00], 0x18
        mov    byte ptr [edi + 0x3c0a], 0x80
        mov    byte ptr [edi + 0x3c00], 0x19
        mov    byte ptr [edi + 0x3c0a], 0x98
        mov    byte ptr [edi + 0x3c00], 0x39
        mov    byte ptr [edi + 0x3c0a], 0x18
        mov    byte ptr [edi + 0x3c00], 6
        mov    al, byte ptr [edi + 0x3c0a]
        and    al, 0xfc
        mov    byte ptr [edi + 0x3c0a], al
        mov    byte ptr [edi + 0x3c00], 0x1e
        mov    byte ptr [edi + 0x3c0a], 0
        pop    edx
        ret    
sub_1_015F:
        push   edx
        push   eax
        mov    dx, 0x3de
        mov    ax, 0
loc_1_0169:
        mov    al, ah
        out    dx, al
        jmp    loc_1_016E
loc_1_016E:
        inc    dx
        xor    al, al
        out    dx, al
        jmp    loc_1_0175
loc_1_0175:
        dec    dx
        inc    ah
        cmp    ah, 6
        jne    loc_1_0169
        pop    eax
        pop    edx
        ret    
loc_1_0181:
        call   sub_1_007D
        ret    
loc_1_0187:
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ax, 0x4f15
        jne    loc_1_019E
        pushal 
        call   sub_1_03A0
        or     ax, ax
        popal  
        js     loc_1_01D3
        clc    
        ret    
loc_1_019E:
        test   dword ptr [0x80c], 0x4000     ; -> obj1:0x080C
        je     loc_1_01D3
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ax, 0x4f10
        jne    loc_1_01D3
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        push   eax
        push   ebx
        mov    ax, word ptr [ebp + 0x1c]
        mov    bx, word ptr [ebp + 0x10]
        call   sub_1_0288
        mov    word ptr [ebp + 0x1c], ax
        mov    word ptr [ebp + 0x10], bx
        pop    ebx
        pop    eax
        pop    ebp
        clc    
        ret    
loc_1_01D3:
        stc    
        ret    
loc_1_01D5:
        pushal 
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ax, 0xbd70
        je     loc_1_01E7
        cmp    ax, 0xbd71
        je     loc_1_0234
        jmp    loc_1_024E
loc_1_01E7:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    eax, dword ptr [0x858]        ; -> loc_1_0858
        mov    word ptr [ebp + 0x1c], 0x2543
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x3c], ax
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        jmp    loc_1_0244
loc_1_0234:
        mov    eax, dword ptr [0x858]        ; -> loc_1_0858
        mov    word ptr [ebp + 8], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x38], ax
loc_1_0244:
        mov    ax, 0xbd
        mov    word ptr [ebp + 0x1c], ax
        popal  
        ret    
loc_1_024E:
        popal  
        stc    
        ret    
loc_1_0251:
        cmp    dx, 0x4f00
        je     loc_1_025A
        jmp    loc_1_0276
loc_1_025A:
        pushal 
        mov    ah, 0x38
        mov    al, 0
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        je     loc_1_0275
        mov    cx, word ptr [0x856]          ; -> loc_1_0856
        mov    word ptr [eax + 0x12], cx
loc_1_0275:
        popal  
loc_1_0276:
        ret    
loc_1_0277:
        or     eax, eax
        jne    loc_1_0283
        mov    eax, 0xffffffff
        clc    
        jmp    loc_1_0284
loc_1_0283:
        stc    
loc_1_0284:
        ret    
        db  000h, 000h, 000h
sub_1_0288:
        cmp    bl, 0
        jne    loc_1_028F
        jmp    loc_1_02A2
loc_1_028F:
        cmp    bl, 1
        jne    loc_1_0296
        jmp    loc_1_02E2
loc_1_0296:
        cmp    bl, 2
        jne    loc_1_029D
        jmp    loc_1_02AB
loc_1_029D:
        mov    ax, 0x14f
        ret    
loc_1_02A2:
        mov    ax, 0x4f
        mov    bh, 7
        mov    bl, 0x10
        ret    
loc_1_02AB:
        push   edi
        mov    edi, dword ptr [0x810]        ; -> obj1:0x0810
        or     edi, edi
        je     loc_1_02DF
        mov    al, 1
        mov    byte ptr [edi + 0x1fde], al
        mov    al, byte ptr [edi + 0x1fdf]
        xor    bh, bh
        and    al, 0x30
        je     loc_1_02D8
        inc    bh
        cmp    al, 0x10
        je     loc_1_02D8
        inc    bh
        cmp    al, 0x20
        je     loc_1_02D8
        shl    bh, 1
loc_1_02D8:
        mov    ax, 0x4f
        pop    edi
        clc    
        ret    
loc_1_02DF:
        pop    edi
        stc    
        ret    
loc_1_02E2:
        push   edi
        mov    edi, dword ptr [0x810]        ; -> obj1:0x0810
        or     edi, edi
        je     loc_1_02DF
        mov    al, 1
        mov    byte ptr [edi + 0x1fde], al
        mov    al, bh
        and    bh, 0xf
        je     loc_1_0301
        and    bl, 7
        je     loc_1_0348
loc_1_0301:
        mov    al, byte ptr [edi + 0x1fdf]
        and    al, 0xcf
        or     bh, bh
        jne    loc_1_031E
        mov    byte ptr [edi + 0x1fdf], al
        call   sub_1_0378
        mov    ax, 0x4f
        jmp    loc_1_034C
loc_1_031E:
        cmp    bh, 1
        jne    loc_1_0327
        or     al, 0x10
        jmp    loc_1_0337
loc_1_0327:
        cmp    bh, 2
        jne    loc_1_0330
        or     al, 0x20
        jmp    loc_1_0337
loc_1_0330:
        cmp    bh, 4
        jne    loc_1_0348
        or     al, 0x30
loc_1_0337:
        mov    byte ptr [edi + 0x1fdf], al
        call   sub_1_034F
        mov    ax, 0x4f
        jmp    loc_1_034C
loc_1_0348:
        mov    ax, 0x14f
loc_1_034C:
        pop    edi
        clc    
        ret    
sub_1_034F:
        mov    al, byte ptr [edi + 0x1fc4]
        xchg   ah, al
        mov    al, 1
        mov    byte ptr [edi + 0x1fc4], al
        mov    al, byte ptr [edi + 0x1fc5]
        or     al, 0x20
        mov    byte ptr [edi + 0x1fc5], al
        xchg   ah, al
        dec    dx
        mov    byte ptr [edi + 0x1fc4], al
        ret    
sub_1_0378:
        mov    al, byte ptr [edi + 0x1fc4]
        xchg   ah, al
        mov    al, 1
        mov    byte ptr [edi + 0x1fc4], al
        mov    al, byte ptr [edi + 0x1fc5]
        and    al, 0xdf
        mov    byte ptr [edi + 0x1fc5], al
        xchg   ah, al
        mov    byte ptr [edi + 0x1fc4], al
        ret    
        db  000h
sub_1_03A0:
        mov    eax, dword ptr [0x810]        ; -> obj1:0x0810
        or     eax, eax
        je     loc_1_03B8
        mov    bx, word ptr [ebp + 0x10]
        cmp    bl, 0
        jne    loc_1_03B8
        call   sub_1_03BD
        ret    
loc_1_03B8:
        mov    ax, 0xffff
        ret    
sub_1_03BD:
        call   sub_1_03CF
        or     ax, ax
        js     sub_1_03CF
        mov    word ptr [ebp + 0x1c], ax
        mov    word ptr [ebp + 0x10], bx
sub_1_03CF:
        call   sub_1_055C
        mov    edx, eax
        shr    edx, 8
        call   sub_1_057D
        mov    cl, 0xf
loc_1_03E0:
        call   sub_1_058A
        or     al, al
        je     loc_1_0409
        mov    al, 0xa0
        call   sub_1_0632
        call   sub_1_05F6
        or     al, al
        je     loc_1_0404
        call   sub_1_05CB
        mov    ax, 0xffff
        jmp    loc_1_0418
loc_1_0404:
        call   sub_1_05CB
loc_1_0409:
        dec    cl
        jne    loc_1_03E0
        mov    ah, 1
        mov    al, 0x4f
        mov    ax, 0x100
        xor    bx, bx
loc_1_0418:
        ret    
sub_1_0419:
        push   edi
        mov    edi, dword ptr [0x810]        ; -> obj1:0x0810
        add    edi, 0x3c00
        mov    byte ptr [edi], cl
        mov    edi, dword ptr [0x810]        ; -> obj1:0x0810
        add    edi, 0x3c0a
        mov    cl, byte ptr [edi]
        pop    edi
        ret    
sub_1_0438:
        push   edi
        mov    edi, dword ptr [0x810]        ; -> obj1:0x0810
        add    edi, 0x3c00
        mov    byte ptr [edi], cl
        mov    edi, dword ptr [0x810]        ; -> obj1:0x0810
        add    edi, 0x3c0a
        mov    byte ptr [edi], ch
        pop    edi
        ret    
sub_1_0457:
        push   ecx
        push   edx
        mov    cl, 0x2a
        call   sub_1_0419
        or     cl, 0x10
        mov    ch, cl
        mov    cl, 0x2a
        call   sub_1_0438
        mov    cl, 0x2b
        call   sub_1_0419
        or     cl, 0x10
        mov    ch, cl
        mov    cl, 0x2b
        call   sub_1_0438
        pop    edx
        pop    ecx
        ret    
sub_1_0482:
        push   ecx
        push   edx
        mov    cl, 0x2a
        call   sub_1_0419
        or     cl, 4
        mov    ch, cl
        mov    cl, 0x2a
        call   sub_1_0438
        mov    cl, 0x2b
        call   sub_1_0419
        or     cl, 4
        mov    ch, cl
        mov    cl, 0x2b
        call   sub_1_0438
        pop    edx
        pop    ecx
        ret    
sub_1_04AD:
        push   ecx
        push   edx
        mov    cl, 0x2a
        call   sub_1_0419
        or     cl, 0x10
        mov    ch, cl
        mov    cl, 0x2a
        call   sub_1_0438
        mov    cl, 0x2b
        call   sub_1_0419
        and    cl, 0xef
        mov    ch, cl
        mov    cl, 0x2b
        call   sub_1_0438
        pop    edx
        pop    ecx
        ret    
sub_1_04D8:
        push   ecx
        push   edx
        mov    cl, 0x2a
        call   sub_1_0419
        or     cl, 4
        mov    ch, cl
        mov    cl, 0x2a
        call   sub_1_0438
        mov    cl, 0x2b
        call   sub_1_0419
        and    cl, 0xfb
        mov    ch, cl
        mov    cl, 0x2b
        call   sub_1_0438
        pop    edx
        pop    ecx
        ret    
sub_1_0503:
        push   ecx
        push   edx
        mov    cl, 0x2a
        call   sub_1_0419
        and    cl, 0xfb
        mov    ch, cl
        mov    cl, 0x2a
        call   sub_1_0438
        mov    cl, 0x2b
        call   sub_1_0419
        shl    cl, 6
        pop    edx
        pop    ecx
        ret    
sub_1_0525:
        push   ecx
        push   edx
        mov    cl, 0x2a
        call   sub_1_0419
        and    cl, 0xef
        mov    ch, cl
        mov    cl, 0x2a
        call   sub_1_0438
        mov    cl, 0x2b
        call   sub_1_0419
        shl    cl, 4
        pop    edx
        pop    ecx
        ret    
sub_1_0547:
        push   ecx
        push   edx
        mov    ecx, edx
        xor    esi, esi
        mov    dx, 0x3da
loc_1_0551:
        inc    esi
        in     al, dx
        and    al, 8
        cmp    ecx, esi
        jne    loc_1_0551
        pop    edx
        pop    ecx
        ret    
sub_1_055C:
        push   ecx
        mov    dx, 0x3da
loc_1_0561:
        in     al, dx
        and    al, 8
        jne    loc_1_0561
loc_1_0566:
        in     al, dx
        and    al, 8
        je     loc_1_0566
        xor    ecx, ecx
loc_1_056D:
        inc    ecx
        in     al, dx
        and    al, 8
        jne    loc_1_056D
loc_1_0573:
        inc    ecx
        in     al, dx
        and    al, 8
        je     loc_1_0573
        mov    eax, ecx
        pop    ecx
        ret    
sub_1_057D:
        push   ecx
        call   sub_1_04AD
        call   sub_1_0547
        pop    ecx
        ret    
sub_1_058A:
        push   ecx
        call   sub_1_0482
        call   sub_1_0547
        call   sub_1_0457
        mov    cl, 0xf
loc_1_059C:
        call   sub_1_0547
        dec    cl
        je     loc_1_05C7
        call   sub_1_0525
        jae    loc_1_059C
        call   sub_1_0503
        jae    loc_1_059C
        call   sub_1_04D8
        call   sub_1_0547
        call   sub_1_04AD
        call   sub_1_0547
loc_1_05C7:
        mov    al, cl
        pop    ecx
        ret    
sub_1_05CB:
        push   ecx
        call   sub_1_04D8
        call   sub_1_0547
        call   sub_1_04AD
        call   sub_1_0547
        call   sub_1_0457
        call   sub_1_0547
        call   sub_1_0482
        call   sub_1_0547
        pop    ecx
        ret    
sub_1_05F6:
        push   ecx
        call   sub_1_0482
        mov    cl, 0xf
loc_1_05FE:
        call   sub_1_0547
        call   sub_1_0503
        jae    loc_1_0610
        dec    cl
        jne    loc_1_05FE
        jmp    loc_1_062E
loc_1_0610:
        call   sub_1_0457
        call   sub_1_0547
        call   sub_1_04AD
        call   sub_1_0547
        call   sub_1_0482
        call   sub_1_0547
loc_1_062E:
        mov    al, cl
        pop    ecx
        ret    
sub_1_0632:
        push   ecx
        mov    cl, 8
        mov    ch, al
loc_1_0637:
        shl    ch, 1
        jb     loc_1_0642
        call   sub_1_04D8
        jmp    loc_1_0647
loc_1_0642:
        call   sub_1_0482
loc_1_0647:
        call   sub_1_0547
        call   sub_1_0457
        call   sub_1_0547
        call   sub_1_04AD
        call   sub_1_0547
        dec    cl
        jne    loc_1_0637
        call   sub_1_0482
        call   sub_1_0547
        pop    ecx
        ret    
sub_1_0670:
        mov    dword ptr [0x85c], 1          ; -> loc_1_085C
        mov    eax, 0x860                    ; -> loc_1_0860
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        push   eax
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        pop    eax
        test   word ptr [eax + 2], 0x40
        jne    loc_1_073A
        mov    eax, dword ptr [eax + 0x2c]
        push   0
        push   0x100
        push   0x8aa                         ; -> loc_1_08AA
        push   0
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d003300
        add    esp, 0x14
        cmp    eax, 0
        jne    loc_1_073A
        push   0x9aa                         ; -> loc_1_09AA
        push   0x8aa                         ; -> loc_1_08AA
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        cmp    eax, 0
        jne    loc_1_073A
loc_1_06D8:
        push   0xa0d                         ; -> obj1:0x0A0D
        push   0x9b9                         ; -> obj1:0x09B9
        push   0xa09                         ; -> obj1:0x0A09
        push   0
        push   0x9ae                         ; -> loc_1_09AE
        push   dword ptr [0x9aa]             ; -> loc_1_09AA
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        cmp    eax, 0xea
        je     loc_1_06D8
        cmp    eax, 0
        jne    loc_1_073A
        mov    ebx, 0x9b9                    ; -> obj1:0x09B9
        xor    ecx, ecx
loc_1_0710:
        cmp    byte ptr [ebx], 0
        je     loc_1_0725
        mov    eax, dword ptr [ebx]
        inc    ebx
        and    eax, 0xffffff
        cmp    eax, 0x504e50
        jne    loc_1_0710
        inc    ecx
loc_1_0725:
        mov    dword ptr [0x85c], ecx        ; -> loc_1_085C
        push   dword ptr [0x9aa]             ; -> loc_1_09AA
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_1_073A:
        mov    eax, dword ptr [0x85c]        ; -> loc_1_085C
        mov    dword ptr [ebp + 0x14], eax
        ret    
sub_1_0743:
        movzx  eax, word ptr [ebp + 0x38]
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        inc    eax
        je     loc_1_07B8
        dec    eax
        movzx  edi, word ptr [ebp]
        add    edi, eax
        mov    dword ptr [0xa11], edi        ; -> obj1:0x0A11
        mov    eax, 0x860                    ; -> loc_1_0860
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        push   eax
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        pop    eax
        test   word ptr [eax + 2], 0x40
        jne    loc_1_07B8
        mov    eax, dword ptr [eax + 0x2c]
        push   0
        push   0x100
        push   0x8aa                         ; -> loc_1_08AA
        push   0
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d003300
        add    esp, 0x14
        cmp    eax, 0
        jne    loc_1_07B8
        mov    edi, dword ptr [0xa11]        ; -> obj1:0x0A11
        mov    ebx, 0x8aa                    ; -> loc_1_08AA
loc_1_07AC:
        mov    al, byte ptr [ebx]
        mov    byte ptr [edi], al
        cmp    al, 0
        je     loc_1_07B8
        inc    ebx
        inc    edi
        jmp    loc_1_07AC
loc_1_07B8:
        ret    
        db  000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'MGAMIL  '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  MGAMIL_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h
loc_1_0856:
        and    byte ptr [eax], al
loc_1_0858:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_085C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0860:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_08AA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_09AA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_09AE:
        dec    eax
        popal  
        jb     loc_1_0A16
        ja     0xa15
        jb     0xa1b
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0A16:
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x1024  R-X
loc_2_0000:
        mov    dword ptr [0x82c], ebx        ; -> obj1:0x082C
        call   sub_2_00E9
        or     ax, ax
        je     loc_2_00E7
        mov    dword ptr [0x810], 0          ; -> obj1:0x0810
        push   0
        push   0x1000
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        or     eax, eax
        je     loc_2_0062
        mov    edi, eax
        mov    esi, eax
        and    eax, 0xf
        and    esi, 0xfffffff0
        shl    esi, 0xc
        or     eax, esi
        mov    dword ptr [0x858], eax        ; -> loc_1_0858
        mov    esi, 0xf0                     ; -> obj2:0x00F0
        mov    ecx, 0xf30
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0x1d5                    ; -> loc_1_01D5
        mov    eax, 0x6d
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
loc_2_0062:
        mov    esi, 0x187                    ; -> loc_1_0187
        mov    eax, 0x10
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_00E7
        mov    dword ptr [edi], 0            ; -> loc_3_0000
        mov    dword ptr [edi + 0x68], 0x181 ; -> loc_1_0181
        mov    dword ptr [edi + 0x10], 7     ; -> loc_6_0007
        mov    dword ptr [edi + 0x18], 0xd   ; -> loc_6_000D
        mov    dword ptr [edi + 0xbc], 0x251 ; -> loc_1_0251
        mov    dword ptr [edi + 0x90], 0x111 ; -> loc_6_0111
        mov    dword ptr [edi + 0xac], 0x277 ; -> loc_1_0277
        mov    ecx, 0xe                      ; -> loc_6_000E
        mov    edx, 0x3de
        VxDCall  VDD.VDD_Takeover_VGA_Port       ; cd2013000a00
        jb     loc_2_00E7
        mov    ecx, 0xe                      ; -> loc_6_000E
        mov    edx, 0x3df
        VxDCall  VDD.VDD_Takeover_VGA_Port       ; cd2013000a00
        jb     loc_2_00E7
        mov    dword ptr [0x830], 0          ; -> obj1:0x0830
        clc    
        jmp    loc_2_00E8
loc_2_00E7:
        stc    
loc_2_00E8:
        ret    
sub_2_00E9:
        mov    ax, 1
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 07Eh, 081h, 0A5h, 081h, 081h, 0BDh, 099h, 081h, 07Eh, 000h, 000h, 000h, 000h, 000h
        db  07Eh, 0FFh, 0DBh, 0FFh, 0FFh, 0C3h, 0E7h, 0FFh, 07Eh, 000h, 000h, 000h, 000h, 000h, 000h, 06Ch
        db  0FEh, 0FEh, 0FEh, 0FEh, 07Ch, 038h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 038h, 07Ch
        db  0FEh, 07Ch, 038h, 010h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 03Ch, 03Ch, 0E7h, 0E7h, 0E7h
        db  018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 000h, 018h, 03Ch, 07Eh, 0FFh, 0FFh, 07Eh, 018h, 018h
        db  03Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 03Ch, 03Ch, 018h, 000h, 000h, 000h
        db  000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0E7h, 0C3h, 0C3h, 0E7h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  000h, 000h, 000h, 000h, 03Ch, 066h, 042h, 042h, 066h, 03Ch, 000h, 000h, 000h, 000h, 0FFh, 0FFh
        db  0FFh, 0FFh, 0C3h, 099h, 0BDh, 0BDh, 099h, 0C3h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 01Eh, 00Eh
        db  01Ah, 032h, 078h, 0CCh, 0CCh, 0CCh, 078h, 000h, 000h, 000h, 000h, 000h, 03Ch, 066h, 066h, 066h
        db  03Ch, 018h, 07Eh, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 03Fh, 033h, 03Fh, 030h, 030h, 030h
        db  070h, 0F0h, 0E0h, 000h, 000h, 000h, 000h, 000h, 07Fh, 063h, 07Fh, 063h, 063h, 063h, 067h, 0E7h
        db  0E6h, 0C0h, 000h, 000h, 000h, 000h, 018h, 018h, 0DBh, 03Ch, 0E7h, 03Ch, 0DBh, 018h, 018h, 000h
        db  000h, 000h, 000h, 000h, 080h, 0C0h, 0E0h, 0F8h, 0FEh, 0F8h, 0E0h, 0C0h, 080h, 000h, 000h, 000h
        db  000h, 000h, 002h, 006h, 00Eh, 03Eh, 0FEh, 03Eh, 00Eh, 006h, 002h, 000h, 000h, 000h, 000h, 000h
        db  018h, 03Ch, 07Eh, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 000h, 000h, 000h, 000h, 000h, 066h, 066h
        db  066h, 066h, 066h, 066h, 000h, 066h, 066h, 000h, 000h, 000h, 000h, 000h, 07Fh, 0DBh, 0DBh, 0DBh
        db  07Bh, 01Bh, 01Bh, 01Bh, 01Bh, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 060h, 038h, 06Ch, 0C6h, 0C6h
        db  06Ch, 038h, 00Ch, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 0FEh
        db  0FEh, 000h, 000h, 000h, 000h, 000h, 018h, 03Ch, 07Eh, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 07Eh
        db  000h, 000h, 000h, 000h, 018h, 03Ch, 07Eh, 018h, 018h, 018h, 018h, 018h, 018h, 000h, 000h, 000h
        db  000h, 000h, 018h, 018h, 018h, 018h, 018h, 018h, 07Eh, 03Ch, 018h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 018h, 00Ch, 0FEh, 00Ch, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  030h, 060h, 0FEh, 060h, 030h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C0h
        db  0C0h, 0C0h, 0FEh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 028h, 06Ch, 0FEh, 06Ch
        db  028h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 038h, 038h, 07Ch, 07Ch, 0FEh, 0FEh
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 0FEh, 07Ch, 07Ch, 038h, 038h, 010h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 018h, 03Ch, 03Ch, 03Ch, 018h, 018h, 000h, 018h, 018h, 000h, 000h, 000h, 000h, 066h
        db  066h, 066h, 024h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 06Ch, 06Ch
        db  0FEh, 06Ch, 06Ch, 06Ch, 0FEh, 06Ch, 06Ch, 000h, 000h, 000h, 018h, 018h, 07Ch, 0C6h, 0C2h, 0C0h
        db  07Ch, 006h, 086h, 0C6h, 07Ch, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 0C2h, 0C6h, 00Ch, 018h
        db  030h, 066h, 0C6h, 000h, 000h, 000h, 000h, 000h, 038h, 06Ch, 06Ch, 038h, 076h, 0DCh, 0CCh, 0CCh
        db  076h, 000h, 000h, 000h, 000h, 030h, 030h, 030h, 060h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 00Ch, 018h, 030h, 030h, 030h, 030h, 030h, 018h, 00Ch, 000h, 000h, 000h
        db  000h, 000h, 030h, 018h, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 018h, 030h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 066h, 03Ch, 0FFh, 03Ch, 066h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  018h, 018h, 07Eh, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 018h, 018h, 018h, 030h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h
        db  018h, 000h, 000h, 000h, 000h, 000h, 002h, 006h, 00Ch, 018h, 030h, 060h, 0C0h, 080h, 000h, 000h
        db  000h, 000h, 000h, 000h, 07Ch, 0C6h, 0CEh, 0DEh, 0F6h, 0E6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
        db  000h, 000h, 018h, 038h, 078h, 018h, 018h, 018h, 018h, 018h, 07Eh, 000h, 000h, 000h, 000h, 000h
        db  07Ch, 0C6h, 006h, 00Ch, 018h, 030h, 060h, 0C6h, 0FEh, 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h
        db  006h, 006h, 03Ch, 006h, 006h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 00Ch, 01Ch, 03Ch, 06Ch
        db  0CCh, 0FEh, 00Ch, 00Ch, 01Eh, 000h, 000h, 000h, 000h, 000h, 0FEh, 0C0h, 0C0h, 0C0h, 0FCh, 006h
        db  006h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 038h, 060h, 0C0h, 0C0h, 0FCh, 0C6h, 0C6h, 0C6h
        db  07Ch, 000h, 000h, 000h, 000h, 000h, 0FEh, 0C6h, 006h, 00Ch, 018h, 030h, 030h, 030h, 030h, 000h
        db  000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 07Ch, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
        db  000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 07Eh, 006h, 006h, 00Ch, 078h, 000h, 000h, 000h, 000h, 000h
        db  000h, 018h, 018h, 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 018h
        db  018h, 000h, 000h, 000h, 018h, 018h, 030h, 000h, 000h, 000h, 000h, 000h, 006h, 00Ch, 018h, 030h
        db  060h, 030h, 018h, 00Ch, 006h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Eh, 000h, 000h
        db  07Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 060h, 030h, 018h, 00Ch, 006h, 00Ch, 018h, 030h
        db  060h, 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 00Ch, 018h, 018h, 000h, 018h, 018h, 000h
        db  000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0DEh, 0DEh, 0DEh, 0DCh, 0C0h, 07Ch, 000h, 000h, 000h
        db  000h, 000h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h, 000h
        db  0FCh, 066h, 066h, 066h, 07Ch, 066h, 066h, 066h, 0FCh, 000h, 000h, 000h, 000h, 000h, 03Ch, 066h
        db  0C2h, 0C0h, 0C0h, 0C0h, 0C2h, 066h, 03Ch, 000h, 000h, 000h, 000h, 000h, 0F8h, 06Ch, 066h, 066h
        db  066h, 066h, 066h, 06Ch, 0F8h, 000h, 000h, 000h, 000h, 000h, 0FEh, 066h, 062h, 068h, 078h, 068h
        db  062h, 066h, 0FEh, 000h, 000h, 000h, 000h, 000h, 0FEh, 066h, 062h, 068h, 078h, 068h, 060h, 060h
        db  0F0h, 000h, 000h, 000h, 000h, 000h, 03Ch, 066h, 0C2h, 0C0h, 0C0h, 0DEh, 0C6h, 066h, 03Ah, 000h
        db  000h, 000h, 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 0C6h, 0C6h, 000h, 000h, 000h
        db  000h, 000h, 03Ch, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 000h
        db  01Eh, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch, 0CCh, 0CCh, 078h, 000h, 000h, 000h, 000h, 000h, 0E6h, 066h
        db  06Ch, 06Ch, 078h, 06Ch, 06Ch, 066h, 0E6h, 000h, 000h, 000h, 000h, 000h, 0F0h, 060h, 060h, 060h
        db  060h, 060h, 062h, 066h, 0FEh, 000h, 000h, 000h, 000h, 000h, 0C6h, 0EEh, 0FEh, 0FEh, 0D6h, 0C6h
        db  0C6h, 0C6h, 0C6h, 000h, 000h, 000h, 000h, 000h, 0C6h, 0E6h, 0F6h, 0FEh, 0DEh, 0CEh, 0C6h, 0C6h
        db  0C6h, 000h, 000h, 000h, 000h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 06Ch, 038h, 000h
        db  000h, 000h, 000h, 000h, 0FCh, 066h, 066h, 066h, 07Ch, 060h, 060h, 060h, 0F0h, 000h, 000h, 000h
        db  000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0D6h, 0DEh, 07Ch, 00Ch, 00Eh, 000h, 000h, 000h, 000h
        db  0FCh, 066h, 066h, 066h, 07Ch, 06Ch, 066h, 066h, 0E6h, 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h
        db  0C6h, 060h, 038h, 00Ch, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 07Eh, 07Eh, 05Ah, 018h
        db  018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h
        db  0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 06Ch, 038h
        db  010h, 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0D6h, 0D6h, 0FEh, 07Ch, 06Ch, 000h
        db  000h, 000h, 000h, 000h, 0C6h, 0C6h, 06Ch, 038h, 038h, 038h, 06Ch, 0C6h, 0C6h, 000h, 000h, 000h
        db  000h, 000h, 066h, 066h, 066h, 066h, 03Ch, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 000h
        db  0FEh, 0C6h, 08Ch, 018h, 030h, 060h, 0C2h, 0C6h, 0FEh, 000h, 000h, 000h, 000h, 000h, 03Ch, 030h
        db  030h, 030h, 030h, 030h, 030h, 030h, 03Ch, 000h, 000h, 000h, 000h, 000h, 080h, 0C0h, 0E0h, 070h
        db  038h, 01Ch, 00Eh, 006h, 002h, 000h, 000h, 000h, 000h, 000h, 03Ch, 00Ch, 00Ch, 00Ch, 00Ch, 00Ch
        db  00Ch, 00Ch, 03Ch, 000h, 000h, 000h, 010h, 038h, 06Ch, 0C6h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 000h, 030h, 030h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 000h
        db  0E0h, 060h, 060h, 078h, 06Ch, 066h, 066h, 066h, 07Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 07Ch, 0C6h, 0C0h, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 01Ch, 00Ch, 00Ch, 03Ch
        db  06Ch, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0FEh
        db  0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 038h, 06Ch, 064h, 060h, 0F0h, 060h, 060h, 060h
        db  0F0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 0CCh, 0CCh, 0CCh, 07Ch, 00Ch, 0CCh
        db  078h, 000h, 000h, 000h, 0E0h, 060h, 060h, 06Ch, 076h, 066h, 066h, 066h, 0E6h, 000h, 000h, 000h
        db  000h, 000h, 018h, 018h, 000h, 038h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 000h
        db  006h, 006h, 000h, 00Eh, 006h, 006h, 006h, 006h, 066h, 066h, 03Ch, 000h, 000h, 000h, 0E0h, 060h
        db  060h, 066h, 06Ch, 078h, 06Ch, 066h, 0E6h, 000h, 000h, 000h, 000h, 000h, 038h, 018h, 018h, 018h
        db  018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0ECh, 0FEh, 0D6h
        db  0D6h, 0D6h, 0C6h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0DCh, 066h, 066h, 066h, 066h
        db  066h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0DCh, 066h, 066h, 066h, 07Ch, 060h, 060h, 0F0h, 000h
        db  000h, 000h, 000h, 000h, 000h, 076h, 0CCh, 0CCh, 0CCh, 07Ch, 00Ch, 00Ch, 01Eh, 000h, 000h, 000h
        db  000h, 000h, 000h, 0DCh, 076h, 066h, 060h, 060h, 0F0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 07Ch, 0C6h, 070h, 01Ch, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 010h, 030h, 030h, 0FCh
        db  030h, 030h, 030h, 036h, 01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0CCh, 0CCh, 0CCh
        db  0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 066h, 066h, 066h, 066h, 03Ch
        db  018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0D6h, 0D6h, 0FEh, 06Ch, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 0C6h, 06Ch, 038h, 038h, 06Ch, 0C6h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 07Eh, 006h, 00Ch, 0F8h, 000h, 000h, 000h
        db  000h, 000h, 000h, 0FEh, 0CCh, 018h, 030h, 066h, 0FEh, 000h, 000h, 000h, 000h, 000h, 00Eh, 018h
        db  018h, 018h, 070h, 018h, 018h, 018h, 00Eh, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 018h, 018h
        db  000h, 018h, 018h, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 070h, 018h, 018h, 018h, 00Eh, 018h
        db  018h, 018h, 070h, 000h, 000h, 000h, 000h, 000h, 076h, 0DCh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 000h, 000h
        db  000h, 000h, 000h, 000h, 03Ch, 066h, 0C2h, 0C0h, 0C0h, 0C2h, 066h, 03Ch, 00Ch, 006h, 07Ch, 000h
        db  000h, 000h, 0CCh, 0CCh, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 00Ch
        db  018h, 030h, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 010h, 038h, 06Ch
        db  000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 000h, 0CCh, 0CCh, 000h, 078h
        db  00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 060h, 030h, 018h, 000h, 078h, 00Ch, 07Ch
        db  0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 038h, 06Ch, 038h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh
        db  076h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Ch, 066h, 060h, 066h, 03Ch, 00Ch, 006h, 03Ch
        db  000h, 000h, 000h, 010h, 038h, 06Ch, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h
        db  000h, 000h, 0CCh, 0CCh, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 060h
        db  030h, 018h, 000h, 07Ch, 0C6h, 0FEh, 0C0h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 066h, 066h
        db  000h, 038h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 018h, 03Ch, 066h, 000h, 038h
        db  018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 060h, 030h, 018h, 000h, 038h, 018h, 018h
        db  018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 010h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h
        db  0C6h, 000h, 000h, 000h, 038h, 06Ch, 038h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 000h
        db  000h, 000h, 018h, 030h, 060h, 000h, 0FEh, 066h, 060h, 07Ch, 060h, 066h, 0FEh, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0CCh, 076h, 036h, 07Eh, 0D8h, 0D8h, 06Eh, 000h, 000h, 000h, 000h, 000h
        db  03Eh, 06Ch, 0CCh, 0CCh, 0FEh, 0CCh, 0CCh, 0CCh, 0CEh, 000h, 000h, 000h, 000h, 010h, 038h, 06Ch
        db  000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 000h, 0C6h, 0C6h, 000h, 07Ch
        db  0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 060h, 030h, 018h, 000h, 07Ch, 0C6h, 0C6h
        db  0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 030h, 078h, 0CCh, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh
        db  076h, 000h, 000h, 000h, 000h, 060h, 030h, 018h, 000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h
        db  000h, 000h, 000h, 000h, 0C6h, 0C6h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 07Eh, 006h, 00Ch, 078h, 000h
        db  000h, 0C6h, 0C6h, 038h, 06Ch, 0C6h, 0C6h, 0C6h, 0C6h, 06Ch, 038h, 000h, 000h, 000h, 000h, 0C6h
        db  0C6h, 000h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 018h, 018h, 03Ch
        db  066h, 060h, 060h, 066h, 03Ch, 018h, 018h, 000h, 000h, 000h, 000h, 038h, 06Ch, 064h, 060h, 0F0h
        db  060h, 060h, 060h, 0E6h, 0FCh, 000h, 000h, 000h, 000h, 000h, 066h, 066h, 03Ch, 018h, 07Eh, 018h
        db  07Eh, 018h, 018h, 000h, 000h, 000h, 000h, 0F8h, 0CCh, 0CCh, 0F8h, 0C4h, 0CCh, 0DEh, 0CCh, 0CCh
        db  0C6h, 000h, 000h, 000h, 000h, 00Eh, 01Bh, 018h, 018h, 018h, 07Eh, 018h, 018h, 018h, 018h, 0D8h
        db  070h, 000h, 000h, 018h, 030h, 060h, 000h, 078h, 00Ch, 07Ch, 0CCh, 0CCh, 076h, 000h, 000h, 000h
        db  000h, 00Ch, 018h, 030h, 000h, 038h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 000h, 018h
        db  030h, 060h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h, 000h, 018h, 030h, 060h
        db  000h, 0CCh, 0CCh, 0CCh, 0CCh, 0CCh, 076h, 000h, 000h, 000h, 000h, 000h, 076h, 0DCh, 000h, 0DCh
        db  066h, 066h, 066h, 066h, 066h, 000h, 000h, 000h, 076h, 0DCh, 000h, 0C6h, 0E6h, 0F6h, 0FEh, 0DEh
        db  0CEh, 0C6h, 0C6h, 000h, 000h, 000h, 000h, 03Ch, 06Ch, 06Ch, 03Eh, 000h, 07Eh, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 038h, 06Ch, 06Ch, 038h, 000h, 07Ch, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 030h, 030h, 000h, 030h, 030h, 060h, 0C6h, 0C6h, 07Ch, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 0C0h, 0C0h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0FEh, 006h, 006h, 006h, 000h, 000h, 000h, 000h, 000h, 0C0h, 0C0h, 0C6h
        db  0CCh, 0D8h, 030h, 060h, 0DCh, 086h, 00Ch, 018h, 03Eh, 000h, 000h, 0C0h, 0C0h, 0C6h, 0CCh, 0D8h
        db  030h, 066h, 0CEh, 09Eh, 03Eh, 006h, 006h, 000h, 000h, 000h, 018h, 018h, 000h, 018h, 018h, 03Ch
        db  03Ch, 03Ch, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 036h, 06Ch, 0D8h, 06Ch, 036h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0D8h, 06Ch, 036h, 06Ch, 0D8h, 000h, 000h, 000h
        db  000h, 000h, 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h, 011h, 044h
        db  055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 055h, 0AAh, 0DDh, 077h
        db  0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 0DDh, 077h, 018h, 018h, 018h, 018h
        db  018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
        db  018h, 0F8h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0F8h, 018h, 0F8h
        db  018h, 018h, 018h, 018h, 018h, 018h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0F6h, 036h, 036h
        db  036h, 036h, 036h, 036h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 036h, 036h, 036h, 036h
        db  036h, 036h, 000h, 000h, 000h, 000h, 000h, 0F8h, 018h, 0F8h, 018h, 018h, 018h, 018h, 018h, 018h
        db  036h, 036h, 036h, 036h, 036h, 0F6h, 006h, 0F6h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
        db  036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 000h, 000h, 000h, 000h
        db  000h, 0FEh, 006h, 0F6h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0F6h
        db  006h, 0FEh, 000h, 000h, 000h, 000h, 000h, 000h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0FEh
        db  000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 018h, 018h, 018h, 0F8h, 018h, 0F8h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0F8h, 018h, 018h, 018h, 018h
        db  018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h
        db  018h, 018h, 018h, 018h, 018h, 018h, 018h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
        db  018h, 018h, 018h, 01Fh, 018h, 018h, 018h, 018h, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 0FFh
        db  018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 01Fh, 018h, 01Fh, 018h, 018h
        db  018h, 018h, 018h, 018h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 037h, 036h, 036h, 036h, 036h
        db  036h, 036h, 036h, 036h, 036h, 036h, 036h, 037h, 030h, 03Fh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 03Fh, 030h, 037h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
        db  036h, 036h, 036h, 0F7h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 0FFh, 000h, 0F7h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 037h
        db  030h, 037h, 036h, 036h, 036h, 036h, 036h, 036h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 0FFh
        db  000h, 000h, 000h, 000h, 000h, 000h, 036h, 036h, 036h, 036h, 036h, 0F7h, 000h, 0F7h, 036h, 036h
        db  036h, 036h, 036h, 036h, 018h, 018h, 018h, 018h, 018h, 0FFh, 000h, 0FFh, 000h, 000h, 000h, 000h
        db  000h, 000h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FFh, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h
        db  036h, 036h, 036h, 03Fh, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 018h, 018h, 018h, 01Fh
        db  018h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 01Fh, 018h, 01Fh
        db  018h, 018h, 018h, 018h, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 036h, 036h
        db  036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 036h, 0FFh, 036h, 036h, 036h, 036h
        db  036h, 036h, 018h, 018h, 018h, 018h, 018h, 0FFh, 018h, 0FFh, 018h, 018h, 018h, 018h, 018h, 018h
        db  018h, 018h, 018h, 018h, 018h, 018h, 018h, 0F8h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 01Fh, 018h, 018h, 018h, 018h, 018h, 018h, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h
        db  0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 0F0h, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh, 00Fh
        db  00Fh, 00Fh, 00Fh, 00Fh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 0DCh, 0D8h, 0D8h, 0DCh, 076h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 07Ch, 0C6h, 0FCh, 0C6h, 0C6h, 0FCh, 0C0h, 0C0h, 040h, 000h, 000h, 000h
        db  0FEh, 0C6h, 0C6h, 0C0h, 0C0h, 0C0h, 0C0h, 0C0h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FEh, 06Ch, 06Ch, 06Ch, 06Ch, 06Ch, 06Ch, 000h, 000h, 000h, 000h, 000h, 0FEh, 0C6h, 060h, 030h
        db  018h, 030h, 060h, 0C6h, 0FEh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Eh, 0D8h, 0D8h
        db  0D8h, 0D8h, 070h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 066h, 066h, 066h, 066h, 07Ch, 060h
        db  060h, 0C0h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 0DCh, 018h, 018h, 018h, 018h, 018h, 000h
        db  000h, 000h, 000h, 000h, 07Eh, 018h, 03Ch, 066h, 066h, 066h, 03Ch, 018h, 07Eh, 000h, 000h, 000h
        db  000h, 000h, 038h, 06Ch, 0C6h, 0C6h, 0FEh, 0C6h, 0C6h, 06Ch, 038h, 000h, 000h, 000h, 000h, 000h
        db  038h, 06Ch, 0C6h, 0C6h, 0C6h, 06Ch, 06Ch, 06Ch, 0EEh, 000h, 000h, 000h, 000h, 000h, 01Eh, 030h
        db  018h, 00Ch, 03Eh, 066h, 066h, 066h, 03Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Eh
        db  0DBh, 0DBh, 07Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 003h, 006h, 07Eh, 0DBh, 0DBh, 0F3h
        db  07Eh, 060h, 0C0h, 000h, 000h, 000h, 000h, 000h, 01Ch, 030h, 060h, 060h, 07Ch, 060h, 060h, 030h
        db  01Ch, 000h, 000h, 000h, 000h, 000h, 000h, 07Ch, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 0C6h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0FEh, 000h, 000h, 0FEh, 000h, 000h, 0FEh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 018h, 018h, 07Eh, 018h, 018h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h
        db  030h, 018h, 00Ch, 006h, 00Ch, 018h, 030h, 000h, 07Eh, 000h, 000h, 000h, 000h, 000h, 00Ch, 018h
        db  030h, 060h, 030h, 018h, 00Ch, 000h, 07Eh, 000h, 000h, 000h, 000h, 000h, 00Eh, 01Bh, 01Bh, 018h
        db  018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h, 018h
        db  0D8h, 0D8h, 070h, 000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 000h, 07Eh, 000h, 018h, 018h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 076h, 0DCh, 000h, 076h, 0DCh, 000h, 000h, 000h
        db  000h, 000h, 000h, 038h, 06Ch, 06Ch, 038h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 018h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Fh, 00Ch, 00Ch
        db  00Ch, 00Ch, 00Ch, 0ECh, 06Ch, 03Ch, 01Ch, 000h, 000h, 000h, 000h, 0D8h, 06Ch, 06Ch, 06Ch, 06Ch
        db  06Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 070h, 0D8h, 030h, 060h, 0C8h, 0F8h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 07Ch, 07Ch, 07Ch, 07Ch, 07Ch, 07Ch
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 01Dh, 000h, 000h, 000h, 000h, 024h, 066h, 0FFh, 066h, 024h, 000h, 000h, 000h, 000h
        db  000h, 022h, 000h, 063h, 063h, 063h, 022h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  02Bh, 000h, 000h, 000h, 018h, 018h, 018h, 0FFh, 018h, 018h, 018h, 000h, 000h, 000h, 000h, 02Dh
        db  000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Dh, 000h
        db  000h, 0C3h, 0E7h, 0FFh, 0DBh, 0C3h, 0C3h, 0C3h, 0C3h, 0C3h, 000h, 000h, 000h, 054h, 000h, 000h
        db  0FFh, 0DBh, 099h, 018h, 018h, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 056h, 000h, 000h, 0C3h
        db  0C3h, 0C3h, 0C3h, 0C3h, 0C3h, 066h, 03Ch, 018h, 000h, 000h, 000h, 057h, 000h, 000h, 0C3h, 0C3h
        db  0C3h, 0C3h, 0DBh, 0DBh, 0FFh, 066h, 066h, 000h, 000h, 000h, 058h, 000h, 000h, 0C3h, 0C3h, 066h
        db  03Ch, 018h, 03Ch, 066h, 0C3h, 0C3h, 000h, 000h, 000h, 059h, 000h, 000h, 0C3h, 0C3h, 0C3h, 066h
        db  03Ch, 018h, 018h, 018h, 03Ch, 000h, 000h, 000h, 05Ah, 000h, 000h, 0FFh, 0C3h, 086h, 00Ch, 018h
        db  030h, 061h, 0C3h, 0FFh, 000h, 000h, 000h, 06Dh, 000h, 000h, 000h, 000h, 000h, 0E6h, 0FFh, 0DBh
        db  0DBh, 0DBh, 0DBh, 000h, 000h, 000h, 076h, 000h, 000h, 000h, 000h, 000h, 0C3h, 0C3h, 0C3h, 066h
        db  03Ch, 018h, 000h, 000h, 000h, 077h, 000h, 000h, 000h, 000h, 000h, 0C3h, 0C3h, 0DBh, 0DBh, 0FFh
        db  066h, 000h, 000h, 000h, 091h, 000h, 000h, 000h, 000h, 06Eh, 03Bh, 01Bh, 07Eh, 0D8h, 0DCh, 077h
        db  000h, 000h, 000h, 09Bh, 000h, 018h, 018h, 07Eh, 0C3h, 0C0h, 0C0h, 0C3h, 07Eh, 018h, 018h, 000h
        db  000h, 000h, 09Dh, 000h, 000h, 0C3h, 066h, 03Ch, 018h, 0FFh, 018h, 0FFh, 018h, 018h, 000h, 000h
        db  000h, 09Eh, 000h, 0FCh, 066h, 066h, 07Ch, 062h, 066h, 06Fh, 066h, 066h, 0F3h, 000h, 000h, 000h
        db  0F1h, 000h, 000h, 018h, 018h, 018h, 0FFh, 018h, 018h, 018h, 000h, 0FFh, 000h, 000h, 000h, 0F6h
        db  000h, 000h, 018h, 018h, 000h, 000h, 0FFh, 000h, 000h, 018h, 018h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x28c  R-X
loc_3_0000:
        pushal 
        mov    eax, dword ptr [ebp + 0x18]
        cmp    eax, 0
        je     loc_3_0261
        cmp    eax, 0xac53962b
        je     loc_3_0036
        cmp    eax, 0xac53962c
        jne    loc_3_0025
        call   sub_1_0670
        jmp    loc_3_0287
loc_3_0025:
        cmp    eax, 0xac53962d
        jne    loc_3_0036
        call   sub_1_0743
        jmp    loc_3_0287
loc_3_0036:
        movzx  eax, word ptr [ebp + 0x40]
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        inc    eax
        je     loc_3_0279
        dec    eax
        movzx  esi, word ptr [ebp + 4]
        add    esi, eax
        mov    eax, dword ptr [0x810]        ; -> obj1:0x0810
        or     eax, eax
        jne    loc_3_012F
        mov    ax, word ptr [ebp + 0x3c]
        shl    eax, 0x10
        mov    ax, word ptr [ebp]
        mov    dword ptr [0x81c], eax        ; -> obj1:0x081C
        mov    ax, word ptr [esi + 0x10]
        cmp    ax, 0x40
        jle    loc_3_0080
        mov    ax, 0x40
loc_3_0080:
        mov    word ptr [0x856], ax          ; -> loc_1_0856
        mov    al, byte ptr [esi + 0x12]
        or     al, al
        jne    loc_3_0097
        or     dword ptr [0x80c], 0x4000     ; -> obj1:0x080C
loc_3_0097:
        push   esi
        add    esi, 0x16
        mov    al, byte ptr [esi + 0x4c]
        and    eax, 0xff
        or     dword ptr [0x80c], eax        ; -> obj1:0x080C
        pop    esi
        mov    eax, dword ptr [esi + 6]
        mov    dword ptr [0x814], eax        ; -> obj1:0x0814
        cmp    word ptr [0x818], 0           ; -> obj1:0x0818
        jne    loc_3_00FD
        push   edx
        mov    dx, 0xcf8
        mov    al, 0x80
        out    dx, al
        jmp    loc_3_00C6
loc_3_00C6:
        mov    dx, 0xbf00
loc_3_00CA:
        add    dx, 0x100
        cmp    dx, 0xd000
        je     loc_3_00F3
        in     al, dx
        jmp    loc_3_00D9
loc_3_00D9:
        cmp    al, 0x2b
        jne    loc_3_00CA
        inc    dx
        in     al, dx
        jmp    loc_3_00E2
loc_3_00E2:
        dec    dx
        cmp    al, 0x10
        jne    loc_3_00CA
        add    dx, 0x41
        mov    word ptr [0x818], dx          ; -> obj1:0x0818
loc_3_00F3:
        mov    dx, 0xcf8
        mov    al, 0
        out    dx, al
        jmp    loc_3_00FC
loc_3_00FC:
        pop    edx
loc_3_00FD:
        mov    eax, dword ptr [esi + 2]
        cmp    eax, 0xac000
        jne    loc_3_0111
        or     dword ptr [0x80c], 0x100      ; -> obj1:0x080C
loc_3_0111:
        push   0
        push   0x4000
        push   eax
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        inc    eax
        je     loc_3_0279
        dec    eax
        mov    dword ptr [0x810], eax        ; -> obj1:0x0810
loc_3_012F:
        mov    eax, dword ptr [esi + 0x1c]
        cmp    eax, dword ptr [0x838]        ; -> obj1:0x0838
        jne    loc_3_016A
        mov    eax, dword ptr [ebp + 0x14]
        cmp    eax, dword ptr [0x834]        ; -> obj1:0x0834
        jne    loc_3_016A
        mov    eax, dword ptr [0x83c]        ; -> obj1:0x083C
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [0x840]        ; -> obj1:0x0840
        mov    dword ptr [ebp + 0x18], eax
        mov    eax, dword ptr [0x844]        ; -> obj1:0x0844
        mov    dword ptr [ebp + 4], eax
        mov    eax, dword ptr [0x848]        ; -> obj1:0x0848
        mov    dword ptr [ebp], eax
        jmp    loc_3_0284
loc_3_016A:
        push   esi
        mov    edx, dword ptr [esi + 2]
        push   0
        push   0x1000
        push   edx
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        mov    dword ptr [0x848], eax        ; -> obj1:0x0848
        push   0
        push   0x80
        push   0x92
        push   1
        push   eax
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        VMMCall  VMM._Allocate_LDT_Selector      ; cd2078000100
        add    esp, 0x14
        mov    dword ptr [0x844], eax        ; -> obj1:0x0844
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
        pop    esi
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [0x834], eax        ; -> obj1:0x0834
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [0x838], eax        ; -> obj1:0x0838
        mov    edx, dword ptr [esi + 6]
        mov    eax, dword ptr [esi + 0x1c]
        cmp    dword ptr [0x834], 0x10       ; -> obj1:0x0834
        jne    loc_3_01D9
        shl    eax, 1
loc_3_01D9:
        cmp    dword ptr [0x834], 0x18       ; -> obj1:0x0834
        jne    loc_3_01E5
        lea    eax, [eax + eax*2]
loc_3_01E5:
        cmp    dword ptr [0x834], 0x20       ; -> obj1:0x0834
        jne    loc_3_01F1
        shl    eax, 2
loc_3_01F1:
        add    edx, eax
        push   0
        push   0x800000
        push   edx
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        mov    dword ptr [0x840], eax        ; -> obj1:0x0840
        push   0
        push   0x80
        push   0x92
        push   0x800
        push   eax
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        VMMCall  VMM._Allocate_LDT_Selector      ; cd2078000100
        add    esp, 0x14
        mov    dword ptr [0x83c], eax        ; -> obj1:0x083C
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
        mov    eax, dword ptr [0x83c]        ; -> obj1:0x083C
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [0x840]        ; -> obj1:0x0840
        mov    dword ptr [ebp + 0x18], eax
        mov    eax, dword ptr [0x844]        ; -> obj1:0x0844
        mov    dword ptr [ebp + 4], eax
        mov    eax, dword ptr [0x848]        ; -> obj1:0x0848
        mov    dword ptr [ebp], eax
        jmp    loc_3_0284
loc_3_0261:
        push   0
        push   0x4000
        push   0xac000
        VMMCall  VMM._MapPhysToLinear            ; cd206c000100
        add    esp, 0xc
        mov    dword ptr [ebp + 0x14], eax
loc_3_0279:
        clc    
        mov    dword ptr [0x810], 0          ; -> obj1:0x0810
loc_3_0284:
        stc    
        jmp    loc_3_0287
loc_3_0287:
        popal  
        ret    
        db  000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x8  R-X
loc_4_0000:
        inc    dword ptr [0x830]             ; -> obj1:0x0830
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x14  R-X
loc_5_0000:
        dec    dword ptr [0x830]             ; -> obj1:0x0830
        jge    loc_5_0012
        mov    dword ptr [0x830], 0          ; -> obj1:0x0830
loc_5_0012:
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x120  R-X
loc_6_0000:
        call   sub_1_007D
        clc    
        ret    
loc_6_0007:
        call   sub_1_007D
        ret    
loc_6_000D:
        ret    
loc_6_000E:
        push   edi
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    edi, dword ptr [0x82c]        ; -> obj1:0x082C
        jne    loc_6_00EC
        cmp    ebx, edi
        je     loc_6_00EC
        cmp    ecx, 4
        je     loc_6_0073
        cmp    ecx, 0
        je     loc_6_00D5
        cmp    ecx, 0xc
        je     loc_6_0042
        cmp    ecx, 8
        je     loc_6_00AF
loc_6_0042:
        cmp    dx, 0x3de
        jne    loc_6_010E
        mov    dword ptr [0x850], 0          ; -> obj1:0x0850
        cmp    al, 7
        jne    loc_6_010E
        mov    byte ptr [0x84c], al          ; -> obj1:0x084C
        mov    dword ptr [0x850], 1          ; -> obj1:0x0850
        jmp    loc_6_010E
loc_6_0073:
        cmp    dx, 0x3de
        jne    loc_6_0098
        mov    dword ptr [0x850], 0          ; -> obj1:0x0850
        cmp    al, 7
        jne    loc_6_010E
        mov    dword ptr [0x850], 1          ; -> obj1:0x0850
        jmp    loc_6_010E
loc_6_0098:
        cmp    dx, 0x3df
        jne    loc_6_010E
        cmp    dword ptr [0x850], 1          ; -> obj1:0x0850
        jne    loc_6_010E
        mov    byte ptr [0x84c], al          ; -> obj1:0x084C
        jmp    loc_6_010E
loc_6_00AF:
        cmp    dx, 0x3de
        jne    loc_6_010E
        mov    dword ptr [0x850], 0          ; -> obj1:0x0850
        cmp    al, 7
        jne    loc_6_010E
        mov    al, byte ptr [0x84c]          ; -> obj1:0x084C
        mov    dword ptr [0x850], 1          ; -> obj1:0x0850
        jmp    loc_6_010E
loc_6_00D5:
        cmp    dx, 0x3df
        jne    loc_6_010E
        cmp    dword ptr [0x850], 1          ; -> obj1:0x0850
        jne    loc_6_010E
        mov    al, byte ptr [0x84c]          ; -> obj1:0x084C
        jmp    loc_6_010E
loc_6_00EC:
        cmp    ecx, 4
        je     loc_6_0104
        cmp    ecx, 0
        je     loc_6_010B
        cmp    ecx, 0xc
        je     loc_6_0100
        cmp    ecx, 8
        je     loc_6_0107
loc_6_0100:
        out    dx, ax
        jmp    loc_6_010E
loc_6_0104:
        out    dx, al
        jmp    loc_6_010E
loc_6_0107:
        in     ax, dx
        jmp    loc_6_010E
loc_6_010B:
        in     al, dx
        jmp    loc_6_010E
loc_6_010E:
        pop    esi
        pop    edi
        ret    
loc_6_0111:
        mov    cx, word ptr [0x856]          ; -> loc_1_0856
        shl    ecx, 0x10
        stc    
        ret    
        db  024h, 04Dh, 047h
VxD__LTEXT ENDS
