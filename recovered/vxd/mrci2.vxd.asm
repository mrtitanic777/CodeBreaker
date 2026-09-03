; MRCI2 -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 6539 instructions recovered by recursive descent

Declare_Virtual_Device MRCI2, 3, 10, MRCI2_Control, 00042h, Undefined_Init_Order
;   DDK version 0400h   init_order 016FFFFFFh   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x124  R-X
MRCI2_Control:
        cmp    eax, 1
        je     loc_2_0000
        cmp    eax, 0x1b
        je     loc_2_0000
        cmp    eax, 5
        je     loc_3_02CC
        clc    
        ret    
        db  000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00042h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'MRCI2   '                  ; DDB_Name
        dd  016FFFFFFh                  ; DDB_Init_Order
        dd  MRCI2_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
        dd  loc_5_517C               ; fixup
        dd  loc_5_61AF               ; fixup
        dd  loc_5_61AF               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_5_61B4               ; fixup
        dd  loc_5_74EF               ; fixup
        dd  loc_5_74EF               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00A8:
        dec    ebp
        push   ebx
        inc    esi
        push   esp
        add    byte ptr [edx], al
        add    byte ptr [edx], al
        call   0xeb0000b5
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        db  080h
loc_1_00BA:
        jmp    0xbf
        db  000h, 000h, 000h, 000h
loc_1_00C0:
        add    dword ptr [ebx], eax
        add    eax, dword ptr [0x7050505]
        pop    es
        pop    es
        pop    es
        pop    es
        pop    es
        pop    es
        pop    es
        or     dword ptr [ecx], ecx
        or     dword ptr [ecx], ecx
        or     dword ptr [ecx], ecx
        or     dword ptr [ecx], ecx
        or     dword ptr [ecx], ecx
        or     dword ptr [ecx], ecx
        or     dword ptr [ecx], ecx
        or     dword ptr [ecx], ecx
        db  000h
loc_1_00E0:
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        push   es
        add    byte ptr [eax + eax], al
        or     al, 0
        adc    al, 0
        sbb    al, 0
        or     byte ptr [eax], al
        sbb    byte ptr [eax], al
        sub    byte ptr [eax], al
        cmp    byte ptr [eax], al
        dec    eax
        add    byte ptr [eax], bl
        push   0x10007800
        add    byte ptr [eax], dh
        add    byte ptr [eax], dl
        jo     loc_1_0106
loc_1_0106:
        nop    
        add    byte ptr [eax - 0xfff3000], dh
        add    byte ptr [eax], dl
        add    dword ptr [eax], esi
        add    dword ptr [eax + 1], edx
        jo     loc_1_0117
        nop    
loc_1_0117:
        add    dword ptr [eax - 0xffe2fff], esi
        add    dword ptr [eax], eax
        db  000h
loc_1_0120:
        aas    
        adc    dword ptr [eax], eax
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x44c  R-X
loc_2_0000:
        mov    eax, 0                        ; -> sub_5_0000
        and    eax, 0xfffff000
        mov    edx, 0xaef0                   ; -> obj5:0xAEF0
        add    edx, 0xfff
        and    edx, 0xfffff000
        sub    edx, eax
        shr    eax, 0xc
        shr    edx, 0xc
        mov    dword ptr [0x98], eax         ; -> obj1:0x0098
        mov    dword ptr [0x94], edx         ; -> obj1:0x0094
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        sub    eax, eax
        mov    dword ptr [0x28], eax         ; -> obj4:0x0028
        mov    dword ptr [0xba], eax         ; -> loc_1_00BA
        call   sub_2_0076
        call   sub_2_01BB
        call   sub_2_032C
        mov    eax, dword ptr [0x28]         ; -> obj4:0x0028
        or     eax, dword ptr [0xba]         ; -> loc_1_00BA
        cmp    eax, 1
        jb     loc_2_0075
        mov    esi, 0x70                     ; -> obj1:0x0070
        VMMCall  VMM.Install_Exception_Handler   ; cd20ef000100
        mov    esi, 0x80                     ; -> obj1:0x0080
        VMMCall  VMM.Install_Exception_Handler   ; cd20ef000100
loc_2_0075:
        ret    
sub_2_0076:
        pushal 
        VMMCall  VMM.VMM_GetSystemInitState      ; cd2011010100
        cmp    eax, 0x20000000
        ja     loc_2_0155
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        push   edx
        push   0
        push   0x11
        VMMCall  VMM._Allocate_Temp_V86_Data_Area ; cd20a9000100
        add    esp, 8
        pop    edx
        or     eax, eax
        je     loc_2_0140
        mov    edi, eax
        mov    word ptr [ebp + 0x1c], 0x4a11
        mov    word ptr [ebp + 0x10], 0
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    word ptr [ebp + 0x1c], 0
        jne    loc_2_013A
        cmp    word ptr [ebp + 0x10], 0x444d
        jne    loc_2_013A
        movzx  ecx, byte ptr [ebp + 0x19]
        movzx  edx, byte ptr [ebp + 0x18]
        sub    dl, 0x41
        add    ecx, edx
        sub    ecx, 0x1a
        sbb    edx, edx
        and    ecx, edx
        add    ecx, 0x1a
        xor    edx, edx
loc_2_00FE:
        mov    word ptr [edi], 0x444d
        mov    byte ptr [edi + 2], 0x49
        mov    word ptr [edi + 3], 0x3f3f
        mov    word ptr [ebp + 0x1c], 0x4404
        inc    dl
        mov    byte ptr [ebp + 0x10], dl
        mov    word ptr [ebp + 0x18], 0x11
        mov    eax, edi
        shr    eax, 4
        mov    word ptr [ebp + 0x3c], ax
        mov    word ptr [ebp + 0x14], 0
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        loop   loc_2_00FE
loc_2_013A:
        VMMCall  VMM._Free_Temp_V86_Data_Area    ; cd20aa000100
loc_2_0140:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
loc_2_0155:
        popal  
        ret    
sub_2_0157:
        movzx  eax, word ptr [edi + 0xc]
        and    ah, 0x7f
        cmp    eax, ebx
        jne    loc_2_0171
        cmp    word ptr [edi + 0xe], 0
        jne    loc_2_0171
        cmp    word ptr [edi + 0x10], 0x8000
        jbe    loc_2_019C
loc_2_0171:
        cmp    word ptr [edi + 0x10], 0x8001
        jae    loc_2_017F
        cmp    word ptr [edi + 6], 0x201
loc_2_017F:
        sbb    ecx, ecx
        and    ecx, ebx
        not    ecx
        and    ecx, eax
        mov    edx, ebx
        not    ecx
        and    edx, ecx
        je     loc_2_01B9
        and    ebx, eax
        cmp    eax, ebx
        jne    loc_2_019A
        cmp    word ptr [edi + 0xe], 0
loc_2_019A:
        clc    
        ret    
loc_2_019C:
        cmp    word ptr [edi + 6], 0x200
        jb     loc_2_01B6
        ja     loc_2_01B9
        cmp    dword ptr [edi], 0x5446534d
        jne    loc_2_01B9
        cmp    word ptr [edi + 4], 0x200
        ja     loc_2_01B9
loc_2_01B6:
        cmp    edi, edi
        ret    
loc_2_01B9:
        stc    
        ret    
sub_2_01BB:
        push   ebx
        sub    edi, edi
        mov    ebp, dword ptr [ebx + 8]
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    word ptr [ebp + 0x1c], 0x4a12
        mov    word ptr [ebp + 0x18], 0x4d52
        mov    word ptr [ebp + 0x14], 0x4349
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        mov    dword ptr [0x28], 0x2eb       ; -> obj4:0x0028
        cmp    word ptr [ebp + 0x18], 0x4943
        jne    loc_2_027B
        cmp    word ptr [ebp + 0x14], 0x524d
        jne    loc_2_027B
        movzx  edi, word ptr [ebp + 0x38]
        mov    word ptr [0x36], di           ; -> loc_4_0036
        shl    edi, 4
        movzx  edx, word ptr [ebp]
        mov    word ptr [0x34], dx           ; -> loc_4_0034
        add    edi, edx
        add    edi, dword ptr [ebx + 4]
        test   byte ptr [edi + 0xd], 0x80
        je     loc_2_022F
        and    dword ptr [0x34], 0           ; -> loc_4_0034
loc_2_022F:
        mov    ebx, 0x2eb
        call   sub_2_0157
        ja     loc_2_0248
        jb     loc_2_0320
        mov    eax, 0x2eb
        jmp    loc_2_027B
loc_2_0248:
        mov    dword ptr [0x28], edx         ; -> obj4:0x0028
        or     eax, 0x2eb
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [0x2c], ecx         ; -> obj4:0x002C
        mov    bx, word ptr [edi + 0xe]
        mov    cx, word ptr [edi + 0x10]
        mov    word ptr [0xe], bx            ; -> obj4:0x000E
        cmp    word ptr [0x10], cx           ; -> obj4:0x0010
        ja     loc_2_027B
        mov    word ptr [0x10], cx           ; -> obj4:0x0010
loc_2_027B:
        push   eax
        VMMCall  VMM.VMM_GetSystemInitState      ; cd2011010100
        cmp    eax, 0x20000000
        mov    eax, 0
        ja     loc_2_029B
        push   1
        push   0x12
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
loc_2_029B:
        pop    ebx
        mov    edx, eax
        or     eax, eax
        je     loc_2_02B3
        pop    ecx
        push   ecx
        add    edx, dword ptr [ecx + 4]
        shl    eax, 0xc
        shr    ax, 0xc
        mov    dword ptr [0x34], eax         ; -> loc_4_0034
loc_2_02B3:
        mov    esi, 0x1a6                    ; -> loc_3_01A6
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        mov    dword ptr [0x9c], eax         ; -> obj1:0x009C
        mov    esi, 0x1a6                    ; -> loc_3_01A6
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        jb     loc_2_0320
        mov    esi, 0                        ; -> obj4:0x0000
        mov    dword ptr [esi + 8], eax
        or     edx, edx
        je     loc_2_02F1
        xchg   edx, edi
        mov    ecx, 0x12
        cld    
        push   esi
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        sub    edi, 0x12
        mov    word ptr [edi + 0xc], bx
        mov    edi, edx
loc_2_02F1:
        or     edi, edi
        je     loc_2_0317
        test   byte ptr [edi + 0xd], 0x80
        jne    loc_2_0317
        mov    ecx, 0x12
        push   ecx
        push   esi
        push   edi
        mov    esi, edi
        mov    dword ptr [0x24], esi         ; -> obj4:0x0024
        mov    edi, 0x12                     ; -> obj4:0x0012
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        pop    ecx
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_2_0317:
        call   sub_2_03ED
        jb     loc_2_0320
        jmp    loc_2_032A
loc_2_0320:
        mov    dword ptr [0x28], 0           ; -> obj4:0x0028
loc_2_032A:
        pop    ebx
        ret    
sub_2_032C:
        mov    eax, 0x4a12
        mov    edx, 0x4d524349
        push   0x2f
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    dword ptr [0xba], 0x3eb       ; -> loc_1_00BA
        cmp    edx, 0x4943524d
        jne    loc_2_03C4
        mov    ebx, 0x3eb
        call   sub_2_0157
        ja     loc_2_0378
        jb     loc_2_03E2
        movzx  eax, word ptr [edi + 0xc]
        and    ah, 0x7f
        mov    dword ptr [0x444], eax        ; -> obj2:0x0444
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [0x448], eax        ; -> sub_2_0448
        jmp    loc_2_03B1
loc_2_0378:
        or     eax, 0x3eb
        mov    dword ptr [0xba], edx         ; -> loc_1_00BA
        mov    ecx, dword ptr [edi + 8]
        mov    dword ptr [0x30], ecx         ; -> loc_4_0030
        mov    bx, word ptr [edi + 0xe]
        mov    cx, word ptr [edi + 0x10]
        mov    word ptr [0xb4], ax           ; -> obj1:0x00B4
        mov    word ptr [0xb6], bx           ; -> obj1:0x00B6
        cmp    word ptr [0xb8], cx           ; -> obj1:0x00B8
        je     loc_2_03B1
        mov    word ptr [0xb8], cx           ; -> obj1:0x00B8
loc_2_03B1:
        test   byte ptr [edi + 0xd], 0x80
        jne    loc_2_03C4
        mov    esi, 0xa8                     ; -> loc_1_00A8
        mov    ecx, 0x12
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_2_03C4:
        call   sub_2_03ED
        jb     loc_2_03E2
        mov    ebx, dword ptr [0x444]        ; -> obj2:0x0444
        or     ebx, ebx
        je     loc_2_03EC
        mov    eax, 0xffffffff
        call   sub_2_0448
        jmp    loc_2_03EC
loc_2_03E2:
        mov    dword ptr [0xba], 0           ; -> loc_1_00BA
loc_2_03EC:
        ret    
sub_2_03ED:
        bts    dword ptr [0x440], 0          ; -> obj2:0x0440
        cmc    
        jae    loc_2_043D
        mov    eax, 0x2f
        mov    esi, 0                        ; -> loc_3_0000
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    eax, 0x2f
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        mov    dword ptr [0xa4], ecx         ; -> obj1:0x00A4
        mov    dword ptr [0xa0], edx         ; -> obj1:0x00A0
        mov    esi, 0x186                    ; -> loc_3_0186
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 0x10
        mov    eax, 0x2f
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
loc_2_043D:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0448:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x2e4  R-X
loc_3_0000:
        cmp    word ptr [ebp + 0x1c], 0x4a12
        jne    loc_3_009C
        mov    cx, word ptr [ebp + 0x18]
        mov    dx, word ptr [ebp + 0x14]
        cmp    cx, 0x4d52
        jne    loc_3_0070
        cmp    dx, 0x4349
        jne    loc_3_0070
        cmp    dword ptr [0x28], 0           ; -> obj4:0x0028
        je     loc_3_0143
        mov    eax, dword ptr [0x34]         ; -> loc_4_0034
        or     eax, eax
        je     loc_3_0143
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        bt     dword ptr [ebx], 5
        cmc    
        sbb    ebx, ebx
        and    eax, ebx
        xchg   cl, ch
        xchg   dl, dh
        xchg   cx, dx
        mov    word ptr [ebp + 0x18], cx
        mov    word ptr [ebp + 0x14], dx
        mov    word ptr [ebp], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x38], ax
        mov    eax, dword ptr [0x28]         ; -> obj4:0x0028
        mov    word ptr [ebp + 0x10], ax
        clc    
        ret    
loc_3_0070:
        mov    edx, dword ptr [ebp + 0x14]
        cmp    edx, 0x4d524349
        jne    loc_3_009C
        cmp    dword ptr [0xba], 0           ; -> loc_1_00BA
        je     loc_3_009C
        xchg   dl, dh
        rol    edx, 0x10
        xchg   dl, dh
        mov    dword ptr [ebp + 0x14], edx
        mov    dword ptr [ebp], 0xa8         ; -> loc_1_00A8
        call   sub_5_016C
        clc    
        ret    
loc_3_009C:
        cmp    word ptr [ebp + 0x1c], 0x4a20
        jne    loc_3_0143
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        test   byte ptr [ebx], 0x20
        mov    edx, dword ptr [0x9c]         ; -> obj1:0x009C
        mov    bx, 0x400
        jne    loc_3_00C5
        mov    edx, dword ptr [8]            ; -> obj4:0x0008
        xor    ebx, ebx
loc_3_00C5:
        dec    word ptr [ebp + 0x18]
        jl     loc_3_0103
        xchg   edi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        add    dword ptr [ebp], 0x14
        cld    
        mov    esi, 0                        ; -> obj4:0x0000
        mov    ecx, 9
        rep movsw word ptr es:[edi], word ptr [esi]
        mov    ax, 0x4d4a
        stosw  word ptr es:[edi], ax
        sub    edi, 0x14
        mov    dword ptr [edi + 8], edx
        and    word ptr [edi + 0xc], 0xfffc
        and    word ptr [edi + 0xe], 0xfffc
        or     word ptr [edi + 0xc], bx
loc_3_0103:
        dec    word ptr [ebp + 0x18]
        jl     loc_3_0143
        xchg   edi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        add    dword ptr [ebp], 0x14
        cld    
        mov    esi, 0                        ; -> obj4:0x0000
        mov    ecx, 9
        rep movsw word ptr es:[edi], word ptr [esi]
        mov    ax, 0x5344
        stosw  word ptr es:[edi], ax
        sub    edi, 0x14
        mov    dword ptr [edi + 8], edx
        and    word ptr [edi + 0xc], 0xff3f
        and    word ptr [edi + 0xe], 0xff3f
        or     word ptr [edi + 0xc], bx
loc_3_0143:
        stc    
        ret    
sub_3_0145:
        movzx  edx, ax
        shr    eax, 0x10
        mov    ecx, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        xchg   ebx, ecx
        test   byte ptr [ecx], 0x20
        jne    loc_3_0163
        shl    eax, 4
        add    eax, edx
loc_3_015F:
        add    eax, dword ptr [ecx + 4]
loc_3_0162:
        ret    
loc_3_0163:
        push   ecx
        push   edx
        push   0
        push   eax
        push   ecx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        pop    edx
        pop    ecx
        add    eax, 1
        jb     loc_3_0162
        lea    eax, [edx + eax - 1]
        cmp    eax, 0x110000
        jae    loc_3_0162
        jmp    loc_3_015F
loc_3_0186:
        call   0
        jb     loc_3_0194
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        ret    
loc_3_0194:
        mov    ecx, dword ptr [0xa4]         ; -> obj1:0x00A4
        mov    edx, dword ptr [0xa0]         ; -> obj1:0x00A0
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_3_01A6:
        movzx  eax, word ptr [ebp + 0x1c]
        sub    esp, 0x14
        push   eax
        push   eax
        inc    eax
        je     loc_3_02BC
        dec    eax
        test   dword ptr [0x28], eax         ; -> obj4:0x0028
        je     loc_3_029D
        xchg   esi, eax
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        lea    edi, [esp + 8]
        mov    ecx, 0x14
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    edi, [esi - 0x14]
        lea    esi, [esp + 8]
        movzx  edx, word ptr [esi + 0xc]
        mov    dword ptr [esp + 4], edx
        push   eax
        mov    eax, dword ptr [esi]
        call   sub_3_0145
        mov    edx, eax
        pop    eax
        jb     loc_3_02B5
        mov    dword ptr [esi], edx
        push   eax
        mov    eax, dword ptr [esi + 8]
        call   sub_3_0145
        mov    edx, eax
        pop    eax
        jb     loc_3_02B5
        mov    ecx, edx
        mov    dword ptr [esi + 8], edx
        sub    ecx, dword ptr [ebx + 4]
        sbb    edx, edx
        or     ecx, edx
        cmp    ecx, 0x110000
        cmc    
        sbb    edx, edx
        or     ecx, edx
        test   eax, 0x200
        je     loc_3_0248
        mov    dx, word ptr [edi + 6]
        mov    word ptr [esi + 6], dx
        push   eax
        push   ecx
        mov    eax, dword ptr [esi + 0x10]
        call   sub_3_0145
        mov    edx, eax
        pop    ecx
        pop    eax
        jb     loc_3_02B5
        mov    dword ptr [esi + 0x10], edx
loc_3_0248:
        push   eax
        push   ecx
        call   sub_5_0000
        pop    ecx
        pop    eax
        jb     loc_3_028D
        push   ebx
        push   edx
        push   edi
        call   sub_5_0111
        mov    edx, edi
        pop    edi
        pop    esi
        pop    ebx
        or     eax, eax
        jne    loc_3_0276
        mov    word ptr [edi + 0xc], dx
        add    word ptr [edi], cx
        add    word ptr [edi + 8], dx
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [edi + 0x10], ecx
loc_3_0276:
        mov    word ptr [ebp + 0x1c], ax
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
loc_3_0280:
        pop    eax
        pop    edx
        mov    esi, esp
        call   sub_5_0093
        add    esp, 0x14
        ret    
loc_3_028D:
        mov    word ptr [ebp + 0x1c], 4
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        add    esp, 0x1c
        ret    
loc_3_029D:
        mov    eax, dword ptr [0x2c]         ; -> obj4:0x002C
        or     eax, eax
        je     loc_3_02B5
        rol    eax, 0x10
        mov    word ptr [ebp + 0x28], ax
        shr    eax, 0x10
        mov    dword ptr [ebp + 0x24], eax
        jmp    loc_3_0280
loc_3_02B5:
        mov    eax, 1
        jmp    loc_3_0276
loc_3_02BC:
        movzx  eax, word ptr [ebp + 0x10]
        not    eax
        and    dword ptr [0x28], eax         ; -> obj4:0x0028
        sub    eax, eax
        jmp    loc_3_0276
loc_3_02CC:
        mov    edi, dword ptr [0x24]         ; -> obj4:0x0024
        or     edi, edi
        je     loc_3_02E3
        mov    esi, 0x12                     ; -> obj4:0x0012
        mov    ecx, 0x12
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_02E3:
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x38  RW-
        db  04Dh, 053h, 046h, 054h, 000h, 002h, 000h, 002h, 000h, 000h, 000h, 000h, 0EBh, 002h, 000h, 000h
        db  000h, 080h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0EBh, 002h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_0030:
        or     eax, dword ptr [ecx]
        add    byte ptr [eax], al
loc_4_0034:
        add    byte ptr [eax], al
loc_4_0036:
        add    byte ptr [eax], al
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0xaef0  R-X
sub_5_0000:
        sub    esp, 8
        test   eax, 0x41
        je     loc_5_008E
        call   sub_5_016C
        jb     loc_5_008E
        mov    eax, dword ptr [esi]
        movzx  edx, word ptr [esi + 4]
        lea    edx, [eax + edx + 0xfff]
        shr    eax, 0xc
        shr    edx, 0xc
        sub    edx, eax
        cmp    edx, 1
        jb     loc_5_0088
        mov    dword ptr [esp], eax
        mov    dword ptr [esp + 4], edx
        push   0
        push   edx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        cmp    eax, 1
        jb     loc_5_0088
        mov    eax, dword ptr [esi + 8]
        movzx  edx, word ptr [esi + 0xc]
        lea    edx, [eax + edx + 0xfff]
        shr    eax, 0xc
        shr    edx, 0xc
        sub    edx, eax
        cmp    edx, 1
        jb     loc_5_0075
        push   0
        push   edx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        cmp    eax, 1
        jae    loc_5_008E
