; BIOSXLAT -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 365 instructions recovered by recursive descent

Declare_Virtual_Device BIOSXLAT, 1, 0, BIOSXLAT_Control, 00013h, Undefined_Init_Order
;   DDK version 0400h   init_order 050000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x9c  R-X
BIOSXLAT_Control:
        cmp    eax, 0
        je     loc_2_0000
        clc    
        ret    
loc_1_000B:
        VMMCall  VMM.Simulate_Iret               ; cd2047800100
loc_1_0011:
        test   dword ptr [ebx], 0x40
        je     loc_1_0043
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        cmp    cx, word ptr [0x98]           ; -> obj1:0x0098
        jne    loc_1_0031
        cmp    dx, word ptr [0x9a]           ; -> obj1:0x009A
        je     loc_1_0043
loc_1_0031:
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
loc_1_0043:
        stc    
        ret    
        db  000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00013h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'BIOSXLAT'                  ; DDB_Name
        dd  050000000h                  ; DDB_Init_Order
        dd  BIOSXLAT_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xfc  R-X
loc_2_0000:
        mov    esi, 0x22                     ; -> loc_5_0022
        mov    edx, 0x10
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        jb     loc_2_00F4
        xchg   edx, eax
        mov    ecx, edx
        shr    ecx, 0x10
        movzx  edx, dx
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        mov    esi, 8                        ; -> loc_4_0008
        mov    edx, 0x13
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        jb     loc_2_00F4
        xchg   edx, eax
        mov    ecx, edx
        shr    ecx, 0x10
        movzx  edx, dx
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        mov    esi, 0x2ec                    ; -> loc_4_02EC
        mov    edx, 0x15
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        jb     loc_2_00F4
        xchg   edx, eax
        mov    ecx, edx
        shr    ecx, 0x10
        movzx  edx, dx
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        mov    esi, 0xb                      ; -> loc_1_000B
        mov    edx, 0x1c
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        jb     loc_2_00F4
        xchg   edx, eax
        mov    ecx, edx
        shr    ecx, 0x10
        movzx  edx, dx
        mov    word ptr [0x98], cx           ; -> obj1:0x0098
        mov    word ptr [0x9a], dx           ; -> obj1:0x009A
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        mov    esi, 0x11                     ; -> loc_1_0011
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    eax, dword ptr [0x78]
        movzx  ecx, ax
        shr    eax, 0x10
        shl    eax, 4
        add    eax, ecx
        push   0
        push   0
        push   0x92
        push   0x100
        push   eax
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        or     edx, eax
        je     loc_2_00F4
        movzx  ecx, ax
        xor    edx, edx
        mov    eax, 0x1e
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        clc    
        ret    
loc_2_00F4:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 3  vsize=0x2f  RW-
        db  001h, 011h, 000h, 014h, 038h, 000h, 010h, 003h
        dd  loc_4_0000               ; fixup
        db  014h, 038h, 000h, 010h, 002h, 018h, 008h, 038h, 000h, 010h, 006h, 010h, 038h, 000h, 015h, 007h
        db  038h, 000h, 015h, 000h, 013h, 003h
        dd  loc_4_02D2               ; fixup
        db  010h, 038h, 000h, 013h, 006h, 000h, 038h, 000h, 013h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x31e  R-X
loc_4_0000:
        movzx  ecx, word ptr [ebp + 0x18]
        lea    ecx, [ecx + ecx*2]
        ret    
loc_4_0008:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        mov    eax, dword ptr [ebp + 0x1c]
        test   byte ptr [ebp + 0x14], 0x80
        je     loc_4_001D
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_4_001D:
        cmp    ah, 2
        jb     loc_4_003C
        je     loc_4_008D
        cmp    ah, 4
        jb     loc_4_008D
        je     loc_4_005D
        cmp    ah, 5
        je     loc_4_0047
        cmp    ah, 8
        jb     loc_4_003C
        je     loc_4_0052
        cmp    ah, 0x18
        je     loc_4_0052
