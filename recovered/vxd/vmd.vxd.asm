; VMD -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 921 instructions recovered by recursive descent

Declare_Virtual_Device VMD, 3, 0, VMD_Control, 0000Ch, Undefined_Init_Order
;   DDK version 0400h   init_order 034000000h   flags 00000h
;   service table: obj1:0xaa4, 3 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0xb2c  R-X
loc_1_0000:
        cmp    byte ptr [0xa98], 0xff        ; -> obj1:0x0A98
        jne    loc_1_004D
        mov    edx, dword ptr [0xa94]        ; -> obj1:0x0A94
        add    edx, ebx
        or     byte ptr [edx], 0x40
        and    byte ptr [edx], 0xee
        mov    word ptr [edx + 2], 0xffff
        mov    word ptr [edx + 4], 0xffff
        mov    word ptr [edx + 6], 0
        mov    word ptr [edx + 8], 0x77ff
        mov    word ptr [edx + 0xa], 0x7700
        lea    edi, [edx + 0x1c]
        mov    esi, 0xa50                    ; -> obj1:0x0A50
        mov    ecx, 0x10
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        xor    eax, eax
        VxDCall  VDD.VDD_Hide_Cursor             ; cd2003000a00
loc_1_004D:
        clc    
        ret    
loc_1_004F:
        cmp    dword ptr [0xa90], 0          ; -> obj1:0x0A90
        je     loc_1_00BC
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_1_00BC
        cmp    eax, 2
        ja     loc_1_00BC
        je     loc_1_00B7
        mov    ebx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [0xa94]        ; -> obj1:0x0A94
        add    edx, ebx
        mov    ecx, dword ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp]
        cmp    word ptr [edx + 0x10], cx
        je     loc_1_0085
        mov    byte ptr [0xaa0], 3           ; -> obj1:0x0AA0
loc_1_0085:
        mov    word ptr [edx + 0x10], cx
        rol    ecx, 0x10
        mov    word ptr [edx + 0x12], cx
        mov    ecx, dword ptr [edx + 0x14]
        mov    dword ptr [edx + 0x18], ecx
        mov    dword ptr [edx + 0x14], edi
        cmp    dword ptr [edx + 0xc], 0
        jne    loc_1_00B7
        mov    esi, 0xc1                     ; -> loc_1_00C1
        mov    eax, 0x400000
        mov    ecx, 9
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        mov    dword ptr [edx + 0xc], esi
loc_1_00B7:
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_1_00BC:
        or     byte ptr [ebp + 0x2c], 1
        ret    
loc_1_00C1:
        mov    dword ptr [edx + 0xc], 0
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    edi, dword ptr [edx + 0x14]
        xor    eax, eax
        btr    word ptr [edx], 0xc
        jb     loc_1_010D
        btr    word ptr [edx], 0xb
        jae    loc_1_0110
        mov    dword ptr [ebp + 0x1c], 3
        mov    eax, 0x33
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    ax, word ptr [ebp + 0x18]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x14]
loc_1_010D:
        mov    dword ptr [edx + 0x18], eax
loc_1_0110:
        test   byte ptr [edx + 1], 2
        jne    loc_1_011B
loc_1_0116:
        mov    dword ptr [edx + 0x18], edi
        jmp    loc_1_0184
loc_1_011B:
        test   byte ptr [edx + 1], 4
        jne    loc_1_0116
        test   byte ptr [edx + 1], 1
        je     loc_1_0116
        mov    ecx, dword ptr [edx + 0x18]
loc_1_012A:
        mov    ax, cx
        add    cx, 8
        sub    ax, 8
        jae    loc_1_013A
        xor    ax, ax
loc_1_013A:
        cmp    cx, di
        ja     loc_1_0145
        mov    word ptr [edx + 0x18], cx
        jmp    loc_1_0154
loc_1_0145:
        cmp    ax, di
        jbe    loc_1_0150
        mov    word ptr [edx + 0x18], ax
        jmp    loc_1_0154
loc_1_0150:
        mov    word ptr [edx + 0x18], di
loc_1_0154:
        shr    edi, 0x10
        shr    ecx, 0x10
        mov    ax, cx
        add    cx, 8
        sub    ax, 8
        jae    loc_1_016A
        xor    ax, ax