loc_5_0075:
        push   0
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
loc_5_0088:
        call   sub_5_019C
        stc    
loc_5_008E:
        lea    esp, [esp + 8]
        ret    
sub_5_0093:
        cmp    eax, -1
        je     loc_5_00E7
        test   eax, 0x41
        je     loc_5_00E7
        push   edx
        call   sub_5_019C
        pop    edx
        mov    eax, dword ptr [esi + 8]
        lea    edx, [eax + edx + 0xfff]
        shr    eax, 0xc
        shr    edx, 0xc
        sub    edx, eax
        push   0
        push   edx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        mov    eax, dword ptr [esi]
        movzx  edx, word ptr [esi + 4]
        lea    edx, [eax + edx + 0xfff]
        shr    eax, 0xc
        shr    edx, 0xc
        sub    edx, eax
        push   0
        push   edx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
loc_5_00E7:
        ret    
loc_5_00E8:
        inc    eax
        je     loc_5_0100
        dec    eax
        test   dword ptr [0xba], eax         ; -> loc_1_00BA
        je     loc_5_00FA
        call   sub_5_0111
        ret    
loc_5_00FA:
        jmp    loc_4_0030
loc_5_0100:
        push   ebx
        not    ebx
        and    dword ptr [0xba], ebx         ; -> loc_1_00BA
        pop    ebx
        ret    
loc_5_010B:
        mov    eax, 1
        ret    
sub_5_0111:
        cmp    eax, 0x20
        je     loc_5_61E2
        cmp    eax, 1
        je     loc_5_447E
        cmp    eax, 2
        je     loc_5_517C
        cmp    eax, 0x40
        je     loc_5_4A3A
        cmp    eax, 0x80
        je     loc_5_61B4
        cmp    eax, 9
        je     loc_5_447E
        cmp    eax, 0x48
        je     loc_5_4A3A
        cmp    eax, 0x68
        je     loc_5_4A3A
        cmp    eax, 0x200
        je     loc_5_4A3A
        mov    eax, 1
        ret    
sub_5_016C:
        cmp    dword ptr [0x90], 0           ; -> obj1:0x0090
        jne    loc_5_0195
        mov    ecx, dword ptr [0x94]         ; -> obj1:0x0094
        jecxz  loc_5_0195
        mov    eax, dword ptr [0x98]         ; -> obj1:0x0098
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        cmp    eax, 1
        jb     loc_5_019B
        clc    
loc_5_0195:
        inc    dword ptr [0x90]              ; -> obj1:0x0090
loc_5_019B:
        ret    
sub_5_019C:
        cmp    dword ptr [0x90], 1           ; -> obj1:0x0090
        jb     loc_5_01C7
        ja     loc_5_01C1
        mov    ecx, dword ptr [0x94]         ; -> obj1:0x0094
        jecxz  loc_5_01C1
        mov    eax, dword ptr [0x98]         ; -> obj1:0x0098
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
loc_5_01C1:
        dec    dword ptr [0x90]              ; -> obj1:0x0090
loc_5_01C7:
        ret    
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
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_5_447E:
        push   ebp
        mov    ebx, 0x4537                   ; -> loc_5_4537
        mov    edi, 0x4730                   ; -> loc_5_4730
        call   sub_5_4B33
        jb     loc_5_44EB
        push   esi
        mov    esi, dword ptr [esi]
        push   eax
        push   edi
        call   sub_5_44EF
        pop    edi
        or     edx, edx
        je     loc_5_44B5
loc_5_449F:
        mov    dword ptr [ebp + 4], 0x200
loc_5_44A6:
        cmp    edi, dword ptr [esp]
        jae    loc_5_44C5
        push   ebx
        push   edx
        call   dword ptr [ebp + 0x34]
        pop    edx
        pop    ebx
        dec    edx
        jne    loc_5_449F
loc_5_44B5:
        and    ebx, 0x1ff
        je     loc_5_44CE
        mov    dword ptr [ebp + 4], ebx
        sub    ebx, ebx
        inc    edx
        jmp    loc_5_44A6
loc_5_44C5:
        mov    edx, 4
        pop    eax
        pop    esi
        jmp    loc_5_44EB
loc_5_44CE:
        mov    edx, 4
        call   sub_5_4A13
        pop    eax
        pop    esi
        sub    edi, dword ptr [esi + 8]
        cmp    di, word ptr [esi + 4]
        jae    loc_5_44EB
        sub    ecx, ecx
        sub    edx, edx
        mov    word ptr [esi + 0xc], di
loc_5_44EB:
        pop    ebp
        mov    eax, edx
        ret    
sub_5_44EF:
        cld    
        mov    dword ptr [edi], 0x5344
        add    edi, 4
        mov    dword ptr [ebp + 8], edi
        mov    dword ptr [ebp], esi
        mov    ecx, 0x1000
        lea    edi, [ebp + 0x38]
        cmp    dword ptr [ebp + 0x34], 0x4730 ; -> loc_5_4730
        je     loc_5_4520
        lea    eax, [esi + 0x8000]
        rep stosd dword ptr es:[edi], eax
        xor    eax, eax
        mov    ecx, 0x20
        ret    
loc_5_4520:
        or     eax, 0xffffffff
        shr    ecx, 1
        rep stosd dword ptr es:[edi], eax
        inc    eax
        mov    ecx, 0x20
        mov    edi, dword ptr [ebp + 8]
        mov    word ptr [edi - 2], 0x100
        ret    
loc_5_4537:
        cmp    dword ptr [ebp + 4], 2
        jb     loc_5_46D2
        mov    dx, word ptr [esi]
        shld   ebx, edx, 0x1c
        xor    ebx, edx
        and    ebx, 0xfff
        mov    edi, esi
        xchg   dword ptr [ebp + ebx*4 + 0x38], edi
        mov    ebx, esi
        sub    ebx, edi
        cmp    ebx, 0x113f
        jb     loc_5_4594
        movzx  ebx, dl
loc_5_4565:
        dec    dword ptr [ebp + 4]
        inc    esi
        rol    bl, 1
        adc    ebx, ebx
        xor    bl, 2
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 9
        jae    loc_5_4537
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
        jmp    loc_5_4537
loc_5_4594:
        mov    edx, esi
        xchg   dword ptr [ebp + 4], ecx
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        xchg   dword ptr [ebp + 4], ecx
        je     loc_5_45A4
        dec    esi
        inc    dword ptr [ebp + 4]
loc_5_45A4:
        sub    esi, edx
        cmp    esi, 2
        jb     loc_5_46C5
        cmp    ebx, 0x40
        jae    loc_5_45DA
        shl    ebx, 2
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 8
        jae    loc_5_460B
loc_5_45C0:
        add    cl, 0x20
        sbb    edi, edi
        shr    edi, cl
        and    edi, ebx
        xor    eax, edi
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edi
        jmp    loc_5_460B
loc_5_45DA:
        cmp    ebx, 0x13f
        ja     loc_5_45F6
        sub    ebx, 0x40
        shl    ebx, 3
        or     ebx, 3
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xb
        jae    loc_5_460B
        jmp    loc_5_45C0
loc_5_45F6:
        sub    ebx, 0x140
        shl    ebx, 3
        or     ebx, 7
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xf
        jb     loc_5_45C0
loc_5_460B:
        lea    ebx, [esi - 2]
        add    esi, edx
        cmp    ebx, 0x1e
        ja     loc_5_464C
        mov    ch, byte ptr [ebx + 0xc0]     ; -> loc_1_00C0
        movzx  ebx, word ptr [ebx + ebx + 0xe0] ; -> loc_1_00E0
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, ch
        jae    loc_5_4537
loc_5_462F:
        add    cl, 0x20
        sbb    edi, edi
        shr    edi, cl
        and    edi, ebx
        xor    eax, edi
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edi
        jmp    loc_5_4537
loc_5_464C:
        inc    ebx
        cmp    ebx, 0x40
        jae    loc_5_466A
        shl    ebx, 6
        xor    ebx, 0x820
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xb
        jae    loc_5_4537
        jmp    loc_5_462F
loc_5_466A:
        cmp    ebx, 0x80
        jae    loc_5_468A
        shl    ebx, 7
        xor    ebx, 0x2040
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xd
        jae    loc_5_4537
        jmp    loc_5_462F
loc_5_468A:
        cmp    ebx, 0x100
        jae    loc_5_46AA
        shl    ebx, 8
        xor    ebx, 0x8080
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xf
        jae    loc_5_4537
        jmp    loc_5_462F
loc_5_46AA:
        shl    ebx, 9
        xor    ebx, 0x20100
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0x11
        jae    loc_5_4537
        jmp    loc_5_462F
loc_5_46C5:
        add    dword ptr [ebp + 4], esi
        mov    esi, edx
        movzx  ebx, byte ptr [edx]
        jmp    loc_5_4565
loc_5_46D2:
        dec    dword ptr [ebp + 4]
        jl     loc_5_4706
        movzx  ebx, byte ptr [esi]
        inc    esi
        rol    bl, 1
        adc    ebx, ebx
        xor    bl, 2
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 9
        jae    loc_5_46D2
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
        jmp    loc_5_46D2
loc_5_4706:
        mov    ebx, 0x7fff
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xf
        jae    loc_5_472C
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
loc_5_472C:
        mov    edi, dword ptr [ebp + 8]
        ret    
loc_5_4730:
        push   eax
        push   ecx
        sub    esp, 0xc
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp + 0xc], ecx
        mov    edx, dword ptr [ebp + 4]
        lea    edx, [esi + edx - 1]
        mov    dword ptr [esp], edx
        and    dword ptr [esp + 8], 0
        mov    edi, esi
        sub    edi, dword ptr [ebp]
loc_5_4751:
        cmp    dword ptr [esp], esi
        jbe    loc_5_49AC
        mov    ax, word ptr [esi]
        and    eax, 0xfff
        mov    ebx, edi
        xchg   word ptr [ebp + eax*2 + 0x38], bx
        mov    eax, edi
        xor    edx, edx
        div    word ptr [0x120]              ; -> loc_1_0120
        mov    word ptr [ebp + edx*2 + 0x2038], bx
loc_5_477C:
        mov    eax, edi
        sub    eax, ebx
        jbe    loc_5_47ED
        cmp    eax, 0x113e
        ja     loc_5_47ED
        mov    edx, esi
        sub    edx, eax
        mov    ecx, dword ptr [esp + 8]
        mov    al, byte ptr [ecx + edx]
        cmp    byte ptr [ecx + esi], al
        jne    loc_5_47D3
        mov    ax, word ptr [edx]
        cmp    word ptr [esi], ax
        jne    loc_5_47D3
        add    edx, 2
        add    esi, 2
loc_5_47A7:
        cmp    dword ptr [esp], esi
        jb     loc_5_47B6
        mov    al, byte ptr [edx]
        inc    edx
        cmp    byte ptr [esi], al
        jne    loc_5_47B6
        inc    esi
        jmp    loc_5_47A7
loc_5_47B6:
        mov    eax, esi
        sub    eax, dword ptr [ebp]
        sub    eax, edi
        cmp    ecx, eax
        jae    loc_5_47CE
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 4], ebx
        cmp    dword ptr [esp], esi
        jb     loc_5_47E8
loc_5_47CE:
        mov    esi, dword ptr [ebp]
        add    esi, edi
loc_5_47D3:
        mov    eax, ebx
        xor    edx, edx
        div    word ptr [0x120]              ; -> loc_1_0120
        mov    bx, word ptr [ebp + edx*2 + 0x2038]
        jmp    loc_5_477C
loc_5_47E8:
        mov    esi, dword ptr [ebp]
        add    esi, edi
loc_5_47ED:
        cmp    dword ptr [esp + 8], 0
        je     loc_5_4967
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, edi
        mov    ecx, dword ptr [esp + 0xc]
        sub    ebx, dword ptr [esp + 4]
        cmp    ebx, 0x40
        jae    loc_5_4831
        shl    ebx, 2
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 8
        jae    loc_5_4862
loc_5_4817:
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
        jmp    loc_5_4862
loc_5_4831:
        cmp    ebx, 0x13f
        ja     loc_5_484D
        sub    ebx, 0x40
        shl    ebx, 3
        or     ebx, 3
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xb
        jae    loc_5_4862
        jmp    loc_5_4817
loc_5_484D:
        sub    ebx, 0x140
        shl    ebx, 3
        or     ebx, 7
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xf
        jb     loc_5_4817
loc_5_4862:
        mov    ebx, dword ptr [esp + 8]
        sub    ebx, 2
        cmp    ebx, 0x1e
        ja     loc_5_489E
        mov    ch, byte ptr [ebx + 0xc0]     ; -> loc_1_00C0
        movzx  ebx, word ptr [ebx + ebx + 0xe0] ; -> loc_1_00E0
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, ch
        jae    loc_5_4902
loc_5_4884:
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
        jmp    loc_5_4902
loc_5_489E:
        inc    ebx
        cmp    ebx, 0x40
        jae    loc_5_48B8
        shl    ebx, 6
        xor    ebx, 0x820
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xb
        jae    loc_5_4902
        jmp    loc_5_4884
loc_5_48B8:
        cmp    ebx, 0x80
        jae    loc_5_48D4
        shl    ebx, 7
        xor    ebx, 0x2040
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xd
        jae    loc_5_4902
        jmp    loc_5_4884
loc_5_48D4:
        cmp    ebx, 0x100
        jae    loc_5_48F0
        shl    ebx, 8
        xor    ebx, 0x8080
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xf
        jae    loc_5_4902
        jmp    loc_5_4884
loc_5_48F0:
        shl    ebx, 9
        xor    ebx, 0x20100
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0x11
        jb     loc_5_4884
loc_5_4902:
        mov    dword ptr [esp + 0xc], ecx
        mov    dword ptr [esp + 0x10], eax
        mov    esi, dword ptr [ebp]
        mov    ecx, dword ptr [esp + 8]
        lea    ebx, [edi + 1]
        add    edi, ecx
        mov    eax, esi
        add    eax, edi
        cmp    dword ptr [esp], eax
        jbe    loc_5_49AA
        cmp    ecx, 0x3f
        jbe    loc_5_4931
        lea    ebx, [ecx + ebx - 0x40]
        mov    ecx, 0x40
loc_5_4931:
        dec    ecx
        je     loc_5_495A
        mov    ax, word ptr [ebx + esi]
        and    eax, 0xfff
        mov    edi, ebx
        xchg   word ptr [ebp + eax*2 + 0x38], di
        mov    eax, ebx
        xor    edx, edx
        div    word ptr [0x120]              ; -> loc_1_0120
        mov    word ptr [ebp + edx*2 + 0x2038], di
        inc    ebx
        jmp    loc_5_4931
loc_5_495A:
        mov    edi, ebx
        mov    dword ptr [esp + 8], ecx
        add    esi, edi
        jmp    loc_5_4751
loc_5_4967:
        movzx  ebx, byte ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        inc    esi
        mov    ecx, dword ptr [esp + 0xc]
        inc    edi
        rol    bl, 1
        adc    ebx, ebx
        xor    bl, 2
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 9
        jae    loc_5_499D
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
loc_5_499D:
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp + 0xc], ecx
        jmp    loc_5_4751
loc_5_49AA:
        add    esi, edi
loc_5_49AC:
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0xc]
        cmp    dword ptr [esp], esi
        jb     loc_5_49E6
        movzx  ebx, byte ptr [esi]
        inc    esi
        rol    bl, 1
        adc    ebx, ebx
        xor    bl, 2
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 9
        jae    loc_5_49E6
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
loc_5_49E6:
        mov    ebx, 0x7fff
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xf
        jae    loc_5_4A0C
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
loc_5_4A0C:
        mov    edi, dword ptr [ebp + 8]
        add    esp, 0x14
        ret    
sub_5_4A13:
        mov    edi, dword ptr [ebp + 8]
        cmp    cl, 0x20
        je     loc_5_4A39
        mov    byte ptr [edi], al
        inc    edi
        cmp    cl, 0x18
        jae    loc_5_4A39
        mov    byte ptr [edi], ah
        inc    edi
        cmp    cl, 0x10
        jae    loc_5_4A39
        shr    eax, 0x10
        mov    byte ptr [edi], al
        inc    edi
        cmp    cl, 8
        jae    loc_5_4A39
        mov    byte ptr [edi], ah
        inc    edi
loc_5_4A39:
        ret    
loc_5_4A3A:
        push   ebp
        mov    ebx, 0x4c11                   ; -> loc_5_4C11
        mov    edi, 0x4e3e                   ; -> loc_5_4E3E
        call   sub_5_4B33
        jb     loc_5_4A76
        mov    dword ptr [ebp + 0x10], esi
        mov    esi, dword ptr [esi]
        mov    dword ptr [ebp + 0xc], eax
        push   edi
        call   sub_5_4BC9
        pop    edi
        mov    dword ptr [ebp + 0x24], ebx
        mov    dword ptr [ebp + 0x18], edx
        mov    dword ptr [ebp + 0x28], eax
        mov    dword ptr [ebp + 0x2c], ecx
        mov    dword ptr [ebp + 0x14], esi
        call   sub_5_4AA7
        cmp    eax, 9
        je     loc_5_4A7A
loc_5_4A74:
        pop    ebp
        ret    
loc_5_4A76:
        mov    eax, edx
        jmp    loc_5_4A74
loc_5_4A7A:
        mov    ebx, ebp
        mov    ecx, dword ptr [ebp + 0x30]
        mov    edx, 0x4a86                   ; -> loc_5_4A86
        jmp    loc_5_4A74
loc_5_4A86:
        mov    eax, 8
        cmp    dword ptr [ebx + 0x30], ecx
        jne    loc_5_4AA6
        mov    dword ptr [ebx + 0x10], esi
        push   ebp
        mov    ebp, ebx
        call   sub_5_4AA7
        mov    ecx, dword ptr [ebp + 0x30]
        mov    edx, 0x4a86                   ; -> loc_5_4A86
        mov    ebx, ebp
        pop    ebp
loc_5_4AA6:
        ret    
sub_5_4AA7:
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x28]
        mov    ecx, dword ptr [ebp + 0x2c]
        cmp    dword ptr [ebp + 0x18], 0
        je     loc_5_4AFB
        mov    edx, eax
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [ebp + 0x1c], eax
        mov    eax, edx
loc_5_4AC6:
        mov    dword ptr [ebp + 4], 0x200
        cmp    edi, dword ptr [ebp + 0xc]
        jae    loc_5_4B2C
        call   dword ptr [ebp + 0x34]
        dec    dword ptr [ebp + 0x18]
        je     loc_5_4AFB
        mov    edx, eax
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        xchg   edx, eax
        sub    edx, dword ptr [ebp + 0x1c]
        cmp    edx, dword ptr [ebp + 0x20]
        jbe    loc_5_4AC6
        mov    dword ptr [ebp + 0x14], esi
        mov    dword ptr [ebp + 0x28], eax
        mov    dword ptr [ebp + 0x2c], ecx
        mov    eax, 9
        jmp    loc_5_4B28
loc_5_4AFB:
        mov    edx, dword ptr [ebp + 0x24]
        and    edx, 0x1ff
        je     loc_5_4B11
        cmp    edi, dword ptr [ebp + 0xc]
        jae    loc_5_4B2C
        mov    dword ptr [ebp + 4], edx
        call   dword ptr [ebp + 0x34]
loc_5_4B11:
        call   sub_5_5153
        mov    esi, dword ptr [ebp + 0x10]
        sub    edi, dword ptr [esi + 8]
        cmp    di, word ptr [esi + 4]
        jae    loc_5_4B2C
        mov    word ptr [esi + 0xc], di
        xor    eax, eax
loc_5_4B28:
        mov    esi, dword ptr [ebp + 0x10]
        ret    
loc_5_4B2C:
        mov    eax, 4
        jmp    loc_5_4B28
sub_5_4B33:
        cld    
        mov    ebp, 0x1c8                    ; -> obj5:0x01C8
        mov    ecx, 0x42b6
        test   eax, 0x200
        je     loc_5_4B59
        mov    ebp, dword ptr [esi + 0x10]
        movzx  ecx, word ptr [esi + 6]
        mov    edx, 7
        cmp    ecx, 0x42b6
        jb     loc_5_4BC8
loc_5_4B59:
        add    dword ptr [ebp + 0x30], 1
        adc    dword ptr [ebp + 0x30], 0
        mov    dword ptr [ebp + 0x34], edi
        test   eax, 8
        jne    loc_5_4B6E
        mov    dword ptr [ebp + 0x34], ebx
loc_5_4B6E:
        mov    dword ptr [ebp + 0x20], 0x5f
        test   eax, 0x20
        jne    loc_5_4B80
        or     dword ptr [ebp + 0x20], 0xffffffff
loc_5_4B80:
        movzx  eax, word ptr [esi + 0xc]
        movzx  ebx, word ptr [esi + 4]
        lea    edx, [ebx + ebx*8]
        shr    edx, 3
        add    edx, 4
        cmp    edx, 0x244
        jb     loc_5_4B9E
        mov    edx, 0x244
loc_5_4B9E:
        sub    eax, edx
        mov    edx, 3
        jb     loc_5_4BC8
        mov    edi, dword ptr [esi + 8]
        add    eax, edi
        push   ecx
        mov    ecx, ebx
        add    ecx, edi
        cmp    ecx, eax
        ja     loc_5_4BB7
        mov    eax, ecx
loc_5_4BB7:
        pop    ecx
        cmp    ebx, 1
        mov    edx, 4
        jb     loc_5_4BC8
        mov    edx, ebx
        shr    edx, 9
        clc    
