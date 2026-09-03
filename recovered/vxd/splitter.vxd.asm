; Splitter -- reconstructed 32-bit disassembly (VxD / LE format)
; 1 objects, 501 instructions recovered by recursive descent

Declare_Virtual_Device Splitter, 3, 10, Splitter_Control, 00498h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h
;   service table: obj1:0x588, 5 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x604  R-X
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d004000
        pop    ecx
        test   eax, eax
        je     loc_1_0015
loc_1_0012:
        cld    
        leave  
        ret    
loc_1_0015:
        mov    ecx, dword ptr [0x5ec]        ; -> obj1:0x05EC
        cmp    word ptr [ecx], 0
        jne    loc_1_0012
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011004000
        push   dword ptr [ebp + 0xc]
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d004000
        pop    ecx
        cld    
        leave  
        ret    
sub_1_0034:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        push   eax
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e004000
        pop    eax
        cld    
        xor    eax, eax
        leave  
        ret    
sub_1_0047:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        cld    
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        movzx  eax, byte ptr [esi - 1]
        movzx  edx, byte ptr [edi - 1]
        sub    eax, edx
        pop    edi
        pop    esi
        leave  
        ret    
loc_1_0066:
        VxDCall  DOSMGR.DOSMGR_Get_IndosPtr      ; cd2004001500
        mov    dword ptr [0x5ec], eax        ; -> obj1:0x05EC
        VxDCall  VNETBIOS.VNETBIOS_Get_Version   ; cd2000001400
        jb     loc_1_0098
        cmp    eax, 0x30a
        jb     loc_1_0098
        mov    eax, 5
        mov    ecx, 0x2c
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_1_0098
        mov    dword ptr [0x5f0], esi        ; -> obj1:0x05F0
loc_1_0098:
        ret    
Splitter_Control:
        cmp    eax, 1
        je     loc_1_0066
        cmp    eax, 0x1b
        je     loc_1_0066
        clc    
        ret    
sub_1_00A5:
        push   esi
        push   edi
        mov    esi, dword ptr [0x5f0]        ; -> obj1:0x05F0
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_00CE
loc_1_00B5:
        mov    cl, byte ptr [eax]
        cmp    byte ptr [ebx + 0x30], cl
        jne    loc_1_00C6
        mov    cl, byte ptr [eax + 1]
        cmp    byte ptr [ebx + 3], cl
        jne    loc_1_00C6
        jmp    loc_1_00CE
loc_1_00C6:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_1_00B5
loc_1_00CE:
        pop    edi
        pop    esi
        ret    
sub_1_00D1:
        push   esi
        push   edi
        mov    esi, dword ptr [0x5f0]        ; -> obj1:0x05F0
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_010E
loc_1_00E1:
        mov    cl, byte ptr [eax]
        cmp    byte ptr [ebx + 0x30], cl
        jne    loc_1_0106
        push   eax
        push   ebx
        push   0x10
        lea    ecx, [ebx + 0x1a]
        push   ecx
        lea    ecx, [eax + 2]
        push   ecx
        call   sub_1_0047
        add    esp, 0xc
        mov    ecx, eax
        pop    ebx
        pop    eax
        or     ecx, ecx
        jne    loc_1_0106
        jmp    loc_1_010E
loc_1_0106:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_1_00E1
loc_1_010E:
        pop    edi
        pop    esi
        ret    
sub_1_0111:
        pushfd 
        cli    
        mov    esi, dword ptr [0x5f0]        ; -> obj1:0x05F0
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        popfd  
        push   esi
        mov    esi, dword ptr [eax + 0x24]
        VMMCall  VMM.List_Destroy                ; cd209c000100
        mov    esi, dword ptr [eax + 0x1c]
        VMMCall  VMM.List_Destroy                ; cd209c000100
        pop    esi
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        ret    
Splitter_Service_00:
        mov    eax, 1
        clc    
        ret    
Splitter_Service_01:
        push   esi
        push   edi
        call   sub_1_00D1
        or     eax, eax
        jne    loc_1_01E8
        mov    esi, dword ptr [0x5f0]        ; -> obj1:0x05F0
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_1_01CC
        mov    cl, byte ptr [ebx + 3]
        mov    byte ptr [eax + 1], cl
        mov    cl, byte ptr [ebx + 0x30]
        mov    byte ptr [eax], cl
        sub    ecx, ecx
        mov    dword ptr [eax + 0x28], ecx
        mov    dword ptr [eax + 0x20], ecx
        mov    dword ptr [eax + 0x18], ecx
        mov    dword ptr [eax + 0x14], 1
        cmp    byte ptr [ebx + 1], 0xd
        jne    loc_1_0186
        or     dword ptr [eax + 0x14], 2
