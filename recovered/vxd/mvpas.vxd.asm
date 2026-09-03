; mvpas -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 1026 instructions recovered by recursive descent

Declare_Virtual_Device mvpas, 4, 0, mvpas_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x5c  R-X
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'mvpas   '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  mvpas_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
mvpas_Control:
        db  083h
        db  0F8h, 022h, 00Fh, 084h
        dd  loc_2_001D               ; fixup
        db  0F8h, 0C3h, 0CCh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x76  R-X
loc_2_0000:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_3_0184
loc_2_0017:
        add    esp, 0x14
        leave  
        clc    
        ret    
loc_2_001D:
        push   3
        push   edx
        push   0                             ; -> loc_2_0000
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e003300
        add    esp, 0x10
        mov    eax, 0
        stc    
        ret    
        db  0CCh, 0CCh
sub_2_0038:
        VMMCall  VMM._RegDeleteKey               ; cd204b810100
        int3   
        int3   
sub_2_0040:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        int3   
        int3   
sub_2_0048:
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f803300
        int3   
        int3   
sub_2_0050:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        int3   
        int3   
sub_2_0058:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        int3   
        int3   
sub_2_0060:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d803300
        int3   
        int3   
sub_2_0068:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        int3   
        int3   
sub_2_0070:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0xbc4  R-X
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10c
        push   ebx
        lea    eax, [ebp - 0x10c]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   edi
        push   0
        push   0x100
        push   eax
        push   0
        push   ebx
        call   sub_2_0060
loc_3_0025:
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_00EF
        mov    edi, 0                        ; -> obj4:0x0068
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0xffffffff
        lea    edi, [ebp - 0x10c]
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x10c]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_2_0058
loc_3_0079:
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_00ED
        mov    esi, 0                        ; -> obj4:0x0000
        mov    edi, 2
loc_3_008A:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 2]
        push   eax
        mov    dword ptr [ebp - 8], edi
        push   ecx
        push   0
        push   0
        push   dword ptr [esi]
        push   dword ptr [ebp - 0xc]
        call   sub_2_0050
loc_3_00A3:
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_00C3
        push   1
        lea    eax, [ebp - 2]
        push   edi
        push   eax
        push   3
        push   dword ptr [esi]
        push   0                             ; -> obj4:0x0060
        push   ebx
        call   sub_2_0048
loc_3_00C0:
        add    esp, 0x1c
loc_3_00C3:
        add    esi, 4
        cmp    esi, 0x10                     ; -> obj4:0x0010
        jb     loc_3_008A
        push   dword ptr [ebp - 0xc]
        call   sub_2_0040
loc_3_00D6:
        add    esp, 4
        lea    eax, [ebp - 0x10c]
        push   eax
        push   0x80000002
        call   sub_2_0038
loc_3_00EA:
        add    esp, 8
loc_3_00ED:
        xor    eax, eax
loc_3_00EF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_00F6:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ebx, 0                        ; -> obj4:0x0000
        push   ebp
        xor    edi, edi
        mov    ebp, 0                        ; -> obj4:0x0010
        mov    esi, dword ptr [esp + 0x1c]
loc_3_010D:
        lea    eax, [esp + 0x14]
        push   1
        lea    ecx, [esp + 0x16]
        push   eax
        mov    dword ptr [esp + 0x1c], 2
        push   ecx
        push   3
        push   dword ptr [ebx]
        push   0                             ; -> obj4:0x0060
        push   esi
        call   sub_2_0068
loc_3_0130:
        add    esp, 0x1c
        test   eax, eax
        je     loc_3_0140
        mov    ax, word ptr [ebp]
        mov    word ptr [esp + 0x12], ax
loc_3_0140:
        cmp    word ptr [esp + 0x12], 0
        je     loc_3_0158
        mov    edx, 1
        lea    ecx, [edi + 4]
        mov    eax, dword ptr [esp + 0x20]
        shl    edx, cl
        or     dword ptr [eax], edx
loc_3_0158:
        cmp    ebx, 0xc                      ; -> obj4:0x000C
        jne    loc_3_016B
        mov    eax, dword ptr [esp + 0x24]
        movzx  ecx, word ptr [esp + 0x12]
        mov    dword ptr [eax], ecx