loc_5_4BC8:
        ret    
sub_5_4BC9:
        cld    
        mov    dword ptr [edi], 0x4d4a
        add    edi, 4
        mov    dword ptr [ebp + 8], edi
        mov    dword ptr [ebp], esi
        mov    ecx, 0x1000
        lea    edi, [ebp + 0x38]
        cmp    dword ptr [ebp + 0x34], 0x4e3e ; -> loc_5_4E3E
        je     loc_5_4BFA
        lea    eax, [esi + 0x8000]
        rep stosd dword ptr es:[edi], eax
        xor    eax, eax
        mov    ecx, 0x20
        ret    
loc_5_4BFA:
        or     eax, 0xffffffff
        shr    ecx, 1
        rep stosd dword ptr es:[edi], eax
        inc    eax
        mov    ecx, 0x20
        mov    edi, dword ptr [ebp + 8]
        mov    word ptr [edi - 2], 0x100
        ret    
loc_5_4C11:
        cmp    dword ptr [ebp + 4], 4
        jb     loc_5_4DBD
        mov    edx, dword ptr [esi]
loc_5_4C1D:
        shld   ebx, edx, 0x14
        xor    ebx, edx
        and    ebx, 0xfff
        mov    edi, esi
        xchg   dword ptr [ebp + ebx*4 + 0x38], edi
        mov    ebx, esi
        sub    ebx, edi
        cmp    ebx, 0x113f
        jb     loc_5_4C7C
        movzx  ebx, dl
loc_5_4C3E:
        dec    dword ptr [ebp + 4]
        inc    esi
        cmp    bl, 0x80
        jae    loc_5_4C6C
        add    ebx, ebx
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 8
        jae    loc_5_4C11
loc_5_4C52:
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
        jmp    loc_5_4C11
loc_5_4C6C:
        rol    bl, 1
        adc    ebx, ebx
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 9
        jae    loc_5_4C11
        jmp    loc_5_4C52
loc_5_4C7C:
        mov    edx, esi
        xchg   dword ptr [ebp + 4], ecx
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        xchg   dword ptr [ebp + 4], ecx
        je     loc_5_4C8C
        dec    esi
        inc    dword ptr [ebp + 4]
loc_5_4C8C:
        sub    esi, edx
        cmp    esi, 3
        jb     loc_5_4DB0
        cmp    ebx, 0x3f
        ja     loc_5_4CC5
        shl    ebx, 3
        or     ebx, 1
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 9
        jae    loc_5_4CF6
loc_5_4CAB:
        add    cl, 0x20
        sbb    edi, edi
        shr    edi, cl
        and    edi, ebx
        xor    eax, edi
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edi
        jmp    loc_5_4CF6
loc_5_4CC5:
        cmp    ebx, 0x13f
        ja     loc_5_4CE1
        sub    ebx, 0x40
        shl    ebx, 4
        or     ebx, 5
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xc
        jae    loc_5_4CF6
        jmp    loc_5_4CAB
loc_5_4CE1:
        sub    ebx, 0x140
        shl    ebx, 4
        or     ebx, 0xd
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0x10
        jb     loc_5_4CAB
loc_5_4CF6:
        lea    ebx, [esi - 3]
        add    esi, edx
        cmp    ebx, 0x1e
        ja     loc_5_4D37
        mov    ch, byte ptr [ebx + 0xc0]     ; -> loc_1_00C0
        movzx  ebx, word ptr [ebx + ebx + 0xe0] ; -> loc_1_00E0
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, ch
        jae    loc_5_4C11
loc_5_4D1A:
        add    cl, 0x20
        sbb    edi, edi
        shr    edi, cl
        and    edi, ebx
        xor    eax, edi
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edi
        jmp    loc_5_4C11
loc_5_4D37:
        inc    ebx
        cmp    ebx, 0x40
        jae    loc_5_4D55
        shl    ebx, 6
        xor    ebx, 0x820
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xb
        jae    loc_5_4C11
        jmp    loc_5_4D1A
loc_5_4D55:
        cmp    ebx, 0x80
        jae    loc_5_4D75
        shl    ebx, 7
        xor    ebx, 0x2040
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xd
        jae    loc_5_4C11
        jmp    loc_5_4D1A
loc_5_4D75:
        cmp    ebx, 0x100
        jae    loc_5_4D95
        shl    ebx, 8
        xor    ebx, 0x8080
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xf
        jae    loc_5_4C11
        jmp    loc_5_4D1A
loc_5_4D95:
        shl    ebx, 9
        xor    ebx, 0x20100
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0x11
        jae    loc_5_4C11
        jmp    loc_5_4D1A
loc_5_4DB0:
        add    dword ptr [ebp + 4], esi
        mov    esi, edx
        movzx  ebx, byte ptr [edx]
        jmp    loc_5_4C3E
loc_5_4DBD:
        cmp    dword ptr [ebp + 4], 3
        jb     loc_5_4DD1
        mov    dl, byte ptr [esi + 2]
        shl    edx, 0x10
        mov    dx, word ptr [esi]
        jmp    loc_5_4C1D
loc_5_4DD1:
        dec    dword ptr [ebp + 4]
        jl     loc_5_4E14
        movzx  ebx, byte ptr [esi]
        inc    esi
        cmp    bl, 0x80
        jae    loc_5_4E04
        add    ebx, ebx
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 8
        jae    loc_5_4DD1
loc_5_4DEA:
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
        jmp    loc_5_4DD1
loc_5_4E04:
        rol    bl, 1
        adc    ebx, ebx
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 9
        jae    loc_5_4DD1
        jmp    loc_5_4DEA
loc_5_4E14:
        mov    ebx, 0xfffd
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0x10
        jae    loc_5_4E3A
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
loc_5_4E3A:
        mov    edi, dword ptr [ebp + 8]
        ret    
loc_5_4E3E:
        sub    esp, 0x10
        push   eax
        push   ecx
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp], ecx
        mov    edx, dword ptr [ebp + 4]
        lea    edx, [esi + edx - 1]
        mov    dword ptr [esp + 8], edx
        mov    edi, esi
        sub    edi, dword ptr [ebp]
loc_5_4E5A:
        cmp    dword ptr [esp + 8], esi
        jbe    loc_5_50DD
        and    dword ptr [esp + 0x10], 0
        mov    ax, word ptr [esi]
        and    eax, 0xfff
        mov    ebx, edi
        xchg   word ptr [ebp + eax*2 + 0x38], bx
        mov    eax, edi
        xor    edx, edx
        div    word ptr [0x120]              ; -> loc_1_0120
        mov    word ptr [ebp + edx*2 + 0x2038], bx
        mov    dword ptr [esp + 0x14], 0x80
loc_5_4E93:
        dec    dword ptr [esp + 0x14]
        je     loc_5_4F08
        mov    eax, edi
        sub    eax, ebx
        jbe    loc_5_4F08
        cmp    eax, 0x113e
        ja     loc_5_4F08
        mov    edx, esi
        sub    edx, eax
        mov    ecx, dword ptr [esp + 0x10]
        mov    al, byte ptr [ecx + edx]
        cmp    byte ptr [ecx + esi], al
        jne    loc_5_4EEE
        mov    ax, word ptr [edx]
        cmp    word ptr [esi], ax
        jne    loc_5_4EEE
        add    edx, 2
        inc    esi
loc_5_4EC2:
        inc    esi
        cmp    dword ptr [esp + 8], esi
        jb     loc_5_4ED0
        mov    al, byte ptr [edx]
        inc    edx
        cmp    byte ptr [esi], al
        je     loc_5_4EC2
loc_5_4ED0:
        mov    eax, esi
        sub    eax, dword ptr [ebp]
        sub    eax, edi
        cmp    ecx, eax
        jae    loc_5_4EE9
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp + 0xc], ebx
        cmp    dword ptr [esp + 8], esi
        jb     loc_5_4F03
loc_5_4EE9:
        mov    esi, dword ptr [ebp]
        add    esi, edi
loc_5_4EEE:
        mov    eax, ebx
        xor    edx, edx
        div    word ptr [0x120]              ; -> loc_1_0120
        mov    bx, word ptr [ebp + edx*2 + 0x2038]
        jmp    loc_5_4E93
loc_5_4F03:
        mov    esi, dword ptr [ebp]
        add    esi, edi
loc_5_4F08:
        cmp    dword ptr [esp + 0x10], 2
        jbe    loc_5_508B
        mov    eax, dword ptr [esp + 4]
        mov    ebx, edi
        mov    ecx, dword ptr [esp]
        sub    ebx, dword ptr [esp + 0xc]
        cmp    ebx, 0x3f
        ja     loc_5_4F4E
        shl    ebx, 3
        or     ebx, 1
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 9
        jae    loc_5_4F7F
loc_5_4F34:
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
        jmp    loc_5_4F7F
loc_5_4F4E:
        cmp    ebx, 0x13f
        ja     loc_5_4F6A
        sub    ebx, 0x40
        shl    ebx, 4
        or     ebx, 5
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xc
        jae    loc_5_4F7F
        jmp    loc_5_4F34
loc_5_4F6A:
        sub    ebx, 0x140
        shl    ebx, 4
        or     ebx, 0xd
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0x10
        jb     loc_5_4F34
loc_5_4F7F:
        mov    ebx, dword ptr [esp + 0x10]
        sub    ebx, 3
        cmp    ebx, 0x1e
        ja     loc_5_4FBB
        mov    ch, byte ptr [ebx + 0xc0]     ; -> loc_1_00C0
        movzx  ebx, word ptr [ebx + ebx + 0xe0] ; -> loc_1_00E0
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, ch
        jae    loc_5_501F
loc_5_4FA1:
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
        jmp    loc_5_501F
loc_5_4FBB:
        inc    ebx
        cmp    ebx, 0x40
        jae    loc_5_4FD5
        shl    ebx, 6
        xor    ebx, 0x820
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xb
        jae    loc_5_501F
        jmp    loc_5_4FA1
loc_5_4FD5:
        cmp    ebx, 0x80
        jae    loc_5_4FF1
        shl    ebx, 7
        xor    ebx, 0x2040
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xd
        jae    loc_5_501F
        jmp    loc_5_4FA1
loc_5_4FF1:
        cmp    ebx, 0x100
        jae    loc_5_500D
        shl    ebx, 8
        xor    ebx, 0x8080
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0xf
        jae    loc_5_501F
        jmp    loc_5_4FA1
loc_5_500D:
        shl    ebx, 9
        xor    ebx, 0x20100
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0x11
        jb     loc_5_4FA1
loc_5_501F:
        mov    dword ptr [esp], ecx
        mov    dword ptr [esp + 4], eax
        mov    esi, dword ptr [ebp]
        mov    ecx, dword ptr [esp + 0x10]
        lea    ebx, [edi + 1]
        add    edi, ecx
        mov    eax, esi
        add    eax, edi
        cmp    dword ptr [esp + 8], eax
        jbe    loc_5_50DB
        cmp    ecx, 0x3f
        jbe    loc_5_504E
        lea    ebx, [ecx + ebx - 0x40]
        mov    ecx, 0x40
loc_5_504E:
        mov    eax, ebx
        xor    edx, edx
        div    word ptr [0x120]              ; -> loc_1_0120
loc_5_5059:
        dec    ecx
        je     loc_5_5082
        mov    ax, word ptr [ebx + esi]
        and    eax, 0xfff
        mov    edi, ebx
        xchg   word ptr [ebp + eax*2 + 0x38], di
        inc    ebx
        mov    word ptr [ebp + edx*2 + 0x2038], di
        inc    edx
        cmp    edx, 0x113f
        jb     loc_5_5059
        xor    edx, edx
        jmp    loc_5_5059
loc_5_5082:
        mov    edi, ebx
        add    esi, edi
        jmp    loc_5_4E5A
loc_5_508B:
        movzx  ebx, byte ptr [esi]
        mov    eax, dword ptr [esp + 4]
        inc    esi
        mov    ecx, dword ptr [esp]
        inc    edi
        cmp    bl, 0x80
        jae    loc_5_50C1
        add    ebx, ebx
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 8
        jae    loc_5_50CF
loc_5_50A7:
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
        jmp    loc_5_50CF
loc_5_50C1:
        rol    bl, 1
        adc    ebx, ebx
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 9
        jb     loc_5_50A7
loc_5_50CF:
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp], ecx
        jmp    loc_5_4E5A
loc_5_50DB:
        add    esi, edi
loc_5_50DD:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp]
        cmp    dword ptr [esp + 8], esi
        jb     loc_5_5126
        movzx  ebx, byte ptr [esi]
        inc    esi
        cmp    bl, 0x80
        jae    loc_5_5118
        add    ebx, ebx
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 8
        jae    loc_5_5126
loc_5_50FE:
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
        jmp    loc_5_5126
loc_5_5118:
        rol    bl, 1
        adc    ebx, ebx
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 9
        jb     loc_5_50FE
loc_5_5126:
        mov    ebx, 0xfffd
        ror    ebx, cl
        xor    eax, ebx
        sub    cl, 0x10
        jae    loc_5_514C
        add    cl, 0x20
        sbb    edx, edx
        shr    edx, cl
        and    edx, ebx
        xor    eax, edx
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], eax
        add    ebx, 4
        mov    dword ptr [ebp + 8], ebx
        mov    eax, edx
loc_5_514C:
        mov    edi, dword ptr [ebp + 8]
        add    esp, 0x18
        ret    
sub_5_5153:
        mov    edi, dword ptr [ebp + 8]
        cmp    cl, 0x20
        je     loc_5_5179
        mov    byte ptr [edi], al
        inc    edi
        cmp    cl, 0x18
        jae    loc_5_5179
        mov    byte ptr [edi], ah
        inc    edi
        cmp    cl, 0x10
        jae    loc_5_5179
        shr    eax, 0x10
        mov    byte ptr [edi], al
        inc    edi
        cmp    cl, 8
        jae    loc_5_5179
        mov    byte ptr [edi], ah
        inc    edi
loc_5_5179:
        ret    
        db  000h, 000h
loc_5_517C:
        push   esi
        call   sub_5_51FC
        sub    ecx, ecx
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        cmp    ax, 0x5344
        jne    loc_5_51A3
        call   sub_5_5260
        jb     loc_5_51A3
        pop    esi
        sub    edi, dword ptr [esi + 8]
        mov    word ptr [esi + 0xc], di
        sub    ecx, ecx
        sub    eax, eax
        ret    
loc_5_51A3:
        pop    esi
        mov    eax, 5
        ret    
loc_5_51AA:
        cmp    dword ptr [esi + 0x10], 0
        jne    loc_5_51BE
        mov    ebx, dword ptr [esi]
        cmp    word ptr [ebx], 0x5344
        je     loc_5_51C8
        jmp    loc_5_76C4
loc_5_51BE:
        test   byte ptr [esi + 0x11], 0x40
        jne    loc_5_76C4
loc_5_51C8:
        push   esi
        call   sub_5_51FC
        jne    loc_5_51DB
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        cmp    ax, 0x5344
        jne    loc_5_51A3
loc_5_51DB:
        call   sub_5_5260
        jb     loc_5_51A3
        pop    esi
        mov    dword ptr [esi + 0x10], eax
        mov    eax, ebx
        xchg   dword ptr [esi], eax
        sub    ebx, eax
        mov    ecx, ebx
        mov    eax, edi
        xchg   dword ptr [esi + 8], eax
        sub    edi, eax
        mov    word ptr [esi + 0xc], di
        sub    eax, eax
        ret    
sub_5_51FC:
        cld    
        movzx  edx, word ptr [esi + 0xc]
        mov    ecx, edx
        shr    edx, 9
        and    ecx, 0x1ff
        je     loc_5_5211
        inc    edx
        sub    ecx, ecx
loc_5_5211:
        or     ecx, dword ptr [esi + 0x10]
        mov    edi, dword ptr [esi + 8]
        mov    ebx, dword ptr [esi]
        ret    
        db  08Bh, 0C0h
loc_5_521C:
        mov    byte ptr [0x87000052], al     ; -> loc_5_5487
        push   esp
        add    byte ptr [eax], al
        push   esi
        add    byte ptr [eax], al
        and    byte ptr [eax], bl
        add    cl, bl                        ; -> loc_5_59D9
        pop    ecx
        add    byte ptr [eax], al
        retf   
        db  05Bh, 000h, 000h
        dd  loc_5_5D84               ; fixup
        dd  loc_5_5F6D               ; fixup
        dd  loc_5_546D               ; fixup
        dd  loc_5_52BB               ; fixup
        dd  loc_5_5806               ; fixup
        dd  loc_5_564A               ; fixup
        dd  loc_5_5BB1               ; fixup
        dd  loc_5_59ED               ; fixup
        dd  loc_5_5F53               ; fixup
        dd  loc_5_5D98               ; fixup
        dd  loc_5_52A2               ; fixup
sub_5_5260:
        push   ebp
        push   edx
        mov    ebp, 0x200
        jecxz  loc_5_52A2
        mov    eax, dword ptr [ebx]
        shld   eax, ecx, 0x10
        and    ecx, 0x3c
        jmp    loc_5_521C
loc_5_5278:
        test   ebp, ebp
        jne    loc_5_5292
        dec    dword ptr [esp]
        je     loc_5_5297
        mov    ebp, 0x200
        and    ecx, 0x3c
        jmp    loc_5_521C
loc_5_528F:
        stc    
        jmp    loc_5_529F
loc_5_5292:
        dec    dword ptr [esp]
        jne    loc_5_528F
loc_5_5297:
        shl    eax, 0x10
        mov    al, cl
        or     ah, 1
loc_5_529F:
        pop    ebp
        pop    ebp
        ret    
loc_5_52A2:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 3
        jp     loc_5_52DA
        shld   edx, eax, 0x1f
        ror    dl, 1
        xor    dl, 0x80
        dec    ebp
loc_5_52B6:
        js     loc_5_528F
        mov    byte ptr [edi], dl
        inc    edi
loc_5_52BB:
        test   ah, 6
        jp     loc_5_5302
        shld   edx, eax, 0x16
        ror    dl, 1
        xor    dl, 0x80
        dec    ebp
loc_5_52CB:
        js     loc_5_52B6
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_5636
loc_5_52DA:
        test   al, 2
        jne    loc_5_5332
        shld   edx, eax, 0x1e
        and    edx, 0x3f
loc_5_52E5:
        test   ah, 1
        je     loc_5_5348
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_52F1:
        js     loc_5_52CB
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_52BB
loc_5_5302:
        test   ah, 4
        jne    loc_5_5366
        shld   edx, eax, 0x15
        and    edx, 0x3f
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_5313:
        test   al, 2
        je     loc_5_5382
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_531E:
        js     loc_5_52F1
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_5636
loc_5_5332:
        test   al, 4
        jne    loc_5_53A3
        shld   edx, eax, 0x1d
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_5871
loc_5_5348:
        test   ah, 2
        je     loc_5_53C6
        shld   ecx, eax, 0x16
        and    ecx, 1
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_535D:
        js     loc_5_531E
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_564A
loc_5_5366:
        test   ah, 8
        jne    loc_5_53E4
        shld   edx, eax, 0x14
        and    edx, 0xff
        add    edx, 0x40
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_5BF5
loc_5_5382:
        test   al, 4
        je     loc_5_540C
        shld   ecx, eax, 0x1d
        and    ecx, 1
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_539A:
        js     loc_5_535D
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_59D9
loc_5_53A3:
        shld   edx, eax, 0x1d
        and    edx, 0xfff
        add    edx, 0x140
        cmp    edx, 0x113f
        jne    loc_5_5FBF
        mov    cl, 0x3c
        jmp    loc_5_5278
loc_5_53C6:
        test   ah, 4
        je     loc_5_5429
        shld   ecx, eax, 0x15
        and    ecx, 3
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_53DB:
        js     loc_5_539A
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_59ED
loc_5_53E4:
        shld   edx, eax, 0x14
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_52E5
        mov    cl, 0x20
        jmp    loc_5_5278
loc_5_540C:
        test   al, 8
        je     loc_5_544D
        shld   ecx, eax, 0x1c
        and    ecx, 3
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5420:
        js     loc_5_53DB
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5D84
loc_5_5429:
        test   ah, 8
        mov    cl, 0xd
        je     loc_5_611C
        shld   ecx, eax, 0x14
        and    ecx, 7
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5444:
        js     loc_5_5420
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5D98
loc_5_544D:
        test   al, 0x10
        mov    cl, 6
        je     loc_5_611C
        shld   ecx, eax, 0x1b
        and    ecx, 7
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5467:
        js     loc_5_5444
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_546D
loc_5_546D:
        test   ah, 3
        jp     loc_5_54A0
        shld   edx, eax, 0x17
        ror    dl, 1
        xor    dl, 0x80
        dec    ebp
loc_5_547D:
        js     loc_5_5467
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_5487:
        test   al, 6
        jp     loc_5_54CD
        shld   edx, eax, 0x1e
        ror    dl, 1
        xor    dl, 0x80
        dec    ebp
loc_5_5496:
        js     loc_5_547D
        mov    byte ptr [edi], dl
        inc    edi
        jmp    loc_5_5806
loc_5_54A0:
        test   ah, 2
        jne    loc_5_54F8
        shld   edx, eax, 0x16
        and    edx, 0x3f
loc_5_54AC:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 1
        je     loc_5_5514
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_54BC:
        js     loc_5_5496
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_5487
loc_5_54CD:
        test   al, 4
        jne    loc_5_5531
        shld   edx, eax, 0x1d
        and    edx, 0x3f
