; VSHARE -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 2242 instructions recovered by recursive descent

Declare_Virtual_Device VSHARE, 1, 0, VSHARE_Control, 00483h, Undefined_Init_Order
;   DDK version 0400h   init_order 058008000h   flags 00000h
;   service table: obj1:0x3dc, 1 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1864  R-X
loc_1_0000:
        push   dword ptr [0x374]             ; -> loc_1_0374
        push   dword ptr [0x37c]             ; -> loc_1_037C
        mov    dword ptr [0x374], ebx        ; -> loc_1_0374
        mov    dword ptr [0x37c], ebp        ; -> loc_1_037C
        call   edx
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        pop    dword ptr [0x37c]             ; -> loc_1_037C
        pop    dword ptr [0x374]             ; -> loc_1_0374
        ret    
sub_1_002D:
        push   ebp
        mov    ebp, esp
        push   dword ptr [0x374]             ; -> loc_1_0374
        push   dword ptr [0x37c]             ; -> loc_1_037C
        mov    dword ptr [0x374], ebx        ; -> loc_1_0374
        mov    dword ptr [0x37c], ebp        ; -> loc_1_037C
        call   dword ptr [ebp + 8]
        pop    dword ptr [0x37c]             ; -> loc_1_037C
        pop    dword ptr [0x374]             ; -> loc_1_0374
        leave  
        ret    4
loc_1_005B:
        push   dword ptr [0x374]             ; -> loc_1_0374
        push   dword ptr [0x37c]             ; -> loc_1_037C
        mov    dword ptr [0x374], ebx        ; -> loc_1_0374
        mov    dword ptr [0x37c], ebp        ; -> loc_1_037C
        push   edx
        call   sub_1_13C0
        pop    dword ptr [0x37c]             ; -> loc_1_037C
        pop    dword ptr [0x374]             ; -> loc_1_0374
        ret    
sub_1_0086:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    ebx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    esi, 0x5b                     ; -> loc_1_005B
        VMMCall  VMM.Schedule_VM_Event           ; cd200f000100
        mov    eax, esi
        pop    ebx
        pop    esi
        leave  
        ret    8
sub_1_00A4:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        leave  
        ret    
sub_1_00B6:
        push   ebp
        mov    ebp, esp
        push   0
        push   dword ptr [ebp + 8]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        leave  
        ret    
sub_1_00C8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        xor    edx, edx
        xor    ebx, ebx
        xor    eax, eax
loc_1_00D6:
        lodsb  al, byte ptr [esi]
        add    ebx, eax
        inc    edx
        and    al, al
        jne    loc_1_00D6
        mov    eax, edx
        shl    eax, 0x10
        mov    ax, bx
        pop    ebx
        pop    esi
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld    
        xor    ecx, ecx
        dec    ecx
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        mov    edx, edi
        xor    al, al
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edi, edx
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        movzx  eax, byte ptr [esi - 1]
        movzx  edx, byte ptr [edi - 1]
        sub    eax, edx
        pop    edi
        pop    esi
        leave  
        ret    8
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld    
        mov    edx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        xor    ecx, ecx
        dec    ecx
        xor    al, al
        mov    edi, esi
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    edi, edx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, edx
        pop    edi
        pop    esi
        leave  
        ret    8
        push   ebp
        mov    ebp, esp
        push   edi
        cld    
        mov    edi, dword ptr [ebp + 8]
        xor    al, al
        xor    ecx, ecx
        dec    ecx
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        mov    eax, ecx
        pop    edi
        leave  
        ret    4
sub_1_0153:
        push   ebp
        mov    ebp, esp
        push   edi
        cld    
        mov    edi, dword ptr [ebp + 8]
        xor    al, al
        mov    ecx, dword ptr [ebp + 0xc]
        rep stosb byte ptr es:[edi], al
        pop    edi
        leave  
        ret    8
loc_1_0167:
        push   esi
        push   ebx
        dec    eax
        inc    ecx
        push   edx
        inc    ebp
        cmp    ah, byte ptr [eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
        je     0x1d8
        jb     loc_1_01E3
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x72], ah
        jb     0x1eb
        jb     0x1ac
        db  000h
sub_1_017F:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    esi, 0x167                    ; -> loc_1_0167
        xor    eax, eax
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
        leave  
        ret    4
        push   ebp
        mov    ebp, esp
        leave  
        ret    8
sub_1_019D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [0x37c]        ; -> loc_1_037C
        mov    byte ptr [edx + 0x1d], 0x12
        mov    byte ptr [edx + 0x1c], al
        mov    eax, dword ptr [0x380]        ; -> loc_1_0380
        shr    eax, 4
        mov    word ptr [edx + 0x3c], ax
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
loc_1_01E3:
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        leave  
        ret    4
sub_1_01F5:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    ebx, dword ptr [0x374]        ; -> loc_1_0374
        mov    eax, dword ptr [0x3b5]        ; -> loc_1_03B5
        add    eax, dword ptr [ebx + 4]
        mov    eax, dword ptr [eax]
        movzx  edx, ax
        shr    eax, 0x10
        shl    eax, 4
        add    eax, edx
        add    eax, dword ptr [ebx + 4]
        mov    edi, eax
        mov    edx, dword ptr [ebp + 8]
loc_1_021D:
        cmp    dx, word ptr [edi + 4]
        jb     loc_1_0243
        sub    dx, word ptr [edi + 4]
        mov    edi, dword ptr [edi]
        cmp    di, -1
        je     loc_1_0259
        mov    eax, edi
        and    eax, 0xffff
        shr    edi, 0x10
        shl    edi, 4
        add    edi, eax
        add    edi, dword ptr [ebx + 4]
        jmp    loc_1_021D
loc_1_0243:
        mov    eax, dword ptr [0x3d4]        ; -> loc_1_03D4
        push   edx
        mul    edx
        pop    edx
        add    edi, eax
        add    edi, 6
        mov    eax, edi
loc_1_0253:
        pop    ebx
        pop    edi
        leave  
        ret    4
loc_1_0259:
        xor    eax, eax
        jmp    loc_1_0253
sub_1_025D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [0x374]        ; -> loc_1_0374
        sub    eax, dword ptr [ebx + 4]
        mov    ecx, dword ptr [0x1820]       ; -> loc_1_1820
        mov    edi, dword ptr [0x1824]       ; -> loc_1_1824
        repne scasd eax, dword ptr es:[edi]
        xor    ecx, ecx
        sub    edi, 4
        cmp    eax, dword ptr [edi]
        mov    eax, ecx
        jne    loc_1_0289
        inc    eax
loc_1_0289:
        pop    edi
        pop    ecx
        pop    ebx
        leave  
        ret    4
VSHARE_PM_API:
        cmp    word ptr [ebp + 0x1c], 0
        jne    loc_1_029D
        mov    word ptr [ebp + 0x1c], 0x110
loc_1_029D:
        ret    
loc_1_029E:
        cmp    byte ptr [ebp + 0x1d], 0x10
        stc    
        jne    loc_1_02A6
        clc    
loc_1_02A6:
        ret    
loc_1_02A7:
        test   edx, 3
        je     loc_1_02B9
        push   0x1680                        ; -> loc_1_1680
        call   sub_1_002D
loc_1_02B9:
        clc    
        ret    
VSHARE_Control:
        cmp    eax, 0xc
        jae    loc_1_02C7
        jmp    dword ptr [eax*4 + 0x2c9]     ; -> obj1:0x02C9
loc_1_02C7:
        clc    
        ret    
        ; ---- dispatch table (12 entries) ----
        dd  loc_2_0317
        dd  loc_2_01A4
        dd  loc_1_02C7
        dd  loc_1_02C7
        dd  loc_1_02C7
        dd  loc_1_02F9
        dd  loc_1_02C7
        dd  loc_1_02C7
        dd  loc_1_02C7
        dd  loc_1_02C7
        dd  loc_1_02C7
        dd  loc_1_02A7
loc_1_02F9:
        push   esi
        push   edi
        mov    eax, dword ptr [0x3a1]        ; -> obj1:0x03A1
        mov    edi, eax
        mov    esi, 0x430                    ; -> obj1:0x0430
        mov    ecx, 0xf
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [0x399]        ; -> obj1:0x0399
        mov    cl, byte ptr [0x470]          ; -> obj1:0x0470
        mov    word ptr [eax], cx
        mov    edx, dword ptr [0x46c]        ; -> obj1:0x046C
        or     edx, edx
        je     loc_1_0339
        mov    eax, dword ptr [0x3bd]        ; -> loc_1_03BD
        mov    dword ptr [eax + 0x1a], edx
        mov    eax, dword ptr [0x3a5]        ; -> loc_1_03A5
        mov    word ptr [eax], 0
loc_1_0339:
        push   0x14b0                        ; -> loc_1_14B0
        call   sub_1_002D
        pop    edi
        pop    esi
        clc    
        ret    
VSHARE_Service_00:
        mov    eax, 0x110
        clc    
        ret    