loc_1_016A:
        cmp    cx, di
        ja     loc_1_0175
        mov    word ptr [edx + 0x1a], cx
        jmp    loc_1_0184
loc_1_0175:
        cmp    ax, di
        jbe    loc_1_0180
        mov    word ptr [edx + 0x1a], ax
        jmp    loc_1_0184
loc_1_0180:
        mov    word ptr [edx + 0x1a], di
loc_1_0184:
        mov    edi, dword ptr [edx + 0x18]
        mov    cx, word ptr [edx + 0x10]
        xor    esi, esi
        cmp    edi, dword ptr [edx + 0x14]
        jne    loc_1_0196
        xchg   word ptr [edx + 0x12], si
loc_1_0196:
        mov    word ptr [ebp + 0x14], cx
        mov    word ptr [ebp + 4], si
        mov    word ptr [ebp + 0x1c], 1
        mov    word ptr [ebp + 0x18], di
        rol    edi, 0x10
        mov    word ptr [ebp + 0x10], di
        push   edx
        mov    ecx, dword ptr [0xa90]        ; -> obj1:0x0A90
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        pop    edx
        mov    edi, dword ptr [edx + 0x14]
        mov    ecx, dword ptr [edx + 0x18]
        cmp    ecx, edi
        jne    loc_1_012A
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_01F0:
        mov    edx, dword ptr [0xa94]        ; -> obj1:0x0A94
        add    edx, ebx
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ax, 0xb
        je     loc_1_0242
        cmp    ax, 3
        je     loc_1_0246
        or     ax, ax
        jne    loc_1_0214
        and    byte ptr [edx + 1], 0xf8
        or     byte ptr [edx + 1], 0x10
loc_1_0214:
        cmp    ax, 0x21
        jne    loc_1_0222
        and    byte ptr [edx + 1], 0xf8
        or     byte ptr [edx + 1], 0x10
loc_1_0222:
        cmp    ax, 0xc
        jne    loc_1_022C
        or     byte ptr [edx + 1], 2
loc_1_022C:
        cmp    ax, 0xa
        jne    loc_1_0236
        or     byte ptr [edx + 1], 4
loc_1_0236:
        cmp    ax, 9
        jne    loc_1_0272
        or     byte ptr [edx + 1], 4
        jmp    loc_1_0272
loc_1_0242:
        or     byte ptr [edx + 1], 1
loc_1_0246:
        cmp    ebx, dword ptr [0xb04]        ; -> obj1:0x0B04
        je     loc_1_0272
        cmp    byte ptr [0xa99], 0xff        ; -> obj1:0x0A99
        jne    loc_1_0272
        inc    byte ptr [0xaa0]              ; -> obj1:0x0AA0
        cmp    byte ptr [0xaa0], 3           ; -> obj1:0x0AA0
        jbe    loc_1_0272
        mov    byte ptr [0xaa0], ah          ; -> obj1:0x0AA0
        VxDCall  VDD.VDD_Check_Update_Soon       ; cd200d000a00
loc_1_0272:
        stc    
        ret    
loc_1_0274:
        cmp    dword ptr [0xa90], 0          ; -> obj1:0x0A90
        je     loc_1_02D9
        mov    edx, dword ptr [0xa94]        ; -> obj1:0x0A94
        add    edx, ebx
        or     byte ptr [edx + 1], 0x10
        cmp    ebx, dword ptr [0xb04]        ; -> obj1:0x0B04
        je     loc_1_02D9
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp + 0x1c], 2
        mov    ecx, dword ptr [0xa90]        ; -> obj1:0x0A90
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Build_Int_Stack_Frame       ; cd204c000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
loc_1_02D9:
        cmp    byte ptr [0xa9f], 0           ; -> obj1:0x0A9F
        je     loc_1_031E
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    dword ptr [ebp + 0x1c], 0
        mov    eax, 0x33
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
loc_1_031E:
        ret    
        db  090h
loc_1_0320:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 1
        jb     loc_1_0337
        je     loc_1_0356
        cmp    eax, 9
        je     loc_1_0362
        cmp    eax, 0xa
        je     loc_1_03AA
loc_1_0335:
        stc    
        ret    
loc_1_0337:
        call   0
        movzx  eax, byte ptr [0xa9e]         ; -> obj1:0x0A9E
        mov    word ptr [ebp + 0x1c], ax
        mov    esi, 0x47a                    ; -> loc_1_047A
        xor    eax, eax
        VMMCall  VMM.Call_When_VM_Returns        ; cd200d000100
        jmp    loc_1_0335