loc_5_54D8:
        test   ah, 2
        je     loc_5_5547
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_54E4:
        js     loc_5_54BC
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_5806
loc_5_54F8:
        test   ah, 4
        jne    loc_5_5569
        shld   edx, eax, 0x15
        and    edx, 0xff
        add    edx, 0x40
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_56A2
loc_5_5514:
        test   al, 2
        je     loc_5_5591
        shld   ecx, eax, 0x1e
        and    ecx, 1
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5528:
        js     loc_5_54E4
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5820
loc_5_5531:
        test   al, 8
        jne    loc_5_55AE
        shld   edx, eax, 0x1c
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_5A17
loc_5_5547:
        test   ah, 4
        je     loc_5_55D1
        shld   ecx, eax, 0x15
        and    ecx, 1
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5560:
        js     loc_5_5528
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5BB1
loc_5_5569:
        shld   edx, eax, 0x15
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_5DEE
        mov    cl, 0x1c
        jmp    loc_5_5278
loc_5_5591:
        test   al, 4
        je     loc_5_55EF
        shld   ecx, eax, 0x1d
        and    ecx, 3
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_55A5:
        js     loc_5_5560
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5BCB
loc_5_55AE:
        shld   edx, eax, 0x1c
        and    edx, 0xfff
        add    edx, 0x140
        cmp    edx, 0x113f
        jne    loc_5_54AC
        mov    cl, 0
        jmp    loc_5_5278
loc_5_55D1:
        test   ah, 8
        je     loc_5_5612
        shld   ecx, eax, 0x14
        and    ecx, 3
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_55E6:
        js     loc_5_55A5
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5F53
loc_5_55EF:
        test   al, 8
        mov    cl, 5
        je     loc_5_611C
        shld   ecx, eax, 0x1c
        and    ecx, 7
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5609:
        js     loc_5_55E6
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5F6D
loc_5_5612:
        test   ah, 0x10
        mov    cl, 0xe
        je     loc_5_611C
        shld   ecx, eax, 0x13
        and    ecx, 7
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_562D:
        js     loc_5_5609
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_52A2
loc_5_5636:
        test   al, 0xc
        jp     loc_5_5669
        shld   edx, eax, 0x1d
        ror    dl, 1
        xor    dl, 0x80
        dec    ebp
loc_5_5645:
        js     loc_5_562D
        mov    byte ptr [edi], dl
        inc    edi
loc_5_564A:
        test   ah, 0x18
        jp     loc_5_5691
        shld   edx, eax, 0x14
        ror    dl, 1
        xor    dl, 0x80
        dec    ebp
loc_5_565A:
        js     loc_5_5645
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_59D9
loc_5_5669:
        test   al, 8
        jne    loc_5_56C1
        shld   edx, eax, 0x1c
        and    edx, 0x3f
loc_5_5674:
        test   ah, 4
        je     loc_5_56D7
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_5680:
        js     loc_5_565A
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_564A
loc_5_5691:
        test   ah, 0x10
        jne    loc_5_56F5
        shld   edx, eax, 0x13
        and    edx, 0x3f
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_56A2:
        test   al, 8
        je     loc_5_5711
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_56AD:
        js     loc_5_5680
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_59D9
loc_5_56C1:
        test   al, 0x10
        jne    loc_5_5732
        shld   edx, eax, 0x1b
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_5C1C
loc_5_56D7:
        test   ah, 8
        je     loc_5_575A
        shld   ecx, eax, 0x14
        and    ecx, 1
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_56EC:
        js     loc_5_56AD
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_59ED
loc_5_56F5:
        test   ah, 0x20
        jne    loc_5_5778
        shld   edx, eax, 0x12
        and    edx, 0xff
        add    edx, 0x40
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_5F97
loc_5_5711:
        test   al, 0x10
        je     loc_5_57A0
        shld   ecx, eax, 0x1b
        and    ecx, 1
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5729:
        js     loc_5_56EC
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5D84
loc_5_5732:
        shld   edx, eax, 0x1b
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_5313
        mov    cl, 4
        jmp    loc_5_5278
loc_5_575A:
        test   ah, 0x10
        je     loc_5_57BD
        shld   ecx, eax, 0x13
        and    ecx, 3
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_576F:
        js     loc_5_5729
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5D98
loc_5_5778:
        shld   edx, eax, 0x12
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_5674
        mov    cl, 0x28
        jmp    loc_5_5278
loc_5_57A0:
        test   al, 0x20
        je     loc_5_57E6
        shld   ecx, eax, 0x1a
        and    ecx, 3
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_57B4:
        js     loc_5_576F
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_546D
loc_5_57BD:
        test   ah, 0x20
        mov    cl, 0xf
        je     loc_5_611C
        shld   ecx, eax, 0x12
        and    ecx, 7
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_57DD:
        js     loc_5_57B4
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5487
loc_5_57E6:
        test   al, 0x40
        mov    cl, 8
        je     loc_5_611C
        shld   ecx, eax, 0x19
        and    ecx, 7
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5800:
        js     loc_5_57DD
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5806
loc_5_5806:
        test   ah, 0xc
        jp     loc_5_5839
        shld   edx, eax, 0x15
        ror    dl, 1
        xor    dl, 0x80
        dec    ebp
loc_5_5816:
        js     loc_5_5800
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_5820:
        test   al, 0x18
        jp     loc_5_5866
        shld   edx, eax, 0x1c
        ror    dl, 1
        xor    dl, 0x80
        dec    ebp
loc_5_582F:
        js     loc_5_5816
        mov    byte ptr [edi], dl
        inc    edi
        jmp    loc_5_5BB1
loc_5_5839:
        test   ah, 8
        jne    loc_5_5891
        shld   edx, eax, 0x14
        and    edx, 0x3f
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_584A:
        test   al, 4
        je     loc_5_58AD
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_5855:
        js     loc_5_582F
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_5820
loc_5_5866:
        test   al, 0x10
        jne    loc_5_58CA
        shld   edx, eax, 0x1b
        and    edx, 0x3f
loc_5_5871:
        test   ah, 8
        je     loc_5_58E0
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_587D:
        js     loc_5_5855
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_5BB1
loc_5_5891:
        test   ah, 0x10
        jne    loc_5_5902
        shld   edx, eax, 0x13
        and    edx, 0xff
        add    edx, 0x40
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_5A45
loc_5_58AD:
        test   al, 8
        je     loc_5_592A
        shld   ecx, eax, 0x1c
        and    ecx, 1
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_58C1:
        js     loc_5_587D
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5BCB
loc_5_58CA:
        test   al, 0x20
        jne    loc_5_5947
        shld   edx, eax, 0x1a
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_5DC1
loc_5_58E0:
        test   ah, 0x10
        je     loc_5_596F
        shld   ecx, eax, 0x13
        and    ecx, 1
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_58F9:
        js     loc_5_58C1
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5F53
loc_5_5902:
        shld   edx, eax, 0x13
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_54D8
        mov    cl, 0x24
        jmp    loc_5_5278
loc_5_592A:
        test   al, 0x10
        je     loc_5_598D
        shld   ecx, eax, 0x1b
        and    ecx, 3
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_593E:
        js     loc_5_58F9
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5F6D
loc_5_5947:
        shld   edx, eax, 0x1a
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_584A
        mov    cl, 8
        jmp    loc_5_5278
loc_5_596F:
        test   ah, 0x20
        je     loc_5_59B0
        shld   ecx, eax, 0x12
        and    ecx, 3
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5984:
        js     loc_5_593E
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_52A2
loc_5_598D:
        test   al, 0x20
        mov    cl, 7
        je     loc_5_611C
        shld   ecx, eax, 0x1a
        and    ecx, 7
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_59A7:
        js     loc_5_5984
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_52BB
loc_5_59B0:
        test   ah, 0x40
        mov    cl, 0x10
        je     loc_5_611C
        shld   ecx, eax, 0x11
        and    ecx, 7
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_59D0:
        js     loc_5_59A7
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5636
loc_5_59D9:
        test   al, 0x30
        jp     loc_5_5A0C
        shld   edx, eax, 0x1b
        ror    dl, 1
        xor    dl, 0x80
        dec    ebp
loc_5_59E8:
        js     loc_5_59D0
        mov    byte ptr [edi], dl
        inc    edi
loc_5_59ED:
        test   ah, 0x60
        jp     loc_5_5A34
        shld   edx, eax, 0x12
        ror    dl, 1
        xor    dl, 0x80
        dec    ebp
loc_5_59FD:
        js     loc_5_59E8
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_5D84
loc_5_5A0C:
        test   al, 0x20
        jne    loc_5_5A64
        shld   edx, eax, 0x1a
        and    edx, 0x3f
loc_5_5A17:
        test   ah, 0x10
        je     loc_5_5A7A
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_5A23:
        js     loc_5_59FD
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_59ED
loc_5_5A34:
        test   ah, 0x40
        jne    loc_5_5A9C
        shld   edx, eax, 0x11
        and    edx, 0x3f
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_5A45:
        test   al, 0x20
        je     loc_5_5ABA
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_5A50:
        js     loc_5_5A23
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_5D84
loc_5_5A64:
        test   al, 0x40
        jne    loc_5_5ADB
        shld   edx, eax, 0x19
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_5FBF
loc_5_5A7A:
        test   ah, 0x20
        je     loc_5_5B03
        shld   ecx, eax, 0x12
        and    ecx, 1
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5A93:
        js     loc_5_5A50
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5D98
loc_5_5A9C:
        test   ah, 0x80
        jne    loc_5_5B26
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        mov    edx, eax
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_52E5
loc_5_5ABA:
        test   al, 0x40
        je     loc_5_5B4C
        shld   ecx, eax, 0x19
        and    ecx, 1
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5AD2:
        js     loc_5_5A93
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_546D
loc_5_5ADB:
        shld   edx, eax, 0x19
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_56A2
        mov    cl, 0xc
        jmp    loc_5_5278
loc_5_5B03:
        test   ah, 0x40
        je     loc_5_5B67
        shld   ecx, eax, 0x11
        and    ecx, 3
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5B1D:
        js     loc_5_5AD2
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5487
loc_5_5B26:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        mov    edx, eax
        and    edx, 0xfff
        add    edx, 0x140
        cmp    edx, 0x113f
        jne    loc_5_5A17
        mov    cl, 0x30
        jmp    loc_5_5278
loc_5_5B4C:
        test   al, 0x80
        je     loc_5_5B90
        mov    cl, ah
        and    ecx, 3
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5B5E:
        js     loc_5_5B1D
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5806
loc_5_5B67:
        test   ah, 0x80
        mov    cl, 0x11
        je     loc_5_611C
        shld   ecx, eax, 0x10
        and    ecx, 7
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5B87:
        js     loc_5_5B5E
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5820
loc_5_5B90:
        test   ah, 1
        mov    cl, 0xa
        je     loc_5_611C
        shld   ecx, eax, 0x17
        and    ecx, 7
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5BAB:
        js     loc_5_5B87
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5BB1
loc_5_5BB1:
        test   ah, 0x30
        jp     loc_5_5BE4
        shld   edx, eax, 0x13
        ror    dl, 1
        xor    dl, 0x80
        dec    ebp
loc_5_5BC1:
        js     loc_5_5BAB
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_5BCB:
        test   al, 0x60
        jp     loc_5_5C11
        shld   edx, eax, 0x1a
        ror    dl, 1
        xor    dl, 0x80
        dec    ebp
loc_5_5BDA:
        js     loc_5_5BC1
        mov    byte ptr [edi], dl
        inc    edi
        jmp    loc_5_5F53
loc_5_5BE4:
        test   ah, 0x20
        jne    loc_5_5C3C
        shld   edx, eax, 0x12
        and    edx, 0x3f
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_5BF5:
        test   al, 0x10
        je     loc_5_5C58
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_5C00:
        js     loc_5_5BDA
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_5BCB
loc_5_5C11:
        test   al, 0x40
        jne    loc_5_5C75
        shld   edx, eax, 0x19
        and    edx, 0x3f
loc_5_5C1C:
        test   ah, 0x20
        je     loc_5_5C89
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_5C28:
        js     loc_5_5C00
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_5F53
loc_5_5C3C:
        test   ah, 0x40
        jne    loc_5_5CAB
        shld   edx, eax, 0x11
        and    edx, 0xff
        add    edx, 0x40
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_5DEE
loc_5_5C58:
        test   al, 0x20
        je     loc_5_5CD3
        shld   ecx, eax, 0x1a
        and    ecx, 1
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5C6C:
        js     loc_5_5C28
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5F6D
loc_5_5C75:
        test   al, 0x80
        jne    loc_5_5CF0
        mov    dl, ah
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_54AC
loc_5_5C89:
        test   ah, 0x40
        je     loc_5_5D18
        shld   ecx, eax, 0x11
        and    ecx, 1
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5CA2:
        js     loc_5_5C6C
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_52A2
loc_5_5CAB:
        shld   edx, eax, 0x11
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_5871
        mov    cl, 0x2c
        jmp    loc_5_5278
loc_5_5CD3:
        test   al, 0x40
        je     loc_5_5D3B
        shld   ecx, eax, 0x19
        and    ecx, 3
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5CE7:
        js     loc_5_5CA2
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_52BB
loc_5_5CF0:
        shld   edx, eax, 0x18
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_5BF5
        mov    cl, 0x10
        jmp    loc_5_5278
loc_5_5D18:
        test   ah, 0x80
        je     loc_5_5D5C
        shld   ecx, eax, 0x10
        and    ecx, 3
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5D32:
        js     loc_5_5CE7
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5636
loc_5_5D3B:
        test   al, 0x80
        mov    cl, 9
        je     loc_5_611C
        mov    cl, ah
        and    ecx, 7
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5D53:
        js     loc_5_5D32
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_564A
loc_5_5D5C:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 1
        mov    cl, 2
        je     loc_5_611C
        shld   ecx, eax, 0x1f
        and    ecx, 7
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5D7B:
        js     loc_5_5D53
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_59D9
loc_5_5D84:
        test   al, 0xc0
        jp     loc_5_5DB8
        shld   edx, eax, 0x19
        ror    dl, 1
        xor    dl, 0x80
        dec    ebp
loc_5_5D93:
        js     loc_5_5D7B
        mov    byte ptr [edi], dl
        inc    edi
loc_5_5D98:
        ror    eax, 1
        test   ah, 0xc0
        rol    eax, 1
        jp     loc_5_5DDE
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        ror    al, 1
        xor    al, 0x80
        dec    ebp
loc_5_5DAE:
        js     loc_5_5D93
        mov    byte ptr [edi], al
        inc    edi
        jmp    loc_5_546D
loc_5_5DB8:
        test   al, 0x80
        jne    loc_5_5E0D
        mov    dl, ah
        and    edx, 0x3f
loc_5_5DC1:
        test   ah, 0x40
        je     loc_5_5E29
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_5DCD:
        js     loc_5_5DAE
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_5D98
loc_5_5DDE:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 1
        jne    loc_5_5E4C
        shld   edx, eax, 0x1f
        and    edx, 0x3f
loc_5_5DEE:
        test   al, 0x80
        je     loc_5_5E62
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_5DF9:
        js     loc_5_5DCD
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_546D
loc_5_5E0D:
        test   ah, 1
        jne    loc_5_5E84
        shld   edx, eax, 0x17
        and    edx, 0xff
        add    edx, 0x40
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_5313
loc_5_5E29:
        test   ah, 0x80
        je     loc_5_5EAC
        shld   ecx, eax, 0x10
        and    ecx, 1
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5E43:
        js     loc_5_5DF9
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5487
loc_5_5E4C:
        test   al, 2
        jne    loc_5_5ECE
        shld   edx, eax, 0x1e
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_5674
loc_5_5E62:
        test   ah, 1
        je     loc_5_5EF1
        shld   ecx, eax, 0x17
        and    ecx, 1
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5E7B:
        js     loc_5_5E43
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5806
loc_5_5E84:
        shld   edx, eax, 0x17
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_5A45
        mov    cl, 0x14
        jmp    loc_5_5278
loc_5_5EAC:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 1
        je     loc_5_5F0F
        shld   ecx, eax, 0x1f
        and    ecx, 3
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5EC5:
        js     loc_5_5E7B
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5820
loc_5_5ECE:
        shld   edx, eax, 0x1e
        and    edx, 0xfff
        add    edx, 0x140
        cmp    edx, 0x113f
        jne    loc_5_5DC1
        mov    cl, 0x38
        jmp    loc_5_5278
loc_5_5EF1:
        test   ah, 2
        je     loc_5_5F32
        shld   ecx, eax, 0x16
        and    ecx, 3
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5F06:
        js     loc_5_5EC5
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5BB1
loc_5_5F0F:
        test   al, 2
        mov    cl, 3
        je     loc_5_611C
        shld   ecx, eax, 0x1e
        and    ecx, 7
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5F29:
        js     loc_5_5F06
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5BCB
loc_5_5F32:
        test   ah, 4
        mov    cl, 0xc
        je     loc_5_611C
        shld   ecx, eax, 0x15
        and    ecx, 7
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_5F4D:
        js     loc_5_5F29
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5F53
loc_5_5F53:
        test   ah, 0xc0
        jp     loc_5_5F88
        shld   edx, eax, 0x11
        ror    dl, 1
        xor    dl, 0x80
        dec    ebp
loc_5_5F63:
        js     loc_5_5F4D
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_5F6D:
        ror    eax, 1
        test   al, 0xc0
        rol    eax, 1
        jp     loc_5_5FB3
        ror    ah, 1
        xor    ah, 0x80
        dec    ebp
loc_5_5F7E:
        js     loc_5_5F63
        mov    byte ptr [edi], ah
        inc    edi
        jmp    loc_5_52A2
loc_5_5F88:
        test   ah, 0x80
        jne    loc_5_5FDF
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        mov    edx, eax
        and    edx, 0x3f
loc_5_5F97:
        test   al, 0x40
        je     loc_5_5FFA
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_5FA2:
        js     loc_5_5F7E
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_5F6D
loc_5_5FB3:
        test   ah, 1
        jne    loc_5_6015
        shld   edx, eax, 0x17
        and    edx, 0x3f
loc_5_5FBF:
        test   ah, 0x80
        je     loc_5_6031
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 2
loc_5_5FCB:
        js     loc_5_5FA2
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        add    edi, 2
        jmp    loc_5_52A2
loc_5_5FDF:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 1
        jne    loc_5_6057
        shld   edx, eax, 0x1f
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_54D8
loc_5_5FFA:
        test   al, 0x80
        je     loc_5_607A
        mov    cl, ah
        and    ecx, 1
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_600C:
        js     loc_5_5FCB
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_52BB
loc_5_6015:
        test   ah, 2
        jne    loc_5_6098
        shld   edx, eax, 0x16
        and    edx, 0xff
        add    edx, 0x40
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_584A
loc_5_6031:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 1
        je     loc_5_60C0
        shld   ecx, eax, 0x1f
        and    ecx, 1
        add    ecx, 3
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_604E:
        js     loc_5_600C
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5636
loc_5_6057:
        shld   edx, eax, 0x1f
        and    edx, 0xfff
        add    edx, 0x140
        cmp    edx, 0x113f
        jne    loc_5_5C1C
        mov    cl, 0x34
        jmp    loc_5_5278
loc_5_607A:
        test   ah, 1
        je     loc_5_60DD
        shld   ecx, eax, 0x17
        and    ecx, 3
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_608F:
        js     loc_5_604E
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_564A
loc_5_6098:
        shld   edx, eax, 0x16
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_5F97
        mov    cl, 0x18
        jmp    loc_5_5278
loc_5_60C0:
        test   al, 2
        je     loc_5_60FD
        shld   ecx, eax, 0x1e
        and    ecx, 3
        add    ecx, 5
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_60D4:
        js     loc_5_608F
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_59D9
loc_5_60DD:
        test   ah, 2
        mov    cl, 0xb
        je     loc_5_611C
        shld   ecx, eax, 0x16
        and    ecx, 7
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_60F4:
        js     loc_5_60D4
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_59ED
loc_5_60FD:
        test   al, 4
        mov    cl, 4
        je     loc_5_611C
        shld   ecx, eax, 0x1d
        and    ecx, 7
        add    ecx, 9
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6113:
        js     loc_5_60F4
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_5D84
loc_5_611C:
        shr    eax, cl
        jae    loc_5_615C
        add    cl, 4
        and    eax, 0xf
        xchg   ecx, eax
        add    ecx, 0x11
loc_5_612A:
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
        js     loc_5_6113
        cmp    edx, 4
        jb     loc_5_6143
        mov    edx, ecx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
loc_5_6143:
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    cl, al
        cmp    ecx, 0x10
        jbe    loc_5_6152
        add    ebx, 2
        sub    ecx, 0x10
loc_5_6152:
        mov    eax, dword ptr [ebx - 2]
        jmp    loc_5_521C
loc_5_615C:
        shr    eax, 1
        jae    loc_5_616D
        add    cl, 6
        and    eax, 0x1f
        xchg   ecx, eax
        add    ecx, 0x21
        jmp    loc_5_612A
loc_5_616D:
        shr    eax, 1
        jae    loc_5_617E
        add    cl, 8
        and    eax, 0x3f
        xchg   ecx, eax
        add    ecx, 0x41
        jmp    loc_5_612A
loc_5_617E:
        shr    eax, 1
        jae    loc_5_6192
        add    cl, 0xa
        and    eax, 0x7f
        xchg   ecx, eax
        add    ecx, 0x81
        jmp    loc_5_612A
loc_5_6192:
        shr    eax, 1
        jae    loc_5_528F
        add    cl, 0xc
        and    eax, 0xff
        xchg   ecx, eax
        add    ecx, 0x101
        jmp    loc_5_612A
loc_5_61AF:
        jmp    loc_5_528F
loc_5_61B4:
        push   esi
        call   sub_5_622C
        sub    ecx, ecx
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        cmp    ax, 0x4d4a
        jne    loc_5_61DB
        call   0x6290
        jb     loc_5_61DB
        pop    esi
        sub    edi, dword ptr [esi + 8]
        mov    word ptr [esi + 0xc], di
        sub    ecx, ecx
        sub    eax, eax
        ret    
loc_5_61DB:
        pop    esi
        mov    eax, 5
        ret    
loc_5_61E2:
        cmp    dword ptr [esi + 0x10], 0
        jne    loc_5_61F6
        mov    ebx, dword ptr [esi]
        cmp    word ptr [ebx], 0x4d4a
        je     loc_5_6200
        jmp    loc_5_51AA