loc_4_003C:
        mov    edx, 0x1f                     ; -> obj3:0x001F
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_4_0047:
        mov    edx, 0x21                     ; -> obj3:0x0021
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_4_0052:
        mov    edx, 0x2a                     ; -> obj3:0x002A
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_4_005D:
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        push   dword ptr [ebp + 0x38]
        push   dword ptr [ebp + 0x10]
        mov    word ptr [ebp + 0x38], 0xf000
        mov    word ptr [ebp + 0x10], 0
        mov    eax, 0x13
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        pop    dword ptr [ebp + 0x10]
        pop    dword ptr [ebp + 0x38]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        ret    
loc_4_008D:
        sub    esp, 4
        mov    edi, esp
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    byte ptr [ebp + 0x1d], 8
        mov    eax, 0x13
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   word ptr [ebp + 0x2c], 1
        jne    loc_4_00EF
        movzx  eax, word ptr [ebp + 0x38]
        shl    eax, 4
        movzx  ecx, word ptr [ebp]
        add    eax, ecx
        movzx  eax, byte ptr [eax + 3]
        xor    ecx, ecx
        bts    ecx, eax
        shl    ecx, 7
        mov    word ptr [edi], cx
        movzx  eax, byte ptr [ebp + 0x18]
        shl    eax, 2
        shr    al, 2
        mov    byte ptr [edi + 2], al
        mov    al, byte ptr [ebp + 0x15]
        mov    byte ptr [edi + 3], al
        jmp    loc_4_0119
loc_4_00EF:
        mov    ecx, dword ptr [0x78]
        movzx  eax, cx
        shr    ecx, 0x10
        shl    ecx, 4
        add    ecx, eax
        movzx  eax, byte ptr [ecx + 3]
        xor    edx, edx
        bts    edx, eax
        shl    edx, 7
        mov    word ptr [edi], dx
        mov    al, byte ptr [ecx + 4]
        mov    byte ptr [edi + 2], al
        mov    byte ptr [edi + 3], 1
loc_4_0119:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        mov    ax, word ptr [ebp + 0x38]
        mov    edx, dword ptr [ebp + 0x10]
        call   sub_4_029C
        jae    loc_4_01BA
        mov    esi, eax
        movzx  eax, byte ptr [ebp + 0x1c]
        mul    word ptr [edi]
        mov    ecx, eax
        mov    eax, esi
        dec    ecx
        and    eax, 0xfff
        add    ecx, eax
        shr    ecx, 0xc
        inc    ecx
        mov    eax, esi
        shr    eax, 0xc
        push   0
        push   ecx
        push   eax
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        test   eax, eax
        jne    loc_4_0174
        add    esp, 0xc
        jmp    loc_4_01BA
loc_4_0174:
        shl    esi, 0xc
        shr    si, 0xc
        push   dword ptr [ebp + 0x10]
        mov    word ptr [ebp + 0x10], si
        shr    esi, 0x10
        xchg   word ptr [ebp + 0x5c], si
        mov    edx, 0x13
        call   sub_5_0000
        mov    word ptr [ebp + 0x5c], si
        pop    dword ptr [ebp + 0x10]
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        test   dword ptr [ebx], 0x180
        je     loc_4_023A
        mov    word ptr [ebp + 0x1e], 0
        jmp    loc_4_023A
loc_4_01BA:
        push   fs
        push   dword ptr [ebp + 0x5c]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        mov    ax, 0x3810
        VxDCall  V86MMGR.V86MMGR_Load_Client_Ptr ; cd2007000600
        VxDCall  V86MMGR.V86MMGR_Get_Xlat_Buff_State ; cd200a000600
        mov    eax, ecx
        xor    edx, edx
        movzx  ecx, word ptr [edi]
        div    ecx
        push   eax
        mov    eax, dword ptr [ebp + 0x1c]
        movzx  ecx, al
        jecxz  loc_4_0221
loc_4_01EA:
        mov    edx, ecx
        cmp    ecx, dword ptr [esp]
        jbe    loc_4_01F4
        mov    edx, dword ptr [esp]
loc_4_01F4:
        call   sub_4_023E
        jb     loc_4_0221
        sub    ecx, edx
        je     loc_4_0221
        add    byte ptr [ebp + 0x18], dl
