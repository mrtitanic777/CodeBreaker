; PAGEFILE -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 688 instructions recovered by recursive descent

Declare_Virtual_Device PAGEFILE, 4, 0, PAGEFILE_Control, 00021h, Undefined_Init_Order
;   DDK version 0400h   init_order 018000000h   flags 00000h
;   service table: obj1:0x78, 10 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0xf8  R-X
PAGEFILE_Control:
        cmp    eax, 0
        je     loc_2_0000
        clc    
        ret    
PageFile_Test_IO_Valid:
        cmp    byte ptr [0xf4], 2            ; -> obj1:0x00F4
        jne    loc_1_002A
        VxDCall  DOSMGR.DOSMGR_Get_DOS_Crit_Status ; cd2008001500
        jne    loc_1_002C
        push   eax
        VxDCall  DOSMGR.DOSMGR_Get_IndosPtr      ; cd2004001500
        cmp    word ptr [eax], 0
        pop    eax
        jne    loc_1_002C
loc_1_002A:
        clc    
        ret    
loc_1_002C:
        stc    
        ret    
PageFile_Read_Or_Write:
        cmp    byte ptr [0xf5], 0            ; -> obj1:0x00F5
        jne    loc_6_027A
        push   eax
        push   ecx
        push   edx
        push   esi
        mov    eax, 0xd600
        mov    al, byte ptr [ebx + 4]
        xor    ecx, ecx
        mov    edx, dword ptr [ebx + 0xc]
        mov    cl, byte ptr [ebx + 7]
        shl    edx, 0xc
        shl    ecx, 0xc
        mov    esi, dword ptr [ebx + 8]
        push   ebx
        mov    ebx, dword ptr [0xf0]         ; -> obj1:0x00F0
        VxDCall  IFSMgr.IFSMgr_Ring0_FileIO      ; cd2032004000
        pop    ebx
        mov    eax, 0
        adc    eax, 0
        mov    byte ptr [ebx + 6], al
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        ret    
        db  000h, 000h
        dd  PageFile_Get_Version     ; fixup
        dd  PageFile_Init_File       ; fixup
        dd  PageFile_Clean_Up        ; fixup
        dd  PageFile_Grow_File       ; fixup
        dd  PageFile_Read_Or_Write   ; fixup
        dd  PageFile_Cancel          ; fixup
        dd  PageFile_Test_IO_Valid   ; fixup
        dd  PageFile_Get_Size_Info   ; fixup
        dd  PageFile_Set_Async_Manager ; fixup
        dd  PageFile_Call_Async_Manager ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00021h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'PAGEFILE'                  ; DDB_Name
        dd  018000000h                  ; DDB_Init_Order
        dd  PAGEFILE_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  PAGEFILE_PM_API          ; DDB_PM_API_Proc
        dd  obj1:0x0078              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x3bf  R-X
loc_2_0000:
        movzx  eax, dx
        shr    edx, 0x10
        shl    edx, 4
        add    edx, eax
        mov    dword ptr [0xa0], edx         ; -> obj3:0x00A0
        clc    
        ret    
PageFile_Init_File:
        pushal 
        mov    eax, 0xffffffff
        xor    esi, esi
        mov    edi, 0x36c                    ; -> loc_2_036C
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
        test   eax, eax
        je     loc_2_0047
        call   sub_2_005A
        mov    ebx, dword ptr [0x80]         ; -> obj3:0x0080
        test   ebx, ebx
        je     loc_2_0047
loc_2_0039:
        popal  
        mov    eax, dword ptr [0x88]         ; -> obj3:0x0088
        mov    ebx, dword ptr [0x80]         ; -> obj3:0x0080
        clc    
        ret    
loc_2_0047:
        mov    dword ptr [0x80], 0           ; -> obj3:0x0080
        mov    byte ptr [0xf4], 1            ; -> obj1:0x00F4
        jmp    loc_2_0039
sub_2_005A:
        enter  0x5c, 0
        pushal 
        mov    byte ptr [ebp - 0x54], 0
        mov    byte ptr [ebp - 0x58], 0
        mov    byte ptr [ebp - 0x5c], 0
        lea    esi, [ebp - 0x40]
        push   0
        push   esi
        VMMCall  VMM._GetDemandPageInfo          ; cd206f000100
        pop    ecx
        pop    ecx
        mov    ecx, dword ptr [esi + 4]
        shl    ecx, 2
        mov    eax, 0x2400
        sub    eax, ecx
        jns    loc_2_008A
        xor    eax, eax