loc_5_61F6:
        test   byte ptr [esi + 0x11], 0x80
        je     loc_5_51AA
loc_5_6200:
        push   esi
        call   sub_5_622C
        jne    loc_5_620B
        add    ebx, 4
loc_5_620B:
        call   0x6290
        jb     loc_5_61DB
        pop    esi
        mov    dword ptr [esi + 0x10], eax
        mov    eax, ebx
        xchg   dword ptr [esi], eax
        sub    ebx, eax
        mov    ecx, ebx
        mov    eax, edi
        xchg   dword ptr [esi + 8], eax
        sub    edi, eax
        mov    word ptr [esi + 0xc], di
        sub    eax, eax
        ret    
sub_5_622C:
        cld    
        movzx  edx, word ptr [esi + 0xc]
        mov    ecx, edx
        shr    edx, 9
        and    ecx, 0x1ff
        je     loc_5_6241
        inc    edx
        sub    ecx, ecx
loc_5_6241:
        or     ecx, dword ptr [esi + 0x10]
        mov    edi, dword ptr [esi + 8]
        mov    ebx, dword ptr [esi]
        ret    
        db  08Bh, 0C0h
loc_5_624C:
        loopne 0x62b0
        add    byte ptr [eax], al
        cmp    eax, 0x2f000065               ; -> loc_5_672F
        add    byte ptr [bx + si], al
        test   eax, 0xa5000069               ; -> loc_5_6BA5
        imul   eax, dword ptr [eax], 0
        pop    ds
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
        pop    ds
        jo     loc_5_6267
loc_5_6267:
        add    byte ptr [ecx + 0x72], cl     ; -> loc_5_7289
        add    byte ptr gs:[eax], al
        pop    ss
        arpl   word ptr [eax], ax
        add    byte ptr [ecx + ebp*2], ah    ; -> loc_5_6964
        add    byte ptr [esi + 0x67], ch     ; -> loc_5_676E
        add    byte ptr [eax], al
        fisubr dword ptr [ebp]
        add    ah, ah                        ; -> loc_5_6BE4
        imul   eax, dword ptr [eax], 0
        inc    esi
        jb     loc_5_6287
loc_5_6287:
        add    byte ptr [eax + esi*2], bl    ; -> loc_5_705C
        add    al, ah                        ; -> loc_5_62E0
        bound  eax, qword ptr [eax]
        add    byte ptr [ebp + 0x52], dl
        mov    ebp, 0x200
        jecxz  loc_5_62E0
        mov    eax, dword ptr [ebx]
        shld   eax, ecx, 0x10
        and    ecx, 0x3c
        jmp    loc_5_624C
loc_5_62A8:
        test   ebp, ebp
        jne    loc_5_62C2
        dec    dword ptr [esp]
        je     loc_5_62C7
        mov    ebp, 0x200
        and    ecx, 0x3c
        jmp    loc_5_624C
loc_5_62BF:
        stc    
        jmp    loc_5_62CF
loc_5_62C2:
        dec    dword ptr [esp]
        jne    loc_5_62BF
loc_5_62C7:
        shl    eax, 0x10
        mov    al, cl
        or     ah, 0x80
loc_5_62CF:
        pop    ebp
        pop    ebp
        ret    
loc_5_62D2:
        test   ah, 1
        je     loc_5_6349
        ror    ah, 1
        dec    ebp
loc_5_62DB:
        js     loc_5_62BF
        mov    byte ptr [edi], ah
        inc    edi
loc_5_62E0:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 1
        jne    loc_5_6306
        ror    al, 1
        dec    ebp
loc_5_62ED:
        js     loc_5_62DB
        mov    byte ptr [edi], al
        inc    edi
        test   ah, 1
        jne    loc_5_6526
        ror    ah, 1
        dec    ebp
loc_5_62FF:
        js     loc_5_62ED
        mov    byte ptr [edi], ah
        inc    edi
        jmp    loc_5_62E0
loc_5_6306:
        test   al, 2
        je     loc_5_637F
        shld   edx, eax, 0x1e
        or     dl, 0x80
        dec    ebp
loc_5_6312:
        js     loc_5_62FF
        mov    byte ptr [edi], dl
        inc    edi
loc_5_6317:
        test   ah, 2
        jne    loc_5_6714
        shld   edx, eax, 0x16
        and    dl, 0x7f
        dec    ebp
loc_5_6328:
        js     loc_5_6312
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 2
        jne    loc_5_694F
        shld   edx, eax, 0x1e
        and    dl, 0x7f
        dec    ebp
loc_5_6342:
        js     loc_5_6328
        mov    byte ptr [edi], dl
        inc    edi
        jmp    loc_5_6317
loc_5_6349:
        test   ah, 2
        jne    loc_5_63B0
        shld   edx, eax, 0x16
        and    edx, 0x3f
loc_5_6355:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 1
        je     loc_5_63CC
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_6365:
        js     loc_5_6342
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_653D
loc_5_637F:
        test   al, 4
        jne    loc_5_63E9
        shld   edx, eax, 0x1d
        and    edx, 0x3f
loc_5_638A:
        test   ah, 2
        je     loc_5_63FF
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_6396:
        js     loc_5_6365
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_6964
loc_5_63B0:
        test   ah, 4
        jne    loc_5_6421
        shld   edx, eax, 0x15
        and    edx, 0xff
        add    edx, 0x40
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_6A1D
loc_5_63CC:
        test   al, 2
        je     loc_5_6449
        shld   ecx, eax, 0x1e
        and    ecx, 1
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_63E0:
        js     loc_5_6396
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_69A9
loc_5_63E9:
        test   al, 8
        jne    loc_5_6466
        shld   edx, eax, 0x1c
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_6E5C
loc_5_63FF:
        test   ah, 4
        je     loc_5_6489
        shld   ecx, eax, 0x15
        and    ecx, 1
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6418:
        js     loc_5_63E0
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6DDA
loc_5_6421:
        shld   edx, eax, 0x15
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_72F6
        mov    cl, 0x1c
        jmp    loc_5_62A8
loc_5_6449:
        test   al, 4
        je     loc_5_64A7
        shld   ecx, eax, 0x1d
        and    ecx, 3
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_645D:
        js     loc_5_6418
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6E1F
loc_5_6466:
        shld   edx, eax, 0x1c
        and    edx, 0xfff
        add    edx, 0x140
        cmp    edx, 0x113f
        jne    loc_5_6355
        mov    cl, 0
        jmp    loc_5_62A8
loc_5_6489:
        test   ah, 8
        je     loc_5_64CA
        shld   ecx, eax, 0x14
        and    ecx, 3
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_649E:
        js     loc_5_645D
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_7246
loc_5_64A7:
        test   al, 8
        mov    cl, 5
        je     loc_5_745C
        shld   ecx, eax, 0x1c
        and    ecx, 7
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_64C1:
        js     loc_5_649E
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_7289
loc_5_64CA:
        test   ah, 0x10
        mov    cl, 0xe
        je     loc_5_745C
        shld   ecx, eax, 0x13
        and    ecx, 7
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_64E5:
        js     loc_5_64C1
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_62E0
loc_5_64EE:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 1
        je     loc_5_656F
        ror    al, 1
        dec    ebp
loc_5_64FB:
        js     loc_5_64E5
        mov    byte ptr [edi], al
        inc    edi
loc_5_6500:
        test   ah, 1
        jne    loc_5_6526
        ror    ah, 1
        dec    ebp
loc_5_6509:
        js     loc_5_64FB
        mov    byte ptr [edi], ah
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 1
        jne    loc_5_6306
        ror    al, 1
        dec    ebp
loc_5_651F:
        js     loc_5_6509
        mov    byte ptr [edi], al
        inc    edi
        jmp    loc_5_6500
loc_5_6526:
        test   ah, 2
        je     loc_5_65A0
        shld   edx, eax, 0x16
        or     dl, 0x80
        dec    ebp
loc_5_6533:
        js     loc_5_651F
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_653D:
        test   al, 2
        jne    loc_5_694F
        shld   edx, eax, 0x1e
        and    dl, 0x7f
        dec    ebp
loc_5_654D:
        js     loc_5_6533
        mov    byte ptr [edi], dl
        inc    edi
        test   ah, 2
        jne    loc_5_6714
        shld   edx, eax, 0x16
        and    dl, 0x7f
        dec    ebp
loc_5_6563:
        js     loc_5_654D
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_653D
loc_5_656F:
        test   al, 2
        jne    loc_5_65D6
        shld   edx, eax, 0x1e
        and    edx, 0x3f
loc_5_657A:
        test   ah, 1
        je     loc_5_65EC
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_6586:
        js     loc_5_6563
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_6317
loc_5_65A0:
        test   ah, 4
        jne    loc_5_660A
        shld   edx, eax, 0x15
        and    edx, 0x3f
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_65B1:
        test   al, 2
        je     loc_5_6626
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_65BC:
        js     loc_5_6586
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_672F
loc_5_65D6:
        test   al, 4
        jne    loc_5_6647
        shld   edx, eax, 0x1d
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_67E1
loc_5_65EC:
        test   ah, 2
        je     loc_5_666A
        shld   ecx, eax, 0x16
        and    ecx, 1
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6601:
        js     loc_5_65BC
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_676E
loc_5_660A:
        test   ah, 8
        jne    loc_5_6688
        shld   edx, eax, 0x14
        and    edx, 0xff
        add    edx, 0x40
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_6C27
loc_5_6626:
        test   al, 4
        je     loc_5_66B0
        shld   ecx, eax, 0x1d
        and    ecx, 1
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_663E:
        js     loc_5_6601
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6BA5
loc_5_6647:
        shld   edx, eax, 0x1d
        and    edx, 0xfff
        add    edx, 0x140
        cmp    edx, 0x113f
        jne    loc_5_70CA
        mov    cl, 0x3c
        jmp    loc_5_62A8
loc_5_666A:
        test   ah, 4
        je     loc_5_66CD
        shld   ecx, eax, 0x15
        and    ecx, 3
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_667F:
        js     loc_5_663E
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6BE4
loc_5_6688:
        shld   edx, eax, 0x14
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_657A
        mov    cl, 0x20
        jmp    loc_5_62A8
loc_5_66B0:
        test   al, 8
        je     loc_5_66F1
        shld   ecx, eax, 0x1c
        and    ecx, 3
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_66C4:
        js     loc_5_667F
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_701F
loc_5_66CD:
        test   ah, 8
        mov    cl, 0xd
        je     loc_5_745C
        shld   ecx, eax, 0x14
        and    ecx, 7
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_66E8:
        js     loc_5_66C4
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_705C
loc_5_66F1:
        test   al, 0x10
        mov    cl, 6
        je     loc_5_745C
        shld   ecx, eax, 0x1b
        and    ecx, 7
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_670B:
        js     loc_5_66E8
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6500
loc_5_6714:
        test   ah, 4
        je     loc_5_67A0
        shld   edx, eax, 0x15
        or     dl, 0x80
        dec    ebp
loc_5_6725:
        js     loc_5_670B
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_672F:
        test   al, 4
        jne    loc_5_675D
        shld   edx, eax, 0x1d
        and    dl, 0x7f
        dec    ebp
loc_5_673B:
        js     loc_5_6725
        mov    byte ptr [edi], dl
        inc    edi
        test   ah, 4
        jne    loc_5_6992
        shld   edx, eax, 0x15
        and    dl, 0x7f
        dec    ebp
loc_5_6751:
        js     loc_5_673B
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_672F
loc_5_675D:
        test   al, 8
        je     loc_5_67D6
        shld   edx, eax, 0x1c
        or     dl, 0x80
        dec    ebp
loc_5_6769:
        js     loc_5_6751
        mov    byte ptr [edi], dl
        inc    edi
loc_5_676E:
        test   ah, 8
        jne    loc_5_6B8A
        shld   edx, eax, 0x14
        and    dl, 0x7f
        dec    ebp
loc_5_677F:
        js     loc_5_6769
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 8
        jne    loc_5_6DC5
        shld   edx, eax, 0x1c
        and    dl, 0x7f
        dec    ebp
loc_5_6799:
        js     loc_5_677F
        mov    byte ptr [edi], dl
        inc    edi
        jmp    loc_5_676E
loc_5_67A0:
        test   ah, 8
        jne    loc_5_6807
        shld   edx, eax, 0x14
        and    edx, 0x3f
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_67B1:
        test   al, 4
        je     loc_5_6823
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_67BC:
        js     loc_5_6799
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_69A9
loc_5_67D6:
        test   al, 0x10
        jne    loc_5_6840
        shld   edx, eax, 0x1b
        and    edx, 0x3f
loc_5_67E1:
        test   ah, 8
        je     loc_5_6856
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_67ED:
        js     loc_5_67BC
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_6DDA
loc_5_6807:
        test   ah, 0x10
        jne    loc_5_6878
        shld   edx, eax, 0x13
        and    edx, 0xff
        add    edx, 0x40
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_6E93
loc_5_6823:
        test   al, 8
        je     loc_5_68A0
        shld   ecx, eax, 0x1c
        and    ecx, 1
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6837:
        js     loc_5_67ED
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6E1F
loc_5_6840:
        test   al, 0x20
        jne    loc_5_68BD
        shld   edx, eax, 0x1a
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_72C0
loc_5_6856:
        test   ah, 0x10
        je     loc_5_68E5
        shld   ecx, eax, 0x13
        and    ecx, 1
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_686F:
        js     loc_5_6837
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_7246
loc_5_6878:
        shld   edx, eax, 0x13
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_638A
        mov    cl, 0x24
        jmp    loc_5_62A8
loc_5_68A0:
        test   al, 0x10
        je     loc_5_6903
        shld   ecx, eax, 0x1b
        and    ecx, 3
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_68B4:
        js     loc_5_686F
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_7289
loc_5_68BD:
        shld   edx, eax, 0x1a
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_67B1
        mov    cl, 8
        jmp    loc_5_62A8
loc_5_68E5:
        test   ah, 0x20
        je     loc_5_6926
        shld   ecx, eax, 0x12
        and    ecx, 3
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_68FA:
        js     loc_5_68B4
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_62E0
loc_5_6903:
        test   al, 0x20
        mov    cl, 7
        je     loc_5_745C
        shld   ecx, eax, 0x1a
        and    ecx, 7
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_691D:
        js     loc_5_68FA
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6317
loc_5_6926:
        test   ah, 0x40
        mov    cl, 0x10
        je     loc_5_745C
        shld   ecx, eax, 0x11
        and    ecx, 7
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6946:
        js     loc_5_691D
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_672F
loc_5_694F:
        test   al, 4
        je     loc_5_69DB
        shld   edx, eax, 0x1d
        or     dl, 0x80
        dec    ebp
loc_5_695F:
        js     loc_5_6946
        mov    byte ptr [edi], dl
        inc    edi
loc_5_6964:
        test   ah, 4
        jne    loc_5_6992
        shld   edx, eax, 0x15
        and    dl, 0x7f
        dec    ebp
loc_5_6971:
        js     loc_5_695F
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 4
        jne    loc_5_675D
        shld   edx, eax, 0x1d
        and    dl, 0x7f
        dec    ebp
loc_5_698B:
        js     loc_5_6971
        mov    byte ptr [edi], dl
        inc    edi
        jmp    loc_5_6964
loc_5_6992:
        test   ah, 8
        je     loc_5_6A0C
        shld   edx, eax, 0x14
        or     dl, 0x80
        dec    ebp
loc_5_699F:
        js     loc_5_698B
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_69A9:
        test   al, 8
        jne    loc_5_6DC5
        shld   edx, eax, 0x1c
        and    dl, 0x7f
        dec    ebp
loc_5_69B9:
        js     loc_5_699F
        mov    byte ptr [edi], dl
        inc    edi
        test   ah, 8
        jne    loc_5_6B8A
        shld   edx, eax, 0x14
        and    dl, 0x7f
        dec    ebp
loc_5_69CF:
        js     loc_5_69B9
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_69A9
loc_5_69DB:
        test   al, 8
        jne    loc_5_6A42
        shld   edx, eax, 0x1c
        and    edx, 0x3f
loc_5_69E6:
        test   ah, 4
        je     loc_5_6A58
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_69F2:
        js     loc_5_69CF
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_676E
loc_5_6A0C:
        test   ah, 0x10
        jne    loc_5_6A76
        shld   edx, eax, 0x13
        and    edx, 0x3f
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_6A1D:
        test   al, 8
        je     loc_5_6A92
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_6A28:
        js     loc_5_69F2
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_6BA5
loc_5_6A42:
        test   al, 0x10
        jne    loc_5_6AB3
        shld   edx, eax, 0x1b
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_6C57
loc_5_6A58:
        test   ah, 8
        je     loc_5_6ADB
        shld   ecx, eax, 0x14
        and    ecx, 1
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6A6D:
        js     loc_5_6A28
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6BE4
loc_5_6A76:
        test   ah, 0x20
        jne    loc_5_6AF9
        shld   edx, eax, 0x12
        and    edx, 0xff
        add    edx, 0x40
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_7099
loc_5_6A92:
        test   al, 0x10
        je     loc_5_6B21
        shld   ecx, eax, 0x1b
        and    ecx, 1
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6AAA:
        js     loc_5_6A6D
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_701F
loc_5_6AB3:
        shld   edx, eax, 0x1b
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_65B1
        mov    cl, 4
        jmp    loc_5_62A8
loc_5_6ADB:
        test   ah, 0x10
        je     loc_5_6B3E
        shld   ecx, eax, 0x13
        and    ecx, 3
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6AF0:
        js     loc_5_6AAA
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_705C
loc_5_6AF9:
        shld   edx, eax, 0x12
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_69E6
        mov    cl, 0x28
        jmp    loc_5_62A8
loc_5_6B21:
        test   al, 0x20
        je     loc_5_6B67
        shld   ecx, eax, 0x1a
        and    ecx, 3
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6B35:
        js     loc_5_6AF0
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6500
loc_5_6B3E:
        test   ah, 0x20
        mov    cl, 0xf
        je     loc_5_745C
        shld   ecx, eax, 0x12
        and    ecx, 7
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6B5E:
        js     loc_5_6B35
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_653D
loc_5_6B67:
        test   al, 0x40
        mov    cl, 8
        je     loc_5_745C
        shld   ecx, eax, 0x19
        and    ecx, 7
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6B81:
        js     loc_5_6B5E
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6964
loc_5_6B8A:
        test   ah, 0x10
        je     loc_5_6C16
        shld   edx, eax, 0x13
        or     dl, 0x80
        dec    ebp
loc_5_6B9B:
        js     loc_5_6B81
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_6BA5:
        test   al, 0x10
        jne    loc_5_6BD3
        shld   edx, eax, 0x1b
        and    dl, 0x7f
        dec    ebp
loc_5_6BB1:
        js     loc_5_6B9B
        mov    byte ptr [edi], dl
        inc    edi
        test   ah, 0x10
        jne    loc_5_6E08
        shld   edx, eax, 0x13
        and    dl, 0x7f
        dec    ebp
loc_5_6BC7:
        js     loc_5_6BB1
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_6BA5
loc_5_6BD3:
        test   al, 0x20
        je     loc_5_6C4C
        shld   edx, eax, 0x1a
        or     dl, 0x80
        dec    ebp
loc_5_6BDF:
        js     loc_5_6BC7
        mov    byte ptr [edi], dl
        inc    edi
loc_5_6BE4:
        test   ah, 0x20
        jne    loc_5_7008
        shld   edx, eax, 0x12
        and    dl, 0x7f
        dec    ebp
loc_5_6BF5:
        js     loc_5_6BDF
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 0x20
        jne    loc_5_7235
        shld   edx, eax, 0x1a
        and    dl, 0x7f
        dec    ebp
loc_5_6C0F:
        js     loc_5_6BF5
        mov    byte ptr [edi], dl
        inc    edi
        jmp    loc_5_6BE4
loc_5_6C16:
        test   ah, 0x20
        jne    loc_5_6C7D
        shld   edx, eax, 0x12
        and    edx, 0x3f
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_6C27:
        test   al, 0x10
        je     loc_5_6C99
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_6C32:
        js     loc_5_6C0F
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_6E1F
loc_5_6C4C:
        test   al, 0x40
        jne    loc_5_6CB6
        shld   edx, eax, 0x19
        and    edx, 0x3f
loc_5_6C57:
        test   ah, 0x20
        je     loc_5_6CCA
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_6C63:
        js     loc_5_6C32
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_7246
loc_5_6C7D:
        test   ah, 0x40
        jne    loc_5_6CEC
        shld   edx, eax, 0x11
        and    edx, 0xff
        add    edx, 0x40
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_72F6
loc_5_6C99:
        test   al, 0x20
        je     loc_5_6D14
        shld   ecx, eax, 0x1a
        and    ecx, 1
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6CAD:
        js     loc_5_6C63
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_7289
loc_5_6CB6:
        test   al, 0x80
        jne    loc_5_6D31
        mov    dl, ah
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_6355
loc_5_6CCA:
        test   ah, 0x40
        je     loc_5_6D59
        shld   ecx, eax, 0x11
        and    ecx, 1
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6CE3:
        js     loc_5_6CAD
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_62E0
loc_5_6CEC:
        shld   edx, eax, 0x11
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_67E1
        mov    cl, 0x2c
        jmp    loc_5_62A8
loc_5_6D14:
        test   al, 0x40
        je     loc_5_6D7C
        shld   ecx, eax, 0x19
        and    ecx, 3
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6D28:
        js     loc_5_6CE3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6317
loc_5_6D31:
        shld   edx, eax, 0x18
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_6C27
        mov    cl, 0x10
        jmp    loc_5_62A8