sub_1_034E:
        VxDCall  IFSMgr.IFSMgr_CheckAccessConflict ; cd2052804000
sub_1_0354:
        VxDCall  IFSMgr.IFSMgr_CheckLocks        ; cd2056804000
sub_1_035A:
        VxDCall  IFSMgr.IFSMgr_CountLocks        ; cd2057804000
sub_1_0360:
        VxDCall  IFSMgr.IFSMgr_LockFile          ; cd2053804000
sub_1_0366:
        VxDCall  IFSMgr.IFSMgr_RemoveLocks       ; cd2055804000
sub_1_036C:
        VxDCall  IFSMgr.IFSMgr_UnlockFile        ; cd2054804000
        add    byte ptr [eax], al
loc_1_0374:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0378:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_037C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0380:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0384:
        add    dword ptr [eax + eax], esp
        add    byte ptr [eax], al
loc_1_0389:
        into   
        add    eax, 0x3300000
        add    byte ptr [eax], al
loc_1_0391:
        and    eax, dword ptr [ebx]
        add    byte ptr [eax], al
loc_1_0395:
        test   byte ptr [0x3030000], 0
        add    byte ptr [ebp + eax + 0x900000], ch
        add    byte ptr [eax], al
loc_1_03A5:
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [esi - 0x51fffffd], bh
        add    eax, 0x33c0000
        add    byte ptr [eax], al
loc_1_03B5:
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        db  000h
loc_1_03BD:
        add    byte ptr es:[eax], al
        add    byte ptr [esi + 0x3e000005], bl
        add    eax, dword ptr [eax]
        add    byte ptr [edx], bh
        add    eax, dword ptr [eax]
        add    byte ptr [edx + 5], dh
        add    byte ptr [eax], al
loc_1_03D4:
        cmp    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_03D8:
        pop    es
        add    byte ptr [eax], al
        dd  VSHARE_Service_00        ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00483h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VSHARE  '                  ; DDB_Name
        dd  058008000h                  ; DDB_Init_Order
        dd  VSHARE_Control           ; DDB_Control_Proc
        dd  VSHARE_PM_API            ; DDB_V86_API_Proc
        dd  VSHARE_PM_API            ; DDB_PM_API_Proc
        dd  obj1:0x03DC              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0480:
        mov    ecx, dword ptr [0x37c]        ; -> loc_1_037C
        mov    ax, word ptr [esp + 4]
        or     word ptr [ecx + 0x2c], 1
        mov    word ptr [ecx + 0x1c], ax
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [0x391]        ; -> loc_1_0391
        mov    byte ptr [ecx + eax], 1
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_04B0:
        push   1
        call   sub_1_0480
        add    esp, 4
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_04C0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        xor    ecx, ecx
        and    word ptr [eax + 0x2c], 0xfffe
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        mov    edi, dword ptr [0x380]        ; -> loc_1_0380
        mov    esi, dword ptr [eax + 4]
        mov    eax, dword ptr [0x3cd]        ; -> obj1:0x03CD
        mov    cx, word ptr [esi + eax]
        add    ecx, esi
        add    edi, ecx
        mov    eax, dword ptr [0x3c1]        ; -> obj1:0x03C1
        mov    eax, dword ptr [esi + eax]
        push   eax
        call   sub_1_1580
        lea    ebp, [esi + eax]
        push   1
        push   edi
        call   sub_1_0F50
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_05C2
        mov    cl, byte ptr [0x1818]         ; -> loc_1_1818
        or     cl, cl
        je     loc_1_058D
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        mov    edx, dword ptr [eax + 4]
        mov    eax, dword ptr [0x3c1]        ; -> obj1:0x03C1
        mov    eax, dword ptr [edx + eax]
        mov    dword ptr [esp + 0x10], eax
        mov    ax, word ptr [0x3d8]          ; -> loc_1_03D8
        cmp    word ptr [esp + 0x10], ax
        jne    loc_1_0586
        mov    eax, dword ptr [0x380]        ; -> loc_1_0380
        xor    edx, edx
        mov    dx, word ptr [esp + 0x12]
        shr    eax, 4
        cmp    eax, edx
        jne    loc_1_0586
        cmp    cl, 1
        jne    loc_1_0570
        mov    edi, 0x1828                   ; -> loc_1_1828
        mov    esi, ebp
        mov    ecx, 0xe
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    byte ptr [0x1818], 2          ; -> loc_1_1818
        jmp    loc_1_058D
loc_1_0570:
        cmp    cl, 3
        jne    loc_1_0586
        mov    esi, 0x1828                   ; -> loc_1_1828
        mov    edi, ebp
        mov    ecx, 0xe
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        jmp    loc_1_058D
loc_1_0586:
        mov    byte ptr [0x1818], 0          ; -> loc_1_1818
loc_1_058D:
        push   ebp
        push   ebx
        call   sub_1_1060
        add    esp, 8
        or     eax, eax
        jne    loc_1_05C2
        mov    eax, dword ptr [ebx + 0xc]
        cmp    dword ptr [eax], 0
        jne    loc_1_05C2
        lea    esi, [ebp + 0xb]
        lea    edi, [ebx + 0x10]
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        cmp    byte ptr [0x384], 1           ; -> loc_1_0384
        jne    loc_1_05C2
        mov    ax, word ptr [ebp + 0x35]
        mov    word ptr [ebx + 0x20], ax
loc_1_05C2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_05D0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     loc_1_0621
        mov    ax, word ptr [esi]
        or     ax, ax
        je     loc_1_05F0
        cmp    ax, 0xffff
        jne    loc_1_0621
loc_1_05F0:
        mov    esi, dword ptr [esi + 0x2b]
        push   esi
        call   sub_1_1270
        add    esp, 4
        mov    eax, dword ptr [esi + 0xc]
        push   esi
        mov    dword ptr [eax + 0x2b], 0
        mov    eax, dword ptr [esi + 0xc]
        mov    word ptr [eax + 0x33], 0
        call   sub_1_12C0
        add    esp, 4
        push   esi
        call   sub_1_00B6
        add    esp, 4
loc_1_0621:
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0630:
        call   sub_1_15D0
        push   eax
        call   sub_1_05D0
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        add    esp, 4
        and    word ptr [eax + 0x2c], 0xfffe
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0650:
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [0x3c5]        ; -> obj1:0x03C5
        mov    ax, word ptr [ecx + eax]
        push   eax
        call   sub_1_1310
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        add    esp, 8
        and    word ptr [eax + 0x2c], 0xfffe
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0680:
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [0x3b1]        ; -> obj1:0x03B1
        mov    ax, word ptr [ecx + eax]
        push   eax
        mov    eax, dword ptr [0x3c5]        ; -> obj1:0x03C5
        mov    ax, word ptr [ecx + eax]
        push   eax
        call   sub_1_1310
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        add    esp, 8
        and    word ptr [eax + 0x2c], 0xfffe
        ret    
loc_1_06B0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_1600
        mov    cx, word ptr [eax + 0xc]
        mov    word ptr [esp + 0x12], cx
        mov    ax, word ptr [eax + 6]
        mov    word ptr [esp + 0x10], ax
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        call   sub_1_1580
        mov    ecx, dword ptr [0x374]        ; -> loc_1_0374
        mov    edi, dword ptr [ecx + 4]
        add    edi, eax
        mov    eax, ecx
loc_1_06E5:
        push   eax
        push   edi
        call   sub_1_17D0
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        mov    ebx, dword ptr [0x3c1]        ; -> obj1:0x03C1
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        je     loc_1_073C
        mov    ecx, dword ptr [esi + 0xc]
        cmp    word ptr [ecx], 1
        jne    loc_1_071D
        push   esi
        call   sub_1_0DC0
        add    esp, 4
        mov    ebx, dword ptr [0x3c1]        ; -> obj1:0x03C1
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
loc_1_071D:
        mov    ebp, dword ptr [eax + 4]
        mov    eax, dword ptr [esi + 0xc]
        sub    eax, ebp
        push   eax
        call   sub_1_15A0
        mov    dword ptr [ebx + ebp], eax
        push   1
        call   sub_1_019D
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        jmp    loc_1_06E5
loc_1_073C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0750:
        push   0x760                         ; -> loc_1_0760
        call   sub_1_0830
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_0760:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        push   esi
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [0x3b1]        ; -> obj1:0x03B1
        mov    ax, word ptr [ecx + eax]
        and    eax, 0xffff
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x14]
        shl    eax, 0x10
        add    eax, ecx
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        push   eax
        mov    eax, dword ptr [esi + 4]
        add    eax, 8
        push   eax
        call   sub_1_0360
        add    esp, 0x14
        or     ax, ax
        je     loc_1_07B1
        push   eax
        call   sub_1_0480
        add    esp, 4
        pop    esi
        ret    