loc_2_008A:
        mov    dword ptr [ebp - 0x44], eax
        xor    esi, esi
        mov    edi, 0x3ad                    ; -> obj2:0x03AD
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        shr    eax, 2
        mov    dword ptr [0x84], eax         ; -> obj3:0x0084
        mov    eax, 0x200
        xor    esi, esi
        mov    edi, 0x39c                    ; -> obj2:0x039C
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        shr    eax, 2
        mov    dword ptr [0x90], eax         ; -> obj3:0x0090
        mov    eax, 0x200000
        xor    esi, esi
        mov    edi, 0x38a                    ; -> obj2:0x038A
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        shr    eax, 2
        mov    dword ptr [0x80], eax         ; -> obj3:0x0080
        xor    esi, esi
        mov    edi, 0x373                    ; -> obj2:0x0373
        VMMCall  VMM.Get_Profile_String          ; cd20b3000100
        jb     loc_2_00F5
        mov    esi, edx
        mov    edi, 0                        ; -> obj3:0x0000
        cld    
loc_2_00ED:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        test   al, al
        jne    loc_2_00ED
        jmp    loc_2_0137
loc_2_00F5:
        xor    esi, esi
        mov    edi, 0x37e                    ; -> obj2:0x037E
        VMMCall  VMM.Get_Profile_String          ; cd20b3000100
        mov    edi, 0                        ; -> obj3:0x0000
        cld    
        jb     loc_2_0117
        mov    bl, byte ptr [edx]
loc_2_010C:
        mov    al, bl
        stosb  byte ptr es:[edi], al
        mov    ax, 0x5c3a
        stosw  word ptr es:[edi], ax
        jmp    loc_2_012B
loc_2_0117:
        mov    byte ptr [ebp - 0x54], 1
        VMMCall  VMM.Get_Config_Directory        ; cd20b7000100
        mov    esi, edx
loc_2_0123:
        lodsb  al, byte ptr [esi]
        test   al, al
        je     loc_2_012B
        stosb  byte ptr es:[edi], al
        jmp    loc_2_0123
loc_2_012B:
        mov    esi, 0x360                    ; -> obj2:0x0360
        mov    ecx, 0xb
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_2_0137:
        movzx  edx, byte ptr [0]             ; -> obj3:0x0000
        and    dl, 0xdf
        sub    dl, 0x40
        mov    dword ptr [0x8c], edx         ; -> obj3:0x008C
        dec    edx
        lea    eax, [ebp - 0x48]
        mov    byte ptr [eax + 2], dl
        mov    byte ptr [eax], 0xa
        push   eax
        VxDCall  BLOCKDEV.Service_0008h          ; cd2008001000
        pop    eax
        cmp    byte ptr [eax + 1], 0
        jne    loc_2_018B
        test   byte ptr [eax + 3], 3
        je     loc_2_018B
        test   byte ptr [eax + 3], 2
        jne    loc_2_018B
        cmp    byte ptr [ebp - 0x54], 0
        je     loc_2_018B
        cmp    byte ptr [ebp - 0x5c], 0
        jne    loc_2_018B
        mov    bl, byte ptr [eax + 2]
        add    bl, 0x41
        mov    edi, 0                        ; -> obj3:0x0000
        mov    byte ptr [ebp - 0x58], 1
        jmp    loc_2_010C
loc_2_018B:
        mov    byte ptr [0xf4], 2            ; -> obj1:0x00F4
        mov    edx, dword ptr [0x8c]         ; -> obj3:0x008C
        dec    edx
        VxDCall  IFSMgr.IFSMgr_Ring0GetDriveInfo ; cd2035004000
        jb     loc_2_0225
        test   eax, 1
        jne    loc_2_01D9
        mov    edx, dword ptr [0x8c]         ; -> obj3:0x008C
        add    dl, 0x40
        lea    eax, [ebp - 0x50]
        mov    byte ptr [eax + 2], dl
        mov    byte ptr [eax], 2
        push   eax
        VxDCall  BLOCKDEV.Service_0008h          ; cd2008001000
        pop    eax
        cmp    byte ptr [eax + 1], 0
        jne    loc_2_01D2
        test   byte ptr [eax + 5], 0x40
        jne    loc_2_01D9