loc_3_016B:
        add    ebp, 2
        add    ebx, 4
        inc    edi
        cmp    ebx, 0x10                     ; -> obj4:0x0010
        jb     loc_3_010D
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_3_0184:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x100
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     loc_3_01C2
        cmp    eax, 1
        je     loc_3_01C9
        cmp    eax, 2
        je     loc_3_0400
        cmp    eax, 3
        je     loc_3_040B
        cmp    eax, 4
        je     loc_3_0423
        mov    eax, 1
        jmp    loc_3_042E
loc_3_01C2:
        xor    eax, eax
        jmp    loc_3_042E
loc_3_01C9:
        cmp    dword ptr [ebp + 0xc], 1
        jbe    loc_3_01D9
        mov    eax, 1
        jmp    loc_3_042E
loc_3_01D9:
        push   0
        mov    edi, dword ptr [ebp + 0x10]
        push   edi
        lea    eax, [ebp - 0x100]
        push   eax
        call   sub_2_0070
loc_3_01EB:
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_042E
        mov    dword ptr [ebp - 0xc], 0
        mov    esi, 0xffff
        cmp    word ptr [ebp - 0xa4], 3
        mov    bx, si
        mov    word ptr [ebp - 6], si
        mov    word ptr [ebp - 2], si
        jae    loc_3_0221
        mov    eax, 0x13
        jmp    loc_3_042E
loc_3_0221:
        mov    ax, word ptr [ebp - 0xa0]
        cmp    word ptr [ebp - 0x52], 1
        mov    cx, word ptr [ebp - 0x9e]
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 4], cx
        jb     loc_3_03F9
        cmp    word ptr [ebp - 0x42], 1
        jb     loc_3_03F9
        push   edi
        call   0
        add    esp, 4
        test   eax, eax
        jne    loc_3_042E
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0xc]
        push   eax
        push   ecx
        push   edi
        call   sub_3_00F6
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_042E
        lea    eax, [ebp - 0x28]
        push   eax
        push   dword ptr [ebp - 4]
        call   sub_3_069E
        add    esp, 8
        test   eax, eax
        jne    loc_3_0294
        mov    eax, 0x24
        jmp    loc_3_042E
loc_3_0294:
        test   byte ptr [ebp - 0x24], 0x40
        je     loc_3_0364
        movzx  eax, word ptr [ebp - 0xa4]
        cmp    eax, 9
        jl     loc_3_02B3
        cmp    eax, 0xa
        jle    loc_3_033C
loc_3_02B3:
        or     byte ptr [ebp - 0xc], 1
        movzx  eax, word ptr [ebp - 0x52]
        cmp    eax, 1
        je     loc_3_0319
        cmp    eax, 2
        je     loc_3_02F3
        cmp    eax, 3
        je     loc_3_0320
loc_3_02CA:
        mov    cx, word ptr [ebp - 2]
loc_3_02CE:
        movzx  eax, word ptr [ebp - 0x42]
        cmp    eax, 1
        je     loc_3_02E9
        cmp    eax, 2
        jne    loc_3_03B5
        movzx  ax, byte ptr [ebp - 0x3f]
        mov    word ptr [ebp - 6], ax
loc_3_02E9:
        movzx  ax, byte ptr [ebp - 0x40]
        jmp    loc_3_03B9
loc_3_02F3:
        cmp    word ptr [ebp - 0xa4], 0xd
        jne    loc_3_030D
        movzx  cx, byte ptr [ebp - 0x50]
        movzx  ax, byte ptr [ebp - 0x4f]
        mov    word ptr [ebp - 2], ax
        jmp    loc_3_02CE
loc_3_030D:
        movzx  bx, byte ptr [ebp - 0x4f]
        mov    si, word ptr [ebp - 0x88]
loc_3_0319:
        movzx  cx, byte ptr [ebp - 0x50]
        jmp    loc_3_02CE