loc_1_07B1:
        push   esi
        call   sub_1_08A0
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_07C0:
        push   0x7d0                         ; -> loc_1_07D0
        call   sub_1_0830
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_07D0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        push   esi
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [0x3b1]        ; -> obj1:0x03B1
        mov    ax, word ptr [ecx + eax]
        and    eax, 0xffff
        xor    ecx, ecx
        mov    cx, word ptr [esi + 0x14]
        shl    eax, 0x10
        add    eax, ecx
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        push   eax
        mov    eax, dword ptr [esp + 0x18]
        push   eax
        mov    eax, dword ptr [esi + 4]
        add    eax, 8
        push   eax
        call   sub_1_036C
        add    esp, 0x14
        or     ax, ax
        je     loc_1_0821
        push   eax
        call   sub_1_0480
        add    esp, 4
        pop    esi
        ret    
loc_1_0821:
        push   esi
        call   sub_1_08A0
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0830:
        push   esi
        push   edi
        cmp    byte ptr [0x384], 1           ; -> loc_1_0384
        mov    edx, dword ptr [0x37c]        ; -> loc_1_037C
        jne    loc_1_0861
        cmp    word ptr [edx + 0x18], 1
        je     loc_1_0855
        push   0x21
        call   sub_1_0480
        add    esp, 4
        pop    edi
        pop    esi
        ret    
loc_1_0855:
        call   sub_1_1640
        mov    edi, dword ptr [eax]
        mov    esi, dword ptr [eax + 4]
        jmp    loc_1_0885
loc_1_0861:
        xor    eax, eax
        mov    ax, word ptr [edx + 0x18]
        shl    eax, 0x10
        xor    ecx, ecx
        mov    cx, word ptr [edx + 0x14]
        lea    edi, [eax + ecx]
        xor    eax, eax
        mov    ax, word ptr [edx + 4]
        shl    eax, 0x10
        xor    ecx, ecx
        mov    cx, word ptr [edx + 0x1c]
        lea    esi, [eax + ecx]
loc_1_0885:
        call   sub_1_15D0
        push   esi
        push   edi
        mov    eax, dword ptr [eax + 0x2b]
        push   eax
        call   dword ptr [esp + 0x18]
        add    esp, 0xc
        pop    edi
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_08A0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    eax, dword ptr [eax + 4]
        mov    esi, dword ptr [eax + 0xc]
        or     esi, esi
        je     loc_1_08C2
        nop    
loc_1_08B0:
        or     byte ptr [esi + 0x1b], 0x10
        push   4
        push   esi
        call   sub_1_1380
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_08B0
loc_1_08C2:
        pop    esi
        ret    4
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_08D0:
        push   ebx
        push   esi
        push   edi
        call   sub_1_15D0
        mov    ecx, dword ptr [0x37c]        ; -> loc_1_037C
        mov    esi, dword ptr [eax + 0x2b]
        and    word ptr [ecx + 0x2c], 0xfffe
        push   eax
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     loc_1_095A
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [eax + 8]
        or     ecx, ecx
        je     loc_1_095A
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        mov    di, word ptr [eax + 0x18]
        or     di, di
        je     loc_1_095A
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        push   esi
        mov    edx, dword ptr [eax + 4]
        mov    eax, dword ptr [0x3b1]        ; -> obj1:0x03B1
        xor    ebx, ebx
        mov    bx, word ptr [edx + eax]
        xor    eax, eax
        mov    ax, word ptr [esi + 0x14]
        shl    ebx, 0x10
        add    ebx, eax
        push   ebx
        mov    eax, edi
        and    eax, 0xffff
        push   eax
        mov    eax, dword ptr [0x389]        ; -> loc_1_0389
        mov    eax, dword ptr [edx + eax]
        push   eax
        push   ecx
        call   sub_1_0354
        add    esp, 0x14
        or     eax, eax
        je     loc_1_095A
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        mov    word ptr [eax + 0x1c], 0x21
        or     word ptr [eax + 0x2c], 1
loc_1_095A:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Bh, 0FFh
loc_1_0960:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_1600
        xor    ecx, ecx
        mov    cx, word ptr [eax + 2]
        xor    esi, esi
        mov    ebx, dword ptr [0x37c]        ; -> loc_1_037C
        mov    si, word ptr [eax + 4]
        mov    ax, word ptr [ebx + 0x34]
        mov    word ptr [ebx + 0x38], ax
        mov    edx, dword ptr [0x3a9]        ; -> obj1:0x03A9
        sub    dx, word ptr [0x380]          ; -> loc_1_0380
        mov    word ptr [ebx], dx
        mov    edx, dword ptr [0x1800]       ; -> obj1:0x1800
        cmp    edx, 0x1800                   ; -> obj1:0x1800
        je     loc_1_09CA
loc_1_09A4:
        mov    eax, ecx
        dec    ecx
        or     eax, eax
        jne    loc_1_09C0
        mov    ebp, dword ptr [edx + 0xc]
        or     ebp, ebp
        je     loc_1_09C0
loc_1_09B2:
        mov    eax, esi
        dec    esi
        or     eax, eax
        je     loc_1_09E0
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        jne    loc_1_09B2
loc_1_09C0:
        mov    edx, dword ptr [edx]
        cmp    edx, 0x1800                   ; -> obj1:0x1800
        jne    loc_1_09A4
loc_1_09CA:
        push   0x12
        call   sub_1_0480
        add    esp, 4
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_09E0:
        lea    edi, [edx + 0x29]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    dword ptr [esp + 0x10], ecx
        mov    esi, edi
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        mov    edi, dword ptr [0x3a9]        ; -> obj1:0x03A9
        shr    ecx, 2
        add    edi, dword ptr [eax + 4]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [esp + 0x10]
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   ebp
        mov    eax, dword ptr [edx + 8]
        push   eax
        call   sub_1_035A
        add    esp, 8
        mov    word ptr [ebx + 0x18], ax
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        mov    cx, word ptr [ebp + 0x14]
        and    word ptr [eax + 0x2c], 0xfffe
        mov    word ptr [eax + 0x10], cx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_0A40:
        push   esi
        call   sub_1_15D0
        mov    esi, eax
        call   sub_1_1600
        mov    cl, byte ptr [esi + 4]
        mov    byte ptr [eax + 0x1e], cl
        mov    cx, word ptr [esi + 0xb]
        mov    word ptr [eax + 0x1a], cx
        mov    word ptr [eax + 0x1c], 0
        mov    ecx, dword ptr [esi + 0x2b]
        or     ecx, ecx
        je     loc_1_0A73
        mov    ecx, dword ptr [ecx + 4]
        mov    cx, word ptr [ecx + 0x25]
        mov    word ptr [eax + 0x1c], cx
loc_1_0A73:
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        pop    esi
        mov    byte ptr [eax + 0x10], 0xc0
        ret    
        db  08Bh, 0FFh
loc_1_0A80:
        push   esi
        push   edi
        call   sub_1_15D0
        mov    edi, eax
        call   sub_1_1600
        mov    esi, eax
        push   edi
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     loc_1_0AC9
        cmp    word ptr [edi], 0
        je     loc_1_0AC9
        mov    eax, dword ptr [edi + 0x2b]
        mov    eax, dword ptr [eax + 4]
        mov    cx, word ptr [esi + 0x1c]
        cmp    word ptr [eax + 0x25], cx
        jne    loc_1_0AC9
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        mov    cx, word ptr [esi + 0x1a]
        and    word ptr [eax + 0x2c], 0xfffe
        mov    word ptr [eax + 0x10], cx
        pop    edi
        pop    esi
        ret    
loc_1_0AC9:
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        pop    edi
        or     word ptr [eax + 0x2c], 1
        pop    esi
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0AE0:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [0x37c]        ; -> loc_1_037C
        or     word ptr [edx + 0x2c], 1
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [0x39d]        ; -> obj1:0x039D
        mov    ax, word ptr [ecx + eax]
        mov    word ptr [edx + 0x1c], ax
        call   sub_1_1600
        mov    edi, eax
        push   edi
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     loc_1_0BDC
        mov    ax, word ptr [edi + 2]
        and    al, 0xf0
        cmp    al, 0x70
        jne    loc_1_0BDC
        sub    esi, esi
        add    eax, 0
loc_1_0B30:
        push   esi
        call   sub_1_01F5
        mov    ebx, eax
        or     ebx, ebx
        je     loc_1_0BDC
        cmp    ebx, edi
        je     loc_1_0B83
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     loc_1_0B83
        cmp    word ptr [ebx], 0
        je     loc_1_0B83
        mov    ax, word ptr [edi + 2]
        cmp    word ptr [ebx + 2], ax
        jne    loc_1_0B83
        mov    eax, dword ptr [ebx + 0x2b]
        mov    ecx, dword ptr [edi + 0x2b]
        mov    eax, dword ptr [eax + 4]
        cmp    dword ptr [ecx + 4], eax
        jne    loc_1_0B83
        mov    ax, word ptr [edi + 0x2f]
        cmp    word ptr [ebx + 0x2f], ax
        jne    loc_1_0B83
        mov    ax, word ptr [edi + 0x31]
        cmp    word ptr [ebx + 0x31], ax
        je     loc_1_0B86
loc_1_0B83:
        inc    esi
        jmp    loc_1_0B30