loc_1_0186:
        cld    
        lea    edi, [eax + 2]
        lea    esi, [ebx + 0x1a]
        mov    ecx, 0x10
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   eax
        mov    eax, 5
        mov    ecx, 4
        VMMCall  VMM.List_Create                 ; cd209b000100
        pop    eax
        jb     loc_1_01D8
        mov    dword ptr [eax + 0x24], esi
        push   eax
        mov    eax, 5
        VMMCall  VMM.List_Create                 ; cd209b000100
        pop    eax
        jb     loc_1_01CF
        mov    dword ptr [eax + 0x1c], esi
        mov    esi, dword ptr [0x5f0]        ; -> obj1:0x05F0
        pushfd 
        cli    
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        popfd  
loc_1_01CC:
        pop    edi
        pop    esi
        ret    
loc_1_01CF:
        mov    esi, dword ptr [eax + 0x24]
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_1_01D8:
        mov    esi, dword ptr [0x5f0]        ; -> obj1:0x05F0
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        xor    eax, eax
        jmp    loc_1_01CC
loc_1_01E8:
        or     dword ptr [eax + 0x14], 1
        jmp    loc_1_01CC
Splitter_Service_02:
        push   esi
        push   edi
        call   sub_1_00D1
        or     eax, eax
        je     loc_1_0247
        mov    edi, eax
        btr    dword ptr [edi + 0x14], 0
        jae    loc_1_0237
        mov    esi, dword ptr [edi + 0x24]
loc_1_0205:
        pushfd 
        cli    
        VMMCall  VMM.List_Remove_First           ; cd20a5000100
        popfd  
        or     eax, eax
        je     loc_1_0237
        dec    dword ptr [edi + 0x28]
        pushal 
        mov    ebx, dword ptr [eax]
        mov    byte ptr [ebx + 1], 0x17
        mov    byte ptr [ebx + 0x31], 0x17
        cmp    dword ptr [ebx + 0x2c], 0
        je     loc_1_022E
        mov    eax, 0x17
        call   dword ptr [ebx + 0x2c]
loc_1_022E:
        popal  
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        jmp    loc_1_0205
loc_1_0237:
        mov    eax, edi
        test   dword ptr [eax + 0x14], 2
        jne    loc_1_024D
        call   sub_1_0111
loc_1_0247:
        VxDCall  VNETBIOS.VNETBIOS_Submit        ; cd2002001400
loc_1_024D:
        xor    eax, eax
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 0x31], al
        pop    edi
        pop    esi
        ret    
Splitter_Service_03:
        push   esi
        mov    dword ptr [0x5f4], eax        ; -> loc_1_05F4
        or     eax, eax
        je     loc_1_027C
        mov    eax, 0x140002
        mov    esi, 0x29f                    ; -> Splitter_Service_04
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        jb     loc_1_0295
        mov    dword ptr [0x5fc], esi        ; -> loc_1_05FC
        jmp    loc_1_0295
loc_1_027C:
        cmp    dword ptr [0x5fc], 0          ; -> loc_1_05FC
        je     loc_1_0295
        mov    eax, 0x140002
        mov    esi, 0x29f                    ; -> Splitter_Service_04
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
loc_1_0295:
        pop    esi
        ret    
        db  0EBh, 006h, 0FFh, 025h
        dd  obj1:0x0600              ; fixup
Splitter_Service_04:
        push   ecx
        push   edx
        push   esi
        push   edi
        push   eax
        cld    
        sub    eax, eax
        mov    al, byte ptr [ebx]
        and    eax, 0xffffff7f
        cmp    eax, 0x36
        je     loc_1_02F1
        cmp    eax, 0x21
        je     loc_1_0372
        cmp    eax, 0x35
        je     loc_1_0466
        cmp    eax, 0x31
        je     loc_1_0310
loc_1_02CA:
        clc    
        pop    eax
loc_1_02CC:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        cld    
        jb     loc_1_02E2
        cmp    dword ptr [0x5fc], 0          ; -> loc_1_05FC
        je     loc_1_02E2
        jmp    loc_1_05FC