loc_1_0356:
        mov    edx, dword ptr [0xa94]        ; -> obj1:0x0A94
        or     byte ptr [edx + ebx], 0x10
        jmp    loc_1_0335
loc_1_0362:
        mov    edx, dword ptr [0xa94]        ; -> obj1:0x0A94
        add    edx, ebx
        test   word ptr [edx], 4
        jne    loc_1_0335
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [edx + 2], ax
        mov    ax, word ptr [ebp + 0x18]
        mov    word ptr [edx + 2], ax
        xor    eax, eax
        VxDCall  VDD.VDD_Hide_Cursor             ; cd2003000a00
        xchg   esi, eax
        mov    ax, 0x3814
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        lea    edi, [edx + 0x1c]
        mov    ecx, 0x10
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        cmp    dword ptr [0xb04], ebx        ; -> obj1:0x0B04
        je     loc_1_0335
        clc    
        ret    
loc_1_03AA:
        mov    edx, dword ptr [0xa94]        ; -> obj1:0x0A94
        add    edx, ebx
        test   word ptr [edx], 4
        jne    loc_1_0335
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [edx + 6], ax
        mov    ax, word ptr [ebp + 0x18]
        mov    word ptr [edx + 8], ax
        mov    ax, word ptr [ebp + 0x14]
        mov    word ptr [edx + 0xa], ax
        cmp    dword ptr [0xb04], ebx        ; -> obj1:0x0B04
        je     loc_1_0335
        cmp    word ptr [edx + 6], 0
        je     loc_1_03F1
        or     eax, 0xffffffff
        VxDCall  VDD.VDD_Hide_Cursor             ; cd2003000a00
loc_1_03F1:
        clc    
        ret    
sub_1_03F3:
        cmp    byte ptr [0xa98], 0xff        ; -> obj1:0x0A98
        jne    loc_1_0440
        test   ebx, ebx
        je     loc_1_0440
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_0440
        mov    eax, dword ptr [0xa94]        ; -> obj1:0x0A94
        mov    ecx, dword ptr [0xa90]        ; -> obj1:0x0A90
        or     ecx, ecx
        jne    loc_1_041F
        test   word ptr [eax + ebx], 0x10
        je     loc_1_0440
loc_1_041F:
        or     byte ptr [eax + ebx], 0x80
        bts    word ptr [eax + ebx], 1
        jb     loc_1_0440
        mov    eax, 0x10
        mov    ecx, 3
        mov    esi, 0x47a                    ; -> loc_1_047A
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014800100
loc_1_0440:
        ret    
loc_1_0441:
        cmp    byte ptr [0xa98], 0xff        ; -> obj1:0x0A98
        jne    loc_1_0453
        add    ebx, dword ptr [0xa94]        ; -> obj1:0x0A94
        or     byte ptr [ebx], 0x20
loc_1_0453:
        clc    
        ret    
loc_1_0455:
        cmp    byte ptr [0xa98], 0xff        ; -> obj1:0x0A98
        jne    loc_1_0478
        mov    eax, dword ptr [0xa94]        ; -> obj1:0x0A94
        and    byte ptr [eax + ebx], 0xdf
        btr    word ptr [eax + ebx], 3
        jae    loc_1_0478
        and    byte ptr [eax + ebx], 0xfd
        call   sub_1_03F3
loc_1_0478:
        clc    
        ret    
loc_1_047A:
        mov    edx, dword ptr [0xa94]        ; -> obj1:0x0A94
        add    edx, ebx
loc_1_0482:
        test   byte ptr [edx], 0x20
        jne    loc_1_0603
        or     byte ptr [edx], 4
        and    byte ptr [edx], 0x7f
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        cmp    dword ptr [0xa90], 0          ; -> obj1:0x0A90
        je     loc_1_04EB
        push   edx
        mov    ax, 2
        or     byte ptr [edx + 1], 8
        cmp    ebx, dword ptr [0xb04]        ; -> obj1:0x0B04
        jne    loc_1_04C6
        and    byte ptr [edx + 1], 0xf7
        inc    ax