loc_1_0B86:
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        push   ebx
        and    word ptr [eax + 0x2c], 0xfffe
        mov    word ptr [ebx], 0
        call   sub_1_05D0
        add    esp, 4
        inc    word ptr [edi]
        sub    edi, edi
loc_1_0BA4:
        push   edi
        call   sub_1_1740
        add    esp, 4
        or     eax, eax
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        mov    ecx, dword ptr [0x3ad]        ; -> obj1:0x03AD
        je     loc_1_0BDC
        cmp    edi, esi
        jne    loc_1_0BD9
        mov    ebx, dword ptr [eax + 4]
        mov    eax, dword ptr [ecx + ebx]
        push   eax
        call   sub_1_1580
        mov    byte ptr [ebx + eax], 0xff
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        mov    word ptr [eax + 0x1c], si
loc_1_0BD9:
        inc    edi
        jmp    loc_1_0BA4
loc_1_0BDC:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0BE0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        or     word ptr [eax + 0x2c], 1
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        mov    byte ptr [0x1818], 1          ; -> loc_1_1818
        mov    eax, dword ptr [eax + 4]
        mov    ecx, dword ptr [0x3cd]        ; -> obj1:0x03CD
        mov    cx, word ptr [eax + ecx]
        and    ecx, 0xffff
        add    ecx, eax
        mov    eax, dword ptr [0x380]        ; -> loc_1_0380
        add    eax, ecx
        mov    dword ptr [esp + 0x10], eax
loc_1_0C20:
        mov    eax, dword ptr [esp + 0x10]
        push   0
        push   eax
        call   sub_1_0F50
        add    esp, 8
        or     eax, eax
        mov    edi, dword ptr [0x3c5]        ; -> obj1:0x03C5
        mov    ebx, dword ptr [0x374]        ; -> loc_1_0374
        je     loc_1_0C92
        mov    edx, dword ptr [eax + 0xc]
        or     edx, edx
        je     loc_1_0C84
loc_1_0C46:
        mov    si, word ptr [edx + 0x18]
        and    si, 0xf0
        mov    ebp, dword ptr [ebx + 4]
        mov    ecx, dword ptr [0x3b1]        ; -> obj1:0x03B1
        mov    cx, word ptr [ebp + ecx]
        cmp    word ptr [edx + 0x16], cx
        jne    loc_1_0C92
        mov    cx, word ptr [edx + 0x14]
        cmp    word ptr [ebp + edi], cx
        jne    loc_1_0C92
        cmp    dword ptr [edx + 8], ebx
        jne    loc_1_0C92
        cmp    si, 0x70
        je     loc_1_0C7E
        or     si, si
        jne    loc_1_0C92
loc_1_0C7E:
        mov    edx, dword ptr [edx]
        or     edx, edx
        jne    loc_1_0C46
loc_1_0C84:
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        call   sub_1_0DC0
        add    esp, 4
        jmp    loc_1_0C20
loc_1_0C92:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0CA0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_15D0
        mov    ebx, eax
        test   word ptr [ebx + 5], 0x8080
        jne    loc_1_0DAC
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     loc_1_0DAC
        mov    eax, dword ptr [ebx + 0x2b]
        mov    ebp, dword ptr [0x37c]        ; -> loc_1_037C
        mov    edx, dword ptr [eax + 4]
        mov    cx, word ptr [ebp + 0x1c]
        mov    word ptr [esp + 0x10], cx
        and    ecx, 0xffff
        or     ecx, ecx
        jl     loc_1_0DAC
        cmp    ecx, 2
        jle    loc_1_0D00
        cmp    ecx, 3
        je     loc_1_0D50
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_0D00:
        lea    esi, [ebx + 0xb]
        lea    edi, [edx + 0x10]
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        cmp    byte ptr [0x384], 1           ; -> loc_1_0384
        jne    loc_1_0D1F
        mov    ax, word ptr [ebx + 0x35]
        mov    word ptr [edx + 0x20], ax
loc_1_0D1F:
        mov    esi, dword ptr [edx + 0xc]
        or     esi, esi
        je     loc_1_0DAC
loc_1_0D2A:
        cmp    dword ptr [ebx + 0x2b], esi
        je     loc_1_0D40
        mov    ax, word ptr [ebp + 0x1c]
        push   eax
        push   esi
        call   sub_1_1380
        mov    ebp, dword ptr [0x37c]        ; -> loc_1_037C
loc_1_0D40:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_0D2A
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Bh, 0FFh
loc_1_0D50:
        mov    ebx, dword ptr [edx + 0xc]
        or     ebx, ebx
        je     loc_1_0DAC
loc_1_0D57:
        cmp    eax, ebx
        jne    loc_1_0D70
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_1_0D57
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D70:
        mov    ecx, dword ptr [0x374]        ; -> loc_1_0374
        cmp    dword ptr [ebx + 8], ecx
        jne    loc_1_0DA0
        mov    esi, dword ptr [ebx + 0xc]
        lea    edi, [edx + 0x10]
        add    esi, 0xb
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        cmp    byte ptr [0x384], 1           ; -> loc_1_0384
        jne    loc_1_0DA0
        mov    ecx, dword ptr [ebx + 0xc]
        mov    cx, word ptr [ecx + 0x35]
        mov    word ptr [edx + 0x20], cx
loc_1_0DA0:
        mov    cx, word ptr [esp + 0x10]
        push   ecx
        push   eax
        call   sub_1_1380
loc_1_0DAC:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0DC0:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    di, di
        mov    esi, dword ptr [esp + 0x18]
        mov    edi, edi
loc_1_0DD0:
        mov    eax, edi
        and    eax, 0xffff
        push   eax
        call   sub_1_01F5
        or     eax, eax
        je     loc_1_0DF0
        cmp    dword ptr [esi + 0xc], eax
        je     loc_1_0DF9
        inc    di
        jmp    loc_1_0DD0
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_0DF0:
        cmp    dword ptr [esi + 0xc], eax
        jne    loc_1_0E7F
loc_1_0DF9:
        mov    ebx, dword ptr [0x374]        ; -> loc_1_0374
        mov    word ptr [esp + 0x10], 0
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [0x385]        ; -> obj1:0x0385
        mov    ax, word ptr [ecx + eax]
        mov    word ptr [esp + 0x12], ax
loc_1_0E17:
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        call   sub_1_1580
        mov    ebx, dword ptr [0x374]        ; -> loc_1_0374
        mov    ebp, dword ptr [ebx + 4]
        add    ebp, eax
        cmp    byte ptr [ebp], 0x4d
        jne    loc_1_0E69
        mov    ax, word ptr [ebp + 1]
        xor    ecx, ecx
        mov    cx, word ptr [esp + 0x12]
        mov    edx, eax
        and    edx, 0xffff
        sub    ecx, edx
        cmp    ecx, -1
        jne    loc_1_0E5C
        push   edi
        push   eax
        call   sub_1_0EB0
        add    esp, 8
        mov    ebx, dword ptr [0x374]        ; -> loc_1_0374
loc_1_0E5C:
        mov    ax, word ptr [ebp + 3]
        inc    ax
        add    word ptr [esp + 0x12], ax
        jmp    loc_1_0E17
loc_1_0E69:
        push   edi
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, dword ptr [0x38d]        ; -> obj1:0x038D
        mov    ax, word ptr [ecx + eax]
        push   eax
        call   sub_1_0EB0
        add    esp, 8
loc_1_0E7F:
        mov    ebx, dword ptr [0x374]        ; -> loc_1_0374
        mov    eax, dword ptr [esi + 0xc]
        mov    edi, dword ptr [ebx + 4]
        sub    eax, edi
        push   eax
        call   sub_1_15A0
        mov    ecx, dword ptr [0x3c1]        ; -> obj1:0x03C1
        mov    dword ptr [edi + ecx], eax
        push   1
        call   sub_1_019D
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  005h, 000h, 000h, 000h, 000h
sub_1_0EB0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 0x10], 0
        mov    ax, word ptr [esp + 0x1c]
        mov    word ptr [esp + 0x12], ax
        mov    si, word ptr [esp + 0x20]
        mov    edi, dword ptr [0x374]        ; -> loc_1_0374
loc_1_0ED3:
        mov    eax, dword ptr [esp + 0x10]
        push   eax
        call   sub_1_1580
        mov    ebx, dword ptr [edi + 4]
        add    ebx, eax
        mov    edi, dword ptr [0x374]        ; -> loc_1_0374
        cmp    word ptr [ebx], 0x20cd
        jne    loc_1_0F3A
        mov    bp, word ptr [ebx + 0x32]
        mov    eax, dword ptr [ebx + 0x34]
        push   eax
        call   sub_1_1580
        mov    ecx, dword ptr [edi + 4]
        add    ecx, eax
        mov    ax, bp
        dec    bp
        or     ax, ax
        mov    edi, dword ptr [0x374]        ; -> loc_1_0374
        je     loc_1_0F2E
        mov    eax, esi
        and    eax, 0xffff