loc_3_0320:
        movzx  bx, byte ptr [ebp - 0x4e]
        movzx  ax, byte ptr [ebp - 0x4f]
        mov    si, word ptr [ebp - 0x88]
        movzx  cx, byte ptr [ebp - 0x50]
        mov    word ptr [ebp - 2], ax
        jmp    loc_3_02CE
loc_3_033C:
        movzx  eax, word ptr [ebp - 0x52]
        cmp    eax, 1
        je     loc_3_035A
        cmp    eax, 2
        jne    loc_3_02CA
        movzx  bx, byte ptr [ebp - 0x4f]
        mov    si, word ptr [ebp - 0x90]
loc_3_035A:
        movzx  cx, byte ptr [ebp - 0x50]
        jmp    loc_3_02CE
loc_3_0364:
        movzx  eax, word ptr [ebp - 0xa4]
        mov    si, word ptr [ebp - 0x90]
        cmp    eax, 0xa
        je     loc_3_03AB
        or     byte ptr [ebp - 0xc], 1
        movzx  eax, word ptr [ebp - 0x52]
        cmp    eax, 1
        je     loc_3_038E
        cmp    eax, 2
        je     loc_3_0398
        jmp    loc_3_02CA
loc_3_038E:
        movzx  cx, byte ptr [ebp - 0x50]
        jmp    loc_3_02CE
loc_3_0398:
        movzx  cx, byte ptr [ebp - 0x50]
        movzx  ax, byte ptr [ebp - 0x4f]
        mov    word ptr [ebp - 2], ax
        jmp    loc_3_02CE
loc_3_03AB:
        movzx  cx, byte ptr [ebp - 0x50]
        jmp    loc_3_02CE
loc_3_03B5:
        mov    ax, word ptr [ebp - 2]
loc_3_03B9:
        cmp    word ptr [ebp - 8], 0xffff
        je     loc_3_03F2
        cmp    word ptr [ebp - 4], 0xffff
        je     loc_3_03F2
        lea    edx, [ebp - 0x28]
        push   edx
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 2]
        push   0
        push   0xffff
        push   dword ptr [ebp - 6]
        push   ebx
        push   esi
        push   eax
        push   ecx
        push   dword ptr [ebp - 4]
        push   edi
        call   sub_3_0790
        add    esp, 0x34
        jmp    loc_3_042E
loc_3_03F2:
        mov    eax, 0x13
        jmp    loc_3_042E
loc_3_03F9:
        mov    eax, 0x13
        jmp    loc_3_042E
loc_3_0400:
        cmp    dword ptr [ebp + 0xc], 0
        mov    eax, 1
        jmp    loc_3_042E
loc_3_040B:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_3_041C
        cmp    eax, 1
        mov    eax, 1
        jmp    loc_3_042E
loc_3_041C:
        mov    eax, 1
        jmp    loc_3_042E
loc_3_0423:
        push   dword ptr [ebp + 0x10]
        call   sub_3_0BC1
        add    esp, 4
loc_3_042E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_0435:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ax, word ptr [eax + 0x14]
        mov    ebp, esp
        sub    esp, 4
        mov    word ptr [ebp - 4], ax
        mov    dx, word ptr [ebp - 4]
        xor    dx, word ptr [ebp + 0xc]
        in     al, dx
        mov    byte ptr [ebp - 1], al
        mov    al, byte ptr [ebp - 1]
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_045A:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ax, word ptr [eax + 0x14]
        mov    ebp, esp
        sub    esp, 4
        mov    word ptr [ebp - 2], ax
        mov    dx, word ptr [ebp - 2]
        xor    dx, word ptr [ebp + 0xc]
        mov    al, byte ptr [ebp + 0x10]
        out    dx, al
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_047C:
        push   ebx
        xor    eax, eax
        push   esi
        mov    ecx, 6
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        mov    edi, esi
        rep stosd dword ptr es:[edi], eax
        mov    di, word ptr [esp + 0x10]
        push   0xb8b
        mov    ax, di
        push   esi
        xor    ax, 0x388
        mov    word ptr [esi + 0x14], ax
        call   sub_3_0435
        add    esp, 8
        mov    bl, al
        cmp    bl, 0xff
        je     loc_3_0513
        mov    al, bl
        shr    al, 5
        movzx  ax, al
        mov    word ptr [esi], ax
        movzx  eax, ax
        test   eax, eax
        jl     loc_3_0513
        cmp    eax, 1
        jle    loc_3_04DA
        cmp    eax, 3
        jl     loc_3_0513
        cmp    eax, 5
        jle    loc_3_04DA
        cmp    eax, 7
        jne    loc_3_0513