loc_4_0202:
        mov    dl, byte ptr [edi + 2]
        cmp    byte ptr [ebp + 0x18], dl
        jbe    loc_4_01EA
        sub    byte ptr [ebp + 0x18], dl
        inc    byte ptr [ebp + 0x15]
        mov    dl, byte ptr [edi + 3]
        cmp    byte ptr [ebp + 0x15], dl
        jbe    loc_4_0202
        mov    byte ptr [ebp + 0x15], 0
        inc    byte ptr [ebp + 0x19]
        jmp    loc_4_0202
loc_4_0221:
        add    esp, 4
        movzx  edx, al
        sub    edx, ecx
        mov    byte ptr [ebp + 0x1c], dl
        pop    dword ptr [ebp + 0x14]
        pop    dword ptr [ebp + 0x18]
        pop    dword ptr [ebp + 0x10]
        pop    dword ptr [ebp + 0x5c]
        pop    fs
loc_4_023A:
        add    esp, 4
        ret    
sub_4_023E:
        push   ecx
        push   edi
        push   eax
        push   edx
        movzx  eax, word ptr [edi]
        mul    edx
        mov    ecx, eax
        pop    edx
        pop    eax
        bt     eax, 8
        VxDCall  V86MMGR.V86MMGR_Allocate_Buffer ; cd2008000600
        mov    word ptr [ebp + 0x10], di
        shr    edi, 0x10
        mov    word ptr [ebp + 0x5c], di
        mov    byte ptr [ebp + 0x1d], ah
        mov    byte ptr [ebp + 0x1c], dl
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        push   eax
        mov    eax, 0x13
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        pop    eax
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        bt     eax, 8
        cmc    
        VxDCall  V86MMGR.V86MMGR_Free_Buffer     ; cd2009000600
        bt     word ptr [ebp + 0x2c], 0
        jb     loc_4_0295
        add    esi, ecx
        clc    
loc_4_0295:
        movzx  edx, byte ptr [ebp + 0x1c]
        pop    edi
        pop    ecx
        ret    
sub_4_029C:
        push   ecx
        push   edx
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        pop    edx
        test   dword ptr [ebx], 0x180
        jne    loc_4_02B7
        movzx  edx, dx
loc_4_02B7:
        add    eax, edx
        cmp    eax, 0x100000
        jb     loc_4_02D0
        sub    eax, dword ptr [ebx + 4]
        jb     loc_4_02CC
        cmp    eax, 0x100000
        jb     loc_4_02D0
loc_4_02CC:
        add    eax, dword ptr [ebx + 4]
        clc    
loc_4_02D0:
        pop    ecx
        ret    
loc_4_02D2:
        push   eax
        mov    eax, dword ptr [0x78]
        movzx  ecx, ax
        shr    eax, 0x10
        shl    eax, 4
        add    ecx, eax
        movzx  ecx, byte ptr [ecx + 4]
        shl    ecx, 2
        pop    eax
        ret    
loc_4_02EC:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        cmp    byte ptr [ebp + 0x1d], 0xc0
        jb     sub_5_0000
        je     loc_4_0313
        cmp    byte ptr [ebp + 0x1d], 0xc1
        jne    sub_5_0000
        mov    edx, 0x1b                     ; -> obj3:0x001B
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_4_0313:
        mov    edx, 0x16                     ; -> obj3:0x0016
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x66  R-X
sub_5_0000:
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    eax, edx
        mov    edx, dword ptr [ebp + 0x2c]
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   dh, 2
        je     loc_5_001C
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
loc_5_001C:
        VMMCall  VMM.End_Nest_Exec               ; cd2086800100
loc_5_0022:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    ah, 0x13
        je     loc_5_005B
        cmp    ah, 0x10
        jne    0
        cmp    al, 2
        je     loc_5_0050
        cmp    al, 9
        je     loc_5_0050
        cmp    al, 0x12
        je     loc_5_0045
        cmp    al, 0x17
        jne    0
loc_5_0045:
        mov    edx, 7                        ; -> obj3:0x0007
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_5_0050:
        mov    edx, 0                        ; -> obj3:0x0000
        VxDCall  V86MMGR.V86MMGR_Xlat_API        ; cd2006800600
loc_5_005B:
        mov    edx, 0x10                     ; -> obj3:0x0010
        VxDCall  Dev_2506.Service_8006h          ; cd2006800625
VxD__LTEXT ENDS