loc_1_0F18:
        xor    edx, edx
        mov    dl, byte ptr [ecx]
        cmp    edx, eax
        jne    loc_1_0F23
        mov    byte ptr [ecx], 0xff
loc_1_0F23:
        inc    ecx
        mov    dx, bp
        dec    bp
        or     dx, dx
        jne    loc_1_0F18
loc_1_0F2E:
        mov    eax, dword ptr [ebx + 0x38]
        mov    dword ptr [esp + 0x10], eax
        cmp    eax, -1
        jne    loc_1_0ED3
loc_1_0F3A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0F50:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x18]
        push   edi
        call   sub_1_00C8
        mov    dword ptr [esp + 0x10], eax
        mov    ebx, dword ptr [0x1800]       ; -> obj1:0x1800
        cmp    ebx, 0x1800                   ; -> obj1:0x1800
        je     loc_1_0FB6
loc_1_0F73:
        mov    ax, word ptr [esp + 0x10]
        cmp    word ptr [ebx + 0x27], ax
        jne    loc_1_0FAC
        mov    eax, edi
        lea    ecx, [ebx + 0x29]
loc_1_0F83:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    loc_1_0FA3
        or     dl, dl
        je     loc_1_0F9F
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    loc_1_0FA3
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    loc_1_0F83
loc_1_0F9F:
        sub    eax, eax
        jmp    loc_1_0FA8
loc_1_0FA3:
        sbb    eax, eax
        sbb    eax, -1
loc_1_0FA8:
        or     eax, eax
        je     loc_1_0FC7
loc_1_0FAC:
        mov    ebx, dword ptr [ebx]
        cmp    ebx, 0x1800                   ; -> obj1:0x1800
        jne    loc_1_0F73
loc_1_0FB6:
        cmp    dword ptr [esp + 0x1c], 0
        jne    loc_1_0FD1
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_0FC7:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_0FD1:
        xor    eax, eax
        mov    ax, word ptr [esp + 0x12]
        add    eax, 0x2b
        push   eax
        call   sub_1_00A4
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        je     loc_1_1044
        push   0x2b
        push   ebp
        call   sub_1_0153
        mov    ax, word ptr [esp + 0x10]
        mov    word ptr [ebp + 0x27], ax
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp + 0x29]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ax, word ptr [0x1808]         ; -> obj1:0x1808
        mov    word ptr [ebp + 0x25], ax
        inc    ax
        mov    word ptr [0x1808], ax         ; -> obj1:0x1808
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp], eax
        mov    dword ptr [ebp + 4], ebx
        mov    dword ptr [ebx], ebp
        mov    dword ptr [eax + 4], ebp
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_1044:
        push   0x24
        call   sub_1_0480
        add    esp, 4
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_1060:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    bp, 1
        mov    edi, dword ptr [esp + 0x18]
        push   edi
        call   sub_1_16F0
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_108A
        push   0x24
        call   sub_1_0480
        add    esp, 4
        jmp    loc_1_1146
loc_1_108A:
        push   0x1c
        push   ebx
        call   sub_1_0153
        mov    esi, dword ptr [esp + 0x14]
        mov    dword ptr [ebx + 4], esi
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebx + 0xc], edi
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [0x3c5]        ; -> obj1:0x03C5
        mov    ax, word ptr [ecx + eax]
        mov    word ptr [edi + 0x2f], ax
        mov    word ptr [ebx + 0x14], ax
        mov    eax, dword ptr [0x3b1]        ; -> obj1:0x03B1
        mov    ax, word ptr [ecx + eax]
        mov    word ptr [edi + 0x31], ax
        mov    word ptr [ebx + 0x16], ax
        mov    ax, word ptr [edi + 2]
        mov    word ptr [ebx + 0x18], ax
        mov    al, byte ptr [edi + 4]
        mov    byte ptr [ebx + 0x1a], al
        mov    ax, word ptr [edi + 5]
        test   al, 0x80
        je     loc_1_10EC
        test   al, 7
        je     loc_1_10EC
        cmp    byte ptr [0x1814], 0          ; -> loc_1_1814
        je     loc_1_1111
loc_1_10EC:
        push   ebx
        call   sub_1_11A0
        add    esp, 4
        mov    bp, ax
        or     bp, bp
        je     loc_1_1111
        push   ebp
        call   sub_1_0480
        add    esp, 4
        push   ebx
        call   sub_1_00B6
        add    esp, 4
        jmp    loc_1_1146
loc_1_1111:
        mov    eax, dword ptr [esi + 0xc]
        or     eax, eax
        je     loc_1_1128
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_1128
        push   ebx
        call   sub_1_00B6
        add    esp, 4
        jmp    loc_1_1146
loc_1_1128:
        mov    dword ptr [edi + 0x2b], ebx
        cmp    dword ptr [esi + 8], 1
        sbb    ecx, ecx
        and    ecx, 0x2000
        add    ecx, 0x4853
        mov    word ptr [edi + 0x33], cx
        mov    dword ptr [esi + 0xc], ebx
        mov    dword ptr [ebx], eax
loc_1_1146:
        mov    eax, ebp
        and    eax, 0xffff
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1160:
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx + 8]
        or     edx, edx
        jne    loc_1_1174
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ecx + 8], eax
        sub    eax, eax
        ret    
loc_1_1174:
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx + 8], eax
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [edx + 0x18]
        mov    word ptr [ecx + 0xc], ax
        xor    eax, eax
        mov    ax, word ptr [edx + 0x14]
        mov    dword ptr [ecx], eax
        mov    eax, 1
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_11A0:
        sub    esp, 0x10
        push   esi
        mov    esi, dword ptr [esp + 0x18]
        sub    eax, eax
        mov    ecx, dword ptr [esi + 4]
        mov    ecx, dword ptr [ecx + 0xc]
        mov    dword ptr [esp + 8], ecx
        cmp    ecx, eax
        je     loc_1_1269
        mov    eax, ecx
        mov    dword ptr [esp + 0xc], eax
        mov    al, byte ptr [esi + 0x1a]
        and    eax, 0xffff00ff
        mov    word ptr [esp + 0x12], ax
        mov    ax, word ptr [esi + 0x18]
        mov    word ptr [esp + 0x10], ax
        xor    eax, eax
        mov    ax, word ptr [esi + 0x14]
        mov    dword ptr [esp + 4], eax
        lea    eax, [esp + 4]
        push   eax
        lea    eax, [esp + 8]
        push   0x1160                        ; -> loc_1_1160
        push   eax
        push   1
        call   sub_1_034E
        add    esp, 0x10
        or     eax, eax
        je     loc_1_1269
        cmp    byte ptr [0x180c], 0          ; -> loc_1_180C
        je     loc_1_1267
        cmp    byte ptr [0x1810], 0          ; -> loc_1_1810
        je     loc_1_1233
        mov    ecx, dword ptr [0x378]        ; -> loc_1_0378
        cmp    dword ptr [esi + 8], ecx
        jne    loc_1_124F
        mov    ecx, dword ptr [esi + 0xc]
        add    ecx, 0x28
        cmp    byte ptr [ecx], 0x49
        jne    loc_1_124F
        inc    ecx
        cmp    byte ptr [ecx], 0x4e
        jne    loc_1_124F
        cmp    byte ptr [ecx + 1], 0x49
        jne    loc_1_124F
loc_1_1233:
        cmp    byte ptr [0x384], 1           ; -> loc_1_0384
        jne    loc_1_124F
        mov    ecx, dword ptr [0x374]        ; -> loc_1_0374
        mov    edx, dword ptr [ecx + 4]
        mov    ecx, dword ptr [0x395]        ; -> loc_1_0395
        or     byte ptr [edx + ecx], 2
loc_1_124F:
        mov    cl, byte ptr [0x1818]         ; -> loc_1_1818
        cmp    cl, 2
        jne    loc_1_1269
        inc    cl
        mov    byte ptr [0x1818], cl         ; -> loc_1_1818
        pop    esi
        add    esp, 0x10
        ret    
loc_1_1267:
        sub    eax, eax
loc_1_1269:
        pop    esi
        add    esp, 0x10
        ret    
        db  08Bh, 0FFh
sub_1_1270:
        push   esi
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        mov    edx, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 4]
        xor    ecx, ecx
        mov    eax, dword ptr [0x3b1]        ; -> obj1:0x03B1
        mov    ax, word ptr [esi + eax]
        and    eax, 0xffff
        mov    cx, word ptr [edx + 0x14]
        shl    eax, 0x10
        add    eax, ecx
        mov    ecx, dword ptr [0x3c9]        ; -> obj1:0x03C9
        cmp    word ptr [esi + ecx], 0x5d04
        je     loc_1_12A6
        sub    eax, eax
loc_1_12A6:
        push   edx
        push   eax
        mov    eax, dword ptr [edx + 4]
        add    eax, 8
        push   eax
        call   sub_1_0366
        add    esp, 0xc
        pop    esi
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_12C0:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax + 4]
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [esi + 0xc]
        cmp    edx, eax
        jne    loc_1_12F0
        or     ecx, ecx
        jne    loc_1_12EA
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        push   esi
        call   sub_1_00B6
        add    esp, 4
        pop    esi
        ret    