loc_5_6D59:
        test   ah, 0x80
        je     loc_5_6D9D
        shld   ecx, eax, 0x10
        and    ecx, 3
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6D73:
        js     loc_5_6D28
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_672F
loc_5_6D7C:
        test   al, 0x80
        mov    cl, 9
        je     loc_5_745C
        mov    cl, ah
        and    ecx, 7
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6D94:
        js     loc_5_6D73
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_676E
loc_5_6D9D:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 1
        mov    cl, 2
        je     loc_5_745C
        shld   ecx, eax, 0x1f
        and    ecx, 7
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6DBC:
        js     loc_5_6D94
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6BA5
loc_5_6DC5:
        test   al, 0x10
        je     loc_5_6E51
        shld   edx, eax, 0x1b
        or     dl, 0x80
        dec    ebp
loc_5_6DD5:
        js     loc_5_6DBC
        mov    byte ptr [edi], dl
        inc    edi
loc_5_6DDA:
        test   ah, 0x10
        jne    loc_5_6E08
        shld   edx, eax, 0x13
        and    dl, 0x7f
        dec    ebp
loc_5_6DE7:
        js     loc_5_6DD5
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 0x10
        jne    loc_5_6BD3
        shld   edx, eax, 0x1b
        and    dl, 0x7f
        dec    ebp
loc_5_6E01:
        js     loc_5_6DE7
        mov    byte ptr [edi], dl
        inc    edi
        jmp    loc_5_6DDA
loc_5_6E08:
        test   ah, 0x20
        je     loc_5_6E82
        shld   edx, eax, 0x12
        or     dl, 0x80
        dec    ebp
loc_5_6E15:
        js     loc_5_6E01
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_6E1F:
        test   al, 0x20
        jne    loc_5_7235
        shld   edx, eax, 0x1a
        and    dl, 0x7f
        dec    ebp
loc_5_6E2F:
        js     loc_5_6E15
        mov    byte ptr [edi], dl
        inc    edi
        test   ah, 0x20
        jne    loc_5_7008
        shld   edx, eax, 0x12
        and    dl, 0x7f
        dec    ebp
loc_5_6E45:
        js     loc_5_6E2F
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_6E1F
loc_5_6E51:
        test   al, 0x20
        jne    loc_5_6EB8
        shld   edx, eax, 0x1a
        and    edx, 0x3f
loc_5_6E5C:
        test   ah, 0x10
        je     loc_5_6ECE
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_6E68:
        js     loc_5_6E45
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_6BE4
loc_5_6E82:
        test   ah, 0x40
        jne    loc_5_6EF0
        shld   edx, eax, 0x11
        and    edx, 0x3f
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_6E93:
        test   al, 0x20
        je     loc_5_6F0E
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_6E9E:
        js     loc_5_6E68
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_701F
loc_5_6EB8:
        test   al, 0x40
        jne    loc_5_6F2F
        shld   edx, eax, 0x19
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_70CA
loc_5_6ECE:
        test   ah, 0x20
        je     loc_5_6F57
        shld   ecx, eax, 0x12
        and    ecx, 1
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6EE7:
        js     loc_5_6E9E
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_705C
loc_5_6EF0:
        test   ah, 0x80
        jne    loc_5_6F7A
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        mov    edx, eax
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_657A
loc_5_6F0E:
        test   al, 0x40
        je     loc_5_6FA0
        shld   ecx, eax, 0x19
        and    ecx, 1
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6F26:
        js     loc_5_6EE7
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6500
loc_5_6F2F:
        shld   edx, eax, 0x19
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_6A1D
        mov    cl, 0xc
        jmp    loc_5_62A8
loc_5_6F57:
        test   ah, 0x40
        je     loc_5_6FBB
        shld   ecx, eax, 0x11
        and    ecx, 3
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6F71:
        js     loc_5_6F26
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_653D
loc_5_6F7A:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        mov    edx, eax
        and    edx, 0xfff
        add    edx, 0x140
        cmp    edx, 0x113f
        jne    loc_5_6E5C
        mov    cl, 0x30
        jmp    loc_5_62A8
loc_5_6FA0:
        test   al, 0x80
        je     loc_5_6FE4
        mov    cl, ah
        and    ecx, 3
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6FB2:
        js     loc_5_6F71
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6964
loc_5_6FBB:
        test   ah, 0x80
        mov    cl, 0x11
        je     loc_5_745C
        shld   ecx, eax, 0x10
        and    ecx, 7
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6FDB:
        js     loc_5_6FB2
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_69A9
loc_5_6FE4:
        test   ah, 1
        mov    cl, 0xa
        je     loc_5_745C
        shld   ecx, eax, 0x17
        and    ecx, 7
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_6FFF:
        js     loc_5_6FDB
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6DDA
loc_5_7008:
        test   ah, 0x40
        je     loc_5_708A
        shld   edx, eax, 0x11
        or     dl, 0x80
        dec    ebp
loc_5_7015:
        js     loc_5_6FFF
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
loc_5_701F:
        test   al, 0x40
        jne    loc_5_704D
        shld   edx, eax, 0x19
        and    dl, 0x7f
        dec    ebp
loc_5_702B:
        js     loc_5_7015
        mov    byte ptr [edi], dl
        inc    edi
        test   ah, 0x40
        jne    loc_5_7274
        shld   edx, eax, 0x11
        and    dl, 0x7f
        dec    ebp
loc_5_7041:
        js     loc_5_702B
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_701F
loc_5_704D:
        test   al, 0x80
        je     loc_5_70BE
        mov    dl, ah
        or     dl, 0x80
        dec    ebp
loc_5_7057:
        js     loc_5_7041
        mov    byte ptr [edi], dl
        inc    edi
loc_5_705C:
        test   ah, 0x80
        jne    loc_5_64EE
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        mov    dl, al
        and    dl, 0x7f
        dec    ebp
loc_5_7070:
        js     loc_5_7057
        mov    byte ptr [edi], dl
        inc    edi
        test   al, 0x80
        jne    loc_5_62D2
        mov    dl, ah
        and    dl, 0x7f
        dec    ebp
loc_5_7083:
        js     loc_5_7070
        mov    byte ptr [edi], dl
        inc    edi
        jmp    loc_5_705C
loc_5_708A:
        test   ah, 0x80
        jne    loc_5_70F0
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        mov    edx, eax
        and    edx, 0x3f
loc_5_7099:
        test   al, 0x40
        je     loc_5_710B
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_70A4:
        js     loc_5_7083
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_7289
loc_5_70BE:
        test   ah, 1
        jne    loc_5_7126
        shld   edx, eax, 0x17
        and    edx, 0x3f
loc_5_70CA:
        test   ah, 0x80
        je     loc_5_7142
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_70D6:
        js     loc_5_70A4
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_62E0
loc_5_70F0:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 1
        jne    loc_5_7168
        shld   edx, eax, 0x1f
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_638A
loc_5_710B:
        test   al, 0x80
        je     loc_5_718B
        mov    cl, ah
        and    ecx, 1
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_711D:
        js     loc_5_70D6
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6317
loc_5_7126:
        test   ah, 2
        jne    loc_5_71A9
        shld   edx, eax, 0x16
        and    edx, 0xff
        add    edx, 0x40
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_67B1
loc_5_7142:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 1
        je     loc_5_71D1
        shld   ecx, eax, 0x1f
        and    ecx, 1
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_715F:
        js     loc_5_711D
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_672F
loc_5_7168:
        shld   edx, eax, 0x1f
        and    edx, 0xfff
        add    edx, 0x140
        cmp    edx, 0x113f
        jne    loc_5_6C57
        mov    cl, 0x34
        jmp    loc_5_62A8
loc_5_718B:
        test   ah, 1
        je     loc_5_71EE
        shld   ecx, eax, 0x17
        and    ecx, 3
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_71A0:
        js     loc_5_715F
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_676E
loc_5_71A9:
        shld   edx, eax, 0x16
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_7099
        mov    cl, 0x18
        jmp    loc_5_62A8
loc_5_71D1:
        test   al, 2
        je     loc_5_7212
        shld   ecx, eax, 0x1e
        and    ecx, 3
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_71E5:
        js     loc_5_71A0
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6BA5
loc_5_71EE:
        test   ah, 2
        mov    cl, 0xb
        je     loc_5_745C
        shld   ecx, eax, 0x16
        and    ecx, 7
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_7209:
        js     loc_5_71E5
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6BE4
loc_5_7212:
        test   al, 4
        mov    cl, 4
        je     loc_5_745C
        shld   ecx, eax, 0x1d
        and    ecx, 7
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_722C:
        js     loc_5_7209
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_701F
loc_5_7235:
        test   al, 0x40
        je     loc_5_72B7
        shld   edx, eax, 0x19
        or     dl, 0x80
        dec    ebp
loc_5_7241:
        js     loc_5_722C
        mov    byte ptr [edi], dl
        inc    edi
loc_5_7246:
        test   ah, 0x40
        jne    loc_5_7274
        shld   edx, eax, 0x11
        and    dl, 0x7f
        dec    ebp
loc_5_7253:
        js     loc_5_7241
        mov    byte ptr [edi], dl
        inc    edi
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 0x40
        jne    loc_5_704D
        shld   edx, eax, 0x19
        and    dl, 0x7f
        dec    ebp
loc_5_726D:
        js     loc_5_7253
        mov    byte ptr [edi], dl
        inc    edi
        jmp    loc_5_7246
loc_5_7274:
        test   ah, 0x80
        je     loc_5_72E6
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        mov    dl, al
        or     dl, 0x80
        dec    ebp
loc_5_7284:
        js     loc_5_726D
        mov    byte ptr [edi], dl
        inc    edi
loc_5_7289:
        test   al, 0x80
        jne    loc_5_62D2
        mov    dl, ah
        and    dl, 0x7f
        dec    ebp
loc_5_7297:
        js     loc_5_7284
        mov    byte ptr [edi], dl
        inc    edi
        test   ah, 0x80
        jne    loc_5_64EE
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        mov    dl, al
        and    dl, 0x7f
        dec    ebp
loc_5_72B0:
        js     loc_5_7297
        mov    byte ptr [edi], dl
        inc    edi
        jmp    loc_5_7289
loc_5_72B7:
        test   al, 0x80
        jne    loc_5_731B
        mov    dl, ah
        and    edx, 0x3f
loc_5_72C0:
        test   ah, 0x40
        je     loc_5_7337
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_72CC:
        js     loc_5_72B0
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_705C
loc_5_72E6:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 1
        jne    loc_5_735A
        shld   edx, eax, 0x1f
        and    edx, 0x3f
loc_5_72F6:
        test   al, 0x80
        je     loc_5_7370
        mov    esi, edi
        sub    esi, edx
        sub    ebp, 3
loc_5_7301:
        js     loc_5_72CC
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [edi + 1], cl
        mov    cl, byte ptr [esi + 2]
        mov    byte ptr [edi + 2], cl
        add    edi, 3
        jmp    loc_5_6500
loc_5_731B:
        test   ah, 1
        jne    loc_5_7392
        shld   edx, eax, 0x17
        and    edx, 0xff
        add    edx, 0x40
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        jmp    loc_5_65B1
loc_5_7337:
        test   ah, 0x80
        je     loc_5_73BA
        shld   ecx, eax, 0x10
        and    ecx, 1
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_7351:
        js     loc_5_7301
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_653D
loc_5_735A:
        test   al, 2
        jne    loc_5_73DC
        shld   edx, eax, 0x1e
        and    edx, 0xff
        add    edx, 0x40
        jmp    loc_5_69E6
loc_5_7370:
        test   ah, 1
        je     loc_5_73FF
        shld   ecx, eax, 0x17
        and    ecx, 1
        add    ecx, 4
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_7389:
        js     loc_5_7351
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6964
loc_5_7392:
        shld   edx, eax, 0x17
        and    edx, 0xfff
        add    edx, 0x140
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        cmp    edx, 0x113f
        jne    loc_5_6E93
        mov    cl, 0x14
        jmp    loc_5_62A8
loc_5_73BA:
        mov    eax, dword ptr [ebx]
        add    ebx, 2
        test   al, 1
        je     loc_5_741D
        shld   ecx, eax, 0x1f
        and    ecx, 3
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_73D3:
        js     loc_5_7389
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_69A9
loc_5_73DC:
        shld   edx, eax, 0x1e
        and    edx, 0xfff
        add    edx, 0x140
        cmp    edx, 0x113f
        jne    loc_5_72C0
        mov    cl, 0x38
        jmp    loc_5_62A8
loc_5_73FF:
        test   ah, 2
        je     loc_5_743C
        shld   ecx, eax, 0x16
        and    ecx, 3
        add    ecx, 6
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_7414:
        js     loc_5_73D3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6DDA
loc_5_741D:
        test   al, 2
        mov    cl, 3
        je     loc_5_745C
        shld   ecx, eax, 0x1e
        and    ecx, 7
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_7433:
        js     loc_5_7414
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_6E1F
loc_5_743C:
        test   ah, 4
        mov    cl, 0xc
        je     loc_5_745C
        shld   ecx, eax, 0x15
        and    ecx, 7
        add    ecx, 0xa
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
loc_5_7453:
        js     loc_5_7433
        rep movsb byte ptr es:[edi], byte ptr [esi]
        jmp    loc_5_7246
loc_5_745C:
        shr    eax, cl
        jae    loc_5_749C
        add    cl, 4
        and    eax, 0xf
        xchg   ecx, eax
        add    ecx, 0x12
loc_5_746A:
        mov    esi, edi
        sub    esi, edx
        sub    ebp, ecx
        js     loc_5_7453
        cmp    edx, 4
        jb     loc_5_7483
        mov    edx, ecx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
loc_5_7483:
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    cl, al
        cmp    ecx, 0x10
        jbe    loc_5_7492
        add    ebx, 2
        sub    ecx, 0x10
loc_5_7492:
        mov    eax, dword ptr [ebx - 2]
        jmp    loc_5_624C
loc_5_749C:
        shr    eax, 1
        jae    loc_5_74AD
        add    cl, 6
        and    eax, 0x1f
        xchg   ecx, eax
        add    ecx, 0x22
        jmp    loc_5_746A
loc_5_74AD:
        shr    eax, 1
        jae    loc_5_74BE
        add    cl, 8
        and    eax, 0x3f
        xchg   ecx, eax
        add    ecx, 0x42
        jmp    loc_5_746A
loc_5_74BE:
        shr    eax, 1
        jae    loc_5_74D2
        add    cl, 0xa
        and    eax, 0x7f
        xchg   ecx, eax
        add    ecx, 0x82
        jmp    loc_5_746A
loc_5_74D2:
        shr    eax, 1
        jae    loc_5_62BF
        add    cl, 0xc
        and    eax, 0xff
        xchg   ecx, eax
        add    ecx, 0x102
        jmp    loc_5_746A
loc_5_74EF:
        jmp    loc_5_62BF
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  003h, 000h, 000h, 000h, 007h, 000h, 000h, 000h, 00Fh, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h
        db  03Fh, 000h, 000h, 000h, 07Fh, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h, 0FFh, 001h, 000h, 000h
        db  0FFh, 003h, 000h, 000h, 0FFh, 007h, 000h, 000h, 0FFh, 00Fh, 000h, 000h, 0FFh, 01Fh, 000h, 000h
        db  0FFh, 03Fh, 000h, 000h, 0FFh, 07Fh, 000h, 000h, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_5_7578:
        pop    es
        add    byte ptr [eax], al
        db  000h
loc_5_757C:
        add    eax, 0x10000000
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [edx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [0xb000000], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        or     al, 0
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        or     eax, 0x2000000
        add    byte ptr [eax], al
        add    byte ptr [esi], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax + eax], al
        add    eax, 0x7000600
        add    byte ptr [eax], cl
        add    byte ptr [ecx], cl
        add    byte ptr [edx], cl
        add    byte ptr [ebx], cl
        add    byte ptr [0x11000f00], cl
        add    byte ptr [ebx], dl
        add    byte ptr [edi], dl
        add    byte ptr [ebx], bl
        add    byte ptr [edi], bl
        add    byte ptr [ebx], ah
        add    byte ptr [ebx], ch
        add    byte ptr [ebx], dh
        add    byte ptr [ebx], bh
        add    byte ptr [ebx], al
        push   ebx
        add    byte ptr [ebx], ah
        jae    loc_5_75FC
loc_5_75FC:
        add    dword ptr [eax], -0x5d
        add    bl, al
        add    bl, ah
        add    byte ptr [edx], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [edx], al
        add    byte ptr [edx], al
        add    byte ptr [edx], al
        add    byte ptr [edx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [eax + eax], al
        add    al, 0
        add    al, 0
        add    al, 0
        add    eax, 0x5000500
        add    byte ptr [0x63000000], al
        add    byte ptr [ebx], ah
loc_5_7648:
        add    dword ptr [eax], eax
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    al, 0
        add    eax, 0x9000700
        add    byte ptr [0x19001100], cl
        add    byte ptr [ecx], ah
        add    byte ptr [ecx], dh
        add    byte ptr [ecx], al
        popal  
        add    byte ptr [ecx + 0x100c100], al
        add    dword ptr [ecx + 0x1020101], eax
        add    eax, dword ptr [ecx]
        add    al, 1
        push   es
        add    dword ptr [eax], ecx
        add    dword ptr [ecx + eax], ecx
        adc    byte ptr [ecx], al
        sbb    byte ptr [ecx], al
        and    byte ptr [ecx], al
        xor    byte ptr [ecx], al
        inc    eax
        add    dword ptr [eax], esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [edx], al
        add    byte ptr [edx], al
        add    byte ptr [ebx], al
        add    byte ptr [ebx], al
        add    byte ptr [eax + eax], al
        add    al, 0
        add    eax, 0x6000500
        add    byte ptr [esi], al
        add    byte ptr [edi], al
        add    byte ptr [edi], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], cl
        add    byte ptr [ecx], cl
        add    byte ptr [ecx], cl
        add    byte ptr [edx], cl
        add    byte ptr [edx], cl
        add    byte ptr [ebx], cl
        add    byte ptr [ebx], cl
        add    byte ptr [eax + eax], cl
        or     al, 0
        or     eax, 0xd00
        mov    eax, eax
loc_5_76C4:
        cmp    dword ptr [esi + 0x10], 0
        jne    loc_5_76FC
        inc    byte ptr [0x76c0]             ; -> obj5:0x76C0
        mov    dx, 0x4000
        mov    dl, byte ptr [0x76c0]         ; -> obj5:0x76C0
        mov    word ptr [esi + 0x10], dx
        mov    eax, 0x8000
        movzx  edx, word ptr [esi + 0xc]
        push   edx
        push   dword ptr [esi + 8]
        push   eax
        push   dword ptr [esi]
        call   sub_5_82A8
        add    esp, 0x10
        or     eax, eax
        je     loc_5_7742
        jmp    loc_5_7768
loc_5_76FC:
        mov    al, byte ptr [esi + 0x10]
        cmp    byte ptr [0x76c0], al         ; -> obj5:0x76C0
        je     loc_5_7742
        movzx  edi, word ptr [esi + 0x12]
        mov    ebx, dword ptr [esi + 8]
        sub    ebx, edi
        movzx  eax, word ptr [esi + 4]
        mov    edx, 0x8000
        push   edx
        push   ebx
        push   eax
        push   dword ptr [esi]
        call   sub_5_82A8
        add    esp, 0x10
        or     eax, eax
        jne    loc_5_7768
        push   edi
        mov    eax, esp
        push   eax
        call   sub_5_831F
        pop    ecx
        pop    edx
        or     eax, eax
        jne    loc_5_7768
        mov    eax, 5
        cmp    edx, edi
        jne    loc_5_7768
loc_5_7742:
        movzx  edi, word ptr [esi + 0xc]
        push   edi
        mov    eax, esp
        push   eax
        call   sub_5_831F
        pop    ecx
        pop    edx
        or     eax, eax
        jne    loc_5_7768
        mov    eax, 5
        cmp    edx, edi
        jne    loc_5_7768
        add    word ptr [esi + 0x12], di
        add    dword ptr [esi + 8], edi
        xor    eax, eax
loc_5_7767:
        ret    
loc_5_7768:
        and    dword ptr [esi + 0x10], 0
        jmp    loc_5_7767
sub_5_776E:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    ebx, ebx
        mov    ebp, dword ptr [0x750c]       ; -> obj5:0x750C
        mov    ch, byte ptr [0x7572]         ; -> obj5:0x7572
        mov    edi, dword ptr [0x74f4]       ; -> obj5:0x74F4
        mov    edx, dword ptr [esp + 0x28]
        mov    eax, dword ptr [edx*4 + 0x752c] ; -> obj5:0x752C
        mov    dword ptr [esp + 0x10], eax
        mov    edx, dword ptr [esp + 0x2c]
        mov    eax, dword ptr [edx*4 + 0x752c] ; -> obj5:0x752C
        mov    dword ptr [esp + 0x14], eax
        cmp    byte ptr [esp + 0x30], 0
        je     loc_5_77C8
        cmp    byte ptr [0x7573], 0          ; -> obj5:0x7573
        je     loc_5_77C8
        mov    esi, dword ptr [0x7524]       ; -> obj5:0x7524
        mov    edx, dword ptr [0x7528]       ; -> obj5:0x7528
        jmp    loc_5_79E1
loc_5_77C8:
        mov    esi, dword ptr [0x74fc]       ; -> obj5:0x74FC
loc_5_77CE:
        cmp    ch, byte ptr [esp + 0x28]
        jae    loc_5_77F5
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_77EA
        lodsb  al, byte ptr [esi]
loc_5_77DF:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_77CE
loc_5_77EA:
        ja     loc_5_7A33
        xor    eax, eax
        inc    esi
        jmp    loc_5_77DF
loc_5_77F5:
        mov    eax, dword ptr [esp + 0x10]
        and    eax, ebp
        shl    eax, 2
        mov    edx, dword ptr [esp + 0x20]
        add    edx, eax
        mov    bl, byte ptr [edx]
loc_5_7806:
        cmp    bl, 0x10
        jbe    loc_5_785F
        cmp    bl, 0x63
        je     loc_5_7A33
        mov    cl, byte ptr [edx + 1]
        shr    ebp, cl
        sub    ch, cl
        sub    bl, 0x10