loc_3_04DA:
        mov    al, bl
        xor    al, 0xe0
        push   eax
        push   0xb8b
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   0xb8b
        push   esi
        call   sub_3_0435
        add    esp, 8
        mov    cl, bl
        xor    cl, al
        test   cl, 0xe0
        je     loc_3_0525
        push   ebx
        push   0xb8b
        push   esi
        call   sub_3_045A
        add    esp, 0xc
loc_3_0513:
        mov    word ptr [esi], 0
        xor    eax, eax
        mov    dword ptr [esi + 4], 0
loc_3_0521:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_0525:
        cmp    word ptr [esi], 0
        jne    loc_3_055E
        or     byte ptr [esi + 4], 3
        push   0
        push   0x7f89
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   0x7f89
        push   esi
        call   sub_3_0435
        add    esp, 8
        test   al, 1
        jne    loc_3_061B
        or     byte ptr [esi + 4], 4
        jmp    loc_3_061B
loc_3_055E:
        push   0x8388
        push   esi
        call   sub_3_0435
        add    esp, 8
        test   al, 0x80
        jne    loc_3_0513
        push   0xef8b
        push   esi
        call   sub_3_0435
        add    esp, 8
        test   al, 4
        je     loc_3_0586
        or     byte ptr [esi + 4], 0x10
loc_3_0586:
        test   al, 8
        je     loc_3_059F
        mov    ecx, dword ptr [esi + 4]
        or     cl, 8
        mov    dword ptr [esi + 4], ecx
        or     cl, 0x40
        mov    dword ptr [esi + 4], ecx
        or     cl, 0x20
        mov    dword ptr [esi + 4], ecx
loc_3_059F:
        movzx  eax, al
        mov    ecx, dword ptr [esi + 4]
        xor    eax, ecx
        and    eax, 3
        xor    eax, ecx
        mov    ecx, eax
        mov    dword ptr [esi + 4], eax
        and    cl, 3
        cmp    cl, 3
        jne    loc_3_05C4
        mov    word ptr [esi], 1
        or     ah, 0x10
        mov    dword ptr [esi + 4], eax
loc_3_05C4:
        mov    eax, dword ptr [esi + 4]
        push   0xff8b
        or     ah, 1
        push   esi
        mov    dword ptr [esi + 4], eax
        or     ah, 8
        mov    dword ptr [esi + 4], eax
        call   sub_3_0435
        add    esp, 8
        test   al, 1
        jne    loc_3_05E9
        or     byte ptr [esi + 5], 2
loc_3_05E9:
        test   al, 4
        je     loc_3_05FB
        mov    eax, dword ptr [esi + 4]
        or     ah, 4
        mov    dword ptr [esi + 4], eax
        and    al, 0xdf
        mov    dword ptr [esi + 4], eax
loc_3_05FB:
        push   0xff88
        push   esi
        call   sub_3_0435
        movzx  ax, al
        mov    word ptr [esi + 2], ax
        add    esp, 8
        cmp    ax, 4
        jb     loc_3_061B
        or     byte ptr [esi + 4], 0x80
loc_3_061B:
        mov    word ptr [esi + 8], di
        mov    eax, 1
        jmp    loc_3_0521
sub_3_0629:
        mov    ecx, dword ptr [esp + 4]
        test   ecx, ecx
        jle    loc_3_063A
loc_3_0631:
        mov    edx, 0x9a01
        dec    ecx
        in     al, dx
        jne    loc_3_0631
loc_3_063A:
        ret    
sub_3_063B:
        push   ebx
        mov    edx, 0x8388
        push   esi
        mov    al, 0xc0
        out    dx, al
        push   edi
        push   ebp
        push   0xa
        xor    edi, edi
        call   sub_3_0629
        mov    bx, word ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x1c]
        shr    bx, 2
        add    esp, 4