loc_1_12EA:
        mov    dword ptr [esi + 0xc], ecx
        pop    esi
        ret    
        db  090h
loc_1_12F0:
        cmp    dword ptr [edx], eax
        je     loc_1_12FA
loc_1_12F4:
        mov    edx, dword ptr [edx]
        cmp    dword ptr [edx], eax
        jne    loc_1_12F4
loc_1_12FA:
        mov    eax, dword ptr [edx]
        pop    esi
        mov    eax, dword ptr [eax]
        mov    dword ptr [edx], eax
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1310:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [0x1800]       ; -> obj1:0x1800
        cmp    eax, 0x1800                   ; -> obj1:0x1800
        je     loc_1_1370
        mov    si, word ptr [esp + 0x14]
        mov    di, word ptr [esp + 0x18]
        mov    ecx, dword ptr [0x374]        ; -> loc_1_0374
loc_1_1330:
        mov    ebx, dword ptr [eax]
        mov    eax, dword ptr [eax + 0xc]
        or     eax, eax
        je     loc_1_1366
loc_1_1339:
        mov    ebp, dword ptr [eax]
        cmp    word ptr [eax + 0x14], si
        jne    loc_1_1360
        cmp    dword ptr [eax + 8], ecx
        jne    loc_1_1360
        or     di, di
        je     loc_1_1351
        cmp    word ptr [eax + 0x16], di
        jne    loc_1_1360
loc_1_1351:
        push   eax
        call   sub_1_0DC0
        add    esp, 4
        mov    ecx, dword ptr [0x374]        ; -> loc_1_0374
loc_1_1360:
        mov    eax, ebp
        or     ebp, ebp
        jne    loc_1_1339
loc_1_1366:
        mov    eax, ebx
        cmp    ebx, 0x1800                   ; -> obj1:0x1800
        jne    loc_1_1330
loc_1_1370:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1380:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    al, 1
        mov    cl, byte ptr [esp + 0xc]
        shl    al, cl
        or     byte ptr [esi + 0x1b], al
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [0x374], eax        ; -> loc_1_0374
        jne    loc_1_13A5
        push   esi
        call   sub_1_13C0
        pop    esi
        ret    8
loc_1_13A5:
        cmp    dword ptr [esi + 0x10], 0
        jne    loc_1_13B5
        push   eax
        push   esi
        call   sub_1_0086
        mov    dword ptr [esi + 0x10], eax
loc_1_13B5:
        pop    esi
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_13C0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esi + 4]
        mov    edi, dword ptr [esi + 0xc]
        mov    dl, byte ptr [esi + 0x1b]
        test   dl, 1
        je     loc_1_13DB
        mov    eax, dword ptr [ecx + 0x12]
        mov    dword ptr [edi + 0xd], eax
loc_1_13DB:
        test   dl, 4
        je     loc_1_1410
        mov    eax, dword ptr [ecx + 0x16]
        mov    dword ptr [edi + 0x11], eax
        mov    al, byte ptr [0x384]          ; -> loc_1_0384
        cmp    al, 1
        mov    bx, word ptr [ecx + 0x10]
        mov    word ptr [edi + 0xb], bx
        jne    loc_1_1403
        mov    word ptr [edi + 0x35], bx
        mov    word ptr [edi + 0x19], 0
        jmp    loc_1_1415
loc_1_1403:
        mov    word ptr [edi + 0x1b], bx
        mov    word ptr [edi + 0x19], 0
        jmp    loc_1_1415
        db  090h
loc_1_1410:
        mov    al, byte ptr [0x384]          ; -> loc_1_0384
loc_1_1415:
        test   dl, 2
        je     loc_1_1455
        mov    ebx, dword ptr [ecx + 0x16]
        mov    dword ptr [edi + 0x11], ebx
        mov    bx, word ptr [ecx + 0x10]
        mov    word ptr [edi + 0xb], bx
        cmp    al, 1
        jne    loc_1_1440
        cmp    word ptr [edi + 0x35], 0
        jne    loc_1_1455
        mov    word ptr [edi + 0x19], 0
        mov    word ptr [edi + 0x35], bx
        jmp    loc_1_1455
        db  090h
loc_1_1440:
        cmp    word ptr [edi + 0x1b], 0
        jne    loc_1_1455
        mov    word ptr [edi + 0x19], 0
        mov    bx, word ptr [edi + 0xb]
        mov    word ptr [edi + 0x1b], bx
loc_1_1455:
        test   dl, 8
        je     loc_1_1482
        mov    ebx, dword ptr [ecx + 0x12]
        mov    dword ptr [edi + 0xd], ebx
        mov    ebx, dword ptr [ecx + 0x16]
        mov    dword ptr [edi + 0x11], ebx
        cmp    al, 1
        mov    ax, word ptr [ecx + 0x10]
        mov    word ptr [edi + 0xb], ax
        jne    loc_1_1478
        mov    word ptr [edi + 0x35], ax
        jmp    loc_1_147C
loc_1_1478:
        mov    word ptr [edi + 0x1b], ax
loc_1_147C:
        mov    word ptr [edi + 0x19], 0
loc_1_1482:
        test   dl, 0x10
        je     loc_1_1499
        cmp    dword ptr [ecx + 8], 0
        mov    word ptr [edi + 0x33], 0x6853
        je     loc_1_1499
        mov    word ptr [edi + 0x33], 0x4853
loc_1_1499:
        mov    byte ptr [esi + 0x1b], 0
        mov    dword ptr [esi + 0x10], 0
        pop    edi
        pop    esi
        pop    ebx
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_14B0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        mov    esi, dword ptr [eax + 4]
        mov    eax, dword ptr [0x3bd]        ; -> loc_1_03BD
        mov    eax, dword ptr [esi + eax + 4]
        push   eax
        call   sub_1_1580
        lea    edi, [esi + eax]
        mov    edi, edi
loc_1_14D0:
        mov    si, word ptr [edi + 4]
        lea    ebx, [edi + 6]
        mov    ax, si
        dec    si
        or     ax, ax
        je     loc_1_1508
        sub    ebp, ebp
loc_1_14E3:
        push   ebx
        call   sub_1_17B0
        add    esp, 4
        cmp    eax, ebp
        mov    eax, dword ptr [0x3d4]        ; -> loc_1_03D4
        je     loc_1_14FC
        mov    word ptr [ebx + 0x33], bp
        mov    dword ptr [ebx + 0x2b], ebp
loc_1_14FC:
        add    ebx, eax
        mov    ax, si
        dec    si
        cmp    ax, bp
        jne    loc_1_14E3
loc_1_1508:
        mov    eax, dword ptr [edi]
        push   eax
        call   sub_1_1580
        mov    ecx, dword ptr [0x374]        ; -> loc_1_0374
        mov    edi, dword ptr [ecx + 4]
        add    edi, eax
        cmp    word ptr [edi], 0xffff
        jne    loc_1_14D0
        mov    esi, dword ptr [ecx + 4]
        mov    eax, dword ptr [0x3bd]        ; -> loc_1_03BD
        mov    eax, dword ptr [esi + eax + 0x1a]
        push   eax
        call   sub_1_1580
        add    eax, esi
        lea    esi, [eax + 6]
        mov    di, word ptr [eax + 4]
        mov    ax, di
        dec    di
        or     ax, ax
        je     loc_1_156E
        sub    ebx, ebx
loc_1_1549:
        push   esi
        call   sub_1_17B0
        add    esp, 4
        cmp    eax, ebx
        mov    eax, dword ptr [0x3d4]        ; -> loc_1_03D4
        je     loc_1_1562
        mov    word ptr [esi + 0x33], bx
        mov    dword ptr [esi + 0x2b], ebx
loc_1_1562:
        add    esi, eax
        mov    ax, di
        dec    di
        cmp    ax, bx
        jne    loc_1_1549
loc_1_156E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1580:
        xor    eax, eax
        mov    ax, word ptr [esp + 6]
        shl    eax, 4
        mov    ecx, dword ptr [esp + 4]
        and    ecx, 0xffff
        add    eax, ecx
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_15A0:
        sub    esp, 4
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esp], eax
        shr    eax, 4
        and    word ptr [esp], 0xf
        mov    word ptr [esp + 2], ax
        mov    eax, dword ptr [esp]
        add    esp, 4
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_15D0:
        sub    esp, 4
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        mov    cx, word ptr [eax + 0x38]
        mov    word ptr [esp + 2], cx
        mov    ax, word ptr [eax]
        mov    word ptr [esp], ax
        mov    eax, dword ptr [esp]
        push   eax
        call   sub_1_1580
        mov    ecx, dword ptr [0x374]        ; -> loc_1_0374
        add    eax, dword ptr [ecx + 4]
        add    esp, 4
        ret    