loc_5_781E:
        cmp    ch, bl
        jae    loc_5_7843
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_7838
        lodsb  al, byte ptr [esi]
loc_5_782D:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_781E
loc_5_7838:
        ja     loc_5_7A33
        xor    eax, eax
        inc    esi
        jmp    loc_5_782D
loc_5_7843:
        xor    eax, eax
        mov    ax, word ptr [edx + 2]
        mov    edx, dword ptr [ebx*4 + 0x752c] ; -> obj5:0x752C
        and    edx, ebp
        add    edx, eax
        shl    edx, 2
        add    edx, dword ptr [esp + 0x20]
        mov    bl, byte ptr [edx]
        jmp    loc_5_7806
loc_5_785F:
        mov    cl, byte ptr [edx + 1]
        shr    ebp, cl
        sub    ch, cl
        cmp    bl, 0x10
        jne    loc_5_78B2
        mov    al, byte ptr [edx + 2]
        stosb  byte ptr es:[edi], al
        dec    dword ptr [0x7504]            ; -> obj5:0x7504
        jne    loc_5_77CE
        mov    byte ptr [0x7573], 0          ; -> obj5:0x7573
loc_5_7882:
        mov    byte ptr [0x7570], 2          ; -> obj5:0x7570
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [0x7514], eax       ; -> obj5:0x7514
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [0x7518], eax       ; -> obj5:0x7518
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [0x751c], eax       ; -> obj5:0x751C
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [0x7520], eax       ; -> obj5:0x7520
        jmp    loc_5_7A11
loc_5_78B2:
        cmp    bl, 0xf
        je     loc_5_7A11
loc_5_78BB:
        cmp    ch, bl
        jae    loc_5_78E0
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_78D5
        lodsb  al, byte ptr [esi]
loc_5_78CA:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_78BB
loc_5_78D5:
        ja     loc_5_7A33
        xor    eax, eax
        inc    esi
        jmp    loc_5_78CA
loc_5_78E0:
        mov    eax, dword ptr [ebx*4 + 0x752c] ; -> obj5:0x752C
        and    eax, ebp
        movzx  edx, word ptr [edx + 2]
        add    eax, edx
        mov    dword ptr [esp + 0x18], eax
        mov    cl, bl
        shr    ebp, cl
        sub    ch, cl
loc_5_78F9:
        cmp    ch, byte ptr [esp + 0x2c]
        jae    loc_5_7920
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_7915
        lodsb  al, byte ptr [esi]
loc_5_790A:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_78F9
loc_5_7915:
        ja     loc_5_7A33
        xor    eax, eax
        inc    esi
        jmp    loc_5_790A
loc_5_7920:
        mov    edx, ebp
        and    edx, dword ptr [esp + 0x14]
        shl    edx, 2
        add    edx, dword ptr [esp + 0x24]
        mov    bl, byte ptr [edx]
loc_5_792F:
        cmp    bl, 0x10
        jbe    loc_5_7988
        cmp    bl, 0x63
        je     loc_5_7A33
        mov    cl, byte ptr [edx + 1]
        shr    ebp, cl
        sub    ch, cl
        sub    bl, 0x10
loc_5_7947:
        cmp    ch, bl
        jae    loc_5_796C
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_7961
        lodsb  al, byte ptr [esi]
loc_5_7956:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_7947
loc_5_7961:
        ja     loc_5_7A33
        xor    eax, eax
        inc    esi
        jmp    loc_5_7956
loc_5_796C:
        xor    eax, eax
        mov    ax, word ptr [edx + 2]
        mov    edx, dword ptr [ebx*4 + 0x752c] ; -> obj5:0x752C
        and    edx, ebp
        add    edx, eax
        shl    edx, 2
        add    edx, dword ptr [esp + 0x24]
        mov    bl, byte ptr [edx]
        jmp    loc_5_792F
loc_5_7988:
        mov    cl, byte ptr [edx + 1]
        shr    ebp, cl
        sub    ch, cl
loc_5_798F:
        cmp    ch, bl
        jae    loc_5_79B4
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_79A9
        lodsb  al, byte ptr [esi]
loc_5_799E:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_798F
loc_5_79A9:
        ja     loc_5_7A33
        xor    eax, eax
        inc    esi
        jmp    loc_5_799E
loc_5_79B4:
        xor    eax, eax
        mov    ax, word ptr [edx + 2]
        mov    edx, dword ptr [ebx*4 + 0x752c] ; -> obj5:0x752C
        and    edx, ebp
        add    eax, edx
        mov    cl, bl
        shr    ebp, cl
        sub    ch, cl
        mov    edx, dword ptr [esp + 0x18]
        mov    dword ptr [0x74fc], esi       ; -> obj5:0x74FC
        mov    esi, edi
        sub    esi, eax
        cmp    esi, dword ptr [0x74f8]       ; -> obj5:0x74F8
        jb     loc_5_7A33
loc_5_79E1:
        sub    edx, 1
        jb     loc_5_77C8
        movsb  byte ptr es:[edi], byte ptr [esi]
        dec    dword ptr [0x7504]            ; -> obj5:0x7504
        jne    loc_5_79E1
        mov    dword ptr [0x7528], edx       ; -> obj5:0x7528
        mov    dword ptr [0x7524], esi       ; -> obj5:0x7524
        mov    esi, dword ptr [0x74fc]       ; -> obj5:0x74FC
        mov    byte ptr [0x7573], 1          ; -> obj5:0x7573
        jmp    loc_5_7882
loc_5_7A11:
        mov    dword ptr [0x750c], ebp       ; -> obj5:0x750C
        mov    byte ptr [0x7572], ch         ; -> obj5:0x7572
        mov    dword ptr [0x74f4], edi       ; -> obj5:0x74F4
        mov    dword ptr [0x74fc], esi       ; -> obj5:0x74FC
        xor    eax, eax
loc_5_7A2B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_5_7A33:
        mov    eax, 1
        jmp    loc_5_7A2B
sub_5_7A3A:
        sub    esp, 0x30
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, 0x11
        xor    eax, eax
        lea    edi, [0xa970]                 ; -> obj5:0xA970
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [esp + 0x3c], eax
        mov    ecx, dword ptr [esp + 0x44]
        mov    eax, dword ptr [esp + 0x48]
loc_5_7A5C:
        mov    esi, dword ptr [ecx]
        add    ecx, 4
        inc    dword ptr [esi*4 + 0xa970]    ; -> obj5:0xA970
        dec    eax
        jne    loc_5_7A5C
        mov    eax, dword ptr [esp + 0x48]
        cmp    eax, dword ptr [0xa970]       ; -> obj5:0xA970
        je     loc_5_7D5D
        mov    edx, dword ptr [esp + 0x60]
        mov    ebx, dword ptr [edx]
        mov    ecx, 1
        lea    eax, [0xa970]                 ; -> obj5:0xA970
loc_5_7A8C:
        cmp    dword ptr [eax + ecx*4], 0
        jne    loc_5_7A98
        inc    ecx
        cmp    ecx, 0x10
        jbe    loc_5_7A8C
loc_5_7A98:
        mov    dword ptr [esp + 0x14], ecx
        cmp    ebx, ecx
        jae    loc_5_7AA2
        mov    ebx, ecx
loc_5_7AA2:
        mov    esi, 0x10
loc_5_7AA7:
        cmp    dword ptr [eax + esi*4], 0
        jne    loc_5_7AB2
        sub    esi, 1
        jne    loc_5_7AA7
loc_5_7AB2:
        mov    dword ptr [esp + 0x18], esi
        cmp    ebx, esi
        jbe    loc_5_7ABC
        mov    ebx, esi
loc_5_7ABC:
        mov    dword ptr [edx], ebx
        mov    dword ptr [esp + 0x24], ebx
        mov    edx, 1
        shl    edx, cl
loc_5_7AC9:
        cmp    ecx, esi
        jae    loc_5_7ADB
        sub    edx, dword ptr [eax + ecx*4]
        jl     loc_5_7D67
        inc    ecx
        shl    edx, 1
        jmp    loc_5_7AC9
loc_5_7ADB:
        sub    edx, dword ptr [eax + esi*4]
        jl     loc_5_7D67
        add    dword ptr [eax + esi*4], edx
        mov    dword ptr [esp + 0x1c], edx
        lea    edi, [0xa9b4]                 ; -> obj5:0xA9B4
        xor    ecx, ecx
        add    eax, 4
        mov    dword ptr [edi + 4], ecx
        lea    ebx, [edi + 8]
loc_5_7AFC:
        dec    esi
        je     loc_5_7B0B
        add    ecx, dword ptr [eax]
        add    eax, 4
        mov    dword ptr [ebx], ecx
        add    ebx, 4
        jmp    loc_5_7AFC
loc_5_7B0B:
        mov    dword ptr [esp + 0x20], ebx
        mov    esi, dword ptr [esp + 0x48]
        mov    ebx, dword ptr [esp + 0x44]
        xor    ecx, ecx
loc_5_7B19:
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        test   eax, eax
        je     loc_5_7B35
        lea    eax, [eax*4 + 0xa9b4]         ; -> obj5:0xA9B4
        mov    edi, dword ptr [eax]
        mov    dword ptr [edi*4 + 0xa070], ecx ; -> obj5:0xA070
        inc    edi
        mov    dword ptr [eax], edi
loc_5_7B35:
        inc    ecx
        cmp    ecx, esi
        jb     loc_5_7B19
        mov    ecx, dword ptr [esp + 0x14]
        lea    edx, [0xa070]                 ; -> obj5:0xA070
        mov    ebp, dword ptr [esp + 0x24]
        mov    esi, 0xffffffff
        xor    edi, edi
        mov    dword ptr [0xa4f0], esi       ; -> obj5:0xA4F0
        mov    dword ptr [esp + 0x28], edi
        mov    dword ptr [esp + 0x2c], edx
        mov    dword ptr [esp + 0x30], esi
        neg    ebp
        mov    dword ptr [esp + 0x34], edi
        mov    dword ptr [0xa9b4], edi       ; -> obj5:0xA9B4
loc_5_7B6D:
        cmp    ecx, dword ptr [esp + 0x18]
        ja     loc_5_7D40
        mov    eax, dword ptr [ecx*4 + 0xa970] ; -> obj5:0xA970
        mov    dword ptr [esp + 0x10], eax
loc_5_7B82:
        dec    dword ptr [esp + 0x10]
        test   eax, eax
        je     loc_5_7D3A
loc_5_7B8E:
        mov    eax, dword ptr [esp + 0x24]
        add    eax, ebp
        cmp    ecx, eax
        jbe    loc_5_7C5D
        inc    dword ptr [esp + 0x30]
        add    ebp, dword ptr [esp + 0x24]
        mov    edx, dword ptr [esp + 0x18]
        sub    edx, ebp
        cmp    edx, dword ptr [esp + 0x24]
        jbe    loc_5_7BB4
        mov    edx, dword ptr [esp + 0x24]
loc_5_7BB4:
        mov    ebx, ecx
        sub    ebx, ebp
        mov    eax, 1
        xchg   ebx, ecx
        shl    eax, cl
        mov    edi, dword ptr [esp + 0x10]
        inc    edi
        cmp    eax, edi
        jbe    loc_5_7BE5
        sub    eax, edi
        lea    esi, [ebx*4 + 0xa970]         ; -> obj5:0xA970
loc_5_7BD3:
        inc    ecx
        cmp    ecx, edx
        jae    loc_5_7BE5
        shl    eax, 1
        add    esi, 4
        cmp    eax, dword ptr [esi]
        jbe    loc_5_7BE5
        sub    eax, dword ptr [esi]
        jmp    loc_5_7BD3
loc_5_7BE5:
        mov    edx, 1
        shl    edx, cl
        mov    edi, dword ptr [esp + 0x3c]
        mov    eax, edi
        add    edi, edx
        cmp    edi, dword ptr [esp + 0x5c]
        ja     loc_5_7D6E
        mov    dword ptr [esp + 0x3c], edi
        mov    edi, dword ptr [esp + 0x30]
        mov    dword ptr [edi*4 + 0xa4f0], eax ; -> obj5:0xA4F0
        test   edi, edi
        je     loc_5_7C56
        mov    word ptr [esp + 0x3a], ax
        mov    dword ptr [esp + 0x34], eax
        mov    eax, dword ptr [esp + 0x24]
        mov    byte ptr [esp + 0x39], al
        mov    eax, ecx
        add    eax, 0x10
        mov    byte ptr [esp + 0x38], al
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [edi*4 + 0xa9b4], eax ; -> obj5:0xA9B4
        mov    ecx, ebp
        sub    ecx, dword ptr [esp + 0x24]
        shr    eax, cl
        mov    ecx, eax
        mov    eax, dword ptr [edi*4 + 0xa4ec] ; -> obj5:0xA4EC
        mov    esi, dword ptr [esp + 0x38]
        add    eax, ecx
        shl    eax, 2
        add    eax, dword ptr [esp + 0x58]
        mov    dword ptr [eax], esi
loc_5_7C56:
        xchg   ecx, ebx
        jmp    loc_5_7B8E
loc_5_7C5D:
        mov    edi, dword ptr [esp + 0x2c]
        mov    eax, ecx
        sub    eax, ebp
        mov    byte ptr [esp + 0x39], al
        mov    al, 0x63
        mov    esi, dword ptr [esp + 0x48]
        lea    esi, [esi*4 + 0xa070]         ; -> obj5:0xA070
        cmp    edi, esi
        jae    loc_5_7CAB
        mov    esi, dword ptr [edi]
        add    edi, 4
        cmp    esi, dword ptr [esp + 0x4c]
        jae    loc_5_7C93
        mov    al, 0xf
        cmp    esi, 0x100
        jae    loc_5_7CA6
        inc    al
        jmp    loc_5_7CA6
loc_5_7C93:
        sub    esi, dword ptr [esp + 0x4c]
        mov    ebx, dword ptr [esp + 0x54]
        mov    al, byte ptr [ebx + esi*2]
        mov    ebx, dword ptr [esp + 0x50]
        mov    si, word ptr [ebx + esi*2]
loc_5_7CA6:
        mov    word ptr [esp + 0x3a], si
loc_5_7CAB:
        mov    byte ptr [esp + 0x38], al
        mov    dword ptr [esp + 0x2c], edi
        mov    eax, 1
        mov    esi, ecx
        sub    ecx, ebp
        shl    eax, cl
        mov    ebx, dword ptr [esp + 0x28]
        mov    ecx, ebp
        mov    edi, dword ptr [esp + 0x38]
        shr    ebx, cl
loc_5_7CCA:
        cmp    ebx, edx
        jae    loc_5_7CE1
        mov    ecx, dword ptr [esp + 0x34]
        add    ecx, ebx
        shl    ecx, 2
        add    ecx, dword ptr [esp + 0x58]
        mov    dword ptr [ecx], edi
        add    ebx, eax
        jmp    loc_5_7CCA
loc_5_7CE1:
        mov    ecx, esi
        mov    ebx, 1
        dec    ecx
        shl    ebx, cl
        mov    edi, dword ptr [esp + 0x28]
loc_5_7CEF:
        mov    eax, edi
        and    eax, ebx
        je     loc_5_7CFB
        xor    edi, ebx
        shr    ebx, 1
        jmp    loc_5_7CEF
loc_5_7CFB:
        xor    edi, ebx
        mov    dword ptr [esp + 0x14], esi
        mov    eax, dword ptr [esp + 0x30]
        lea    esi, [0xa9b4]                 ; -> obj5:0xA9B4
        mov    ecx, ebp
loc_5_7D0D:
        mov    ebx, 1
        shl    ebx, cl
        dec    ebx
        and    ebx, edi
        cmp    ebx, dword ptr [esi + eax*4]
        je     loc_5_7D23
        dec    eax
        sub    ecx, dword ptr [esp + 0x24]
        jmp    loc_5_7D0D
loc_5_7D23:
        mov    dword ptr [esp + 0x30], eax
        mov    ebp, ecx
        mov    dword ptr [esp + 0x28], edi
        mov    ecx, dword ptr [esp + 0x14]
        mov    eax, dword ptr [esp + 0x10]
        jmp    loc_5_7B82
loc_5_7D3A:
        inc    ecx
        jmp    loc_5_7B6D
loc_5_7D40:
        xor    eax, eax
        mov    ebx, dword ptr [esp + 0x1c]
        cmp    ebx, 0
        je     loc_5_7D55
        mov    ebx, dword ptr [esp + 0x18]
        cmp    ebx, 1
        je     loc_5_7D55
        inc    eax
loc_5_7D55:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x30
        ret    
loc_5_7D5D:
        xor    eax, eax
        mov    ebx, dword ptr [esp + 0x60]
        mov    dword ptr [ebx], ebx
        jmp    loc_5_7D55
loc_5_7D67:
        mov    eax, 2
        jmp    loc_5_7D55
loc_5_7D6E:
        mov    eax, 3
        jmp    loc_5_7D55
sub_5_7D75:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [0x750c]       ; -> obj5:0x750C
        mov    ch, byte ptr [0x7572]         ; -> obj5:0x7572
        mov    esi, dword ptr [0x74fc]       ; -> obj5:0x74FC
        cmp    ch, 5
        jae    loc_5_7DB4
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_7DA9
        lodsb  al, byte ptr [esi]
loc_5_7D9E:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_7DB4
loc_5_7DA9:
        ja     loc_5_80DF
        xor    eax, eax
        inc    esi
        jmp    loc_5_7D9E
loc_5_7DB4:
        mov    eax, ebp
        and    eax, 0x1f
        add    eax, 0x101
        cmp    eax, 0x11e
        ja     loc_5_80DF
        mov    dword ptr [esp + 0x20], eax
        mov    cl, 5
        shr    ebp, cl
        sub    ch, cl
        cmp    ch, 5
        jae    loc_5_7DF9
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_7DEE
        lodsb  al, byte ptr [esi]
loc_5_7DE3:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_7DF9
loc_5_7DEE:
        ja     loc_5_80DF
        xor    eax, eax
        inc    esi
        jmp    loc_5_7DE3
loc_5_7DF9:
        mov    eax, ebp
        and    eax, 0x1f
        inc    eax
        cmp    eax, 0x1e
        ja     loc_5_80DF
        mov    dword ptr [esp + 0x24], eax
        mov    cl, 5
        shr    ebp, cl
        sub    ch, cl
        cmp    ch, 4
        jae    loc_5_7E38
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_7E2D
        lodsb  al, byte ptr [esi]
loc_5_7E22:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_7E38
loc_5_7E2D:
        ja     loc_5_80DF
        xor    eax, eax
        inc    esi
        jmp    loc_5_7E22
loc_5_7E38:
        mov    eax, ebp
        and    eax, 0xf
        add    eax, 4
        mov    dword ptr [esp + 0x14], eax
        mov    cl, 4
        shr    ebp, cl
        sub    ch, cl
        xor    ebx, ebx
        lea    edi, [0x7580]                 ; -> obj5:0x7580
loc_5_7E52:
        cmp    ebx, dword ptr [esp + 0x14]
        jae    loc_5_7E96
        cmp    ch, 3
        jae    loc_5_7E7E
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_7E73
        lodsb  al, byte ptr [esi]
loc_5_7E68:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_7E7E
loc_5_7E73:
        ja     loc_5_80DF
        xor    eax, eax
        inc    esi
        jmp    loc_5_7E68
loc_5_7E7E:
        mov    eax, ebp
        mov    edx, dword ptr [edi + ebx*4]
        and    eax, 7
        mov    dword ptr [edx*4 + 0xa9f8], eax ; -> obj5:0xA9F8
        mov    cl, 3
        shr    ebp, cl
        sub    ch, cl
        inc    ebx
        jmp    loc_5_7E52
loc_5_7E96:
        xor    eax, eax
loc_5_7E98:
        cmp    ebx, 0x13
        jae    loc_5_7EAA
        mov    edx, dword ptr [edi + ebx*4]
        mov    dword ptr [edx*4 + 0xa9f8], eax ; -> obj5:0xA9F8
        inc    ebx
        jmp    loc_5_7E98
loc_5_7EAA:
        mov    ebx, ecx
        lea    eax, [esp + 0x18]
        mov    dword ptr [eax], 7
        lea    edx, [0xa9f8]                 ; -> obj5:0xA9F8
        push   eax
        push   0x320
        push   0x8d18                        ; -> obj5:0x8D18
        push   0
        push   0
        push   0x13
        push   0x13
        push   edx
        call   sub_5_7A3A
        add    esp, 0x20
        test   eax, eax
        jne    loc_5_80D7
        mov    ecx, ebx
        mov    edx, dword ptr [esp + 0x20]
        add    edx, dword ptr [esp + 0x24]
        xor    edi, edi
        mov    dword ptr [esp + 0x10], edi
loc_5_7EF0:
        cmp    edi, edx
        jae    loc_5_8031
        mov    ebx, dword ptr [esp + 0x18]
loc_5_7EFC:
        cmp    ch, bl
        jae    loc_5_7F21
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_7F16
        lodsb  al, byte ptr [esi]
loc_5_7F0B:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_7EFC
loc_5_7F16:
        ja     loc_5_80DF
        xor    eax, eax
        inc    esi
        jmp    loc_5_7F0B
loc_5_7F21:
        mov    eax, ebp
        and    eax, dword ptr [ebx*4 + 0x752c] ; -> obj5:0x752C
        shl    eax, 2
        add    eax, 0x8d18                   ; -> obj5:0x8D18
        mov    cl, byte ptr [eax + 1]
        shr    ebp, cl
        sub    ch, cl
        movzx  ebx, word ptr [eax + 2]
        cmp    ebx, 0x10
        jae    loc_5_7F50
        mov    dword ptr [esp + 0x10], ebx
        mov    dword ptr [edi*4 + 0xa9f8], ebx ; -> obj5:0xA9F8
        inc    edi
        jmp    loc_5_7EF0