loc_3_0660:
        mov    cl, byte ptr [edi]            ; -> obj4:0x0070
        mov    ebp, 0x14
loc_3_066B:
        mov    edx, 0x9a01
        mov    al, cl
        out    dx, al
        mov    al, bl
        dec    ebp
        out    dx, al
        jne    loc_3_066B
        push   esi
        push   dword ptr [esp + 0x18]
        call   sub_3_047C
        add    esp, 8
        test   eax, eax
        jne    loc_3_0697
        inc    edi
        cmp    edi, 4
        jb     loc_3_0660
        xor    eax, eax
loc_3_0692:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_0697:
        mov    eax, 1
        jmp    loc_3_0692
sub_3_069E:
        push   esi
        push   edi
        mov    si, word ptr [esp + 0xc]
        mov    edi, dword ptr [esp + 0x10]
        push   edi
        push   esi
        call   sub_3_047C
        add    esp, 8
        test   eax, eax
        jne    loc_3_06CA
        push   edi
        push   esi
        call   sub_3_063B
        add    esp, 8
        test   eax, eax
        mov    eax, 0
        je     loc_3_06CF
loc_3_06CA:
        mov    eax, 1
loc_3_06CF:
        pop    edi
        pop    esi
        ret    
sub_3_06D2:
        mov    eax, dword ptr [esp + 4]
        movzx  eax, word ptr [eax]
        cmp    eax, 3
        jl     loc_3_06E8
        cmp    eax, 5
        jle    loc_3_06EC
        cmp    eax, 7
        je     loc_3_06EC
loc_3_06E8:
        xor    eax, eax
        jmp    loc_3_06F1
loc_3_06EC:
        mov    eax, 1
loc_3_06F1:
        ret    
sub_3_06F2:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   0
        push   0xb8a
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   0xb8b
        push   esi
        call   sub_3_0435
        add    esp, 8
        and    al, 0xf3
        push   eax
        push   0xb8b
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   esi
        call   sub_3_06D2
        add    esp, 4
        test   eax, eax
        je     loc_3_075A
        push   0xb88
        push   esi
        call   sub_3_0435
        add    esp, 8
        mov    bl, al
        or     al, 0x20
        push   eax
        push   0xb88
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        jmp    loc_3_075E
loc_3_075A:
        mov    bl, byte ptr [esp + 0xb]
loc_3_075E:
        push   0x19
        push   0xf8a
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   esi
        call   sub_3_06D2
        add    esp, 4
        test   eax, eax
        je     loc_3_078A
        push   ebx
        push   0xb88
        push   esi
        call   sub_3_045A
        add    esp, 0xc
loc_3_078A:
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_3_0790:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cli    
        mov    esi, dword ptr [ebp + 0x38]
        push   esi
        push   dword ptr [ebp + 0xc]
        call   sub_3_063B
        add    esp, 8
        test   eax, eax
        jne    loc_3_07B7
        mov    eax, 0x24
        jmp    loc_3_0BBA
loc_3_07B7:
        test   byte ptr [esi + 5], 4
        jne    loc_3_07EB
        push   0x8388
        push   esi
        call   sub_3_0435
        add    esp, 8
        and    al, 0xfd
        mov    edi, dword ptr [ebp + 0x34]
        test   edi, 0x20
        je     loc_3_07DA
        or     al, 4
loc_3_07DA:
        push   eax
        push   0x8388
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        jmp    loc_3_07EE
loc_3_07EB:
        mov    edi, dword ptr [ebp + 0x34]
loc_3_07EE:
        push   0xa
        push   0x4388
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        xor    al, al
        test   edi, 0x80
        je     loc_3_080D
        mov    eax, dword ptr [ebp + 0x30]
        and    al, 0xf
loc_3_080D:
        push   eax
        push   0xbf88
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   0
        push   0xb8a
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   0x36
        push   0x138b
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   0xd
        push   0x1388
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   0
        push   0x1388
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   0x74
        push   0x138b
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   0
        push   0x1389
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   0
        push   0x1389
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        test   byte ptr [esi + 4], 0x20
        je     loc_3_08B2
        push   0
        push   0xb88
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   8
        push   0xb88
        push   esi
        call   sub_3_045A
        add    esp, 0xc