sub_1_1600:
        sub    esp, 4
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        mov    cx, word ptr [eax + 0x3c]
        mov    word ptr [esp + 2], cx
        mov    ax, word ptr [eax + 4]
        mov    word ptr [esp], ax
        mov    eax, dword ptr [esp]
        push   eax
        call   sub_1_1580
        mov    ecx, dword ptr [0x374]        ; -> loc_1_0374
        add    eax, dword ptr [ecx + 4]
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1640:
        sub    esp, 4
        mov    eax, dword ptr [0x37c]        ; -> loc_1_037C
        mov    cx, word ptr [eax + 0x3c]
        mov    word ptr [esp + 2], cx
        mov    ax, word ptr [eax + 0x14]
        mov    word ptr [esp], ax
        mov    eax, dword ptr [esp]
        push   eax
        call   sub_1_1580
        mov    ecx, dword ptr [0x374]        ; -> loc_1_0374
        add    eax, dword ptr [ecx + 4]
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1680:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [0x1800]       ; -> obj1:0x1800
        cmp    esi, 0x1800                   ; -> obj1:0x1800
        je     loc_1_16E7
        mov    edi, dword ptr [esp + 0x10]
loc_1_1699:
        mov    ebp, dword ptr [esi + 0xc]
        mov    esi, dword ptr [esi]
        or     ebp, ebp
        je     loc_1_16DF
        mov    ebx, 0xff
loc_1_16A7:
        mov    ebp, dword ptr [ebp]
        mov    eax, dword ptr [0x374]        ; -> loc_1_0374
        cmp    dword ptr [edi + 8], eax
        jne    loc_1_16D9
        push   edi
        call   sub_1_12C0
        add    esp, 4
        mov    eax, dword ptr [edi + 0xc]
        push   eax
        call   sub_1_025D
        or     eax, eax
        je     loc_1_16D0
        mov    byte ptr [edi + 0x1b], bl
        jmp    loc_1_16D9
        db  090h
loc_1_16D0:
        push   edi
        call   sub_1_00B6
        add    esp, 4
loc_1_16D9:
        mov    edi, ebp
        or     ebp, ebp
        jne    loc_1_16A7
loc_1_16DF:
        cmp    esi, 0x1800                   ; -> obj1:0x1800
        jne    loc_1_1699
loc_1_16E7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  090h
sub_1_16F0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_17B0
        add    esp, 4
        or     eax, eax
        je     loc_1_1729
        mov    eax, dword ptr [esi + 0x2b]
        cmp    byte ptr [eax + 0x1b], 0xff
        jne    loc_1_1720
        mov    word ptr [esi + 0x33], 0
        mov    dword ptr [esi + 0x2b], 0
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
loc_1_1720:
        push   1
        call   sub_1_017F
        pop    esi
        ret    
loc_1_1729:
        push   0x1c
        call   sub_1_00A4
        add    esp, 4
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1740:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [0x374]        ; -> loc_1_0374
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [0x38d]        ; -> obj1:0x038D
        mov    ax, word ptr [ecx + eax]
        mov    word ptr [esp + 6], ax
        mov    word ptr [esp + 4], 0
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_1580
        add    eax, dword ptr [esi + 4]
        xor    ecx, ecx
        mov    cx, word ptr [eax + 0x32]
        mov    edx, dword ptr [esp + 0xc]
        cmp    ecx, edx
        jle    loc_1_17A1
        mov    eax, dword ptr [eax + 0x34]
        mov    dword ptr [esp + 4], eax
        add    word ptr [esp + 4], dx
        mov    eax, dword ptr [esp + 4]
        push   eax
        call   sub_1_1580
        mov    ecx, dword ptr [0x374]        ; -> loc_1_0374
        pop    esi
        add    eax, dword ptr [ecx + 4]
        add    esp, 4
        ret    
loc_1_17A1:
        sub    eax, eax
        pop    esi
        add    esp, 4
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_17B0:
        mov    eax, dword ptr [esp + 4]
        mov    ax, word ptr [eax + 0x33]
        cmp    ax, 0x6853
        je     loc_1_17C7
        cmp    ax, 0x4853
        je     loc_1_17C7
        sub    eax, eax
        ret    
loc_1_17C7:
        mov    eax, 1
        ret    
        db  08Dh, 049h, 000h
sub_1_17D0:
        mov    eax, dword ptr [esp + 4]
        push   0
        push   eax
        call   sub_1_0F50
        add    esp, 8
        or     eax, eax
        je     loc_1_17FB
        mov    eax, dword ptr [eax + 0xc]
        or     eax, eax
        je     loc_1_17FB
        mov    ecx, dword ptr [esp + 8]
        mov    edi, edi
loc_1_17F0:
        cmp    dword ptr [eax + 8], ecx
        je     loc_1_17FD
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_17F0
loc_1_17FB:
        sub    eax, eax
loc_1_17FD:
        ret    
        db  08Bh, 0FFh
        dd  obj1:0x1800              ; fixup
        dd  obj1:0x1800              ; fixup
        db  000h, 000h, 000h, 000h
loc_1_180C:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_1810:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1814:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1818:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1820:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1824:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1828:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x40e  R-X
        db  053h, 056h, 057h, 055h, 0A1h
        dd  loc_1_0374               ; fixup
        db  08Bh, 070h, 004h, 0A1h
        dd  loc_1_03BD               ; fixup
        db  08Bh, 044h, 006h, 004h, 050h, 0E8h
        dd  sub_1_1580               ; fixup
        db  003h, 0C6h, 08Bh, 035h
        dd  loc_1_0374               ; fixup
        db  033h, 0C9h, 066h, 08Bh, 048h, 004h, 001h, 00Dh
        dd  loc_1_1820               ; fixup
        db  08Bh, 000h, 050h, 0E8h
        dd  sub_1_1580               ; fixup
        db  08Bh, 035h
        dd  loc_1_0374               ; fixup
        db  003h, 046h, 004h, 066h, 081h, 038h, 0FFh, 0FFh, 075h, 0DCh, 08Bh, 076h, 004h, 0A1h
        dd  loc_1_03BD               ; fixup
        db  08Bh, 044h, 006h, 01Ah, 050h, 0E8h
        dd  sub_1_1580               ; fixup
        db  033h, 0C9h, 066h, 08Bh, 04Ch, 006h, 004h, 0A1h
        dd  loc_1_1820               ; fixup
        db  003h, 0C1h, 0A3h
        dd  loc_1_1820               ; fixup
        db  0C1h, 0E0h, 002h, 050h, 0E8h
        dd  sub_1_00A4               ; fixup
        db  083h, 0C4h, 004h, 0A3h
        dd  loc_1_1824               ; fixup
        db  00Bh, 0C0h, 00Fh, 084h, 0BCh, 000h, 000h, 000h, 08Bh, 00Dh
        dd  loc_1_1820               ; fixup
        db  0C1h, 0E1h, 002h, 051h, 050h, 0E8h
        dd  sub_1_0153               ; fixup
        db  0A1h
        dd  loc_1_0374               ; fixup
        db  08Bh, 070h, 004h, 0A1h
        dd  loc_1_03BD               ; fixup
        db  08Bh, 044h, 006h, 004h, 050h, 0E8h
        dd  sub_1_1580               ; fixup
        db  08Dh, 014h, 006h, 08Bh, 035h
        dd  loc_1_1824               ; fixup
        db  0A1h
        dd  loc_1_0374               ; fixup
        db  066h, 08Bh, 04Ah, 004h, 08Bh, 078h, 004h, 08Dh, 05Ah, 006h, 02Bh, 0DFh, 066h, 08Bh, 0C1h, 066h
        db  049h, 066h, 00Bh, 0C0h, 074h, 016h, 0A1h
        dd  loc_1_03D4               ; fixup
        db  089h, 01Eh, 083h, 0C6h, 004h, 003h, 0D8h, 066h, 08Bh, 0E9h, 066h, 049h, 066h, 00Bh, 0EDh, 075h
        db  0EFh, 08Bh, 002h, 050h, 0E8h
        dd  sub_1_1580               ; fixup
        db  08Dh, 014h, 007h, 0A1h
        dd  loc_1_0374               ; fixup
        db  066h, 081h, 03Ah, 0FFh, 0FFh, 075h, 0BDh, 08Bh, 078h, 004h, 0A1h
        dd  loc_1_03BD               ; fixup
        db  08Bh, 044h, 007h, 01Ah, 050h, 0E8h
        dd  sub_1_1580               ; fixup
        db  003h, 0F8h, 0A1h
        dd  loc_1_0374               ; fixup
        db  066h, 08Bh, 057h, 004h, 083h, 0C7h, 006h, 02Bh, 078h, 004h, 066h, 08Bh, 0C2h, 066h, 04Ah, 066h
        db  00Bh, 0C0h, 074h, 017h, 0A1h
        dd  loc_1_03D4               ; fixup
        db  090h, 089h, 03Eh, 083h, 0C6h, 004h, 003h, 0F8h, 066h, 08Bh, 0CAh, 066h, 04Ah, 066h, 00Bh, 0C9h
        db  075h, 0EFh, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h
        db  000h