loc_5_7F50:
        jne    loc_5_7FA9
        cmp    ch, 2
        jae    loc_5_7F78
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_7F6D
        lodsb  al, byte ptr [esi]
loc_5_7F62:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_7F78
loc_5_7F6D:
        ja     loc_5_80DF
        xor    eax, eax
        inc    esi
        jmp    loc_5_7F62
loc_5_7F78:
        mov    eax, ebp
        and    eax, 3
        add    eax, 3
        mov    ebx, eax
        mov    cl, 2
        shr    ebp, cl
        sub    ch, cl
loc_5_7F88:
        add    eax, edi
        cmp    eax, edx
        ja     loc_5_80E6
        mov    eax, dword ptr [esp + 0x10]
loc_5_7F96:
        test   ebx, ebx
        je     loc_5_7EF0
        mov    dword ptr [edi*4 + 0xa9f8], eax ; -> obj5:0xA9F8
        inc    edi
        dec    ebx
        jmp    loc_5_7F96
loc_5_7FA9:
        cmp    ebx, 0x11
        jne    loc_5_7FEE
        cmp    ch, 3
        jae    loc_5_7FD4
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_7FC9
        lodsb  al, byte ptr [esi]
loc_5_7FBE:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_7FD4
loc_5_7FC9:
        ja     loc_5_80DF
        xor    eax, eax
        inc    esi
        jmp    loc_5_7FBE
loc_5_7FD4:
        mov    eax, ebp
        and    eax, 7
        add    eax, 3
        mov    ebx, eax
        mov    cl, 3
        shr    ebp, cl
        sub    ch, cl
        mov    dword ptr [esp + 0x10], 0
        jmp    loc_5_7F88
loc_5_7FEE:
        cmp    ch, 7
        jae    loc_5_8014
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_8009
        lodsb  al, byte ptr [esi]
loc_5_7FFE:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_8014
loc_5_8009:
        ja     loc_5_80DF
        xor    eax, eax
        inc    esi
        jmp    loc_5_7FFE
loc_5_8014:
        mov    eax, ebp
        and    eax, 0x7f
        add    eax, 0xb
        mov    ebx, eax
        mov    cl, 7
        shr    ebp, cl
        sub    ch, cl
        mov    dword ptr [esp + 0x10], 0
        jmp    loc_5_7F88
loc_5_8031:
        mov    dword ptr [0x750c], ebp       ; -> obj5:0x750C
        mov    byte ptr [0x7572], ch         ; -> obj5:0x7572
        mov    dword ptr [0x74fc], esi       ; -> obj5:0x74FC
        lea    eax, [esp + 0x18]
        mov    ebx, dword ptr [esp + 0x20]
        lea    edx, [0xa9f8]                 ; -> obj5:0xA9F8
        mov    dword ptr [eax], 9
        push   eax
        push   0x320
        push   0x8d18                        ; -> obj5:0x8D18
        push   0x760a                        ; -> obj5:0x760A
        push   0x75cc                        ; -> obj5:0x75CC
        push   0x101
        push   ebx
        push   edx
        call   sub_5_7A3A
        add    esp, 0x20
        test   eax, eax
        jne    loc_5_80DF
        lea    eax, [esp + 0x1c]
        mov    dword ptr [eax], 6
        mov    ecx, dword ptr [esp + 0x24]
        lea    ebx, [ebx*4 + 0xa9f8]         ; -> obj5:0xA9F8
        push   eax
        push   0x96
        push   0x9998                        ; -> obj5:0x9998
        push   0x7684                        ; -> obj5:0x7684
        push   0x7648                        ; -> loc_5_7648
        push   0
        push   ecx
        push   ebx
        call   sub_5_7A3A
        add    esp, 0x20
        test   eax, eax
        jne    loc_5_80DF
        mov    eax, dword ptr [esp + 0x1c]
        mov    ebx, dword ptr [esp + 0x18]
        push   0
        push   eax
        push   ebx
        push   0x9998                        ; -> obj5:0x9998
        push   0x8d18                        ; -> obj5:0x8D18
        call   sub_5_776E
        add    esp, 0x14
loc_5_80D7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
loc_5_80DF:
        mov    eax, 1
        jmp    loc_5_80D7
loc_5_80E6:
        mov    eax, 2
        jmp    loc_5_80D7
sub_5_80ED:
        cmp    byte ptr [esp + 4], 0
        push   esi
        push   edi
        mov    esi, dword ptr [0x74fc]       ; -> obj5:0x74FC
        jne    loc_5_81B8
        mov    cl, byte ptr [0x7572]         ; -> obj5:0x7572
        mov    ebp, dword ptr [0x750c]       ; -> obj5:0x750C
        and    cl, 7
        shr    ebp, cl
        sub    ch, cl
loc_5_8113:
        cmp    ch, 0x10
        jae    loc_5_8139
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_812E
        lodsb  al, byte ptr [esi]
loc_5_8123:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_8113
loc_5_812E:
        ja     loc_5_81C0
        xor    eax, eax
        inc    esi
        jmp    loc_5_8123
loc_5_8139:
        mov    edx, ebp
        and    edx, 0xffff
        mov    cl, 0x10
        shr    ebp, cl
        sub    ch, cl
        test   ch, ch
        jne    loc_5_81C0
        mov    byte ptr [0x7572], ch         ; -> obj5:0x7572
loc_5_8151:
        mov    edi, dword ptr [0x74f4]       ; -> obj5:0x74F4
        cmp    dword ptr [0x7504], edx       ; -> obj5:0x7504
        jb     loc_5_8184
        mov    ecx, edx
        sub    dword ptr [0x7504], edx       ; -> obj5:0x7504
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    dword ptr [0x74f4], edx       ; -> obj5:0x74F4
loc_5_8179:
        xor    eax, eax
loc_5_817B:
        mov    dword ptr [0x74fc], esi       ; -> obj5:0x74FC
        pop    edi
        pop    esi
        ret    
loc_5_8184:
        mov    eax, dword ptr [0x7504]       ; -> obj5:0x7504
        sub    edx, eax
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    byte ptr [0x7570], 1          ; -> obj5:0x7570
        mov    dword ptr [0x7510], edx       ; -> obj5:0x7510
        add    dword ptr [0x74f4], eax       ; -> obj5:0x74F4
        mov    dword ptr [0x7504], 0         ; -> obj5:0x7504
        jmp    loc_5_8179
loc_5_81B8:
        mov    edx, dword ptr [0x7510]       ; -> obj5:0x7510
        jmp    loc_5_8151
loc_5_81C0:
        mov    eax, 1
        jmp    loc_5_817B
sub_5_81C7:
        push   esi
        push   ebp
        mov    ebp, dword ptr [0x750c]       ; -> obj5:0x750C
        mov    ch, byte ptr [0x7572]         ; -> obj5:0x7572
        mov    esi, dword ptr [0x74fc]       ; -> obj5:0x74FC
        cmp    ch, 1
        jae    loc_5_81FD
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_81F6
        lodsb  al, byte ptr [esi]
loc_5_81EB:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_81FD
loc_5_81F6:
        ja     loc_5_8263
        xor    eax, eax
        inc    esi
        jmp    loc_5_81EB
loc_5_81FD:
        mov    eax, ebp
        and    eax, 1
        mov    byte ptr [0x7571], al         ; -> obj5:0x7571
        mov    cl, 1
        shr    ebp, cl
        sub    ch, cl
        cmp    ch, 2
        jae    loc_5_822F
        xor    eax, eax
        cmp    esi, dword ptr [0x7500]       ; -> obj5:0x7500
        jae    loc_5_8228
        lodsb  al, byte ptr [esi]
loc_5_821D:
        mov    cl, ch
        shl    eax, cl
        or     ebp, eax
        add    ch, 8
        jmp    loc_5_822F
loc_5_8228:
        ja     loc_5_8263
        xor    eax, eax
        inc    esi
        jmp    loc_5_821D
loc_5_822F:
        mov    eax, ebp
        and    eax, 3
        mov    cl, 2
        shr    ebp, cl
        sub    ch, cl
        mov    dword ptr [0x74fc], esi       ; -> obj5:0x74FC
        mov    dword ptr [0x750c], ebp       ; -> obj5:0x750C
        mov    byte ptr [0x7572], ch         ; -> obj5:0x7572
        jmp    dword ptr [eax*4 + 0x8253]    ; -> obj5:0x8253
        ; ---- dispatch table (4 entries) ----
        dd  loc_5_8271
        dd  loc_5_827D
        dd  loc_5_82A0
        dd  loc_5_826A
loc_5_8263:
        mov    eax, 1
        jmp    loc_5_82A5
loc_5_826A:
        mov    eax, 2
        jmp    loc_5_82A5
loc_5_8271:
        push   0
        call   sub_5_80ED
        add    esp, 4
        jmp    loc_5_82A5
loc_5_827D:
        push   0
        mov    eax, dword ptr [0x757c]       ; -> loc_5_757C
        push   eax
        mov    ecx, dword ptr [0x7578]       ; -> loc_5_7578
        push   ecx
        push   0x8c98                        ; -> obj5:0x8C98
        push   0x8478                        ; -> loc_5_8478
        call   sub_5_776E
        add    esp, 0x14
        jmp    loc_5_82A5
loc_5_82A0:
        call   sub_5_7D75
loc_5_82A5:
        pop    ebp
        pop    esi
        ret    
sub_5_82A8:
        cld    
        cmp    dword ptr [0x7574], 0         ; -> obj5:0x7574
        je     loc_5_8307
loc_5_82B2:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax], 0x5153
        jne    loc_5_8300
        mov    ecx, dword ptr [esp + 8]
        add    ecx, eax
        add    eax, 4
        mov    dword ptr [0x7500], ecx       ; -> obj5:0x7500
        mov    dword ptr [0x74fc], eax       ; -> obj5:0x74FC
        mov    eax, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 0x10]
        mov    dword ptr [0x74f8], eax       ; -> obj5:0x74F8
        mov    dword ptr [0x74f4], eax       ; -> obj5:0x74F4
        mov    dword ptr [0x7508], edx       ; -> obj5:0x7508
        xor    eax, eax
        mov    byte ptr [0x7572], al         ; -> obj5:0x7572
        mov    dword ptr [0x750c], eax       ; -> obj5:0x750C
        mov    byte ptr [0x7571], al         ; -> obj5:0x7571
        mov    byte ptr [0x7570], al         ; -> obj5:0x7570
loc_5_82FF:
        ret    
loc_5_8300:
        mov    eax, 5
        jmp    loc_5_82FF
loc_5_8307:
        call   sub_5_83CD
        test   eax, eax
        jne    loc_5_8318
        inc    dword ptr [0x7574]            ; -> obj5:0x7574
        jmp    loc_5_82B2
loc_5_8318:
        mov    eax, 1
        jmp    loc_5_82FF
sub_5_831F:
        push   ebx
        push   edi
        push   esi
        cld    
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi]
        cmp    dword ptr [0x7508], eax       ; -> obj5:0x7508
        ja     loc_5_8336
        mov    eax, dword ptr [0x7508]       ; -> obj5:0x7508
loc_5_8336:
        mov    dword ptr [0x7504], eax       ; -> obj5:0x7504
        mov    edi, eax
        test   eax, eax
        je     loc_5_83C5
        mov    al, byte ptr [0x7570]         ; -> obj5:0x7570
        test   al, al
        jne    loc_5_8376
loc_5_834E:
        xor    ebx, ebx
loc_5_8350:
        cmp    byte ptr [0x7571], bl         ; -> obj5:0x7571
        jne    loc_5_83BF
        cmp    dword ptr [0x7504], ebx       ; -> obj5:0x7504
        je     loc_5_83BF
        call   sub_5_81C7
        test   eax, eax
        je     loc_5_8350
        sub    eax, 3
        cmp    eax, 1
        sbb    eax, eax
        add    eax, 3
        jmp    loc_5_83C9
loc_5_8376:
        mov    byte ptr [0x7570], 0          ; -> obj5:0x7570
        cmp    al, 1
        jne    loc_5_838D
        push   1
        call   sub_5_80ED
        add    esp, 4
        jmp    loc_5_834E
loc_5_838D:
        cmp    al, 2
        jne    loc_5_83B8
        mov    eax, dword ptr [0x7520]       ; -> obj5:0x7520
        mov    ecx, dword ptr [0x751c]       ; -> obj5:0x751C
        mov    edx, dword ptr [0x7518]       ; -> obj5:0x7518
        mov    ebx, dword ptr [0x7514]       ; -> obj5:0x7514
        push   1
        push   eax
        push   ecx
        push   edx
        push   ebx
        call   sub_5_776E
        add    esp, 0x14
        jmp    loc_5_834E
loc_5_83B8:
        mov    eax, 3
        jmp    loc_5_83C9
loc_5_83BF:
        sub    edi, dword ptr [0x7504]       ; -> obj5:0x7504
loc_5_83C5:
        mov    dword ptr [esi], edi
        xor    eax, eax
loc_5_83C9:
        pop    esi
        pop    edi
        pop    ebx
        ret    
sub_5_83CD:
        sub    esp, 0
        push   edi
        push   ebx
        lea    edi, [0x9bf0]                 ; -> obj5:0x9BF0
        mov    ebx, edi
        mov    eax, 8
        mov    ecx, 0x90
        rep stosd dword ptr es:[edi], eax
        mov    eax, 9
        mov    ecx, 0x70
        rep stosd dword ptr es:[edi], eax
        mov    eax, 7
        mov    ecx, 0x18
        rep stosd dword ptr es:[edi], eax
        mov    eax, 8
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        push   0x7578                        ; -> loc_5_7578
        push   0x208
        push   0x8478                        ; -> loc_5_8478
        push   0x760a                        ; -> obj5:0x760A
        push   0x75cc                        ; -> obj5:0x75CC
        push   0x101
        push   0x120
        push   ebx
        call   sub_5_7A3A
        add    esp, 0x20
        test   eax, eax
        jne    loc_5_8472
        mov    edi, ebx
        mov    eax, 5
        mov    ecx, 0x1e
        rep stosd dword ptr es:[edi], eax
        push   0x757c                        ; -> loc_5_757C
        push   0x20
        push   0x8c98                        ; -> obj5:0x8C98
        push   0x7684                        ; -> obj5:0x7684
        push   0x7648                        ; -> loc_5_7648
        push   0
        push   0x1e
        push   ebx
        call   sub_5_7A3A
        add    esp, 0x20
        cmp    eax, 1
        jg     loc_5_8472
        xor    eax, eax
loc_5_8472:
        pop    ebx
        pop    edi
        add    esp, 0
        ret    
loc_5_8478:
        and    ecx, dword ptr [ebp + 0x52]
        inc    ebx
        dec    ecx
        xor    ah, byte ptr [eax]
        push   esi
        imul   esi, dword ptr [edx + 0x74], 0x206c6175
        inc    esp
        jbe    loc_5_84F4
        arpl   word ptr [ebp + 0x20], sp
        and    byte ptr [eax], ch
        push   esi
        jb     0x8507
        imul   ebp, dword ptr [edi + 0x6e], 0x302e3420
        sub    dword ptr [eax], eax
        add    byte ptr [ecx], cl
        dec    ebp
        push   edx
        inc    ebx
        dec    ecx
        xor    bl, byte ptr [edi + 0x44]
        inc    esp
        inc    edx
        add    dword ptr [eax], eax
        add    bh, bh
        adc    byte ptr [eax], al
        inc    dword ptr [ecx]
        add    byte ptr [eax], dh
        add    al, ah
        add    dword ptr [eax], eax
        add    al, ah
        add    dword ptr [eax + eax], esi
        push   esi
        push   ebx
        pop    edi
        push   esi
        inc    ebp
        push   edx
        push   ebx
        dec    ecx
        dec    edi
        dec    esi
        pop    edi
        dec    ecx
        dec    esi
        inc    esi
        dec    edi
        add    byte ptr [ebp + 0xfeef04], bh
        add    byte ptr [ecx], al
        add    byte ptr [eax], ch
        add    byte ptr [eax + eax], al
        push   edi
        add    al, 0
        add    byte ptr [eax], ch
        add    byte ptr [eax + eax], al
        push   edi
        add    al, 0
        add    byte ptr [edi], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [0x42000000], al
loc_5_84F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx + 1], dh
        add    byte ptr [eax], al
        push   ebx
        je     0x8578
        imul   ebp, dword ptr [esi + 0x67], 0x656c6946
        dec    ecx
        outsb  dx, byte ptr [esi]
        outsw  dx, word ptr [esi]
        add    byte ptr [eax], al
        pop    esi
        add    dword ptr [eax], eax
        add    byte ptr [eax], dh
        xor    al, 0x30
        cmp    dword ptr [eax], esi
        xor    al, 0x45
        xor    al, 0
        add    byte ptr [eax], al
        add    byte ptr [esi], ah
        add    byte ptr [esi], dl
        add    byte ptr [ebx + 0x6f], al
        insd   dword ptr es:[edi], dx
        jo     0x858d
        outsb  dx, byte ptr [esi]
        jns    0x857d
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[ebp + 0x69], cl
        arpl   word ptr [edx + 0x6f], si
        jae    loc_5_85A9
        je     0x855d
        inc    ebx
        outsd  dx, dword ptr [esi]
        jb     loc_5_85B1
        outsd  dx, dword ptr [esi]
        jb     loc_5_85A5
        je     0x85af
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], bh
        add    byte ptr [eax + eax], ah
        inc    esi
        imul   ebp, dword ptr [ebp + 0x44], 0x72637365
        imul   esi, dword ptr [eax + 0x74], 0x6e6f69
        dec    ebp
        push   edx
        inc    ebx
        dec    ecx
        xor    ah, byte ptr [eax]
        push   esi
        imul   esi, dword ptr [edx + 0x74], 0x206c6175
        inc    esp
        jbe    0x85da
        arpl   word ptr [ebp + 0x20], sp
        and    byte ptr [eax], ch
        push   esi
        jb     0x85ed
        imul   ebp, dword ptr [edi + 0x6e], 0x302e3420
        sub    dword ptr [eax], eax
        sbb    al, byte ptr [eax]
        or     al, byte ptr [eax]
        inc    esi
        imul   ebp, dword ptr [ebp + 0x56], 0x69737265
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr [esi + ebp], dh
        xor    al, 0x30
        xor    dword ptr cs:[ecx], esi
        xor    dword ptr [ecx], esi
        add    byte ptr [eax], al
        add    byte ptr [edx], bl
        add    byte ptr [esi], al
        add    byte ptr [ecx + 0x6e], cl
loc_5_85A5:
        je     loc_5_860C
        jb     loc_5_8617
loc_5_85A9:
        popal  
        insb   byte ptr es:[edi], dx
        dec    esi
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[eax], al
loc_5_85B1:
        add    byte ptr [eax], al
        dec    ebp
        push   edx
        inc    ebx
        dec    ecx
        xor    al, byte ptr [eax]
        add    byte ptr [eax], al
        cmp    al, byte ptr [eax]
        add    byte ptr es:[ebp + 0x67], cl
        popal  
        insb   byte ptr es:[edi], dx
        inc    ebx
        outsd  dx, dword ptr [esi]
        jo     loc_5_8641
        jb     0x8633
        push   0x43000074
        outsd  dx, dword ptr [esi]
        jo     0x864c
        jb     loc_5_863E
        push   0x20a92074
        dec    ebp
        imul   esp, dword ptr [ebx + 0x72], 0x666f736f
loc_5_85E3:
        je     0x8605
        inc    ebx
        outsd  dx, dword ptr [esi]
        jb     loc_5_8659
        and    byte ptr cs:[ecx], dh
        cmp    dword ptr [eax], edi
        cmp    byte ptr [0x35393931], ch
        add    byte ptr [eax], al
        add    byte ptr [edx], ah
        add    byte ptr [edx], cl
        add    byte ptr [edi + 0x72], cl
        imul   esp, dword ptr [edi + 0x69], 0x466c616e
        imul   ebp, dword ptr [ebp + 0x6e], 0x656d61
loc_5_860C:
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x52], cl
        inc    ebx
        dec    ecx
        xor    ch, byte ptr [esi]
        push   esi
        pop    eax
loc_5_8617:
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [0x50002500], dh
        jb     loc_5_8691
        jne    loc_5_8688
        je     loc_5_8675
        popal  
        insd   dword ptr es:[edi], dx
        add    byte ptr gs:[ebp + 0x69], cl
        arpl   word ptr [edx + 0x6f], si
        jae    0x86a1
        je     loc_5_85E3
        and    byte ptr [edi + 0x69], dl
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr fs:[esi]
        ja     0x86b0
        scasb  al, byte ptr es:[edi]
loc_5_863E:
        and    byte ptr [edi + 0x70], cl
loc_5_8641:
        jb     0x86a5
        je     0x86af
        outsb  dx, byte ptr [esi]
        and    byte ptr [bp + di + 0x79], dl
        jae    0x86c1
        insd   dword ptr es:[edi], dx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ds
        add    byte ptr [edx], cl
        add    byte ptr [eax + 0x72], dl
loc_5_8659:
        outsd  dx, dword ptr [esi]
        jne    0x86c0
        je     0x86b5
        jb     0x86d5
        imul   ebp, dword ptr [edi + 0x6e], 0x2e340000
        xor    al, 0x30
        xor    dword ptr cs:[ecx], esi
        xor    dword ptr [ecx], esi
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ah
loc_5_8675:
        add    byte ptr [eax], al
        push   esi
        popal  
        jb     0x86c1
        imul   ebp, dword ptr [ebp + 0x49], 0x6f666e
        adc    al, 0
        add    al, 0
        push   esp
loc_5_8688:
        jb     0x86eb
        outsb  dx, byte ptr [esi]
        jae    0x86f9
        popal  
        je     0x86f9
        outsd  dx, dword ptr [esi]
loc_5_8691:
        outsb  dx, byte ptr [esi]
        add    byte ptr [ecx], cl
        add    al, 0xe4
        db  004h
VxD__LTEXT ENDS