loc_1_04C6:
        mov    word ptr [ebp + 0x1c], ax
        mov    ecx, dword ptr [0xa90]        ; -> obj1:0x0A90
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Build_Int_Stack_Frame       ; cd204c000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        pop    edx
        and    byte ptr [edx], 0xf9
        jmp    loc_1_05E2
loc_1_04EB:
        mov    word ptr [ebp + 0x1c], 9
        mov    ax, word ptr [edx + 2]
        mov    word ptr [ebp + 0x10], ax
        mov    ax, word ptr [edx + 4]
        mov    word ptr [ebp + 0x18], ax
        push   fs
        mov    ax, ds
        mov    fs, ax
        lea    esi, [edx + 0x1c]
        mov    ecx, 0x40
        stc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        pop    fs
        jb     loc_1_05E2
        mov    eax, edi
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x38], ax
        cmp    ebx, dword ptr [0xb04]        ; -> obj1:0x0B04
        je     loc_1_0594
        or     byte ptr [edx], 1
        mov    ecx, 8
        cld    
        movzx  eax, di
        shr    edi, 0xc
        and    edi, 0xfffffff0
        add    edi, eax
        push   ecx
        or     eax, 0xffffffff
        rep stosd dword ptr es:[edi], eax
        pop    ecx
        xor    eax, eax
        rep stosd dword ptr es:[edi], eax
        mov    eax, 0x33
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    word ptr [edx + 6], 0
        je     loc_1_0571
        or     eax, 0xffffffff
        VxDCall  VDD.VDD_Hide_Cursor             ; cd2003000a00
        jmp    loc_1_05D3
loc_1_0571:
        mov    word ptr [ebp + 0x1c], 0xa
        mov    word ptr [ebp + 0x18], 0xffff
        xor    eax, eax
        mov    word ptr [ebp + 0x10], ax
        mov    word ptr [ebp + 0x14], ax
        mov    eax, 0x33
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        jmp    loc_1_05D3
loc_1_0594:
        and    byte ptr [edx], 0xfe
        mov    eax, 0x33
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    word ptr [ebp + 0x1c], 0xa
        mov    ax, word ptr [edx + 6]
        mov    word ptr [ebp + 0x10], ax
        mov    ax, word ptr [edx + 8]
        mov    word ptr [ebp + 0x18], ax
        mov    ax, word ptr [edx + 0xa]
        mov    word ptr [ebp + 0x14], ax
        mov    eax, 0x33
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        xor    eax, eax
        VxDCall  VDD.VDD_Hide_Cursor             ; cd2003000a00
loc_1_05D3:
        and    byte ptr [edx], 0xf9
        clc    
        mov    ecx, 0x40
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
loc_1_05E2:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        btr    word ptr [edx], 7
        jb     loc_1_0482
        ret    
loc_1_0603:
        or     byte ptr [edx], 8
        ret    
loc_1_0607:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        mov    esi, ebx
        add    esi, dword ptr [0xa94]        ; -> obj1:0x0A94
        test   word ptr [esi], 0x40
        jne    loc_1_062C
        mov    eax, 0x33
        VMMCall  VMM.Get_V86_Int_Vector          ; cd2042000100
        je     loc_1_066F
        or     byte ptr [esi], 0x40
loc_1_062C:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 0x14
        je     loc_1_066B
        cmp    eax, 0x18
        je     loc_1_066B
        cmp    eax, 0x19
        je     loc_1_066B
        cmp    eax, 0x1f
        je     loc_1_066B
        cmp    eax, 9
        je     loc_1_068A
        cmp    eax, 0xc
        je     loc_1_06CD
        cmp    eax, 0x15
        je     sub_1_075B
        cmp    eax, 0x16
        je     loc_1_0784
        cmp    eax, 0x17
        je     loc_1_07FE
        jmp    sub_1_0670
loc_1_066B:
        or     byte ptr [ebp + 0x2c], 1
loc_1_066F:
        ret    
sub_1_0670:
        push   eax
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    eax, 0x33
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        pop    eax
        ret    
loc_1_068A:
        push   fs
        mov    ax, 0x3814
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
        mov    ecx, 0x40
        stc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        jb     loc_1_06CA
        push   word ptr [ebp + 0x5c]
        push   dword ptr [ebp + 0x14]
        mov    word ptr [ebp + 0x14], di
        shr    edi, 0x10
        mov    word ptr [ebp + 0x5c], di
        call   sub_1_0670
        pop    word ptr [ebp + 0x14]
        pop    word ptr [ebp + 0x5c]
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
loc_1_06CA:
        pop    fs
        ret    