loc_3_08B2:
        mov    eax, dword ptr [esi + 4]
        test   al, 8
        je     loc_3_08D2
        test   ah, 4
        je     loc_3_08C2
        push   0
        jmp    loc_3_08C4
loc_3_08C2:
        push   0x18
loc_3_08C4:
        push   0x838a
        push   esi
        call   sub_3_045A
        add    esp, 0xc
loc_3_08D2:
        xor    al, al
        test   edi, 0x10
        je     loc_3_08DE
        mov    al, 0x40
loc_3_08DE:
        test   edi, 0x40
        je     loc_3_08E8
        or     al, 0x80
loc_3_08E8:
        push   eax
        push   0xf388
        push   esi
        call   sub_3_045A
        movzx  eax, word ptr [ebp + 0x14]
        add    esp, 0xc
        mov    al, byte ptr [eax]            ; -> obj4:0x0078
        push   eax
        push   0xf389
        push   esi
        call   sub_3_045A
        movzx  ecx, word ptr [ebp + 0x10]
        add    esp, 0xc
        mov    al, byte ptr [ecx]            ; -> obj4:0x0090
        push   eax
        push   0xf38a
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   8
        push   0xbf8a
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        mov    eax, dword ptr [esi + 4]
        test   ah, 4
        je     loc_3_0951
        test   al, 4
        jne    loc_3_0951
        test   edi, 1
        jne    loc_3_0951
        push   0x68
        jmp    loc_3_0953
loc_3_0951:
        push   0x49
loc_3_0953:
        push   0x838b
        push   esi
        call   sub_3_045A
        mov    di, word ptr [ebp + 0x2c]
        add    esp, 0xc
        cmp    di, 0xffff
        je     loc_3_09C2
        push   0xf38a
        push   esi
        call   sub_3_0435
        movzx  ecx, di
        add    esp, 8
        and    al, 0xf
        mov    cl, byte ptr [ecx]            ; -> obj4:0x0090
        shl    cl, 4
        or     al, cl
        push   eax
        push   0xf38a
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   0x838b
        push   esi
        call   sub_3_0435
        add    esp, 8
        or     al, 0x24
        push   eax
        push   0x838b
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   0xa
        call   sub_3_0629
        add    esp, 4
loc_3_09C2:
        push   esi
        call   sub_3_06F2
        add    esp, 4
        cmp    word ptr [esi], 1
        je     loc_3_09F0
        push   0xb89
        push   esi
        call   sub_3_0435
        add    esp, 8
        and    al, 0xfd
        push   eax
        push   0xb89
        push   esi
        call   sub_3_045A
        add    esp, 0xc
loc_3_09F0:
        mov    di, word ptr [ebp + 0x24]
        xor    ebx, ebx
        mov    word ptr [esi + 0x10], di
        mov    al, byte ptr [ebp + 0x28]
        push   ebx
        mov    byte ptr [ebp - 1], bl
        push   0xf788
        mov    byte ptr [ebp - 2], bl
        push   esi
        mov    byte ptr [esi + 0x12], al
        call   sub_3_045A
        add    esp, 0xc
        push   ebx
        push   0xf789
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   ebx
        push   0xfb8a
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        test   byte ptr [esi + 4], 0x80
        je     loc_3_0A8B
        cmp    di, 0xffff
        je     loc_3_0A4C
        mov    byte ptr [ebp - 2], 1
        mov    ax, di
        and    al, 0xf0
        mov    byte ptr [ebp - 1], al
loc_3_0A4C:
        push   dword ptr [ebp - 2]
        push   0xf788
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   dword ptr [ebp - 1]
        push   0xf789
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        cmp    di, 0xffff
        je     loc_3_0A7C
        mov    bx, word ptr [ebp + 0x28]
        and    bl, 3
loc_3_0A7C:
        push   ebx
        push   0xfb8a
        push   esi
        call   sub_3_045A
        add    esp, 0xc