loc_1_02E2:
        ret    
loc_1_02E3:
        sub    eax, eax
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx + 0x31], al
        add    esp, 4
        stc    
        jmp    loc_1_02CC
loc_1_02F1:
        mov    eax, dword ptr [0x5f4]        ; -> loc_1_05F4
        or     eax, eax
        je     loc_1_02CA
        cmp    dword ptr [ebx + 0x2c], eax
        je     loc_1_02CA
        push   ebx
        call   sub_1_00D1
        pop    ebx
        or     eax, eax
        je     loc_1_02CA
        or     dword ptr [eax + 0x14], 2
        jmp    loc_1_02CA
loc_1_0310:
        push   ebx
        call   sub_1_00D1
        pop    ebx
        or     eax, eax
        je     loc_1_02CA
        mov    edi, eax
        btr    dword ptr [edi + 0x14], 1
        jae    loc_1_0359
        mov    esi, dword ptr [edi + 0x1c]
loc_1_0327:
        pushfd 
        cli    
        VMMCall  VMM.List_Remove_First           ; cd20a5000100
        popfd  
        or     eax, eax
        je     loc_1_0359
        pushal 
        mov    ebx, dword ptr [eax]
        mov    byte ptr [ebx + 1], 0x17
        mov    byte ptr [ebx + 0x31], 0x17
        cmp    dword ptr [ebx + 0x2c], 0
        je     loc_1_034D
        mov    eax, 0x17
        call   dword ptr [ebx + 0x2c]
loc_1_034D:
        popal  
        dec    dword ptr [edi + 0x20]
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        jmp    loc_1_0327
loc_1_0359:
        mov    eax, edi
        test   dword ptr [eax + 0x14], 1
        jne    loc_1_02E3
        call   sub_1_0111
        jmp    loc_1_02CA
loc_1_0372:
        push   ebx
        call   sub_1_00A5
        pop    ebx
        or     eax, eax
        je     loc_1_02CA
        mov    edi, eax
        sub    edx, edx
        mov    eax, dword ptr [0x5f4]        ; -> loc_1_05F4
        or     eax, eax
        je     loc_1_0396
        cmp    dword ptr [ebx + 0x2c], eax
        jne    loc_1_0396
        inc    edx
        jmp    loc_1_03B5
loc_1_0396:
        test   dword ptr [edi + 0x14], 1
        jne    loc_1_03B5
        cmp    dword ptr [edi + 0x18], 0
        jne    loc_1_02CA
        mov    eax, edi
        call   sub_1_0111
        jmp    loc_1_02CA
loc_1_03B5:
        mov    esi, dword ptr [edi + edx*8 + 0x1c]
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_1_02CA
        mov    dword ptr [eax], ebx
        pushfd 
        cli    
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        inc    dword ptr [edi + edx*8 + 0x20]
        mov    ecx, dword ptr [edi + 0x18]
        cmp    ecx, dword ptr [edi + edx*8 + 0x20]
        jb     loc_1_03F0
        popfd  
        mov    byte ptr [ebx + 1], 0xff
        mov    byte ptr [ebx + 0x31], 0xff
        sub    eax, eax
        add    esp, 4
        stc    
        jmp    loc_1_02CC
loc_1_03F0:
        inc    dword ptr [edi + 0x18]
        popfd  
        mov    esi, eax
        push   edx
        push   ebx
        mov    ecx, 0x40
        add    ecx, 0x5ad
        push   ecx
        push   0
        call   0
        add    esp, 8
        pop    ebx
        pop    edx
        or     eax, eax
        jne    loc_1_042E
        mov    eax, esi
        mov    esi, dword ptr [edi + edx*8 + 0x1c]
        pushfd 
        cli    
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        popfd  
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        jmp    loc_1_02CA
loc_1_042E:
        mov    edi, 0x4e6                    ; -> loc_1_04E6
        mov    dword ptr [eax + 0x2c], edi
        mov    cl, byte ptr [ebx]
        mov    byte ptr [eax], cl
        or     byte ptr [eax], 0x80
        mov    word ptr [eax + 8], 0x5ad
        mov    edi, eax
        add    edi, 0x40
        mov    dword ptr [eax + 4], edi
        mov    cl, byte ptr [ebx + 0x30]
        mov    byte ptr [eax + 0x30], cl
        mov    cl, byte ptr [ebx + 3]
        mov    byte ptr [eax + 3], cl
        mov    byte ptr [ebx + 1], 0xff
        mov    byte ptr [ebx + 0x31], 0xff
        mov    ebx, eax
        jmp    loc_1_02CA