loc_2_01D2:
        mov    byte ptr [0xf4], 3            ; -> obj1:0x00F4
loc_2_01D9:
        mov    eax, 0xd500
        mov    bx, 0x6092
        xor    ecx, ecx
        mov    dl, 0x11
        mov    dh, 0x13
        mov    esi, 0                        ; -> obj3:0x0000
        VxDCall  IFSMgr.IFSMgr_Ring0_FileIO      ; cd2032004000
        jb     loc_2_022C
        mov    dword ptr [0xf0], eax         ; -> obj1:0x00F0
loc_2_01FA:
        mov    edx, dword ptr [0x84]         ; -> obj3:0x0084
        call   sub_7_0000
        jae    loc_2_021A
        mov    edx, dword ptr [ebp - 0x44]
        shr    edx, 2
        mov    dword ptr [0x84], edx         ; -> obj3:0x0084
        call   sub_7_0000
        jb     loc_2_021D
loc_2_021A:
        popal  
        leave  
        ret    
loc_2_021D:
        VxDCall  PAGEFILE.PageFile_Clean_Up      ; cd2002002100
        jmp    loc_2_022C
loc_2_0225:
        call   sub_2_026A
        jae    loc_2_01FA
loc_2_022C:
        mov    byte ptr [0xf5], 0            ; -> obj1:0x00F5
        mov    edi, 0                        ; -> obj3:0x0000
        cmp    byte ptr [ebp - 0x54], 0
        je     loc_2_0117
        mov    byte ptr [ebp - 0x5c], 1
        cmp    byte ptr [ebp - 0x58], 0
        mov    byte ptr [ebp - 0x58], 0
        jne    loc_2_0117
        mov    dword ptr [0x80], 0           ; -> obj3:0x0080
        cmp    dword ptr [ebp - 0x44], 0
        je     loc_2_021A
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
sub_2_026A:
        pushal 
        mov    eax, 0x21
        VMMCall  VMM.Get_V86_Int_Vector          ; cd2042000100
        shl    ecx, 0x10
        mov    cx, dx
        mov    dword ptr [0x94], ecx         ; -> obj3:0x0094
        VMMCall  VMM.Get_PSP_Segment             ; cd20b9000100
        mov    word ptr [0xaa], ax           ; -> obj3:0x00AA
        mov    ah, 0x3c
        xor    ecx, ecx
        mov    edx, 0                        ; -> obj3:0x0000
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_2_0334
        mov    ebx, eax
        mov    ah, 0x3e
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    ah, 0x3d
        mov    al, 0x92
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jb     loc_2_0334
        mov    dword ptr [0xf0], eax         ; -> obj1:0x00F0
        push   0x408
        push   0x1000
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_2_0336
        shr    eax, 0xc
        mov    dword ptr [0x98], eax         ; -> obj3:0x0098
        call   sub_6_01B7
        push   0x202
        push   0x190
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        pop    ecx
        pop    ecx
        test   eax, eax
        je     loc_2_0336
        shl    eax, 0xc
        shr    ax, 0xc
        add    ax, 0x190
        mov    dword ptr [0x9c], eax         ; -> obj3:0x009C
        mov    esi, 0x199                    ; -> loc_6_0199
        mov    eax, 0x24
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    esi, 0x1aa                    ; -> loc_6_01AA
        mov    eax, 0x23
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        inc    byte ptr [0xf5]               ; -> obj1:0x00F5
        clc    
loc_2_0334:
        popal  
        ret    
loc_2_0336:
        VxDCall  PAGEFILE.PageFile_Clean_Up      ; cd2002002100
        stc    
        jmp    loc_2_0334
PageFile_Set_Async_Manager:
        cmp    dword ptr [0x35c], 0x358      ; -> loc_2_035C
        jne    loc_2_0358
        mov    dword ptr [0x35c], eax        ; -> loc_2_035C
        clc    
        ret    
PageFile_Call_Async_Manager:
        jmp    loc_2_035C
loc_2_0358:
        stc    
        ret    
        db  000h, 000h