loc_3_0A8B:
        mov    ax, word ptr [ebp + 0x18]
        cmp    ax, 0xffff
        je     loc_3_0B6E
        cmp    word ptr [esi + 2], 4
        mov    word ptr [esi + 0xc], ax
        jb     loc_3_0AE7
        or     byte ptr [ebp - 2], 2
        shr    ax, 4
        and    al, 0xf
        or     byte ptr [ebp - 1], al
        test   byte ptr [esi + 4], 0x40
        je     loc_3_0AE7
        mov    ax, word ptr [ebp + 0x1c]
        mov    cx, word ptr [ebp + 0x20]
        mov    byte ptr [esi + 0xf], al
        movzx  edx, ax
        mov    byte ptr [esi + 0xe], cl
        shl    cl, 3
        mov    al, byte ptr [edx]            ; -> obj4:0x0080
        or     al, cl
        shl    al, 3
        or     bl, al
        push   ebx
        push   0xfb8a
        push   esi
        call   sub_3_045A
        add    esp, 0xc
loc_3_0AE7:
        push   dword ptr [ebp - 2]
        push   0xf788
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   dword ptr [ebp - 1]
        push   0xf789
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   0xb88
        push   esi
        call   sub_3_0435
        add    esp, 8
        or     al, 0x10
        push   eax
        push   0xb88
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   0xb8a
        push   esi
        call   sub_3_0435
        add    esp, 8
        and    al, 0xf9
        or     al, 0x39
        push   eax
        push   0xb8a
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        test   byte ptr [esi + 4], 0x40
        je     loc_3_0B6E
        push   0xb89
        push   esi
        call   sub_3_0435
        add    esp, 8
        or     al, 2
        push   eax
        push   0xb89
        push   esi
        call   sub_3_045A
        add    esp, 0xc
loc_3_0B6E:
        push   0xb8a
        push   esi
        call   sub_3_0435
        add    esp, 8
        and    al, 0xf1
        or     al, 0x21
        push   eax
        push   0xb8a
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   0
        push   0xb8b
        push   esi
        call   sub_3_045A
        add    esp, 0xc
        push   1
        call   sub_3_0629
        add    esp, 4
        push   0xb8b
        push   esi
        call   sub_3_0435
        add    esp, 8
        sti    
        xor    eax, eax
loc_3_0BBA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_0BC1:
        xor    eax, eax
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0xa0  RW-
        dd  obj4:0x0048              ; fixup
        dd  obj4:0x0038              ; fixup
        dd  obj4:0x0024              ; fixup
        dd  obj4:0x0018              ; fixup
        db  001h, 000h, 000h, 000h, 001h, 000h, 001h, 000h, 057h, 061h, 069h, 074h, 020h, 073h, 074h, 061h
        db  074h, 065h, 073h, 000h, 045h, 06Eh, 061h, 062h, 06Ch, 065h, 020h, 077h, 061h, 072h, 06Dh, 020h
        db  062h, 06Fh, 06Fh, 074h, 000h, 000h, 000h, 000h, 055h, 073h, 065h, 020h, 032h, 038h, 04Dh, 048h
        db  07Ah, 020h, 063h, 06Ch, 06Fh, 063h, 06Bh, 000h, 045h, 06Eh, 061h, 062h, 06Ch, 065h, 020h, 06Ah
        db  06Fh, 079h, 073h, 074h, 069h, 063h, 06Bh, 020h, 064h, 065h, 063h, 06Fh, 064h, 065h, 000h, 000h
        db  043h, 06Fh, 06Eh, 066h, 069h, 067h, 000h, 000h, 05Ch, 043h, 06Fh, 06Eh, 066h, 069h, 067h, 000h
        db  0BCh, 0BDh, 0BEh, 0BFh, 000h, 000h, 000h, 000h, 004h, 001h, 002h, 003h, 000h, 005h, 006h, 007h
        db  000h, 000h, 001h, 002h, 000h, 003h, 000h, 004h, 000h, 001h, 005h, 006h, 007h, 000h, 000h, 000h
        db  000h, 000h, 001h, 002h, 003h, 004h, 005h, 006h, 000h, 001h, 007h, 008h, 009h, 000h, 00Ah, 00Bh
VxD__LDATA ENDS