loc_1_0466:
        sub    edx, edx
        mov    eax, dword ptr [0x5f4]        ; -> loc_1_05F4
        or     eax, eax
        je     loc_1_0477
        cmp    dword ptr [ebx + 0x2c], eax
        jne    loc_1_0477
        inc    edx
loc_1_0477:
        mov    esi, dword ptr [0x5f0]        ; -> obj1:0x05F0
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_02CA
loc_1_0489:
        push   eax
        mov    esi, dword ptr [eax + edx*8 + 0x1c]
        pushfd 
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_04D1
loc_1_0498:
        cmp    dword ptr [eax], ebx
        jne    loc_1_04C9
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        popfd  
        pushal 
        mov    ebx, dword ptr [eax]
        mov    byte ptr [ebx + 1], 0xb
        mov    byte ptr [ebx + 0x31], 0xb
        cmp    dword ptr [ebx + 0x2c], 0
        je     loc_1_04BC
        mov    eax, 0xb
        call   dword ptr [ebx + 0x2c]
loc_1_04BC:
        popal  
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        pop    eax
        jmp    loc_1_02E3
loc_1_04C9:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_1_0498
loc_1_04D1:
        popfd  
        pop    eax
        mov    esi, dword ptr [0x5f0]        ; -> obj1:0x05F0
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_1_0489
        jmp    loc_1_02CA
loc_1_04E6:
        pushal 
        cld    
        push   ebx
        call   sub_1_00A5
        pop    ebx
        or     eax, eax
        je     loc_1_0579
        mov    edi, eax
        mov    edx, 1
loc_1_04FE:
        mov    esi, dword ptr [edi + edx*8 + 0x1c]
        VMMCall  VMM.List_Remove_First           ; cd20a5000100
        je     loc_1_0576
        dec    dword ptr [edi + edx*8 + 0x20]
        push   eax
        push   edx
        push   esi
        push   edi
        mov    eax, dword ptr [eax]
        sub    ecx, ecx
        mov    cx, word ptr [eax + 8]
        cmp    cx, word ptr [ebx + 8]
        jge    loc_1_0532
        cmp    byte ptr [ebx + 1], 0
        jne    loc_1_052D
        mov    edx, 6
        jmp    loc_1_053D
loc_1_052D:
        mov    dl, byte ptr [ebx + 1]
        jmp    loc_1_053D
loc_1_0532:
        mov    dl, byte ptr [ebx + 1]
        mov    cx, word ptr [ebx + 8]
        mov    word ptr [eax + 8], cx
loc_1_053D:
        mov    edi, dword ptr [eax + 4]
        mov    esi, dword ptr [ebx + 4]
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, 0x10
        lea    edi, [eax + 0xa]
        lea    esi, [ebx + 0xa]
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    byte ptr [eax + 1], dl
        mov    byte ptr [eax + 0x31], dl
        pushal 
        mov    ebx, eax
        sub    eax, eax
        cmp    dword ptr [ebx + 0x2c], eax
        je     loc_1_056B
        mov    al, byte ptr [ebx + 1]
        call   dword ptr [ebx + 0x2c]
        cld    
loc_1_056B:
        popal  
        pop    edi
        pop    esi
        pop    edx
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_1_0576:
        dec    edx
        je     loc_1_04FE
loc_1_0579:
        dec    dword ptr [edi + 0x18]
        push   ebx
        call   sub_1_0034
        add    esp, 4
        popal  
        ret    
        db  000h
        dd  Splitter_Service_00      ; fixup
        dd  Splitter_Service_01      ; fixup
        dd  Splitter_Service_02      ; fixup
        dd  Splitter_Service_03      ; fixup
        dd  Splitter_Service_04      ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00498h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'Splitter'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  Splitter_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x0588              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 025h, 053h, 070h, 06Ch, 069h, 074h, 074h, 065h
loc_1_05F4:
        jb     0x616
        push   esi
        db  069h, 072h, 074h, 075h, 061h
loc_1_05FC:
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x76], al
        db  069h, 063h, 065h
VxD__LTEXT ENDS