loc_1_06CD:
        push   fs
        mov    ax, 0x3814
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
        add    ebx, dword ptr [0xa94]        ; -> obj1:0x0A94
        mov    word ptr [ebx + 0x5c], fs
        mov    dword ptr [ebx + 0x5e], esi
        mov    ax, word ptr [0xa9a]          ; -> obj1:0x0A9A
        mov    cx, word ptr [0xa9c]          ; -> obj1:0x0A9C
        xchg   word ptr [ebp + 0x5c], ax
        xchg   word ptr [ebp + 0x14], cx
        call   sub_1_0670
        mov    word ptr [ebp + 0x5c], ax
        mov    word ptr [ebp + 0x14], cx
        pop    fs
        ret    
loc_1_070B:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        add    ebx, dword ptr [0xa94]        ; -> obj1:0x0A94
        mov    cx, word ptr [ebx + 0x5c]
        test   cx, cx
        je     loc_1_075A
        mov    edx, dword ptr [ebx + 0x5e]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        ret    
loc_1_075A:
        ret    
sub_1_075B:
        push   eax
        push   ebx
        add    ebx, dword ptr [0xa94]        ; -> obj1:0x0A94
        mov    ax, word ptr [ebx + 0x62]
        test   ax, ax
        jne    loc_1_0779
        call   sub_1_0670
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [ebx + 0x62], ax
loc_1_0779:
        add    ax, 6
        mov    word ptr [ebp + 0x10], ax
        pop    ebx
        pop    eax
        ret    
loc_1_0784:
        push   fs
        push   dword ptr [ebp + 0x14]
        push   word ptr [ebp + 0x5c]
        mov    ax, 0x3814
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
        mov    edi, ebx
        add    edi, dword ptr [0xa94]        ; -> obj1:0x0A94
        movzx  ecx, word ptr [edi + 0x62]
        test   ecx, ecx
        jne    loc_1_07C1
        mov    ecx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    word ptr [ebp + 0x1c], 0x15
        call   sub_1_075B
        mov    dword ptr [ebp + 0x1c], eax
        xchg   dword ptr [ebp + 0x10], ecx
        movzx  ecx, cx
loc_1_07C1:
        push   edi
        clc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        jb     loc_1_07FB
        mov    dword ptr [ebp + 0x14], edi
        shr    edi, 0x10
        mov    word ptr [ebp + 0x5c], di
        pop    edi
        call   sub_1_0670
        stc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        add    esi, ecx
        mov    ax, word ptr [edi + 0x5c]
        mov    word ptr [esi], ax
        mov    eax, dword ptr [edi + 0x5e]
        mov    dword ptr [esi + 2], eax
loc_1_07F1:
        pop    word ptr [ebp + 0x5c]
        pop    dword ptr [ebp + 0x14]
        pop    fs
        ret    
loc_1_07FB:
        pop    edi
        jmp    loc_1_07F1
loc_1_07FE:
        push   fs
        push   word ptr [ebp + 0x5c]
        push   dword ptr [ebp + 0x14]
        mov    ax, 0x3814
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
        mov    edi, ebx
        add    edi, dword ptr [0xa94]        ; -> obj1:0x0A94
        movzx  ecx, word ptr [edi + 0x62]
        test   ecx, ecx
        jne    loc_1_083B
        mov    ecx, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    word ptr [ebp + 0x1c], 0x15
        call   sub_1_075B
        mov    dword ptr [ebp + 0x1c], eax
        xchg   dword ptr [ebp + 0x10], ecx
        movzx  ecx, cx
loc_1_083B:
        add    esi, ecx
        mov    ax, word ptr [esi]
        mov    word ptr [edi + 0x5c], ax
        mov    eax, dword ptr [esi + 2]
        mov    dword ptr [edi + 0x5e], eax
        sub    esi, ecx
        stc    
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        jb     loc_1_086C
        mov    dword ptr [ebp + 0x14], edi
        shr    edi, 0x10
        mov    word ptr [ebp + 0x5c], di
        pop    edi
        call   sub_1_0670
        clc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
loc_1_086C:
        pop    dword ptr [ebp + 0x14]
        pop    word ptr [ebp + 0x38]
        pop    fs
        ret    
        db  000h, 000h