sub_2_0150:
        push   esi
        push   edi
        test   edx, 0x10000
        jne    loc_2_0184
        dec    byte ptr [0x384]              ; -> loc_1_0384
        mov    dword ptr [0x3d4], 0x35       ; -> loc_1_03D4
        mov    word ptr [0x3d8], 0x6d1       ; -> loc_1_03D8
        mov    edi, 0x385                    ; -> obj1:0x0385
        mov    esi, 0x32c                    ; -> loc_2_032C
        mov    ecx, 0x13
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_2_0184:
        movzx  edx, dx
        shl    edx, 4
        mov    dword ptr [0x380], edx        ; -> loc_1_0380
        mov    ecx, 0x13
        mov    edi, 0x385                    ; -> obj1:0x0385
loc_2_019A:
        add    dword ptr [edi], edx
        add    edi, 4
        loop   loc_2_019A
        pop    edi
        pop    esi
        ret    
loc_2_01A4:
        push   edi
        push   esi
        mov    dword ptr [0x374], ebx        ; -> loc_1_0374
        mov    dword ptr [0x378], ebx        ; -> loc_1_0378
        mov    dword ptr [0x37c], ebp        ; -> loc_1_037C
        call   sub_2_0150
        mov    eax, dword ptr [0x3a1]        ; -> obj1:0x03A1
        mov    esi, eax
        mov    edi, 0x430                    ; -> obj1:0x0430
        mov    ecx, 0xf
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [0x399]        ; -> obj1:0x0399
        mov    al, byte ptr [eax]
        mov    byte ptr [0x470], al          ; -> obj1:0x0470
        mov    eax, dword ptr [0x3bd]        ; -> loc_1_03BD
        mov    eax, dword ptr [eax + 0x1a]
        mov    esi, eax
        push   eax
        call   sub_1_1580
        cmp    word ptr [eax + 4], 4
        jne    loc_2_0253
        mov    eax, dword ptr [0x3a5]        ; -> loc_1_03A5
        mov    ax, word ptr [eax]
        or     ax, ax
        jne    loc_2_0253
        push   0x204
        mov    eax, 0x3c0
        cmp    byte ptr [0x384], 1           ; -> loc_1_0384
        je     loc_2_0219
        mov    eax, 0x360
loc_2_0219:
        push   eax
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        or     eax, eax
        je     loc_2_0253
        mov    dword ptr [eax], 0xffffffff
        mov    word ptr [eax + 4], 0x10
        mov    dword ptr [0x46c], esi        ; -> obj1:0x046C
        push   eax
        call   sub_1_15A0
        mov    edx, eax
        mov    eax, dword ptr [0x3bd]        ; -> loc_1_03BD
        mov    dword ptr [eax + 0x1a], edx
        mov    eax, dword ptr [0x3a5]        ; -> loc_1_03A5
        mov    word ptr [eax], 8
loc_2_0253:
        mov    esi, 0x3b4                    ; -> obj2:0x03B4
        mov    edi, 0x3bb                    ; -> loc_2_03BB
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        jb     loc_2_026C
        inc    al
        mov    byte ptr [0x180c], al         ; -> loc_1_180C
loc_2_026C:
        mov    esi, 0x3b4                    ; -> obj2:0x03B4
        mov    edi, 0x3d3                    ; -> obj2:0x03D3
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        jb     loc_2_0283
        mov    byte ptr [0x1814], al         ; -> loc_1_1814
loc_2_0283:
        mov    esi, 0x3b4                    ; -> obj2:0x03B4
        mov    edi, 0x3ec                    ; -> obj2:0x03EC
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        jb     loc_2_029A
        mov    byte ptr [0x1810], al         ; -> loc_1_1810
loc_2_029A:
        call   0
        mov    edi, 0x378                    ; -> loc_2_0378
        mov    ecx, 0xf
        mov    esi, 0                        ; -> loc_1_0000
loc_2_02AE:
        mov    edx, dword ptr [edi]
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        stosd  dword ptr es:[edi], eax
        loop   loc_2_02AE
        mov    eax, dword ptr [0x3a1]        ; -> obj1:0x03A1
        mov    edi, eax
        mov    esi, 0x378                    ; -> loc_2_0378
        mov    ecx, 0xf
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, 0xe
        push   0
        push   ecx
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        or     eax, eax
        je     loc_2_030B
        mov    edi, eax
        mov    esi, eax
        and    esi, 0xf
        shr    eax, 4
        shl    eax, 0x10
        or     esi, eax
        mov    eax, dword ptr [0x3a1]        ; -> obj1:0x03A1
        xchg   dword ptr [eax + 0x20], esi
        mov    ecx, 0xe
        mov    dword ptr [0x40a], esi        ; -> obj2:0x040A
        mov    esi, 0x400                    ; -> obj2:0x0400
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_2_030B:
        mov    eax, dword ptr [0x399]        ; -> obj1:0x0399
        mov    byte ptr [eax], 0xff
        clc    
        pop    esi
        pop    edi
        ret    
loc_2_0317:
        push   esi
        mov    eax, 0x2f
        mov    esi, 0x29e                    ; -> loc_1_029E
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        clc    
        pop    esi
        ret    
        db  000h
loc_2_032C:
        and    al, 0
        add    byte ptr [eax], al
        pushal 
        add    eax, 0x2de0000
        add    byte ptr [eax], al
        rol    dword ptr [edx], 1
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    cl, 2
        add    byte ptr [eax], al
        inc    esp
        add    eax, 0x5c0000
        add    byte ptr [eax], al
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax + 3], ah
        add    byte ptr [eax], al
        inc    esi
        add    eax, 0x2e80000
        add    byte ptr [eax], al
        sub    al, byte ptr [eax]
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [esi], ah
        add    byte ptr [eax], al
        add    byte ptr [esi], dh
        add    eax, 0x2ea0000
        add    byte ptr [eax], al
        out    2, al
        add    byte ptr [eax], al
        dec    edx
        db  005h, 000h, 000h
loc_2_0378:
        mov    al, 4
        add    byte ptr [eax], al
        rol    byte ptr [eax + eax], 0
        xor    byte ptr [esi], al
        add    byte ptr [eax], al
        push   eax
        push   es
        add    byte ptr [eax], al
        add    byte ptr [esi], 0
        add    byte ptr [eax + 0x50000006], dh ; -> loc_1_06B0
        pop    es
        add    byte ptr [eax], al
        rol    byte ptr [edi], 0
        add    al, dl                        ; -> loc_1_08D0
        or     byte ptr [eax], al
        add    byte ptr [eax + 9], ah        ; -> loc_1_0960
        add    byte ptr [eax], al
        inc    eax
        or     al, byte ptr [eax]
        add    byte ptr [eax - 0x1ffffff6], al ; -> loc_1_0A80
        or     al, byte ptr [eax]
        add    al, ah                        ; -> loc_1_0BE0
        or     eax, dword ptr [eax]
        add    byte ptr [eax + 0x3300000c], ah ; -> loc_1_0CA0
        cmp    byte ptr [esi], dh
        inc    ebp
        dec    esi
        dec    eax
        db  000h
loc_2_03BB:
        dec    ecx
        inc    edi
        dec    esi
        dec    edi
        push   edx
        inc    ebp
        push   ebx
        dec    eax
        inc    ecx
        push   edx
        dec    ecx
        dec    esi
        inc    edi
        push   esi
        dec    ecx
        dec    edi
        dec    esp
        inc    ecx
        push   esp
        dec    ecx
        dec    edi
        dec    esi
        push   ebx
        add    byte ptr [edx + edx*2 + 0x41], dl
        inc    esp
        dec    ecx
        push   esp
        dec    ecx
        dec    edi
        dec    esi
        inc    ecx
        dec    esp
        inc    esp
        inc    ebp
        push   esi
        dec    ecx
        inc    ebx
        inc    ebp
        push   ebx
        dec    eax
        inc    ecx
        push   edx
        dec    ecx
        dec    esi
        inc    edi
        add    byte ptr [ebp + 0x4e], al
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        push   ebx
        dec    eax
        inc    ecx
        push   edx
        dec    ecx
        dec    esi
        inc    edi
        push   eax
        dec    edi
        push   eax
        push   ebp
        push   eax
        push   ebx
        add    byte ptr [esi], ah
        cmp    byte ptr [ebp + 0x34], 0x48
        je     0x409
        clc    
        retf   
        db  0EAh, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x2a  R-X
        db  0B9h, 001h, 080h, 0F7h, 0C3h, 001h, 000h, 075h, 01Ah, 0B9h, 001h, 000h, 0B4h, 052h, 0CDh, 021h
        db  026h, 00Fh, 0B6h, 016h, 004h, 000h, 083h, 0FAh, 001h, 077h, 008h, 066h, 0C1h, 0E2h, 010h, 08Ch
        db  0C2h, 033h, 0C9h, 033h, 0DBh, 033h, 0F6h, 08Bh, 0C1h, 0C3h
VxD__LTEXT ENDS