loc_2_035C:
        pop    eax
        add    eax, dword ptr [eax]
        add    byte ptr [edi + 0x49], dl
        dec    esi
        xor    edi, dword ptr [eax]
        push   ebx
        push   edi
        push   eax
        add    byte ptr [eax], al
loc_2_036C:
        push   eax
        popal  
        imul   ebp, dword ptr [bp + 0x67], 0x67615000
        imul   ebp, dword ptr [esi + 0x67], 0x656c6946
        add    byte ptr [eax + 0x61], dl
        imul   ebp, dword ptr [bp + 0x67], 0x76697244
        add    byte ptr gs:[ebp + 0x61], cl
        js     0x3de
        popal  
        imul   ebp, dword ptr [bp + 0x67], 0x656c6946
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0x6e694d00
        push   ebp
        jae    0x407
        jb     0x3e8
        imul   esi, dword ptr [ebx + 0x6b], 0x63617053
        add    byte ptr gs:[ebp + 0x69], cl
        outsb  dx, byte ptr [esi]
        push   eax
        popal  
        imul   ebp, dword ptr [bp + 0x67], 0x656c6946
        push   ebx
        db  069h, 07Ah, 065h, 000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 3  vsize=0xc0  RW-
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_00B0:
        adc    dword ptr [ecx], eax
        add    byte ptr [eax], al
        sbb    al, byte ptr [ecx]
        add    byte ptr [eax], al
        jb     0xbb
        add    byte ptr [eax], al
        jae    0xbf
        add    byte ptr [eax], al
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xaa  R-X
        db  053h, 04Dh, 041h, 052h, 054h, 041h, 041h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 0F7h, 0C3h, 003h, 000h, 075h, 067h, 0B8h, 007h, 016h, 0BBh, 021h
        db  000h, 033h, 0C9h, 0CDh, 02Fh, 00Bh, 0C0h, 075h, 00Ah, 08Ch, 0C2h, 066h, 0C1h, 0E2h, 010h, 08Bh
        db  0D7h, 0EBh, 047h, 0B8h, 000h, 03Dh, 0BAh, 000h, 000h, 0CDh, 021h, 072h, 03Ah, 08Bh, 0D8h, 0B8h
        db  000h, 044h, 0CDh, 021h, 072h, 02Dh, 0F7h, 0C2h, 080h, 000h, 074h, 027h, 0BAh, 009h, 000h, 0B9h
        db  02Ch, 000h, 0B8h, 002h, 044h, 0CDh, 021h, 072h, 01Ah, 083h, 0F8h, 02Ch, 075h, 015h, 0B4h, 03Eh
        db  0CDh, 021h, 066h, 033h, 0D2h, 080h, 03Eh, 018h, 000h, 003h, 072h, 00Eh, 066h, 08Bh, 016h, 031h
        db  000h, 0EBh, 007h, 0B4h, 03Eh, 0CDh, 021h, 066h, 033h, 0D2h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h
        db  000h, 0C3h, 033h, 0DBh, 033h, 0F6h, 0B8h, 001h, 080h, 0C3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x34  R-X
PageFile_Clean_Up:
        mov    edx, dword ptr [0x84]         ; -> obj3:0x0084
        call   sub_7_0000
        mov    ebx, dword ptr [0xf0]         ; -> obj1:0x00F0
        cmp    byte ptr [0xf5], 0            ; -> obj1:0x00F5
        jne    loc_5_0026
        mov    eax, 0xd700
        VxDCall  IFSMgr.IFSMgr_Ring0_FileIO      ; cd2032004000
loc_5_0025:
        ret    