VMD_Control:
        cmp    eax, 0
        je     loc_2_0000
        cmp    eax, 1
        je     loc_2_0214
        cmp    eax, 2
        je     loc_2_0160
        cmp    eax, 0xf
        je     loc_1_08B5
        cmp    eax, 0x10
        je     loc_1_0441
        cmp    eax, 0x11
        je     loc_1_0455
        cmp    eax, 9
        je     loc_1_0274
        clc    
        ret    
loc_1_08B5:
        test   edx, edx
        je     loc_1_08BE
        cmp    edx, 0xc
        jne    loc_1_0939
loc_1_08BE:
        xchg   dword ptr [0xb04], ebx        ; -> obj1:0x0B04
        cmp    byte ptr [0xb0a], 0xff        ; -> loc_1_0B0A
        je     loc_1_0939
        call   sub_1_03F3
        push   ebx
        mov    ebx, dword ptr [0xb04]        ; -> obj1:0x0B04
        call   sub_1_03F3
        pop    ebx
        movzx  esi, byte ptr [0xb0b]         ; -> obj1:0x0B0B
        test   esi, esi
        jne    loc_1_0923
        cmp    byte ptr [0xb08], 0xff        ; -> obj1:0x0B08
        jne    loc_1_0939
        mov    eax, dword ptr [0xb00]        ; -> obj1:0x0B00
        cmp    byte ptr [0xb09], 3           ; -> obj1:0x0B09
        je     loc_1_090F
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 0x20
        je     loc_1_0939
loc_1_090F:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        mov    ebx, dword ptr [0xb04]        ; -> obj1:0x0B04
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        jmp    loc_1_0939
loc_1_0923:
        mov    eax, 0xf
        mov    edx, 0xe
        mov    ebx, dword ptr [0xb04]        ; -> obj1:0x0B04
        VMMCall  VMM.System_Control              ; cd2093000100
loc_1_0939:
        clc    
        ret    
VMD_Get_Version:
        mov    eax, 0x300
        clc    
        ret    
VMD_Set_Mouse_Type:
        cmp    byte ptr [0xb0a], 0xff        ; -> loc_1_0B0A
        jne    loc_1_09EE
        pushal 
        mov    byte ptr [0xb09], cl          ; -> obj1:0x0B09
        test   eax, eax
        jns    loc_1_0964
        neg    eax
        VxDCall  VPICD.VPICD_Convert_Int_To_IRQ  ; cd200e000300
        jb     loc_1_09E1
loc_1_0964:
        cmp    al, 2
        jne    loc_1_096A
        mov    al, 9
loc_1_096A:
        mov    byte ptr [0xb0a], al          ; -> loc_1_0B0A
        cmp    byte ptr [0xb09], 4           ; -> obj1:0x0B09
        je     loc_1_09DE
        cmp    byte ptr [0xb09], 0x82        ; -> obj1:0x0B09
        jne    loc_1_098C
        mov    byte ptr [0xb09], 2           ; -> obj1:0x0B09
        mov    al, dl
        jmp    loc_1_0999
loc_1_098C:
        cmp    byte ptr [0xb09], 2           ; -> obj1:0x0B09
        jne    loc_1_09B7
        sub    al, 5
        neg    al
loc_1_0999:
        push   eax
        VxDCall  VCD.VCD_Get_Version             ; cd2000000e00
        test   eax, eax
        pop    eax
        je     loc_1_09B7
        mov    byte ptr [0xb0b], al          ; -> obj1:0x0B0B
        movzx  eax, al
        xor    edx, edx
        VxDCall  VCD.VCD_Set_Port_Global         ; cd2001000e00
        jmp    loc_1_09DE
loc_1_09B7:
        movzx  edi, byte ptr [0xb0a]         ; -> loc_1_0B0A
        mov    word ptr [0xb0c], di          ; -> loc_1_0B0C
        mov    edi, 0xb0c                    ; -> loc_1_0B0C
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_1_09E1
        mov    dword ptr [0xb00], eax        ; -> obj1:0x0B00
        mov    byte ptr [0xb08], 0xff        ; -> obj1:0x0B08
loc_1_09DE:
        popal  
        clc    
        ret    
loc_1_09E1:
        mov    byte ptr [0xb0a], 0xff        ; -> loc_1_0B0A
        popal  
        xor    eax, eax
        inc    eax
        stc    
        ret    
loc_1_09EE:
        xor    eax, eax
        stc    
        ret    
VMD_Get_Mouse_Owner:
        mov    ebx, dword ptr [0xb04]        ; -> obj1:0x0B04
        ret    
VMD_PM_API:
        movzx  eax, word ptr [ebp + 0x1c]
        test   eax, eax
        jne    loc_1_0A0D
        mov    word ptr [ebp + 0x1c], 0x300
loc_1_0A07:
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_1_0A0D:
        test   ah, ah
        jne    loc_1_0A16
        jmp    loc_1_004F
loc_1_0A16:
        cmp    ax, 0x100
        jne    loc_1_0A31
        movzx  eax, byte ptr [ebp + 0x11]
        neg    eax
        movzx  ecx, byte ptr [ebp + 0x10]
        mov    dl, byte ptr [ebp + 0x18]
        VxDCall  VMD.VMD_Set_Mouse_Type          ; cd2001000c00
        jae    loc_1_0A07
loc_1_0A31:
        or     word ptr [ebp + 0x2c], 1
        ret    
        db  090h
loc_1_0A38:
        mov    ebx, dword ptr [0xb04]        ; -> obj1:0x0B04
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002800300
loc_1_0A44:
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003800300
        db  0FFh, 03Fh, 0FFh, 01Fh, 0FFh, 00Fh, 0FFh, 007h, 0FFh, 003h, 0FFh, 001h, 0FFh, 000h, 07Fh, 000h
        db  03Fh, 000h, 01Fh, 000h, 0FFh, 001h, 0FFh, 010h, 0FFh, 030h, 07Fh, 0F8h, 07Fh, 0F8h, 07Fh, 0FCh
        db  000h, 000h, 000h, 040h, 000h, 060h, 000h, 070h, 000h, 078h, 000h, 07Ch, 000h, 07Eh, 000h, 07Fh
        db  080h, 07Fh, 000h, 07Ch, 000h, 06Ch, 000h, 046h, 000h, 006h, 000h, 003h, 000h, 003h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
        dd  VMD_Get_Version          ; fixup
        dd  VMD_Set_Mouse_Type       ; fixup
        dd  VMD_Get_Mouse_Owner      ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0000Ch                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VMD     '                  ; DDB_Name
        dd  034000000h                  ; DDB_Init_Order
        dd  VMD_Control              ; DDB_Control_Proc
        dd  VMD_PM_API               ; DDB_V86_API_Proc
        dd  VMD_PM_API               ; DDB_PM_API_Proc
        dd  obj1:0x0AA4              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h
loc_1_0B0A:
        inc    dword ptr [eax]
loc_1_0B0C:
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        cmp    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esp
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, dh
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x25f  R-X
loc_2_0000:
        mov    esi, 0x607                    ; -> loc_1_0607
        xor    edx, edx
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        jae    loc_2_0015
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_2_0015:
        mov    ecx, eax
        shr    ecx, 0x10
        mov    edx, eax
        movzx  edx, dx
        mov    eax, 0x33
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045800100
loc_2_002A:
        test   edx, edx
        je     loc_2_008A
        mov    byte ptr [0xa98], 0xff        ; -> obj1:0x0A98
        cmp    dh, 2
        jne    loc_2_0054
        xor    eax, eax
        mov    edi, 0x23c                    ; -> loc_2_023C
        xor    esi, esi
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        or     al, al
        je     loc_2_004F
        mov    al, 0x21
loc_2_004F:
        mov    byte ptr [0xa9e], al          ; -> obj1:0x0A9E
loc_2_0054:
        cmp    dh, 4
        jne    loc_2_0075
        mov    eax, 1
        mov    edi, 0x24a                    ; -> obj2:0x024A
        xor    esi, esi
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        or     al, al
        je     loc_2_0075
        inc    byte ptr [0xa9f]              ; -> obj1:0x0A9F
loc_2_0075:
        cmp    dh, 0
        je     loc_2_008A
        test   dl, dl
        je     loc_2_008A
        movzx  eax, dl
        movzx  ecx, dh
        VxDCall  VMD.VMD_Set_Mouse_Type          ; cd2001000c00