loc_5_0026:
        mov    ah, 0x3e
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jmp    loc_5_0025
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x378  R-X
loc_6_0000:
        pushal 
        xor    esi, esi
        mov    edi, edx
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        inc    byte ptr [0xac]               ; -> obj3:0x00AC
        shl    edx, 0xc
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        mov    eax, dword ptr [0x9c]         ; -> obj3:0x009C
        mov    word ptr [ebp + 0x30], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x34], ax
        call   sub_6_01DD
        mov    eax, dword ptr [0xf0]         ; -> obj1:0x00F0
        mov    dword ptr [ebp + 0x10], eax
        mov    dword ptr [ebp + 0x14], edx
        shr    edx, 0x10
        mov    dword ptr [ebp + 0x18], edx
        mov    word ptr [ebp + 0x1c], 0x4200
        call   sub_6_0241
        jb     loc_6_00BA
        mov    byte ptr [ebp + 0x1d], 0x40
        mov    dword ptr [ebp + 0x18], esi
        call   sub_6_0241
        jb     loc_6_00BA
        mov    byte ptr [ebp + 0x1d], 0x45
        call   sub_6_0241
        jb     loc_6_0087
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp + 0x10], eax
        mov    byte ptr [ebp + 0x1d], 0x3e
        call   sub_6_0241
loc_6_0087:
        mov    eax, dword ptr [0xf0]         ; -> obj1:0x00F0
        mov    dword ptr [ebp + 0x10], eax
        mov    word ptr [ebp + 0x1c], 0x4202
        mov    dword ptr [ebp + 0x18], esi
        mov    dword ptr [ebp + 0x14], esi
        call   sub_6_0241
        jb     loc_6_00BA
        mov    eax, dword ptr [ebp + 0x14]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x1c]
        shr    eax, 0xc
        mov    dword ptr [0x88], eax         ; -> obj3:0x0088
        cmp    eax, edi
        jne    loc_6_00BA
        mov    esi, esp
loc_6_00BA:
        call   sub_6_0215
        dec    byte ptr [0xac]               ; -> obj3:0x00AC
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        sub    esi, 1
        popal  
        ret    
PageFile_Get_Version:
        mov    bl, byte ptr [0xf4]           ; -> obj1:0x00F4
sub_6_00E5:
        mov    eax, 0x400
        clc    
        ret    
PageFile_Cancel:
        ret    
PAGEFILE_PM_API:
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_6_010B
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    ax, 4
        jae    loc_6_010B
        and    byte ptr [ebp + 0x2c], 0xfe
        call   sub_3_00B0
loc_6_010A:
        ret    
loc_6_010B:
        or     byte ptr [ebp + 0x2c], 1
        jmp    loc_6_010A
loc_6_0111:
        call   sub_6_00E5
        mov    dword ptr [ebp + 0x1c], eax
        ret    
loc_6_011A:
        xor    eax, eax
        cmp    dword ptr [0x80], eax         ; -> obj3:0x0080
        je     loc_6_0168
        xchg   edi, eax
        mov    ax, 0x3c04
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        inc    edi
        je     loc_6_016C
        dec    edi
        mov    esi, 0                        ; -> obj3:0x0000
        mov    ecx, 0x20
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [0x80]         ; -> obj3:0x0080
        shl    eax, 0xc
        mov    dword ptr [ebp + 0x18], eax
        mov    eax, dword ptr [0x84]         ; -> obj3:0x0084
        shl    eax, 0xc
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, 1
        cmp    byte ptr [0xf4], 3            ; -> obj1:0x00F4
        je     loc_6_0168
        or     ah, 0x40
loc_6_0168:
        mov    dword ptr [ebp + 0x1c], eax
loc_6_016B:
        ret    
loc_6_016C:
        or     byte ptr [ebp + 0x2c], 1
        jmp    loc_6_016B
loc_6_0172:
        ret    
loc_6_0173:
        xor    eax, eax
        mov    word ptr [ebp + 0x14], ax
        mov    word ptr [ebp + 0x1c], ax
        cmp    dword ptr [0x80], eax         ; -> obj3:0x0080
        je     loc_6_0198
        mov    eax, dword ptr [0x88]         ; -> obj3:0x0088
        shl    eax, 0xc
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
loc_6_0198:
        ret    
loc_6_0199:
        cmp    byte ptr [0xac], 0            ; -> obj3:0x00AC
        jne    loc_6_01A4
        stc    
        ret    
loc_6_01A4:
        mov    byte ptr [ebp + 0x1c], 3
        clc    
        ret    
loc_6_01AA:
        cmp    byte ptr [0xac], 0            ; -> obj3:0x00AC
        jne    loc_6_01B5
        stc    
        ret    
loc_6_01B5:
        clc    
        ret    
sub_6_01B7:
        pushal 
        VMMCall  VMM._GetNulPageHandle           ; cd206a000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        push   0x10
        push   0
        push   1
        push   dword ptr [0x98]              ; -> obj3:0x0098
        push   ebx
        push   eax
        VMMCall  VMM._MapIntoV86                 ; cd205d000100
        add    esp, 0x18
        popal  
        ret    
sub_6_01DD:
        mov    byte ptr [ebp + 0x1d], 0x51
        call   sub_6_0241
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [0xa8], ax           ; -> obj3:0x00A8
        movzx  eax, word ptr [0xaa]          ; -> obj3:0x00AA
        VxDCall  DOSMGR.DOSMGR_MMGR_PSP_Change_Notifier ; cd2012001500
        mov    dword ptr [ebp + 0x10], eax
        shl    eax, 4
        push   dword ptr [eax + 0x2e]
        pop    dword ptr [0xa4]              ; -> obj3:0x00A4
        mov    byte ptr [ebp + 0x1d], 0x50
        call   sub_6_0241
        ret    
sub_6_0215:
        mov    ax, word ptr [0xa8]           ; -> obj3:0x00A8
        VxDCall  DOSMGR.DOSMGR_MMGR_PSP_Change_Notifier ; cd2012001500
        mov    dword ptr [ebp + 0x10], eax
        mov    byte ptr [ebp + 0x1d], 0x50
        call   sub_6_0241
        movzx  eax, word ptr [0xaa]          ; -> obj3:0x00AA
        shl    eax, 4
        push   dword ptr [0xa4]              ; -> obj3:0x00A4
        pop    dword ptr [eax + 0x2e]
        ret    
sub_6_0241:
        push   eax
        push   ecx
        push   edx
        mov    ecx, dword ptr [0x94]         ; -> obj3:0x0094
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Build_Int_Stack_Frame       ; cd204c000100
        and    byte ptr [ebp + 0x2d], 0xfb
        VMMCall  VMM._EnterMustComplete          ; cd2035010100
        VMMCall  VMM._ResumeExecMustComplete     ; cd2037010100
        pushfd 
        VMMCall  VMM._LeaveMustComplete          ; cd2036010100
        popfd  
        jb     loc_6_0276
        bt     word ptr [ebp + 0x2c], 0
loc_6_0276:
        pop    edx
        pop    ecx
        pop    eax
        ret    
loc_6_027A:
        pushal 
        mov    di, 1
        mov    esi, ebx
        mov    eax, dword ptr [esi + 8]
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        shr    eax, 0xc
        push   0
        push   1
        push   dword ptr [0x98]              ; -> obj3:0x0098
        push   ebx
        push   eax
        VMMCall  VMM._LinMapIntoV86              ; cd2062000100
        add    esp, 0x14
        test   eax, eax
        je     loc_6_0370
        mov    ch, byte ptr [esi + 4]
        mov    eax, dword ptr [0xa0]         ; -> obj3:0x00A0
        test   eax, eax
        je     loc_6_02B8
        inc    byte ptr [eax]
loc_6_02B8:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    ebp, dword ptr [ebx + 8]
        mov    eax, dword ptr [0x9c]         ; -> obj3:0x009C
        mov    word ptr [ebp + 0x30], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x34], ax
        inc    byte ptr [0xac]               ; -> obj3:0x00AC
        call   sub_6_01DD
        mov    word ptr [ebp + 0x1c], 0x4200
        mov    eax, dword ptr [0xf0]         ; -> obj1:0x00F0
        mov    dword ptr [ebp + 0x10], eax
        mov    eax, dword ptr [esi + 0xc]
        shl    eax, 0xc
        mov    dword ptr [ebp + 0x14], eax
        shr    eax, 0x10
        mov    dword ptr [ebp + 0x18], eax
        call   sub_6_0241
        jb     loc_6_0340
        add    ch, 0x3f
        mov    byte ptr [ebp + 0x1d], ch
        mov    word ptr [ebp + 0x18], 0x1000
        mov    eax, dword ptr [0x98]         ; -> obj3:0x0098
        shl    eax, 8
        mov    word ptr [ebp + 0x3c], ax
        mov    word ptr [ebp + 0x14], 0
        call   sub_6_0241
        jb     loc_6_0340
        cmp    word ptr [ebp + 0x1c], 0x1000
        jne    loc_6_0340
        mov    di, 0