loc_2_008A:
        push   0
        push   0x64
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_2_015A
        mov    dword ptr [0xa94], eax        ; -> obj1:0x0A94
        VxDCall  VDD.VDD_Get_Version             ; cd2000000a00
        cmp    eax, 0x30a
        jb     loc_2_00B7
        mov    byte ptr [0xa99], 0xff        ; -> obj1:0x0A99
loc_2_00B7:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x1607
        mov    word ptr [ebp + 0x10], 0xc
        mov    word ptr [ebp + 0x18], 1
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  eax, word ptr [ebp + 0x1c]
        mov    cx, word ptr [ebp + 0x3c]
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 4]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        test   eax, eax
        jne    loc_2_0129
        mov    dword ptr [0xa90], ecx        ; -> obj1:0x0A90
        mov    esi, 0x1f0                    ; -> loc_1_01F0
        mov    eax, 0x33
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        jmp    loc_2_0139
loc_2_0129:
        mov    eax, 0x33
        mov    esi, 0x320                    ; -> loc_1_0320
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
loc_2_0139:
        mov    esi, 0x70b                    ; -> loc_1_070B
        xor    edx, edx
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        mov    word ptr [0xa9c], ax          ; -> obj1:0x0A9C
        shr    eax, 0x10
        mov    word ptr [0xa9a], ax          ; -> obj1:0x0A9A
        jmp    loc_1_0000
loc_2_015A:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_2_0160:
        cmp    byte ptr [0xa98], 0xff        ; -> obj1:0x0A98
        jne    loc_2_01FF
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x1607
        mov    word ptr [ebp + 0x10], 0xc
        mov    word ptr [ebp + 0x18], 0
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  eax, word ptr [ebp + 0x18]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        test   eax, eax
        jne    loc_2_01FF
        movzx  edi, word ptr [0xce]
        mov    eax, edi
        dec    edi
        shl    edi, 4
        cmp    byte ptr [edi], 0x4d
        jne    loc_2_0201
        cmp    word ptr [edi + 1], ax
        jne    loc_2_0201
        movzx  ecx, word ptr [edi + 3]
        shl    ecx, 4
        add    edi, 0x10
        mov    esi, 0x220                    ; -> obj2:0x0220
        mov    dword ptr [esi + 8], edi
        mov    dword ptr [esi + 0xc], ecx
        mov    dword ptr [esi + 0x10], 0x200
        push   0
        push   esi
        VMMCall  VMM._AddInstanceItem            ; cd20a6000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_2_020E
loc_2_01FF:
        clc    
        ret    
loc_2_0201:
        mov    esi, 0x234                    ; -> loc_2_0234
        VxDCall  DOSMGR.DOSMGR_Instance_Device   ; cd2007001500
        jmp    loc_2_01FF
loc_2_020E:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_2_0214:
        mov    dword ptr [0xb04], ebx        ; -> obj1:0x0B04
        jmp    loc_2_002A
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h
loc_2_0234:
        dec    ebp
        push   ebx
        and    al, 0x4d
        dec    edi
        push   ebp
        push   ebx
        inc    ebp
loc_2_023C:
        dec    ebp
        outsd  dx, dword ptr [esi]
        jne    0x2b3
        push   ebx
        outsd  dx, dword ptr [esi]
        je     0x28f
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [eax + eax + 0x4d], 0x6573756f
        dec    eax
        popal  
        jb     0x2b7
        push   edx
        jae    0x2bc
        je     0x29a
        je     0x2a4
        outsb  dx, byte ptr [esi]
        db  069h, 074h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x42  R-X
        db  0F7h, 0C3h, 003h, 000h, 075h, 032h, 006h, 0B8h, 033h, 035h, 0CDh, 021h, 066h, 033h, 0D2h, 08Ch
        db  0C2h, 00Bh, 0D3h, 074h, 01Ah, 026h, 080h, 03Fh, 0CFh, 075h, 005h, 066h, 033h, 0D2h, 0EBh, 00Fh
        db  066h, 0BAh, 0FFh, 0FFh, 0FFh, 0FFh, 0B8h, 024h, 000h, 033h, 0C9h, 0CDh, 033h, 08Bh, 0D1h, 033h
        db  0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 007h, 0C3h, 033h, 0DBh, 033h, 0F6h, 0B8h, 001h, 080h, 007h
        db  0C3h, 020h
VxD__LTEXT ENDS