loc_6_0340:
        call   sub_6_0215
        dec    byte ptr [0xac]               ; -> obj3:0x00AC
        call   sub_6_01B7
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        mov    eax, dword ptr [0xa0]         ; -> obj3:0x00A0
        test   eax, eax
        je     loc_6_0370
        dec    byte ptr [eax]
loc_6_0370:
        mov    eax, edi
        mov    byte ptr [esi + 6], al
        popal  
        ret    
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0x130  R-X
sub_7_0000:
        cmp    byte ptr [0xf5], 0            ; -> obj1:0x00F5
        jne    loc_6_0000
        mov    ebx, dword ptr [0xf0]         ; -> obj1:0x00F0
        xor    ecx, ecx
        mov    eax, 0xd601
        push   edx
        shl    edx, 0xc
        VxDCall  IFSMgr.IFSMgr_Ring0_FileIO      ; cd2032004000
        pop    edx
        jb     loc_7_002D
        mov    dword ptr [0x88], edx         ; -> obj3:0x0088
loc_7_002D:
        ret    
PageFile_Grow_File:
        sub    esp, 4
        push   edx
        push   ecx
        mov    edx, dword ptr [0x88]         ; -> obj3:0x0088
        add    edx, ecx
        js     loc_7_00DC
        cmp    edx, dword ptr [0x84]         ; -> obj3:0x0084
        ja     loc_7_005B
        mov    edx, dword ptr [0x84]         ; -> obj3:0x0084
        cmp    edx, dword ptr [0x88]         ; -> obj3:0x0088
        je     loc_7_00DC
loc_7_005B:
        cmp    edx, dword ptr [0x80]         ; -> obj3:0x0080
        ja     loc_7_00ED
        or     ecx, ecx
        jns    loc_7_0090
        mov    ecx, dword ptr [0x8c]         ; -> obj3:0x008C
        dec    ecx
        lea    eax, [esp + 8]
        push   edx
        push   0
        push   0
        push   eax
        push   ecx
        VxDCall  IFSMgr.IFSMgr_GetLockState      ; cd206f004000
        add    esp, 0x10
        pop    edx
        cmp    dword ptr [esp + 8], -1
        jne    loc_7_00DC
        jmp    loc_7_00D7
loc_7_0090:
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, ecx
        mov    ax, 0x3601
        mov    edx, dword ptr [0x8c]         ; -> obj3:0x008C
        cmp    byte ptr [0xf5], 0            ; -> obj1:0x00F5
        je     loc_7_00B3
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        jmp    loc_7_00B9
loc_7_00B3:
        VxDCall  IFSMgr.IFSMgr_Ring0_FileIO      ; cd2032004000
loc_7_00B9:
        mov    edx, dword ptr [esp]
        jb     loc_7_00D1
        imul   ecx, eax
        imul   ecx, ebx
        shr    ecx, 0xc
        sub    ecx, edx
        cmp    ecx, dword ptr [0x90]         ; -> obj3:0x0090
        jl     loc_7_00ED
loc_7_00D1:
        add    edx, dword ptr [0x88]         ; -> obj3:0x0088
loc_7_00D7:
        call   0
loc_7_00DC:
        pop    ecx
        pop    edx
        mov    eax, dword ptr [0x88]         ; -> obj3:0x0088
        mov    ebx, dword ptr [0x80]         ; -> obj3:0x0080
        add    esp, 4
        ret    
loc_7_00ED:
        stc    
        jmp    loc_7_00DC
PageFile_Get_Size_Info:
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, eax
        mov    ah, 0x36
        mov    edx, dword ptr [0x8c]         ; -> obj3:0x008C
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        imul   ecx, eax
        imul   ecx, ebx
        shr    ecx, 0xc
        mov    esi, 0                        ; -> obj3:0x0000
        mov    eax, dword ptr [0x88]         ; -> obj3:0x0088
        mov    ebx, dword ptr [0x80]         ; -> obj3:0x0080
        add    ecx, eax
        mov    edx, dword ptr [0x84]         ; -> obj3:0x0084
        cmp    ecx, ebx
        jb     loc_7_012D
        mov    ecx, ebx
loc_7_012D:
        ret    
        db  025h, 050h
VxD__LTEXT ENDS
